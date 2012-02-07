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
#include "dev/pcf2123_spi.h"

static long pastTime = 0;
static long lastT = 0;
static uint8_t sec = 0;
static uint8_t min = 24;
static uint8_t hou = 14;
static uint8_t day = 27;
static uint8_t mon = 11;
static uint8_t yea = 11;


void dj_timer_init()
{
	clock_init();

#ifdef PLATFORM_HAS_RTC_PCF2123
	if(RTC_is_up())
		pastTime = (RTC_get_seconds()-sec)*1000+(RTC_get_minutes()-min)*60000+(RTC_get_hours()-hou)*1440000;
#endif
		//TODO: the same whit month and year (overflow?).

}
//TODO: aggiornare pastTime prelevando il tempo da RTC PCF2123.
// per evitare problemi di overflow sarà necessario fare una differenza
// dal punto di reset (data nascita di Giulia) però nel caso di settaggio
// della data a livello applicazione sarà necessario rinfrescare il punto di reset.
// bisogna anche aggiornare (in quel caso) tutti i time-stamp dei thread ?

long dj_timer_getTimeMillis()
{
	//LELE: modifiche al fine di evitare problemi di overflow 
	// nel valore del tempo attuale (comporta la non schedulazione 
	// dei thread della macchina virtuale). deriva dal fatto che
	// in contiki il clock_time e' definito come unsigned short
	// ora il tempo (ms) in darjeeling Ã¨ rappresentato da un long (32bit)
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
