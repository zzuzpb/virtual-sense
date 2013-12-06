/*
 *	javax_virtualsense_actuators_Leds.c
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
 * Controls the leds on the board.
 *
 * @author Emanuele Lattanzi
 *
 */

#include <stdint.h>

#include "common/array.h"
#include "common/execution/execution.h"
#include "base_definitions.h"

#include "dev/leds.h"



// void javax.virtualsense.actuators.Leds.setLed(int, boolean)
void javax_virtualsense_actuators_Leds_void_setLed_int_boolean()
{
	int16_t on = dj_exec_stackPopShort();
	int32_t nr = dj_exec_stackPopInt();


	if (nr==0) if (on) leds_on(LEDS_YELLOW); else leds_off(LEDS_YELLOW);
	if (nr==1) if (on) leds_on(LEDS_GREEN); else leds_off(LEDS_GREEN);
	if (nr==2) if (on) leds_on(LEDS_ORANGE); else leds_off(LEDS_ORANGE);

}
