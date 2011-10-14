/*
 *	javax_darjeeling_actuators_Leds.c
 *
 *	Copyright (c) 2008 CSIRO, Delft University of Technology.
 *
 *	This file is part of Darjeeling.
 *
 *	Darjeeling is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	Darjeeling is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with Darjeeling.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <stdio.h>

#include "common/array.h"
#include "common/execution/execution.h"
#include "base_definitions.h"


// void javax.darjeeling.actuators.Leds.setLed(int, boolean)
void javax_darjeeling_actuators_Leds_void_setLed_int_boolean()
{
	int16_t on = dj_exec_stackPopShort();
	int32_t nr = dj_exec_stackPopInt();

	if (nr==0) if (on) printf("leds_on(LEDS_GREEN)\n"); else printf("leds_off(LEDS_GREEN)\n");
	if (nr==1) if (on) printf("leds_on(LEDS_RED)\n"); else printf("leds_off(LEDS_RED)\n");
	if (nr==2) if (on) printf("leds_on(LEDS_YELLOW)\n"); else printf("leds_off(LEDS_YELLOW)\n");
	if (nr==3) if (on) printf("leds_on(LEDS_ORANGE)\n"); else printf("leds_off(LEDS_ORANGE)\n");
}
