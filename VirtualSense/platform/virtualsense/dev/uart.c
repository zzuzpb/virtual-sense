/*
 *  uart.c
 *
 *  Copyright (c) 2011 DiSBeF, University of Urbino.
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
 * UART code for MSP43054xx.
 *
 * @author Emanuele Lattanzi
 *
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
  if(((char)c)=='\n')
	  uartSendChar('\r'); //Under linux using minicom the '\n' char does not
  	  	  	  	  	  	  // insert a carriage return
  else
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
