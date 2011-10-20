/*
 *	javax_virtualsense_sensosors_Temperature.c
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
 * Reads temeparature from the sensor.
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
//#include "common/heap/heap.h"
//#include "common/djtimer.h"

// int javax.virtualsense.Darjeeling.getTemperature()
void javax_virtualsense_sensors_Temperature_int_getTemperature()
{
	dj_exec_createAndThrow(BASE_CDEF_java_lang_VirtualMachineError);
}

// short javax.virtualsense.sensors.Temperature.getTemperature()
void javax_virtualsense_sensors_Temperature_short_getTemperature()
{
        // push result on the stack
        dj_exec_stackPushShort(0);
}
// int javax.virtualsense.sensors.Temperature.getBoardTemperature()
void javax_darjeeling_sensors_Temperature_int_getBoardTemperature()
{
        dj_exec_createAndThrow(BASE_CDEF_java_lang_VirtualMachineError);
}
