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
 * Temperature access methods.
 *
 * @author Matteo Dromedari
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// generated at infusion time
#include "base_definitions.h"

#include "common/execution/execution.h"
#include "common/heap/heap.h"
#include "dev/adc.h"
#include "dev/barometer_MPL115A2.h"



//public static native int getValue();
void javax_virtualsense_sensors_Temperature_short_getValue()
{
	dj_exec_stackPushShort(read_temperature_barometer_MPL115A2());
}




//public static native int getBoardValue();
void javax_virtualsense_sensors_Temperature_int_getBoardValue()
{
	// Temperature in Celsius
    	// ((A10/4096*1500mV) - 894mV)*(1/3.66mV) = (A10/4096*410) - 244
    	// = (A10 - 2438) * (410 / 4096)
	dj_exec_stackPushInt((int)((read_adc_channel(BOARD_TEMP_CHANNEL, REF_1_5V) - (uint16_t)894) / (uint16_t)4));
}



