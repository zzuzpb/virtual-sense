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

#include <stdint.h>
#include <string.h>
#include <stdio.h>

// generated at infusion time
#include "base_definitions.h"

#include "common/execution/execution.h"
#include "common/heap/heap.h"
#include "dev/adc.h"

//#include "common/djtimer.h"

// short javax.virtualsense.sensors.getNrADCs()
void javax_virtualsense_sensors_ADC_short_getNrADCs()
{
	dj_exec_stackPushShort(NUM_CHANNEL);
}



// short javax.virtualsense.sensors.ADC.read(short channel short ref);
void javax_virtualsense_sensors_ADC_short_read_short_short()
{
	uint16_t ref = dj_exec_stackPopShort();
	uint16_t channel = dj_exec_stackPopShort();
	//printf("channel: %d, ref: %d p ", channel, ref);
	uint16_t value = read_adc_channel(channel, ref);
	dj_exec_stackPushShort(value);
}

