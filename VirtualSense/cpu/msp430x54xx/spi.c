/*
 * Copyright (c) 2011, Swedish Institute of Computer Science
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
 */

//#include <msp430.h>
#include "contiki.h"

/*
 * This is SPI initialization code for the MSP430X architecture.
 *
 */

//unsigned char spi_busy = 0;

/*
 * Initialize SPI bus.
 */
void
spi_init(void)
{
  // Initialize ports for communication with SPI units.

  UCB1CTL1 |=  UCSWRST;                //reset usci
  UCB1CTL1 |=  UCSSEL_2;               //smclk while usci is reset
  UCB1CTL0 = ( UCMSB | UCMST | UCSYNC | UCCKPH);//UCCKPL); // MSB-first 8-bit, Master, Synchronous, 3 pin SPI master, no ste, watch-out for clock-phase UCCKPH

  UCB1BR1 = 0x00;
  UCB1BR0 = 0x0002;

//  UCB0MCTL = 0;                       // Dont need modulation control.

  P3SEL |= BV(MOSI); 					// Select Peripheral functionality
  P5SEL |= BV(SCK) | BV(MISO);

  P5DIR |= BV(SCK);						// Configure as outputs MOSI, SCK.
  P3DIR |= BV(MOSI);

  P5DIR &= ~BV(MISO);					// Configure MISO as input.

  //ME1   |= USPIE0;            // Module enable ME1 --> U0ME? xxx/bg

  // Clear pending interrupts before enable!!!
  UCB1IE &= ~UCRXIFG;
  UCB1IE &= ~UCTXIFG;
  UCB1CTL1 &= ~UCSWRST;         // Remove RESET before enabling interrupts

  //Enable UCB0 Interrupts
  //IE2 |= UCB0TXIE;              // Enable USCI_B0 TX Interrupts
  //IE2 |= UCB0RXIE;              // Enable USCI_B0 RX Interrupts
}
