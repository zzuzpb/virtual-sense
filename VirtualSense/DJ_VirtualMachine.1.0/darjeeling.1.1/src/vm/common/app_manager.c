/*
 *	application_manager.c
 *
*  Copyright (c) 2012 DiSBeF, University of Urbino.
 *
 *	This file is part of VirtualSense.
 *
 *	VirtualSense is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	VirtualSense is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with VirtualSense.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * The VirtualSense application manager headers.
 *
 * @author Emanuele Lattanzi
 *
 */
#include "common/types.h"
#include "common/array.h"
#include "common/panic.h"
#include "common/debug.h"
#include "common/vmthread.h"

static int16_t waiting_thread_id;
static int8_t command_id;
static int8_t execution_context_id;


void app_manager_wakeUpPlatformThread(uint8_t c_id, uint8_t ex_id){
	dj_thread *w_thread;
	w_thread = dj_vm_getThreadById(dj_exec_getVM(), waiting_thread_id);
	// put message on the buffer
	command_id = c_id;
	execution_context_id = ex_id;
	if(w_thread != nullref){
			w_thread->status = THREADSTATUS_RUNNING;
			// we need to ensure that this thread will take the CPU before other running thread
			w_thread->need_resched = 1;
			command_id = c_id;
			execution_context_id = ex_id;

	}
}

void app_manager_setPlatformThreadID(int16_t id){
		// save the waiting  thread id.
		waiting_thread_id = id;
}

int8_t app_manager_getCommandID(){
	return command_id;
}
int8_t app_manager_getExecutionContextID(){
	return execution_context_id;
}

// return the pointer to the application infusion on the flash memory
// it looks on the application table to retrieve the pointer
dj_di_pointer app_manager_getApplicationPointer(int16_t unique_id){
	dj_di_pointer p;
	p = arch_getApplicationPointer(unique_id);
	printf("Here te pointer is %lu\n", p);
	return p;
}





