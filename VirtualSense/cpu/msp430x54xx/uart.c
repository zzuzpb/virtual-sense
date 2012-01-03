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

#include <stdlib.h>
#include <msp430.h>
#include <legacymsp430.h>
#include "uart.h"
#include "contiki.h"

char halUsbReceiveBuffer[255];
unsigned char bufferSize=0;

/*void uartSend(char string[])
{
  int i;
  for (i=0; string[i]!='\0'; i++)
  {
   while (!(UCA0IFG & UCTXIFG));
   UCA0TXBUF = string[i];
  }
} */

void uartSendChar(char c)
{
   while (!(UCA0IFG & UCTXIFG));
   UCA0TXBUF = c;
}

int
putchar(int c)
{
  uartSendChar((char)c);
  /*if(((char)c)=='\n')
	uart1_writeb('\r');*/
  return c;
}


void uartInit(void)
{    

 /*P3SEL = 0x30;                             // P3.4,5 = USCI_A0 TXD/RXD
 UCA0CTL1 |= UCSWRST;                      // **Put state machine in reset**
 UCA0CTL0 = UCMODE_0;					   // UART
 UCA0CTL0 &= ~UC7BIT;          			   // 8bit
 UCA0CTL1 |= UCSSEL_2;                     // SMCLK
 UCA0BR0 = 16;                             // 1MHz 115200 (see User's Guide)
 UCA0BR1 = 1;                              // 1MHz 115200
 UCA0MCTL |= UCBRS_1 + UCBRF_0;            // Modulation UCBRSx=1, UCBRFx=0
 UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
 UCA0IE |= UCRXIE; */                        // Enable USCI_A0 RX interrupt

/*  P3SEL |= BIT7 + BIT6;
  P3DIR |= BIT6;
  P3DIR &= ~BIT7;
 */
  P3SEL = 0x30;                             // P3.4,5 = USCI_A0 TXD/RXD
  UCA0CTL1 |= UCSWRST;
  UCA0CTL0 = UCMODE_0;			// UART
  UCA0CTL0 &= ~UC7BIT;          // 8bit
  UCA0CTL1 |= UCSSEL_2;			// SMCLK
  UCA0BR0 = 16;                 // Baud Rate Control Register 0 => Tx error rate (16MHz) = 0.3  16Mhz & 57600 => 277
  UCA0BR1 = 1;
  UCA0MCTL = 0xE;			// UCBRSx = 7, UCBRFx = 0, oversampling disabled.
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


int charReceived;
#if 0
void usbTest( void )
{
  unsigned char quit;  
  volatile unsigned char i;

  /*lcdClearScreen();
  lcdPrintLine("baudrate = 57600", 0, 0);
  lcdPrintLine("Type on PC      ", 1, 0);
  lcdPrintLine("Type Q for quit ", 2, 0); */
  uartSend (" ---- TYPE ON HERE ---- ");

  quit = 0; 
  charReceived = 0;
  
  while (quit == 0)
  {
    //__bis_SR_register(LPM0_bits + GIE);     
    while (!charReceived) { clock_delay(10); }
    charReceived = 0;

    if (bufferSize > 0)
    {  
      for (i = 0; i < bufferSize; i++)
        if (halUsbReceiveBuffer[i] == 'Q' || halUsbReceiveBuffer[i] == 'q') 
          quit = 1;    
      //lcdPrint(halUsbReceiveBuffer,  OVERWRITE_TEXT);
      //lcdCursor();
      for (i = 0; i < bufferSize; i++)
        halUsbReceiveBuffer[i] = '\0';
      bufferSize  = 0;
    }
  }
  //lcdCursorOff();
}
#endif

/************************************************************************/
interrupt(USCI_A0_VECTOR) USCI_A0_ISR (void)
{
  halUsbReceiveBuffer[bufferSize++] = UCA0RXBUF;
  charReceived = 1;
   //__bic_SR_register_on_exit(LPM3_bits);
}

