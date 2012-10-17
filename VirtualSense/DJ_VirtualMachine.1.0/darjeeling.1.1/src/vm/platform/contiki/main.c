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

#include "platform-conf.h"
#include "loader.h"

#ifdef PLATFORM_HAS_RF
#include "net/rime.h"
#endif

#ifdef HAS_USART
#include "dev/rs232.h"
#endif

/*---------------------------------------------------------------------------*/
PROCESS(darjeeling_process, "Dj");
AUTOSTART_PROCESSES(&darjeeling_process);
/*---------------------------------------------------------------------------*/
PROCESS(command_manager_process, "Command manager");

static struct broadcast_conn broadcast_command;
static void broadcast_command_recv(struct broadcast_conn *c, const rimeaddr_t *from);
static const struct broadcast_callbacks broadcast_command_call = {broadcast_command_recv};

static unsigned char mem[HEAPSIZE];
static dj_infusion *to_init = NULL;
//static unsigned char temp_di[TEMP_DI_SIZE]; // temporary buffer to copy di files from far rom
static struct etimer et;
static dj_vm * vm;
static long nextScheduleTime = 0;
static long deltaSleep = 0;
static uint8_t resume_from_hibernation = 0; /* to resume after hibernation */
uint16_t index = 0;


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
	
	// starting the command manager process
	process_start(&command_manager_process, NULL);

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
		//printf("#");
		/*index++;
		//LELE: test to load
		if(index == 20)
			app_manager_wakeUpPlatformThread(0,1);
		if(index == 40)
					app_manager_wakeUpPlatformThread(1,1);
		if(index == 60)
					app_manager_wakeUpPlatformThread(0,2);
		if(index == 80)
					app_manager_wakeUpPlatformThread(1,2);
		if(index == 100)
					app_manager_wakeUpPlatformThread(0,3);
		if(index == 120)
					app_manager_wakeUpPlatformThread(1,3);
		if(index == 140)
					app_manager_wakeUpPlatformThread(2,1);
		if(index == 160)
					app_manager_wakeUpPlatformThread(2,2); */
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

PROCESS_THREAD(command_manager_process, ev, data)
{
  /*char buff[COMMAND_APPS_PACKET_SIZE-sizeof(COMMAND_APPS_HEADER)];
  int16_t app_id = -1;
  uint8_t command_id = -1;
  */

  PROCESS_BEGIN();
  lock_MAC();

  // open the broadcast command connection
  broadcast_open(&broadcast_command, COMMAND_APPS_PORT, &broadcast_command_call);

  printf("Command manager process started\n");
  while(1) {
	  printf("Nothing to do for now\n");
	  PROCESS_YIELD();
	  /*
	  }else {
		  // send command packet
		  printf("sending command %u for app %d\n", command_id, app_id);
		  app_id = -1;
		  buff[0] = app_id << 8;
		  buff[1] = app_id & 0xff;
		  buff[2] = command_id;

		  packetbuf_copyfrom(COMMAND_APPS_HEADER, sizeof(COMMAND_APPS_HEADER));
		  ((char *)packetbuf_dataptr())[sizeof(COMMAND_APPS_HEADER)] = buff;
		  packetbuf_set_datalen(COMMAND_APPS_PACKET_SIZE);
		  printf("packet prepared\n");
	      lock_RF();
	      broadcast_send(&broadcast);
	      release_RF();
	      printf("done\n");

	  }*/
  }
  PROCESS_END();
}
/*---------------------------------------------------------------------------*/




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
#endif

/* This function is called whenever a broadcast_command message is received. */
static void
broadcast_command_recv(struct broadcast_conn *c, const rimeaddr_t *from)
{
	  unsigned char buff[COMMAND_APPS_PACKET_SIZE];
	  uint8_t command_id;
	  uint8_t app_id;
	  uint8_t command_header = sizeof(COMMAND_APPS_HEADER);
	  int length = packetbuf_datalen();
	  int i = 0;

	  packetbuf_set_attr(PACKETBUF_ADDR_RECEIVER, node_id);
	  packetbuf_set_attr(PACKETBUF_ADDR_SENDER, ((from->u8[1]<<8) + from->u8[0]));
	  printf("broadcast COMMAND message received from %d.%d with RSSI %d, LQI %u\n",
			  from->u8[0], from->u8[1],
			  packetbuf_attr(PACKETBUF_ATTR_RSSI),
			  packetbuf_attr(PACKETBUF_ATTR_LINK_QUALITY));
	  packetbuf_copyto(buff);
	  printf("received packet bytes: ");
	  for(i=0; i< length; i++)
		  printf("0x%x ", buff[i]);
	  printf("\n");
	  printf("Packet length is %d\n", length);
	  app_id = buff[command_header];
	  printf("The header is %s\n",buff);
	  command_id = buff[command_header+1];
	  printf("The command id is %u\n", command_id);
	  printf("The app id is %u\n", app_id);
	  app_manager_wakeUpPlatformThread(command_id, app_id);
	  release_RF(); // release RF lock to allow power manager to shutdown the radio module
	  //release_MAC(); // release mac to allow power manger to stop duty cycle.
	  process_poll(&darjeeling_process); //TODO: ?
}


void dj_main_runDeferredInitializer(dj_infusion *infusion){
	to_init = infusion;
}



