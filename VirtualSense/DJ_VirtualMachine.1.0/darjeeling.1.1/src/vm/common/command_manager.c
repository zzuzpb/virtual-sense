/*
 *	command_manager.c
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
#include "common/command_manager.h"


static uint8_t _command_id;
static int16_t _execution_context_id;
static int16_t waiting_thread_id;


void command_manager_wakeUpPlatformThread(uint8_t command_id, int16_t execution_context_id){
	dj_thread *w_thread;
	w_thread = dj_vm_getThreadById(dj_exec_getVM(), waiting_thread_id);
	if(w_thread != nullref){
			w_thread->status = THREADSTATUS_RUNNING;
			// we need to ensure that this thread will take the CPU before other running thread
			w_thread->need_resched = 1;
			command_manager_setCommandID(command_id);
			command_manager_setExecutionContextID(execution_context_id);

	}
}

void command_manager_setPlatformThreadID(int16_t id){
		// save the waiting  thread id.
		waiting_thread_id = id;
}

uint8_t command_manager_getCommandID(){
	return _command_id;
}
int16_t command_manager_getExecutionContextID(){
	return _execution_context_id;
}
void command_manager_setCommandID(uint8_t command_id){
	_command_id = command_id;
}
void command_manager_setExecutionContextID(int16_t execution_context_id){
	_execution_context_id = execution_context_id;
}





