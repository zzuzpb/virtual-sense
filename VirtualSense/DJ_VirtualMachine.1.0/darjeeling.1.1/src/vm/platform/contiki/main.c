/*
 *	main.c
 *
 *	Copyright (c) 2008 CSIRO, Delft University of Technology.
 *
 *	This file is part of Darjeeling.
 *
 *	Darjeeling is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	Darjeeling is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with Darjeeling.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "contiki.h"
#include "dev/leds.h"
#include "dev/digitalio.h"
#include "node-id.h"

#include "stdio.h"

#include "common/debug.h"
#include "common/vm.h"
#include "common/heap/heap.h"
#include "common/infusion.h"
#include "common/types.h"
#include "common/vmthread.h"
#include "common/djtimer.h"
#include "common/execution/execution.h"
#include "common/app_manager.h"
#include "common/command_manager.h"

#include "platform-conf.h"
#include "loader.h"

#define SINK 0

#ifdef PLATFORM_HAS_RF
#include "net/rime.h"
#endif

#ifdef HAS_USART
#include "dev/rs232.h"
#endif

#if SINK

#include "../../../apps/shell/shell.h"
#include "../../../apps/serial-shell/serial-shell.h"
#include "../../../apps/shell/virtualsense-gateway-shell.h"


/*---------------------------------------------------------------------------*/
PROCESS(virtualsense_shell_process, "VirtualSense gateway shell");
//AUTOSTART_PROCESSES(&virtualsense_shell_process);
/*---------------------------------------------------------------------------*/
#endif
/*---------------------------------------------------------------------------*/
PROCESS(darjeeling_process, "Dj");
AUTOSTART_PROCESSES(&darjeeling_process);
/*---------------------------------------------------------------------------*/
#if !SINK
PROCESS(VM_command_manager_process, "Command manager");
#endif

static struct broadcast_conn broadcast_command;
static void broadcast_command_recv(struct broadcast_conn *c, const rimeaddr_t *from);
static const struct broadcast_callbacks broadcast_command_call = {broadcast_command_recv};
static uint8_t  last_command_id = 0;
static uint8_t  last_command	= 0;
static uint16_t last_app_id	    = 0;

static void digital_io_callback(uint8_t port);

static unsigned char mem[HEAPSIZE];
static dj_infusion *to_init = NULL;
//static unsigned char temp_di[TEMP_DI_SIZE]; // temporary buffer to copy di files from far rom
static struct etimer et;
static dj_vm * vm;
static long nextScheduleTime = 0;
static long deltaSleep = 0;
static uint8_t resume_from_hibernation = 0; /* to resume after hibernation */

/****************************************************************************************
 *  radio call-back handler:
 *  The contiki radio driver will invoke this call-back whenever a new radio packet
 *  is arrived.
 *
 * ***************************************************************************************/

#ifdef PLATFORM_HAS_RF
/* These hold the broadcast and unicast structures, respectively. */
static struct broadcast_conn broadcast;
static struct unicast_conn unicast;
uint16_t receiver_thread_id;
#endif


