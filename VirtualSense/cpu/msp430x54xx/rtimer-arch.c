/*
 * Copyright (c) 2005, Swedish Institute of Computer Science
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file is part of the Contiki operating system.
 *
 * @(#)$Id: clock.c,v 1.26 2010/12/16 22:50:21 adamdunkels Exp $
 */


#include "contiki.h"
#include <msp430.h>
#include <legacymsp430.h>

#include "sys/energest.h"
#include "sys/clock.h"
#include "sys/etimer.h"
#include "rtimer-arch.h"
#include "watchdog.h"

#define MAX_COUNTER (~((rtimer_clock_t)0) / 2)

static volatile unsigned long seconds;

static volatile rtimer_clock_t count = 0;
static volatile rtimer_clock_t next_period = 0;


/* last_tar is used for calculating clock_fine */
static volatile uint16_t last_tar = 0;
/*---------------------------------------------------------------------------*/
interrupt(TIMER0_B0_VECTOR) timerb0 (void) {


	//printf("%u\n", TBIV);
	watchdog_start();
    rtimer_run_next();   // calls rtimer_arch_schedule(rtimer_clock_t t)
     if(process_nevents() > 0) {
       LPM4_EXIT;
     }
     watchdog_stop();

}


/*---------------------------------------------------------------------------*/
rtimer_clock_t
rtimer_arch_now(void)
{
	 unsigned short t1, t2;
	  do {
	    t1 = TB0R;
	    t2 = TB0R;
	  } while(t1 != t2);
	  return t1;
}


void
rtimer_arch_init(void)
{
  dint();

  TB0CTL = TBSSEL_1 | TBCLR | ID_1 ;
  TB0CCTL0 = OUTMOD_4 | CCIE;

  /* Interrupt after X ms. */
   /* Start Timer_A in continuous mode. */
   TA0CTL |= MC_2;

   count = 0;

   /* Enable interrupts. */
   eint();

}

/*---------------------------------------------------------------------------*/
void
rtimer_arch_schedule(rtimer_clock_t t)
{
	TB0CTL &= ~MC_2;
	/*if(t > TB0CCR0){
		TB0CCR0 = t;
	}
	else{

		TB0CCR0 = MAX_COUNTER - TB0CCR0 + t;

	}*/

	TB0CCR0 = t;
	TB0CTL |= MC_2;
}
/*---------------------------------------------------------------------------*/
