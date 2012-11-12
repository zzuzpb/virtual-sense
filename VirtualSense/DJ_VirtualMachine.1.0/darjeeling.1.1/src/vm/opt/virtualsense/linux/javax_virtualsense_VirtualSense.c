/*
 *	javax_virtualsense_VirtualSense.c
 *
 *  Copyright (c) 2012 DiSBeF, University of Urbino.
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
 * Task (thread) native methods.
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

#include <locale.h>
#include <time.h>



// int javax.virtualsense.VirtualSense.getNodeId()
void javax_virtualsense_VirtualSense_short_getNodeId()
{
	 // push result on the stack
	 dj_exec_stackPushShort(0);
}


void javax_virtualsense_VirtualSense_void_printTime(){
	char s[100];
	int dim;
	time_t t      = time (NULL);
	struct tm *tp = localtime (&t);

	setlocale (LC_ALL, "");
	dim = strftime (s, 100, "TIME %H:%M del %d %B %Y.", tp);
	printf ("%d: %s", dim, s);
}
