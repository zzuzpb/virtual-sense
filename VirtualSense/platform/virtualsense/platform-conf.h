/*
 * Copyright (c) 2010, Swedish Institute of Computer Science.
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
 * $Id: platform-conf.h,v 1.2 2010/12/16 22:52:27 adamdunkels Exp $
 */

/**
 * \file
 *         A brief description of what this file is
 * \author
 *         Niclas Finne <nfi@sics.se>
 *         Joakim Eriksson <joakime@sics.se>
 */

#ifndef __PLATFORM_CONF_H__
#define __PLATFORM_CONF_H__

#include <msp430f5437a.h>
/*
 * Definitions below are dictated by the hardware and not really
 * changeable!
 */
/* Platform TMOTE_SKY */
#define VIRTUAL_SENSE 1

#ifndef BV
#define BV(x) (1<<(x))
#endif

#define PLATFORM_HAS_LEDS
#define PLATFORM_HAS_BUTTON
#define PLATFORM_HAS_UART
#define PLATFORM_HAS_RTC_PCF2123
#define PLATFORM_HAS_DS2411
#define PLATFORM_HAS_EEPROM

#ifdef PLATFORM_HAS_EEPROM
#define BASE_EEPROM 	0
#else
#define BASE_EEPROM		0x10000 // second MCU flash block
#endif



//#define PRINTF printf
#ifndef PLATFORM_HAS_UART
#define printf(...)
#endif
/* CPU target speed in Hz */
#define F_CPU 3900000uL /*2457600uL*/

/* Our clock resolution, this is the same as Unix HZ. */
#define CLOCK_CONF_SECOND 128UL

#define BAUD2UBR(baud) ((F_CPU/baud))

#define CCIF
#define CLIF

#define CC_CONF_INLINE inline

#define HAVE_STDINT_H
#define MSP430_MEMCPY_WORKAROUND 1
#include "msp430def.h"


/* Types for clocks and uip_stats */
typedef unsigned short uip_stats_t;
typedef unsigned long clock_time_t;
typedef unsigned long off_t;

/* the low-level radio driver */
#define NETSTACK_CONF_RADIO   cc2420_driver

/* LED ports */
#define LEDS_PxDIR 		P8DIR
#define LEDS_PxOUT 		P8OUT
#define LEDS_CONF_1  	0x40
#define LEDS_CONF_2    	0x20
#define LEDS_CONF_3 	0x10
#define LEDS_CONF_4   	0x08
#define LEDS_CONF_5   	0x04
#define LEDS_CONF_6   	0x02
#define LEDS_CONF_7   	0x01

/* DCO speed resynchronization for more robust UART, etc. */
#ifndef DCOSYNCH_CONF_ENABLED
#define DCOSYNCH_CONF_ENABLED 1
#endif /* DCOSYNCH_CONF_ENABLED */
#ifndef DCOSYNCH_CONF_PERIOD
#define DCOSYNCH_CONF_PERIOD 30
#endif /* DCOSYNCH_CONF_PERIOD */

/* MSP430F54xx family symbols */
#define SYSCLK_1MHZ             1
#define SYSCLK_4MHZ             4
#define SYSCLK_8MHZ             8
#define SYSCLK_12MHZ            12
#define SYSCLK_16MHZ            16
#define SYSCLK_20MHZ            20
#define SYSCLK_25MHZ            25

#define DCO_MULT_1MHZ           30
#define DCO_MULT_4MHZ           122
#define DCO_MULT_8MHZ           244
#define DCO_MULT_12MHZ          366
#define DCO_MULT_16MHZ          488
#define DCO_MULT_20MHZ          610
#define DCO_MULT_25MHZ          763

#define DCORSEL_1MHZ            DCORSEL_2
#define DCORSEL_4MHZ            DCORSEL_3
#define DCORSEL_8MHZ            DCORSEL_4
#define DCORSEL_12MHZ           DCORSEL_5
#define DCORSEL_16MHZ           DCORSEL_5
#define DCORSEL_20MHZ           DCORSEL_6
#define DCORSEL_25MHZ           DCORSEL_6

