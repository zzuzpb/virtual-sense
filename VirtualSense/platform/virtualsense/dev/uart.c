/*
 * Copyright (c) 2006, Swedish Institute of Computer Science
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
 * @(#)$Id: uart1.c,v 1.18 2010/03/05 12:06:03 fros4943 Exp $
 */

/*
 * Machine dependent MSP430 UART0 code.
 */

#include "platform-conf.h"
#include <stdlib.h>
#include <msp430.h>
#include <legacymsp430.h>
#include "uart.h"
#include "contiki.h"


char halUsbReceiveBuffer[255];
unsigned char bufferSize=0;

void uartSendChar(char c)
{
   while (!(UCA0IFG & UCTXIFG));
   UCA0TXBUF = c;
}

int
putchar(int c)
{
  uartSendChar((char)c);
  return c;
}

/*char uartGetChar(void){
	//while (!(UCA0IFG & UCRXIFG));
	_BIS_SR(GIE | SCG0 | SCG1 | CPUOFF);
	return halUsbReceiveBuffer[--bufferSize];
}*/


void uartInit(unsigned char clock_speed)
{    
  P3SEL = 0x30;                             // P3.4,5 = USCI_A0 TXD/RXD
  UCA0CTL1 |= UCSWRST;
  UCA0CTL0 = UCMODE_0;			// UART
  UCA0CTL0 &= ~UC7BIT;          // 8bit
  UCA0CTL1 |= UCSSEL_2;			// SMCLK

  switch (clock_speed)
     {
     case SYSCLK_1MHZ:
    	 UCA0BR0 = 0x68;
    	 UCA0BR1 = 0x00;
       break;
     case SYSCLK_4MHZ: //OK with 9600
    	 UCA0BR0 = 0xA0;
    	 UCA0BR1 = 0x01;
       break;
     case SYSCLK_8MHZ:
    	 UCA0BR0 = 0x3E;
    	 UCA0BR1 = 0x03;
       break;
     case SYSCLK_12MHZ:
    	 UCA0BR0 = 0xB0;
    	 UCA0BR1 = 0x04;
       break;
     case SYSCLK_16MHZ: //OK with 57600
    	 UCA0BR0 = 0x15;
    	 UCA0BR1 = 0x01;
       break;
     case SYSCLK_20MHZ: //OK with 57600
    	 UCA0BR0 = 0x5B;
    	 UCA0BR1 = 0x01;
       break;
     case SYSCLK_25MHZ: //OK with 57600
    	 UCA0BR0 = 0xB2;
    	 UCA0BR1 = 0x01;
         break;
     }

  //UCA0BR0 = 16;                 // Baud Rate Control Register 0 => Tx error rate (16MHz) = 0.3  16Mhz & 57600 => 277
  UCA0BR1 = 1;
  //UCA0MCTL = 0xE;			// UCBRSx = 7, UCBRFx = 0, oversampling disabled. //LELE: ORIG
  UCA0MCTL |= UCBRS_1 + UCBRF_0;            // Modulation UCBRSx=1, UCBRFx=0
  UCA0CTL1 &= ~UCSWRST;  		// clear
  UCA0IE |= UCRXIE;
  
  _BIS_SR(GIE);                 // Enable Interrupts
  /* delay */
  long int cnt;
  for (cnt=1;cnt<=562500;++cnt);
  printf(" ---- UART Initialized ---- ");
}

void uartShutDown(void)
{
  UCA0IE &= ~UCRXIE;
  UCA0CTL1 = UCSWRST;                          //Reset
  P3SEL &= ~BIT6;
  P3DIR |= BIT6;
  P3OUT &= ~BIT6;
}

/************************************************************************/
/*interrupt(USCI_A0_VECTOR) USCI_A0_ISR (void)
{
	 switch(UCA0IV)
	  {
	  case 0:break;                             // Vector 0 - no interrupt
	  case 2:                                   // Vector 2 - RXIFG
	    //UCA0TXBUF = UCA0RXBUF;                  // TX -> RXed character
	    halUsbReceiveBuffer[bufferSize++] = UCA0RXBUF;
	    __bic_SR_register_on_exit(LPM3_bits);
	    break;
	  case 4:break;                             // Vector 4 - TXIFG
	  default: break;
	  }
	 printf(".");

}
*/
