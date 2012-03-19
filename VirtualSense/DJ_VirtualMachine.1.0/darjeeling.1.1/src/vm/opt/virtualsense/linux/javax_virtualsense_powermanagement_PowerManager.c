/*
 *	javax_virtualsense_powermanagement_PowerManager.c
 *
 *  Copyright (c) 2011 DiSBeF, University of Urbino.
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
 * Controls power manager of the board
 * based on modified contiki operating system.
 *
 * @author Emanuele Lattanzi
 *
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// generated at infusion time
#include "base_definitions.h"
#include "virtualsense_definitions.h"

#include "common/execution/execution.h"
#include "common/heap/heap.h"
#include "common/djtimer.h"
#include "common/debug.h"
#include "common/system_hibernation.h"


// int javax.virtualsense.powermanagement.PowerManager.getBatteryVoltage()
void javax_virtualsense_powermanagement_PowerManager_int_getBatteryVoltage()
{
	dj_exec_createAndThrow(BASE_CDEF_java_lang_VirtualMachineError);
}


// int javax.virtualsense.powermanagement.PowerManager.getSolarVoltage()
void javax_virtualsense_powermanagement_PowerManager_int_getSolarVoltage()
{
	dj_exec_createAndThrow(BASE_CDEF_java_lang_VirtualMachineError);
}


// int javax.virtualsense.powermanagement.PowerManager.getSolarCurrent()
void javax_virtualsense_powermanagement_PowerManager_int_getSolarCurrent()
{
	dj_exec_createAndThrow(BASE_CDEF_java_lang_VirtualMachineError);
}


//void javax.virtualsense.powermanagement.PowerManager.slowDownClockByFactor(short)
void javax_virtualsense_powermanagement_PowerManager_void_slowDownClockByFactor_int()
{
	uint32_t factor = dj_exec_stackPopInt();
	printf("dj_timer_slowDownClockByFactor((uint16_t)factor) = %d\n",factor);
}

//void javax.virtualsense.powermanagement.PowerManager.setSystemClockMillis(int)
void javax_virtualsense_powermanagement_PowerManager_void_setSystemClockMillis_int()
{
	uint32_t millis = dj_exec_stackPopInt();
	printf("dj_timer_setSystemClockMillis(millis) = %d\n", millis);
}

void javax_virtualsense_powermanagement_PowerManager_void_setMCUFrequency_short()
{
	unsigned char cpu_speed = (unsigned char)dj_exec_stackPopShort();

	switch (cpu_speed)
	 {
	 case 1:
		 printf("MCU freq 1MHZ\n");

	    break;
	 case 4:
		 printf("MCU freq 4MHZ\n");

	   break;
	 case 8:
		 printf("MCU freq 8MHZ\n");

	   break;
	 case 12:
		 printf("MCU freq 12MHZ\n");
	   break;
	 case 16:
		 printf("MCU freq 16MHZ\n");

	   break;
	 case 20:
		 printf("MCU freq 20MHZ\n");

	   break;
	 case 25:
		 printf("MCU freq 25MHZ\n");
		 break;
	 //default:
	 		//printf("Default %d\n", cpu_speed);
	 }

}


//void javax.virtualsense.powermanagement.PowerManager.setSystemClockMillis(int)
void javax_virtualsense_powermanagement_PowerManager_void_systemHibernation()
{
	dj_vm *vm;
	dj_thread *thread;
	dj_thread *currentThread;
	uint8_t saved = 0;


	dj_mem_gc();
	vm = dj_exec_getVM();
	currentThread = vm->currentThread;

	DEBUG_LOG("VM pointer %p\n", vm);
	DEBUG_LOG("heap left pointer %p\n", (dj_mem_get_base_pointer()+dj_mem_get_left_pointer()));
	DEBUG_LOG("heap right pointer %p\n", (dj_mem_get_base_pointer()+dj_mem_get_right_pointer()));
	DEBUG_LOG("current thread pointer %p\n", currentThread);

	if(currentThread!=NULL){
		DEBUG_LOG("Preparing hibernation triggered by thread %d\n", currentThread->id);
		DEBUG_LOG("PC before hibernation %d\n", currentThread->frameStack->pc);
		dj_exec_deactivateThread(currentThread);
	}

	/* hibernate all threads */
	thread = vm->threads;
	while (thread!=NULL){
		thread->hibernated = 1;
		thread=thread->next;
	}

	DEBUG_LOG("Saving machine state on non-volatile memory\n");
	saved = save_heap(dj_mem_get_base_pointer(),
					  dj_mem_get_left_pointer(),
					  dj_mem_get_right_pointer(),
					  dj_mem_get_panic_exception_object_pointer(),
					  dj_mem_get_ref_stack());
	if(saved){
		DEBUG_LOG("Ibernation done....\n");
		/* on linux we need to shut-down the actual process
		 * in MSP430 platform we will put MCU on LPM4.5 state
		 */
		exit(1);
	}else {
		dj_exec_createAndThrow(VIRTUALSENSE_CDEF_javax_virtualsense_powermanagement_HibernationException);
	}
}


//void javax.virtualsense.powermanagement.PowerManager.standby()
void javax_virtualsense_powermanagement_PowerManager_void_standby()
{
	printf("Standby invoked \n");
}

//void javax.virtualsense.powermanagement.PowerManager.deepSleep()
void javax_virtualsense_powermanagement_PowerManager_void_deepSleep()
{
	printf("Deep sleep invoked \n");
}

void javax_virtualsense_powermanagement_PowerManager_void_scheduleRTCInterruptAfter_int(){
	int32_t minutes = dj_exec_stackPopInt();
	printf("scheduleRTCInterruptAfter %d \n", minutes);
}