#define VCORE_1MHZ              PMMCOREV_0
#define VCORE_4MHZ              PMMCOREV_0
#define VCORE_8MHZ              PMMCOREV_0
#define VCORE_12MHZ             PMMCOREV_0
#define VCORE_16MHZ             PMMCOREV_1
#define VCORE_20MHZ             PMMCOREV_2
#define VCORE_25MHZ             PMMCOREV_3

/*#define VCORE_1MHZ              PMMCOREV_2
#define VCORE_4MHZ              PMMCOREV_2
#define VCORE_8MHZ              PMMCOREV_2
#define VCORE_12MHZ             PMMCOREV_2
#define VCORE_16MHZ             PMMCOREV_2
#define VCORE_20MHZ             PMMCOREV_1
#define VCORE_25MHZ             PMMCOREV_0
#define VCORE_1_75V             PMMCOREV_1*/
/* end MSP430F54xx family symbols */

#define ROM_ERASE_UNIT_SIZE  512
#define XMEM_ERASE_UNIT_SIZE (64*1024L)


#define CFS_CONF_OFFSET_TYPE    long


/* Use the first 64k of external flash for node configuration */
#define NODE_ID_XMEM_OFFSET     (0 * XMEM_ERASE_UNIT_SIZE)

/* Use the second 64k of external flash for codeprop. */
#define EEPROMFS_ADDR_CODEPROP  (1 * XMEM_ERASE_UNIT_SIZE)

#define CFS_XMEM_CONF_OFFSET    (2 * XMEM_ERASE_UNIT_SIZE)
#define CFS_XMEM_CONF_SIZE      (1 * XMEM_ERASE_UNIT_SIZE)

#define CFS_RAM_CONF_SIZE 4096

/*
 * SPI bus configuration for the TMote Sky.
 */

/* SPI input/output registers. */
#define SPI_TXBUF U0TXBUF
#define SPI_RXBUF U0RXBUF

                                /* USART0 Tx ready? */
#define SPI_WAITFOREOTx() while ((U0TCTL & TXEPT) == 0)
                                /* USART0 Rx ready? */
#define SPI_WAITFOREORx() while ((IFG1 & URXIFG0) == 0)
                                /* USART0 Tx buffer ready? */
#define SPI_WAITFORTxREADY() while ((IFG1 & UTXIFG0) == 0)

#define SCK            1  /* P3.1 - Output: SPI Serial Clock (SCLK) */
#define MOSI           2  /* P3.2 - Output: SPI Master out - slave in (MOSI) */
#define MISO           3  /* P3.3 - Input:  SPI Master in - slave out (MISO) */

/*
 * SPI bus - M25P80 external flash configuration.
 */

#define FLASH_PWR       3       /* P4.3 Output */
#define FLASH_CS        4       /* P4.4 Output */
#define FLASH_HOLD      7       /* P4.7 Output */

/* Enable/disable flash access to the SPI bus (active low). */

#define SPI_FLASH_ENABLE()  ( P6OUT &= ~BV(FLASH_CS) )
#define SPI_FLASH_DISABLE() ( P6OUT |=  BV(FLASH_CS) )

#define SPI_FLASH_HOLD()                ( P6OUT &= ~BV(FLASH_HOLD) )
#define SPI_FLASH_UNHOLD()              ( P6OUT |=  BV(FLASH_HOLD) )

/*
 * SPI bus - CC2420 pin configuration.
 */

#define CC2420_CONF_SYMBOL_LOOP_COUNT 800

