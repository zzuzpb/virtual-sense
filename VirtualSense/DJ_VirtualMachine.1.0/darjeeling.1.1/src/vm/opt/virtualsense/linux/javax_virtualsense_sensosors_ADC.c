/*
 *	javax_virtualsense_sensosors_ADC.c
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
 * ADC access methods.
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
#define NUM_CHANNEL 4

//#include "common/djtimer.h"

// short javax.virtualsense.sensors.getNrADCs()
void javax_virtualsense_sensors_ADC_short_getNrADCs()
{
	dj_exec_stackPushShort(NUM_CHANNEL);
}



// short javax.virtualsense.sensors.ADC.read(short channel);
void javax_virtualsense_sensors_ADC_short_read_short()
{
	uint16_t channel = dj_exec_stackPopShort();
	uint16_t value = (uint16_t)rand()*1000;
	dj_exec_stackPushShort(value);
}
/*
// int javax.virtualsense.sensors.Temperature.getBoardTemperature()
void javax_darjeeling_sensors_Temperature_int_getBoardTemperature()
{
        dj_exec_createAndThrow(BASE_CDEF_java_lang_VirtualMachineError);
}
*/
