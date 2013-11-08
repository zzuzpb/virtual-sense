/*
 * Copyright (c) 2007, Swedish Institute of Computer Science.
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
 * $Id: radio-test.c,v 1.5 2009/03/12 21:58:21 adamdunkels Exp $
 *
 * -----------------------------------------------------------------
 *
 * Author  : Adam Dunkels, Joakim Eriksson, Niclas Finne
 * Created : 2006-03-07
 * Updated : $Date: 2009/03/12 21:58:21 $
 *           $Revision: 1.5 $
 *
 * Simple application to indicate connectivity between two nodes:
 *
 * - Red led indicates a packet sent via radio (one packet sent each second)
 * - Green led indicates that this node can hear the other node but not
 *   necessary vice versa (unidirectional communication).
 * - Blue led indicates that both nodes can communicate with each
 *   other (bidirectional communication)
 */

#include "contiki.h"
#include "dev/leds.h"

#include "net/rime.h"
#include "net/netstack.h"
#include "dev/radio.h"
#include "power-interface.h"


#include <stdio.h>
#include "dev/watchdog.h"

#define HEADER "RTST"
#define PACKET_SIZE 20
#define PORT 9345



PROCESS(pulse_test_process, "Pulse-test");
AUTOSTART_PROCESSES(&pulse_test_process);
static struct etimer pulse_timer;

static void
abc_recv(struct abc_conn *c)
{

}

static const struct abc_callbacks abc_call = {abc_recv};
static struct abc_conn abc;

/*---------------------------------------------------------------------*/
PROCESS_THREAD(pulse_test_process, ev, data)
{
  static uint8_t state =0;
  static uint16_t index = 0;
  PROCESS_BEGIN();
  P2DIR |= BIT4;
  P5DIR |= (BIT0 | BIT1 | BIT6 | BIT7);
  P5DS |= (BIT0 | BIT1 | BIT6 | BIT7);

  P2DIR &= ~(BIT7);
  P2REN |= BIT7;			                   // Disable P2.0 internal resistance
  P2IE  |= BIT7;                           // P2.0 and P2.2 interrupt enabled
  P2IES &= ~ BIT7;                           // P2.0 and P2.2 Lo/Hi edge

  P8OUT &= ~ BIT0;
  P8OUT &= ~ BIT1;
  P8OUT &= ~ BIT2;
  P8OUT &= ~ BIT3;
  P8OUT &= ~ BIT4;
  P8OUT &= ~ BIT5;
  P8OUT &= ~ BIT6;

  //watchdog_stop();

  // enable network

  lock_RF();
  lock_MAC();// lock mac layer to prevent duty cycle shutdown;
  abc_open(&abc, PORT, &abc_call);
  printf("abc_open\n");

  P8OUT |= BIT0;
  while(1) {
	  //etimer_set(&pulse_timer, CLOCK_SECOND);
	  P8OUT &= ~ BIT2;

	  // going sleep
	  __bis_SR_register(LPM3_bits+GIE);
	  P8OUT |= BIT2;

	  watchdog_periodic();
	  /* send packet */
	  	packetbuf_copyfrom(HEADER, sizeof(HEADER));
	  	/* send arbitrary data to fill the packet size */
	  	packetbuf_set_datalen(PACKET_SIZE);
	   	lock_RF();
	  	abc_send(&abc);


	  /* end send packet */


	  etimer_set(&pulse_timer, CLOCK_SECOND/20);
	  // wait event
	  PROCESS_WAIT_EVENT();
	  P8OUT &= ~ BIT2;

  }
  PROCESS_END();
}

interrupt(PORT2_VECTOR)
     irq_p2(void)
{
	/* interrupt service routine for button on P2.0 and external RTC (pcf2123) on P2.2*/


	P2IFG &= ~(BIT7);                          // P2.0 and P2.2 IFG cleared
	LPM4_EXIT;
}

/*---------------------------------------------------------------------*/