/* P1.0 - Input: FIFOP from CC2420 */
#define CC2420_FIFOP_PORT(type)   P1##type
#define CC2420_FIFOP_PIN          0
/* P1.3 - Input: FIFO from CC2420 */
#define CC2420_FIFO_PORT(type)     P1##type
#define CC2420_FIFO_PIN            3
/* P1.4 - Input: CCA from CC2420 */
#define CC2420_CCA_PORT(type)      P1##type
#define CC2420_CCA_PIN             4
/* P4.1 - Input:  SFD from CC2420 */
#define CC2420_SFD_PORT(type)      P4##type
#define CC2420_SFD_PIN             1
/* P4.2 - Output: SPI Chip Select (CS_N) */
#define CC2420_CSN_PORT(type)      P6##type
#define CC2420_CSN_PIN             2
/* P4.5 - Output: VREG_EN to CC2420 */
#define CC2420_VREG_PORT(type)     P6##type
#define CC2420_VREG_PIN            5
/* P4.6 - Output: RESET_N to CC2420 */
#define CC2420_RESET_PORT(type)    P6##type //LELE
#define CC2420_RESET_PIN           6

#define CC2420_IRQ_VECTOR PORT1_VECTOR

/* Pin status. */
#define CC2420_FIFOP_IS_1 (!!(CC2420_FIFOP_PORT(IN) & BV(CC2420_FIFOP_PIN)))
#define CC2420_FIFO_IS_1  (!!(CC2420_FIFO_PORT(IN) & BV(CC2420_FIFO_PIN)))
#define CC2420_CCA_IS_1   (!!(CC2420_CCA_PORT(IN) & BV(CC2420_CCA_PIN)))
#define CC2420_SFD_IS_1   (!!(CC2420_SFD_PORT(IN) & BV(CC2420_SFD_PIN)))

/* The CC2420 reset pin. */
#define SET_RESET_INACTIVE()   (CC2420_RESET_PORT(OUT) |=  BV(CC2420_RESET_PIN))
#define SET_RESET_ACTIVE()     (CC2420_RESET_PORT(OUT) &= ~BV(CC2420_RESET_PIN))

/* CC2420 voltage regulator enable pin. */
#define SET_VREG_ACTIVE()       (CC2420_VREG_PORT(OUT) |=  BV(CC2420_VREG_PIN))
#define SET_VREG_INACTIVE()     (CC2420_VREG_PORT(OUT) &= ~BV(CC2420_VREG_PIN))

/* CC2420 rising edge trigger for external interrupt 0 (FIFOP). */
#define CC2420_FIFOP_INT_INIT() do {                  \
    CC2420_FIFOP_PORT(IES) &= ~BV(CC2420_FIFOP_PIN);  \
    CC2420_CLEAR_FIFOP_INT();                         \
  } while(0)

/* FIFOP on external interrupt 0. */
#define CC2420_ENABLE_FIFOP_INT()  do {CC2420_FIFOP_PORT(IE) |= BV(CC2420_FIFOP_PIN);} while(0)
#define CC2420_DISABLE_FIFOP_INT() do {CC2420_FIFOP_PORT(IE) &= ~BV(CC2420_FIFOP_PIN);} while(0)
#define CC2420_CLEAR_FIFOP_INT()   do {CC2420_FIFOP_PORT(IFG) &= ~BV(CC2420_FIFOP_PIN);} while(0)

/*
 * Enables/disables CC2420 access to the SPI bus (not the bus).
 * (Chip Select)
 */

 /* ENABLE CSn (active low) */
#define CC2420_SPI_ENABLE()     (CC2420_CSN_PORT(OUT) &= ~BV(CC2420_CSN_PIN))
 /* DISABLE CSn (active low) */
#define CC2420_SPI_DISABLE()    (CC2420_CSN_PORT(OUT) |=  BV(CC2420_CSN_PIN))
#define CC2420_SPI_IS_ENABLED() ((CC2420_CSN_PORT(OUT) & BV(CC2420_CSN_PIN)) != BV(CC2420_CSN_PIN))

#endif /* __PLATFORM_CONF_H__ */
