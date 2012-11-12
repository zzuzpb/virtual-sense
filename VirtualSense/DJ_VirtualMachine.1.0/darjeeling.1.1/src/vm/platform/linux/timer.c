/*
 *	timer.c
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
#include <time.h>
#include <sys/times.h>
#include <sys/types.h>

#include "common/types.h"

static struct timeval start;


void dj_timer_init()
{
	//printf("INIT TIMER\n");
	gettimeofday(&start, NULL);
}

uint32_t dj_timer_getTimeMillis()
{

	// The clock() function does not run on linux fedora 14 on top of VM player.
	// It returns time with a 1000 milliseconds resolution.

	 struct timeval now;
	 uint32_t elapsed;
	 gettimeofday(&now, NULL);

	 elapsed = (now.tv_sec - start.tv_sec)*1000 + (now.tv_usec - start.tv_usec)/1000;
     //printf("Elapsed time: %lu milliseconds\n", elapsed);

	return elapsed;
}
