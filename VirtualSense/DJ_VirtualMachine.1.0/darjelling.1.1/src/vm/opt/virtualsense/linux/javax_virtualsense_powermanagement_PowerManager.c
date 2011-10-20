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

#include "common/execution/execution.h"
#include "common/heap/heap.h"
#include "common/djtimer.h"

// int javax.darjeeling.Darjeeling.getBatteryVoltage()
void javax_darjeeling_PowerManager_int_getBatteryVoltage()
{
	dj_exec_createAndThrow(BASE_CDEF_java_lang_VirtualMachineError);
}


// int javax.darjeeling.Darjeeling.getSolarVoltage()
void javax_darjeeling_PowerManager_int_getSolarVoltage()
{
	dj_exec_createAndThrow(BASE_CDEF_java_lang_VirtualMachineError);
}


// int javax.darjeeling.Darjeeling.getSolarCurrent()
void javax_darjeeling_PowerManager_int_getSolarCurrent()
{
	dj_exec_createAndThrow(BASE_CDEF_java_lang_VirtualMachineError);
}


//void javax.darjeeling.Darjeeling.slowDownClockByFactor(short)
void javax_darjeeling_PowerManager_void_slowDownClockByFactor_int()
{
	uint32_t factor = dj_exec_stackPopInt();
	printf("dj_timer_slowDownClockByFactor((uint16_t)factor) = %d\n",factor);
}

//void javax.darjeeling.Darjeeling.setSystemClockMillis(int)
void javax_darjeeling_PowerManager_void_setSystemClockMillis_int()
{
	uint32_t millis = dj_exec_stackPopInt();
	printf("dj_timer_setSystemClockMillis(millis) = %d\n", millis);
}
