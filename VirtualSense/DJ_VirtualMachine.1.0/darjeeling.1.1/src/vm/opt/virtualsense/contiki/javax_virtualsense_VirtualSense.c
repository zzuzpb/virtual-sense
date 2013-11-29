/*
 *	javax_lang_Darjeeling.c
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
#include <stdint.h>
#include <string.h>
#include <stdio.h>

// generated at infusion time
#include "base_definitions.h"

#ifdef PLATFORM_HAS_RTC_PCF2123
#include "dev/pcf2123_spi.h"
#endif
#include "common/execution/execution.h"
#include "common/heap/heap.h"
#include "common/djtimer.h"
//#include "node-id.h"



// int javax.virtualsense.VirtualSense.getNodeId()
void javax_virtualsense_VirtualSense_short_getNodeId()
{
	 // push result on the stack
	 dj_exec_stackPushShort(1/*node_id */); //TODO to implement
}


void javax_virtualsense_VirtualSense_void_printTime(){
#ifdef PLATFORM_HAS_RTC_PCF2123
		  printf(" TIME %u:%u:%u", RTC_get_hours(),RTC_get_minutes(),RTC_get_seconds()) ;
#endif

}
