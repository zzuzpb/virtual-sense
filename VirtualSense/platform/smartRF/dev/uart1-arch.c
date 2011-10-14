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
 * @(#)$Id: uart1.c,v 1.11 2009/03/02 22:01:26 adamdunkels Exp $
 */

/*
 * Machine dependent MSP430 UART1 code.
 */

#include <stdlib.h>

#include <msp430.h>
#include <legacymsp430.h>
//#include <io.h>
//#include <signal.h>

#include "hal_types.h"
#include "hal_uart.h"

#include "sys/energest.h"
#include "dev/uart1.h"


#include "dev/leds.h"
#include "dev/watchdog.h"

#include "lib/ringbuf.h"

static int (*uart1_input_handler)(unsigned char c);
static uint8_t rx_in_progress;

static volatile uint8_t transmitting;

#define TX_WITH_INTERRUPT 0

#if TX_WITH_INTERRUPT
#define TXBUFSIZE 64

static struct ringbuf txbuf;
static uint8_t txbuf_data[TXBUFSIZE];
#endif /* TX_WITH_INTERRUPT */

/***********************************************************************************
* @fn      halUartRxIntEnable
*
* @brief   Enable UART RX interrupt
*
* @param   none
*
* @return  none
*/
static void halUartRxIntEnable(void)
{
    IE2 |= UCA0RXIE;
}


/***********************************************************************************
* @fn      halUartEnableRxFlow
*
* @brief   Signal ready/not ready to receive characters on UART
*
* @param   uint8 enable - TRUE to signal ready to receive on UART
*                         FALSE to signal not ready to receive on UART
*
* @return  none
*/
void halUartEnableRxFlow(uint8 enable)
{
    // Enable RX flow
    if(enable) {
        //HAL_RTS_CLR();
    }
    // Disable RX flow
    else {
        //HAL_RTS_SET();
    }
}




/*---------------------------------------------------------------------------*/
uint8_t
uart1_active(void)
{
  return rx_in_progress | transmitting; // 0;//((~ UTCTL1) & TXEPT) | rx_in_progress | transmitting;
}
/*---------------------------------------------------------------------------*/
void
uart1_set_input(int (*input)(unsigned char c))
{
  //uart1_input_handler = input;
}
/*---------------------------------------------------------------------------*/
void
uart1_writeb(unsigned char c)
{
  watchdog_periodic();
#if TX_WITH_INTERRUPT

  /* Put the outgoing byte on the transmission buffer. If the buffer
     is full, we just keep on trying to put the byte into the buffer
     until it is possible to put it there. */
  while(ringbuf_put(&txbuf, c) == 0);

  /* If there is no transmission going, we need to start it by putting
     the first byte into the UART. */
  if(transmitting == 0) {
    transmitting = 1;
    UCA0TXBUF = ringbuf_get(&txbuf);
  }

#else /* TX_WITH_INTERRUPT */

 /* Loop until the transmission buffer is available. */
  while(!(IFG2 & UCA0TXIFG));    // Wait for TX buffer ready to receive new byte
        UCA0TXBUF = c; //c;            // Output character
#endif /* TX_WITH_INTERRUPT */
}
/*---------------------------------------------------------------------------*/
#if ! WITH_UIP /* If WITH_UIP is defined, putchar() is defined by the SLIP driver */
int
putchar(int c)
{
  uart1_writeb((char)c);
  if(((char)c)=='\n')
	uart1_writeb('\r');
  return c;
}
#endif /* ! WITH_UIP */

/***********************************************************************************
* @fn      halUartInit
*
* @brief   Initalise UART. Supported baudrates are: 19220, 38400, 57600 and 115200
*
* @param   uint8 baudrate
*          uint8 options - this parameter is ignored
*
* @return  none
*/
void halUartInit(uint8 baudrate, uint8 options)
{
    // For the moment, this UART implementation only
    // supports communication 8N1
    // i.e. ignore options arguments.

    UCA0CTL1 |= UCSWRST;                    // Keep USART1 in reset state

    UCA0CTL1 |= UCSSEL1;                    // Set clock source SMCLK
    UCA0CTL1 &= ~UCSSEL0;

    P3SEL |= BIT4;                          // P3.4 = USART1 TXD
    P3SEL |= BIT5;                          // P3.5 = USART1 RXD

    switch (baudrate) {
    case HAL_UART_BAUDRATE_9600:
        UCA0BR0 = 0x41;                     // 8MHz 9600
        UCA0BR1 = 0x03;                     // 8MHz 9600
        break;

    case HAL_UART_BAUDRATE_19200:
        UCA0BR0 = 0xA0;                     // 8MHz 19200
        UCA0BR1 = 0x01;                     // 8MHz 19200
        break;

    case HAL_UART_BAUDRATE_38400:
        UCA0BR0 = 0xD0;                     // 8MHz 38400
        UCA0BR1 = 0x00;                     // 8MHz 38400
        break;

    case HAL_UART_BAUDRATE_57600:
        UCA0BR0 = 0x8A;                     // 8MHz 57600
        UCA0BR1 = 0x00;                     // 8MHz 57600
        break;

    case HAL_UART_BAUDRATE_115200:
        UCA0BR0 = 0x45;                     // 8MHz 115200
        UCA0BR1 = 0x00;                     // 8MHz 115200
        break;

    default:
        break;
    }

    UCA0CTL0 &= ~UCPEN;                     // No parity
    UCA0CTL0 &= ~UCSPB;                     // 1 stop bit
    UCA0CTL0 &= ~UC7BIT;                    // 8 data bits

    UCA0CTL1 &= ~UCSWRST;                   // Initialize USART1 state machine

    //bufInit(&rbRxBuf);

    // Enable RX interrupt
    halUartRxIntEnable();

    // Set RTS pin to output
    //HAL_RTS_DIR_OUT();

    // Enable RX Flow
    halUartEnableRxFlow(TRUE);
    
    rx_in_progress = 0;

    transmitting = 0;
  

}

#if 0
/*---------------------------------------------------------------------------*/
/**
 * Initalize the RS232 port.
 *
 */
void
uart1_init(unsigned long ubr)
{
  /* RS232 */
  P3DIR &= ~0x80;			/* Select P37 for input (UART1RX) */
  P3DIR |= 0x40;			/* Select P36 for output (UART1TX) */
  P3SEL |= 0xC0;			/* Select P36,P37 for UART1{TX,RX} */

  UCTL1 = SWRST | CHAR;                 /* 8-bit character, UART mode */

#if 0
   U1RCTL &= ~URXEIE; /* even erroneous characters trigger interrupts */
#endif

  UTCTL1 = SSEL1;                       /* UCLK = MCLK */

  UBR01 = ubr;
  UBR11 = ubr >> 8;
  /*
   * UMCTL1 values calculated using
   * http://mspgcc.sourceforge.net/baudrate.html
   * Table assumes that F_CPU = 2,457,600 Hz.
   */
  switch(ubr) {
  case UART1_BAUD2UBR(115200ul):
    UMCTL1 = 0x4a;
    break;
  case UART1_BAUD2UBR(57600ul):
    UMCTL1 = 0x5b;
    break;
  default:
    /* 9600, 19200, 38400 don't require any correction */
    UMCTL1 = 0x00;
  }

  ME2 &= ~USPIE1;			/* USART1 SPI module disable */
  ME2 |= (UTXE1 | URXE1);               /* Enable USART1 TXD/RXD */

  UCTL1 &= ~SWRST;

  /* XXX Clear pending interrupts before enable!!! */
  IFG2 &= ~URXIFG1;
  U1TCTL |= URXSE;

  rx_in_progress = 0;

  transmitting = 0;
  
  IE2 |= URXIE1;                        /* Enable USART1 RX interrupt  */
#if TX_WITH_INTERRUPT
  IE2 |= UTXIE1;                        /* Enable USART1 RX interrupt  */
  ringbuf_init(&txbuf, txbuf_data, sizeof(txbuf_data));
#endif /* TX_WITH_INTERRUPT */
}

#endif
/*---------------------------------------------------------------------------*/
/*interrupt(USCIAB0RX)
uart1_rx_interrupt(void)
{
  ENERGEST_ON(ENERGEST_TYPE_IRQ);
   if(uart1_input_handler != NULL) {
	if(uart1_input_handler(UCA0RXBUF)) {
	  LPM4_EXIT;
	}
   }
  ENERGEST_OFF(ENERGEST_TYPE_IRQ);

}*/
#if 0
  if(!(URXIFG1 & IFG2)) {
    /* Edge detect if IFG not set? */
    U1TCTL &= ~URXSE; /* Clear the URXS signal */
    U1TCTL |= URXSE;  /* Re-enable URXS - needed here?*/
    rx_in_progress = 1;
    LPM4_EXIT;
  } else {
    rx_in_progress = 0;
    /* Check status register for receive errors. */
    if(URCTL1 & RXERR) {
      volatile unsigned dummy;
      dummy = RXBUF1;   /* Clear error flags by forcing a dummy read. */
    } else {
      if(uart1_input_handler != NULL) {
	if(uart1_input_handler(UCA0RXBUF)) {
	  LPM4_EXIT;
	}
      }
    }
  }
  ENERGEST_OFF(ENERGEST_TYPE_IRQ);
}
#endif
/*---------------------------------------------------------------------------*/
#if TX_WITH_INTERRUPT
interrupt(USCIAB0TX)
uart1_tx_interrupt(void)
{
  ENERGEST_ON(ENERGEST_TYPE_IRQ);

  if(ringbuf_elements(&txbuf) == 0) {
    transmitting = 0;
  } else {
    UCA0TXBUF = ringbuf_get(&txbuf);
  }
  
  ENERGEST_OFF(ENERGEST_TYPE_IRQ);
}
#endif /* TX_WITH_INTERRUPT */
/*---------------------------------------------------------------------------*/
