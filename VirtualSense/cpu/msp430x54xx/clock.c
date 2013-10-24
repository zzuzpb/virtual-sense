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

#define INTERVAL (RTIMER_ARCH_SECOND / CLOCK_SECOND)

#define MAX_TICKS (~((clock_time_t)0) / 2)

static volatile unsigned long seconds;

static volatile clock_time_t count = 0;
static volatile uint16_t clock_divider = 1;
static volatile uint8_t was_standby = 0;

/* last_tar is used for calculating clock_fine */
static volatile uint16_t last_tar = 0;
/*---------------------------------------------------------------------------*/
interrupt(TIMER0_A0_VECTOR) timera0 (void) {
  ENERGEST_ON(ENERGEST_TYPE_IRQ);
  watchdog_start();



    /* HW timer bug fix: Interrupt handler called before TR==CCR.
     * Occurrs when timer state is toggled between STOP and CONT. */
    while(TA0CTL & MC_1 && TA0CCR0 - TA0R == 1);

    /* Make sure interrupt time is future */
    do {
    	TA0CCR0 += clock_divider * INTERVAL; //LELE clock divider
        count +=clock_divider;

      /* Make sure the CLOCK_CONF_SECOND is a power of two, to ensure
	 that the modulo operation below becomes a logical and and not
	 an expensive divide. Algorithm from Wikipedia:
	 http://en.wikipedia.org/wiki/Power_of_two */
#if (CLOCK_CONF_SECOND & (CLOCK_CONF_SECOND - 1)) != 0
#error CLOCK_CONF_SECOND must be a power of two (i.e., 1, 2, 4, 8, 16, 32, 64, ...).
#error Change CLOCK_CONF_SECOND in contiki-conf.h.
#endif
      seconds = count/CLOCK_CONF_SECOND;
      /*if(count % CLOCK_CONF_SECOND == 0) { //LELE: to avoid under estimation
	++seconds;
        energest_flush();
      }*/
    } while((TA0CCR0 - TA0R) > (clock_divider * INTERVAL)); //LELE: clock_divider

    last_tar = TA0R;


    if(was_standby){ //LELE: to manage the standby invocation on PowerManager
        	was_standby = 0;
        	LPM4_EXIT;
    }else if(etimer_pending() &&
       //(etimer_next_expiration_time() - count - 1) > MAX_TICKS) {
    	((count + clock_divider) > etimer_next_expiration_time())) {
      etimer_request_poll();

    }


   if(process_nevents() >= 0) {
    LPM4_EXIT;
    }

  watchdog_stop();

  ENERGEST_OFF(ENERGEST_TYPE_IRQ);
}
/*---------------------------------------------------------------------------*/
clock_time_t
clock_time(void)
{
  clock_time_t t1, t2;
  do {
    t1 = count;
    t2 = count;
  } while(t1 != t2);
  return t1;
}
/*---------------------------------------------------------------------------*/
void
clock_set(clock_time_t clock, clock_time_t fclock)
{
  TA0R = fclock;
  TA0CCR0 = fclock + clock_divider * INTERVAL;
  count = clock;
}
/*---------------------------------------------------------------------------*/
int
clock_fine_max(void)
{
  return clock_divider * INTERVAL;
}
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
uint16_t
get_clock_divider(void)
{
  return clock_divider;
}
/*---------------------------------------------------------------------------*/
unsigned short
clock_fine(void)
{
  unsigned short t;
  /* Assign last_tar to local varible that can not be changed by interrupt */
  t = last_tar;
  /* perform calc based on t, TA0R will not be changed during interrupt */
  return (unsigned short) (TA0R - t);
}
/*---------------------------------------------------------------------------*/
void
clock_init(void)
{
  dint();

  TA0CTL = TASSEL_1 | TACLR | ID_1;
  TA0CCTL0 = OUTMOD_4 | CCIE;

  /* Interrupt after X ms. */
  TA0CCR0 = clock_divider * INTERVAL;

   /* Start Timer_A in continuous mode. */
   TA0CTL |= MC_2;

   count = 0;

   /* Enable interrupts. */
   eint();

}

/*---------------------------------------------------------------------------*/
void
clock_slow_down(uint16_t factor)
{
  dint();
  if(factor >= 1)
	  clock_divider = factor;

  TA0CTL = TASSEL_1 | TACLR | ID_1;
  TA0CCTL0 = OUTMOD_4 | CCIE;

  /* Interrupt after X ms. */
  TA0CCR0 = clock_divider * INTERVAL;
  /* Start Timer_A in continuous mode. */
  TA0CTL |= MC_2;

  /*Enable interrupts. */
  eint();

}

/*---------------------------------------------------------------------------*/
void
standby(void)
{
 was_standby = 1;
}
/*---------------------------------------------------------------------------*/
uint8_t
get_standby(void)
{
 return was_standby;
}

/*---------------------------------------------------------------------------*/
/**
 * Delay the CPU for a multiple of 2.83 us.
 */
void
clock_delay(unsigned int i)
{
  asm("add #-1, r15");
  asm("jnz $-2");
  /*
   * This means that delay(i) will delay the CPU for CONST + 3x
   * cycles. On a 2.4756 CPU, this means that each i adds 1.22us of
   * delay.
   *
   * do {
   *   --i;
   * } while(i > 0);
   */
}
/*---------------------------------------------------------------------------*/
/**
 * Wait for a multiple of 10 ms.
 *
 */
void
clock_wait(int i)
{
  clock_time_t start;

  start = clock_time();
  while(clock_time() - start < (clock_time_t)i);
}
/*---------------------------------------------------------------------------*/
void
clock_set_seconds(unsigned long sec)
{

}
/*---------------------------------------------------------------------------*/
unsigned long
clock_seconds(void)
{
  unsigned long t1, t2;
  do {
    t1 = seconds;
    t2 = seconds;
  } while(t1 != t2);
  return t1;
}
/*---------------------------------------------------------------------------*/
rtimer_clock_t
clock_counter(void)
{
  return TA0R;
}
/*---------------------------------------------------------------------------*/
