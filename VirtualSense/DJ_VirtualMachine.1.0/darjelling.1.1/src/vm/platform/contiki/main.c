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

#include "stdio.h"

#include "common/debug.h"
#include "common/vm.h"
#include "common/heap/heap.h"
#include "common/infusion.h"
#include "common/types.h"
#include "common/vmthread.h"
#include "common/djtimer.h"
#include "common/execution/execution.h"

#include "loader.h"

#ifdef HAS_USART
#include "dev/rs232.h"
#endif

/*---------------------------------------------------------------------------*/
PROCESS(blink_process, "Darjeeling");
AUTOSTART_PROCESSES(&blink_process);
/*---------------------------------------------------------------------------*/

static unsigned char mem[HEAPSIZE];
static struct etimer et;
static dj_vm * vm;
static long nextScheduleTime = 0;
static long deltaSleep = 0;

PROCESS_THREAD(blink_process, ev, data)
{

	PROCESS_EXITHANDLER(goto exit;)
	PROCESS_BEGIN();

	// initialise memory manager
	dj_mem_init(mem, HEAPSIZE);

	// initialise timer
	dj_timer_init();

	// init hw
	leds_init();

#ifdef HAS_USART
	rs232_init(RS232_PORT_0, USART_BAUD_57600, USART_PARITY_NONE | USART_DATA_BITS_8 | USART_STOP_BITS_1);
	rs232_redirect_stdout(RS232_PORT_0);
#endif

	// create a new VM
	vm = dj_vm_create();

	// tell the execution engine to use the newly created VM instance
	dj_exec_setVM(vm);
	
	// load the embedded infusions
	dj_loadEmbeddedInfusions(vm);
	
	while (true)
	{
		// start the main execution loop
		if (dj_vm_countLiveThreads(vm)>0)
		{
			nextScheduleTime = dj_vm_schedule(vm);
			//printf("Next time = %ld\n", nextScheduleTime);
			while (vm->currentThread!=NULL){ /* LELE: inserito while per schedulare pi� thread
												nella stessa epoca (in questo modo per� non
												abbiamo preemption ?? */
				if (vm->currentThread->status==THREADSTATUS_RUNNING)
					dj_exec_run(RUNSIZE);
				nextScheduleTime = dj_vm_schedule(vm);
			}

		}
		deltaSleep = (nextScheduleTime - dj_timer_getTimeMillis())/10;
		if(deltaSleep <= 0) deltaSleep = 1;
		//printf("delta time = %ld\n", deltaSleep);
		// can't get PROCESS_YIELD to work, quick hack to wait 1 clock tick
	    etimer_set(&et, (clock_time_t)deltaSleep);
	    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
	}

exit:
	leds_off(LEDS_ALL);
	PROCESS_END();
}
