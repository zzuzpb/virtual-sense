/*
 *	javax_virtualsense_platform_CommandManager.c
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
 * Task (thread) native methods.
 *
 * @author Emanuele Lattanzi
 *
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// generated at infusion time
#include "base_definitions.h"

#include "common/execution/execution.h"
#include "common/heap/heap.h"
#include "common/djtimer.h"

#include "common/app_manager.h"



void javax_virtualsense_platform_CommandManager_short__readExecutionContextID(){
	dj_exec_stackPushShort((int16_t)app_manager_getExecutionContextID());
}

void javax_virtualsense_platform_CommandManager_short__readCommandID(){
	dj_exec_stackPushShort((int16_t)app_manager_getCommandID());

}
void javax_virtualsense_platform_CommandManager_void__waitForMessage(){
	dj_thread * waiting_thread;
	waiting_thread = dj_exec_getCurrentThread();
	app_manager_setPlatformThreadID(waiting_thread->id);
	waiting_thread->status = THREADSTATUS_BLOCKED_FOR_IO;
	dj_exec_breakExecution();

}
