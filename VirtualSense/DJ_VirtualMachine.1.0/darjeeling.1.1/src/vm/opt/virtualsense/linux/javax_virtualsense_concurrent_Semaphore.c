/*
 *	javax_virtualsense_concurrent_Semaphore.c
 *
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
 * Native semaphore implementation.
 *
 * @author Emanuele Lattanzi
 *
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// generated at infusion time
#include "virtualsense_definitions.h"
#include "base_definitions.h"

#include "common/execution/execution.h"
#include "common/heap/heap.h"
#include "common/djtimer.h"

static uint16_t semaphores = 0;

// short javax.virtualsense.concurrent.Semaphore._create()
void javax_virtualsense_concurrent_Semaphore_short__create()
{
	semaphores++;
	dj_exec_stackPushShort(semaphores);
}

// void javax.virtualsense.concurrent.Samphore._waitForSemaphore(short)
void javax_virtualsense_concurrent_Semaphore_void__waitForSemaphore_short(){

	uint16_t sem = dj_exec_stackPopShort();
	dj_thread *thread = dj_exec_getCurrentThread();
	thread->sem_id = sem;

	//printf("The waiting thread on semaphore is %d\n", thread->id);
	// if the semaphore is just created there is no waiting threads
	//printf("wait for native semaphore %d thread %d\n", sem, thread->id);
	thread->status = THREADSTATUS_BLOCKED;
	dj_exec_breakExecution();
}


// void javax.virtualsense.concurrent.Samphore._wakeupWaitingThread(Semaphore)
void javax_virtualsense_concurrent_Semaphore_void__wakeupWaitingThread_short(){
	dj_thread *thread = dj_exec_getCurrentThread();
	uint16_t sem = dj_exec_stackPopShort();
	dj_thread *wake_thread;

	//printf("wakeup from native semaphore by thread %d on semaphore %d\n", thread->id, sem);
	wake_thread = dj_vm_getThreadBySem(dj_exec_getVM(), sem);
	//printf("The waiting thread is %d\n", wake_thread->id);
	//wake-up the waiting thread and remove it from the semaphore queue.

	wake_thread->status = THREADSTATUS_RUNNING;
	wake_thread->sem_id = 0;
	//remove_FIFO_enqueue_thread_from_semaphore(semaphore);

}