PROCESS_THREAD(darjeeling_process, ev, data)
{

	PROCESS_EXITHANDLER(goto exit;)
	PROCESS_BEGIN();

	// Initialize memory manager
	dj_mem_init(mem, HEAPSIZE);

	//load the VM from the restored heap
	//load the heap content from the hibernation file if found
	resume_from_hibernation = load_machine(mem);

	// initialise timer
	dj_timer_init();

	// init hw
	leds_init();
	P4OUT &= ~BIT4;

	if(resume_from_hibernation){
			//printf("Loading VM from hibernation\n");
			vm = dj_vm_load_from_heap(mem);
			/*DEBUG_LOG("VM_POINTER %p\n", vm);
			DEBUG_LOG("heap left pointer %p\n", (dj_mem_get_base_pointer()+dj_mem_get_left_pointer()));
			DEBUG_LOG("heap right pointer %p\n", (dj_mem_get_base_pointer()+dj_mem_get_right_pointer()));
			DEBUG_LOG("current thread pointer %p\n", vm->currentThread);
			DEBUG_LOG("There are %d live threads\n", dj_vm_countLiveThreads(vm)); */
			//heap_dump(((void *)mem));
	}else { // we have to create a new VM
			//printf("Creating a new VM\n");
	    	vm = dj_vm_create();
	}


	// create a new VM
	/*vm = dj_vm_create();*/

	// tell the execution engine to use the newly created VM instance
	dj_exec_setVM(vm);
	
	// load the embedded infusions
	//if(!resume_from_hibernation)

	dj_loadEmbeddedInfusions(vm);
	//printf("Loaded embedded infusion\n");
	

#if SINK
	// starting shell process if we are sink
	process_start(&virtualsense_shell_process, NULL);
#else
	// starting the command manager process
	process_start(&VM_command_manager_process, NULL);
#endif

	// load application table from storage memory
	app_manager_loadApplicationTable();

	//start digital I/O interface and callbacks
	init_digitalio_interface(digital_io_callback);

	while (true)
	{
		// start the main execution loop
		if (dj_vm_countLiveThreads(vm)>0)
		{
			nextScheduleTime = dj_vm_schedule(vm);


			//nextScheduleTime = 2147483647;
			DEBUG_LOG("Next time = %ld now %ld\n", nextScheduleTime, dj_timer_getTimeMillis());
			while (vm->currentThread!=NULL){ /* LELE: inserito while per schedulare pi� thread
												nella stessa epoca (in questo modo per� non
												abbiamo preemption ?? */
				if (vm->currentThread->status==THREADSTATUS_RUNNING)
					dj_exec_run(RUNSIZE);
				if(to_init != NULL){// execute the deferred initilization needed by the run-time app loading
					dj_vm_runClassInitialisers(vm, to_init);
					to_init = NULL;
				}
				nextScheduleTime = dj_vm_schedule(vm);

			}

		}
		deltaSleep = (nextScheduleTime - dj_timer_getTimeMillis())/10;
		if(deltaSleep <= 0) deltaSleep = 1;
		DEBUG_LOG("delta time = %ld\n", deltaSleep);
		// can't get PROCESS_YIELD to work, quick hack to wait 1 clock tick
	    etimer_set(&et, (clock_time_t)deltaSleep);
	    PROCESS_YIELD_UNTIL((etimer_expired(&et) || ev == PROCESS_EVENT_POLL));
	}

exit:
	leds_off(LEDS_ALL);
	PROCESS_END();
}

/*---------------------------------------------------------------------------*/
#if !SINK
PROCESS_THREAD(VM_command_manager_process, ev, data)
{
  struct virtualsense_apps_command_msg *msg;
  static struct etimer pulse_timer;
  PROCESS_BEGIN();
  lock_MAC();

  // open the broadcast command connection
  broadcast_open(&broadcast_command, COMMAND_APPS_PORT, &broadcast_command_call);

  printf("Command manager process started\n");
  while(1) {
	  PROCESS_YIELD();
	  etimer_set(&pulse_timer, CLOCK_SECOND/10); // wait to eliminate collision
	  PROCESS_WAIT_EVENT();

		  // send command packet
	  //printf("forwarding command %u %u for app %d\n", last_command_id, last_command, last_app_id);
	  packetbuf_clear();
	  msg = (struct virtualsense_apps_command_msg *)packetbuf_dataptr();
	  packetbuf_set_datalen(sizeof(struct virtualsense_apps_command_msg));
	  memcpy(msg->header, COMMAND_APPS_HEADER, sizeof(COMMAND_APPS_HEADER));
	  msg->id = last_command_id;
	  msg->app_id = last_app_id;
	  msg->command = last_command;
	  //printf("packet prepared\n");
	  lock_RF();

	  broadcast_send(&broadcast_command);
	  release_RF();
	  //printf("done\n");
  }
  PROCESS_END();
  release_MAC();
}
#endif
/*---------------------------------------------------------------------------*/

