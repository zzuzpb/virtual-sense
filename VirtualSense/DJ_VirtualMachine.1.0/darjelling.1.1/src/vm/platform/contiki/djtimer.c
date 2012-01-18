/*
 *	djtimer.c
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
#include "common/types.h"
#include "sys/clock.h"

static long pastTime = 0;
static long lastT = 0;

void dj_timer_init()
{
	clock_init();
}
//TODO: usare RTC_PCF2123 ?? sempre?

long dj_timer_getTimeMillis()
{
	//LELE: modifiche al fine di evitare problemi di overflow 
	// nel valore del tempo attuale (comporta la non schedulazione 
	// dei thread della macchina virtuale). deirva dal fatto che 
	// in contiki il clock_time e' definito come unsigned short
	// ora il tempo (ms) in darjeeling è rappresentato da un long (32bit)
	// dovrebbe andare in overflow dopo 49 giorni.
	// TODO: Si potrebbe utilizzare un long long ?
	long t = clock_time();
	t*=10;
	if(t < lastT)
		pastTime+=lastT;
	lastT = t;
	return pastTime+t;
}
/************************************************************************/
/* LELE.
 * moltiplica il timer interrupt period per factor
 *
 ************************************************************************/
void dj_timer_slowDownClockByFactor(uint16_t factor)
{
	clock_slow_down(factor);

}

void dj_timer_setSystemClockMillis(uint32_t millis)
{

	clock_slow_down((uint16_t)(millis/10));

}
