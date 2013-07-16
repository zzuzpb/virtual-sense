/*
 *	javax_virtualsense_sensosors_Light.c
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
 * Light access methods.
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


//public static native int getBoardValue();
void javax_virtualsense_sensors_Light_int_getValue()
{
	uint32_t p2d, p5d, p5o = 0x0000;
	
	// Get state of port 2 and 5
	p2d = P2DIR;
	p5d = P5DIR;
	p5o = P5OUT;

	// Set medium gain
	P5DIR  &= ~(BIT6 + BIT5);
	P5OUT  &= ~BIT6;
	P5OUT  |= BIT5;

	// Enable light sensor
	P2DIR  &= ~BIT6;
	P2OUT  |= BIT6;
	uint32_t r = read_adc_channel(LIGHT_CHANNEL, REF_2_5V);	 

	//printf("red: %d -- ",r);
	// Read ADC light channel and convert mV in lx (Lux = lm * m^2)
	dj_exec_stackPushInt((r * 10000) / 1254);

	// Disable light sensor
	P2OUT &= ~BIT6;
	
	// Restore state of port 2 and 5
	P2DIR = p2d;
	P5DIR = p5d;
	P5OUT = p5o;
}



