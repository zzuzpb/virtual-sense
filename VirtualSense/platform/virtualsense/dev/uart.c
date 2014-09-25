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

static int (*uart_input_handler)(unsigned char c);
static volatile uint8_t transmitting;

/*---------------------------------------------------------------------------*/
uint8_t
uart_active(void)
{
  return (UCA0STAT & UCBUSY) | transmitting;
}
/*---------------------------------------------------------------------------*/
void
uart_set_input(int (*input)(unsigned char c))
{
  uart_input_handler = input;
}
/*---------------------------------------------------------------------------*/



//char halUsbReceiveBuffer[255];
unsigned char bufferSize=0;
#ifdef PLATFORM_HAS_UART
void uartSendChar(char c)
{
   while (!(UCA0IFG & UCTXIFG));
   UCA0TXBUF = c;
}
#endif

int
putchar(int c)
{
#ifdef PLATFORM_HAS_UART
  /*if(((char)c)=='\n')
	  uartSendChar('\0a'); //Under linux using minicom the '\n' char does not
  	  	  	  	  	  	  // insert a carriage return
  else*/
	  uartSendChar((char)c);
#endif

  return c;
}

void uartInit(unsigned char clock_speed)
{    
#ifdef PLATFORM_HAS_UART

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
  //UCA0BR1 = 1;
  //UCA0MCTL = 0xE;			// UCBRSx = 7, UCBRFx = 0, oversampling disabled. //LELE: ORIG
  //UCA0MCTL |= UCBRS_1 + UCBRF_0;            // Modulation UCBRSx=1, UCBRFx=0
  UCA0MCTL = UCBRS_3;             /* Modulation UCBRSx = 3 */
  //UCA0CTL1 &= ~UCSWRST;  		// clear

  /* XXX Clear pending interrupts before enable */
  	  UCA0IE &= ~UCRXIFG;
  	  UCA0IE &= ~UCTXIFG;

  	  UCA0CTL1 &= ~UCSWRST;                   /* Initialize USCI state machine **before** enabling interrupts */
  	  UCA0IE |= UCRXIE;                        /* Enable UCA0 RX interrupt */

  //UCA0IE |= UCRXIE;
  
  _BIS_SR(GIE);                 // Enable Interrupts
  /* delay */
#endif
  long int cnt;
  for (cnt=1;cnt<=562500;++cnt);
  //printf("---- UART Initialized ----\n");
}

void uartShutDown(void)
{
#ifdef PLATFORM_HAS_UART
  UCA0IE &= ~UCRXIE;
  UCA0CTL1 = UCSWRST;                          //Reset
  P3SEL &= ~BIT6;
  P3DIR |= BIT6;
  P3OUT &= ~BIT6;
#endif
}
interrupt(USCI_A0_VECTOR)
uart_rx_interrupt(void)
{
  uint8_t c;
  if(UCA0IV == 2) {
    if(UCA0STAT & UCRXERR) {
      c = UCA0RXBUF;   /* Clear error flags by forcing a dummy read. */
    } else {
      c = UCA0RXBUF;
      if(uart_input_handler != NULL) {
        if(uart_input_handler(c)) {
          LPM4_EXIT;
        }
      }
    }
  }
}