#if SINK
PROCESS_THREAD(virtualsense_shell_process, ev, data)
{
  PROCESS_BEGIN();
  serial_shell_init();
  shell_reboot_init();
  lock_MAC();
   // open the broadcast command connection
  broadcast_open(&broadcast_command, COMMAND_APPS_PORT, &broadcast_command_call);
  shell_gateway_init(&broadcast_command);
  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
#endif



/* This function is called whenever a broadcast message is received. */
static void
broadcast_recv(struct broadcast_conn *c, const rimeaddr_t *from)
{
	dj_thread *rec_thread;
  	/* Print out a message. */
  packetbuf_set_attr(PACKETBUF_ADDR_RECEIVER, node_id);
  packetbuf_set_attr(PACKETBUF_ADDR_SENDER, ((from->u8[1]<<8) + from->u8[0]));
  /*printf("broadcast message received from %d.%d with RSSI %d, LQI %u\n",
		  from->u8[0], from->u8[1],
  	      packetbuf_attr(PACKETBUF_ATTR_RSSI),
  	      packetbuf_attr(PACKETBUF_ATTR_LINK_QUALITY));*/

  rec_thread = dj_vm_getThreadById(dj_exec_getVM(), receiver_thread_id);
  if(rec_thread != nullref){
	  //printf("Thread  id %d status %d\n", rec_thread->id, rec_thread->status);
	  rec_thread->status = THREADSTATUS_RUNNING;
	  // we need to ensure that this thread will take the CPU before other running thread
	  rec_thread->need_resched = 1;
  }
  release_RF(); // release RF lock to allow power manager to shutdown the radio module
  //release_MAC(); // release mac to allow power manger to stop duty cycle.
  process_poll(&darjeeling_process);
}

/* This is where we define what function to be called when a broadcast
   is received. We pass a pointer to this structure in the
   broadcast_open() call below. */
static const struct broadcast_callbacks broadcast_call = {broadcast_recv};
/*---------------------------------------------------------------------------*/

/* This function is called for every incoming unicast packet. */
static void
recv_uc(struct unicast_conn *c, const rimeaddr_t *from)
{
	dj_thread *rec_thread;
	rec_thread = dj_vm_getThreadById(dj_exec_getVM(), receiver_thread_id);
	/*printf("unicast packet received from %d.%d with RSSI %d, LQI %u\n",
           from->u8[0], from->u8[1],
           packetbuf_attr(PACKETBUF_ATTR_RSSI),
           packetbuf_attr(PACKETBUF_ATTR_LINK_QUALITY));*/
	packetbuf_set_attr(PACKETBUF_ADDR_RECEIVER, node_id);
	packetbuf_set_attr(PACKETBUF_ADDR_SENDER, ((from->u8[1]<<8) + from->u8[0]));

	if(rec_thread != nullref){
		rec_thread->status = THREADSTATUS_RUNNING;
		// we need to ensure that this thread will take the CPU before other running thread
		rec_thread->need_resched = 1;

	}
    release_RF(); // release RF lock to allow power manager to shutdown the radio module
    //release_MAC(); // release mac to allow power manger to stop duty cycle.
	process_poll(&darjeeling_process);

}
static const struct unicast_callbacks unicast_callbacks = {recv_uc};
struct broadcast_conn broadcast_network_init(void){
		// save the receiver thread pointer.
		receiver_thread_id = dj_exec_getCurrentThread()->id;
		broadcast_open(&broadcast, 129, &broadcast_call);
		//unicast_open(&unicast, 146, &unicast_callbacks);
        return broadcast;

}
struct unicast_conn unicast_network_init(void){
		// save the receiver thread pointer.
		receiver_thread_id = dj_exec_getCurrentThread()->id;
		unicast_open(&unicast, 146, &unicast_callbacks);
        return unicast;

}


/* This function is called whenever a broadcast_command message is received. */
static void
broadcast_command_recv(struct broadcast_conn *c, const rimeaddr_t *from)
{
	  struct virtualsense_apps_command_msg *msg;
	  packetbuf_set_attr(PACKETBUF_ADDR_RECEIVER, node_id);
	  packetbuf_set_attr(PACKETBUF_ADDR_SENDER, ((from->u8[1]<<8) + from->u8[0]));
	  /*printf("broadcast COMMAND message received from %d.%d with RSSI %d, LQI %u\n",
			  from->u8[0], from->u8[1],
			  packetbuf_attr(PACKETBUF_ATTR_RSSI),
			  packetbuf_attr(PACKETBUF_ATTR_LINK_QUALITY));*/
	  msg = (struct virtualsense_apps_command_msg *)packetbuf_dataptr();
#if !SINK
	  if(msg->id != last_command_id){ //The same command should be received only one time
		  last_command_id = msg->id;
		  last_app_id = (msg->app_id <<8) +(msg->app_id >> 8); //WHY ?
		  last_command = msg->command;
		  command_manager_wakeUpPlatformThread(last_command, last_app_id);
		  process_poll(&darjeeling_process);
		  process_poll(&VM_command_manager_process);
	  }
#else

	  printf("SINK_ received a command message\n");
	  printf("command: %u %u for app %d\n", msg->id, msg->command, msg->app_id);
#endif
}


void dj_main_runDeferredInitializer(dj_infusion *infusion){
	to_init = infusion;
}

static void digital_io_callback(uint8_t port){
	//printf("Call port %d\n", port);
	//TODO: wake-up all threads waiting for this interrupt.
	uint16_t sem = (uint16_t)port;
	dj_thread *wake_thread;
	wake_thread = dj_vm_getThreadBySem(dj_exec_getVM(), (sem+1));
	while( wake_thread != NULL){//Increase the port number by 1 to match semaphore number (zero is not allowed)
		printf("w thread %d\n", wake_thread->id);
		wake_thread->status = THREADSTATUS_RUNNING;
		wake_thread->sem_id = 0;
		wake_thread->need_resched = 1;
		wake_thread = dj_vm_getThreadBySem(dj_exec_getVM(), (sem+1));
	}
	process_poll(&darjeeling_process);


}



