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

#include <msp430f5435a.h>
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
#define PLATFORM_HAS_EEPROM
#define PLATFORM_HAS_EUI48
#define PLATFORM_HAS_RF

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
#define SERIAL_LINE_CONF_BUFSIZE 32

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
typedef unsigned short rtimer_clock_t;
typedef unsigned long off_t;

/* the low-level radio driver */
//#define NETSTACK_CONF_RADIO   cc2520_driver
/* the frame filtering implementation on CC2520 */
//#define WITH_FRAME_FILTERING
//#define WITH_CC2520_LPM2

/* LED ports */
#define LEDS_PxDIR 		P8DIR
#define LEDS_PxOUT 		P8OUT
#define LEDS_CONF_1  	0x01
#define LEDS_CONF_2    	0x02
#define LEDS_CONF_3 	0x04

/* Interrupt port definitions */
#define INT_PORT_DIR 			P2DIR
#define INT_PORT_REN			P2REN
#define INT_PORT_IE				P2IE
#define INT_PORT_IES 			P2IES
#define INT_PORT_IFG			P2IFG

#define INT_P0BIT				BIT0
#define INT_P1BIT				BIT3
#define INT_P2BIT				BIT5
#define INT_P3BIT				BIT6
#define INT_P4BIT				BIT7
#define INT_PRTCBIT			    BIT4

#define INT_P0					0
#define INT_P1					1
#define INT_P2					2
#define INT_P3					3
#define INT_P4					4
#define INT_PRTC			    5

/* digital I/O port definitions */
#define DIO_PORT_0_DIR 			P7DIR
#define DIO_PORT_1_DIR 			P7DIR
#define DIO_PORT_2_DIR 			P8DIR
#define DIO_PORT_3_DIR 			P8DIR

#define DIO_PORT_0_IN 			P7IN
#define DIO_PORT_1_IN 			P7IN
#define DIO_PORT_2_IN 			P8IN
#define DIO_PORT_3_IN 			P8IN

#define DIO_PORT_0_OUT 			P7OUT
#define DIO_PORT_1_OUT 			P7OUT
#define DIO_PORT_2_OUT 			P8OUT
#define DIO_PORT_3_OUT 			P8OUT

#define DIO_P0BIT				BIT4
#define DIO_P1BIT				BIT5
#define DIO_P2BIT				BIT5
#define DIO_P3BIT				BIT6

#define DIO_P0					0
#define DIO_P1					1
#define DIO_P2					2
#define DIO_P3					3

/* ADC port definitions */
#define ADC_PORT_DIR        	P6DIR
#define ADC_PORT_OUT           	P6OUT
#define ADC_PORT_SEL           	P6SEL

#define ADC_CH0_BIT          	BIT0
#define ADC_CH1_BIT           	BIT1
#define ADC_CH2_BIT          	BIT2
#define ADC_CH3_BIT        		BIT3
#define ADC_CH4_BIT           	BIT4
#define ADC_CH5_BIT         	BIT5
#define ADC_CH6_BIT           	BIT6

#define ADC_CH_0               	ADC12INCH_0
#define ADC_CH_1             	ADC12INCH_1
#define ADC_CH_2            	ADC12INCH_2
#define ADC_CH_3            	ADC12INCH_3
#define ADC_CH_4           		ADC12INCH_4
#define ADC_CH_5             	ADC12INCH_5
#define ADC_CH_6              	ADC12INCH_6

#define ADC_CH_INTEMP      		ADC12INCH_10
#define ADC_CH_VBAT          	ADC12INCH_11
#define ADC_CH_LIGHT          	ADC12INCH_1

#define ADC_INTREF           	ADC12SREF_1
#define ADC_EXTREF            	ADC12SREF_7

#define ADC_INTREF_1_5         	REFVSEL_0
#define ADC_INTREF_2       		REFVSEL_1
#define ADC_INTREF_2_5        	REFVSEL_3

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
#define VCORE_12MHZ             PMMCOREV_1
#define VCORE_16MHZ             PMMCOREV_2
#define VCORE_20MHZ             PMMCOREV_2
#define VCORE_25MHZ             PMMCOREV_3

/* end MSP430F54xx family symbols */

#define ROM_ERASE_UNIT_SIZE  128
#define EEPROM_ERASE_UNIT_SIZE (128)


#define CFS_CONF_OFFSET_TYPE    long


/* Use the first 128 byte (first page) for node configuration or 1 bank of CPU flash*/
#define NODE_ID_EEPROM_OFFSET     (BASE_EEPROM + 0 * EEPROM_ERASE_UNIT_SIZE)

/* Use second 32k of external flash or bank 1 of CPU  flash for system needs (hibernation) */
#define SYSTEM_EEPROM_FS_BASE  (BASE_EEPROM + 1 * EEPROM_ERASE_UNIT_SIZE)
#define SYSTEM_EEPROM_FS_SIZE (EEPROM_ERASE_UNIT_SIZE * 64)

#define APPS_EEPROM_FS_BASE  (SYSTEM_EEPROM_FS_BASE + SYSTEM_EEPROM_FS_SIZE)



/* Use the bank 2 of CPU (min 23K) flash for applications storage. starting at 0x20000*/
#define FLASH_SEGMENT_SIZE		512
#define APP_NODES_TABLE_BASE   (0x20000) // the first segment is used to store the app table
#define APPS_FLASH_FS_BASE   (APP_NODES_TABLE_BASE + 1 * FLASH_SEGMENT_SIZE)







/*
 * SPI bus configuration
 */

/* SPI input/output registers. */
#define SPI_TXBUF UCB1TXBUF
#define SPI_RXBUF UCB1RXBUF

                                /* USART0 Tx ready? */
#define SPI_WAITFOREOTx() while ((UCB1STAT & UCBUSY) != 0)
                                /* USART0 Rx ready? */
#define SPI_WAITFOREORx() while ((UCB1IFG & UCRXIFG) == 0)
                                /* USART0 Tx buffer ready? */
#define SPI_WAITFORTxREADY() while ((UCB1IFG & UCTXIFG) == 0)
/*                                 /\* USART0 Tx ready? *\/ */
/* #define SPI_WAITFOREOTx() while (!(UCB0IFG & UCRXIFG)) */
/*                                 /\* USART0 Rx ready? *\/ */
/* #define SPI_WAITFOREORx() while (!(UCB0IFG & UCRXIFG)) */
/*                                 /\* USART0 Tx buffer ready? *\/ */
/* #define SPI_WAITFORTxREADY() while (!(UCB0IFG & UCRXIFG)) */
/* #define SPI_BUSY_WAIT() 		while ((UCB0STAT & UCBUSY) == 1) */

#define MOSI           7	/* P3.7 - Output: SPI Master out - slave in (MOSI) */
#define MISO           4	/* P5.4 - Input:  SPI Master in - slave out (MISO) */
#define SCK            5	/* P5.5 - Output: SPI Serial Clock (SCLK) */
/* #define SCK            1  /\* P3.1 - Output: SPI Serial Clock (SCLK) *\/ */
/* #define MOSI           2  /\* P3.2 - Output: SPI Master out - slave in (MOSI) *\/ */
/* #define MISO           3  /\* P3.3 - Input:  SPI Master in - slave out (MISO) *\/ */

/*
 * SPI bus - M25P80 external flash configuration.
 */
//#define FLASH_PWR       //3       /* P4.3 Output */
//#define FLASH_CS        //4       /* P4.4 Output */
//#define FLASH_HOLD      //7       /* P4.7 Output */

/* Enable/disable flash access to the SPI bus (active low). */

//#define SPI_FLASH_ENABLE()  //( P4OUT &= ~BV(FLASH_CS) )
//#define SPI_FLASH_DISABLE() //( P4OUT |=  BV(FLASH_CS) )

//#define SPI_FLASH_HOLD()               // ( P4OUT &= ~BV(FLASH_HOLD) )
//#define SPI_FLASH_UNHOLD()              //( P4OUT |=  BV(FLASH_HOLD) )

/*
 * SPI bus - CC2520 pin configuration.
 */
#define CC2520_CONF_SYMBOL_LOOP_COUNT 2604      /* 326us msp430X @ 16MHz */

/* P1.6 - Input: FIFOP from CC2520 */
#define CC2520_FIFOP_PORT(type)    P1##type
#define CC2520_FIFOP_PIN           6
/* P1.5 - Input: FIFO from CC2520 */
#define CC2520_FIFO_PORT(type)     P1##type
#define CC2520_FIFO_PIN            5
/* P1.1 - Input: CCA from CC2520 */
#define CC2520_CCA_PORT(type)      P1##type
#define CC2520_CCA_PIN             1
/* P1.2 - Input:  SFD from CC2520 */
#define CC2520_SFD_PORT(type)      P1##type
#define CC2520_SFD_PIN             2
/* P4.1 - Output: SPI Chip Select (CS_N) */
#define CC2520_CSN_PORT(type)      P4##type
#define CC2520_CSN_PIN             1
/* P1.0 - Output: VREG_EN to CC2520 */
#define CC2520_VREG_PORT(type)     P1##type
#define CC2520_VREG_PIN            0
/* P4.0 - Output: RESET_N to CC2520 */
#define CC2520_RESET_PORT(type)    P4##type
#define CC2520_RESET_PIN           0

/* P4.2 - Output: GND enable for CC2520 */
#define CC2520_GND_EN_PORT(type)   P4##type
#define CC2520_GND_EN_PIN          2
/* P1.7 - Output: GPIO5 for CC2520 */
#define CC2520_GPIO5_PORT(type)   P1##type
#define CC2520_GPIO5_PIN          7


#define CC2520_IRQ_VECTOR PORT1_VECTOR

/* Pin status.CC2520 */
#define CC2520_FIFOP_IS_1 (!!(CC2520_FIFOP_PORT(IN) & BV(CC2520_FIFOP_PIN)))
#define CC2520_FIFO_IS_1  (!!(CC2520_FIFO_PORT(IN) & BV(CC2520_FIFO_PIN)))
#define CC2520_CCA_IS_1   (!!(CC2520_CCA_PORT(IN) & BV(CC2520_CCA_PIN)))
#define CC2520_SFD_IS_1   (!!(CC2520_SFD_PORT(IN) & BV(CC2520_SFD_PIN)))

/* The CC2520 reset pin. */
#define SET_RESET_INACTIVE()   (CC2520_RESET_PORT(OUT) |=  BV(CC2520_RESET_PIN))
#define SET_RESET_ACTIVE()     (CC2520_RESET_PORT(OUT) &= ~BV(CC2520_RESET_PIN))

/* CC2520 voltage regulator enable pin. */
#define SET_VREG_ACTIVE()       (CC2520_VREG_PORT(OUT) |=  BV(CC2520_VREG_PIN))
#define SET_VREG_INACTIVE()     (CC2520_VREG_PORT(OUT) &= ~BV(CC2520_VREG_PIN))

/* CC2520 GND enable pin. */
#define CC2520_GND_ENABLE()		(CC2520_GND_EN_PORT(OUT) |=  BV(CC2520_GND_EN_PIN))
#define CC2520_GND_DISABLE()	(CC2520_GND_EN_PORT(OUT) &= ~BV(CC2520_GND_EN_PIN))

/* CC2520 rising edge trigger for external interrupt 0 (FIFOP). */
#define CC2520_FIFOP_INT_INIT() do {                  \
    CC2520_FIFOP_PORT(IES) &= ~BV(CC2520_FIFOP_PIN);  \
    CC2520_CLEAR_FIFOP_INT();                         \
  } while(0)

/* FIFOP on external interrupt 0. */
/* FIFOP on external interrupt 0. */
#define CC2520_ENABLE_FIFOP_INT()          do { P1IE |= BV(CC2520_FIFOP_PIN); } while (0)
#define CC2520_DISABLE_FIFOP_INT()         do { P1IE &= ~BV(CC2520_FIFOP_PIN); } while (0)
#define CC2520_CLEAR_FIFOP_INT()           do { P1IFG &= ~BV(CC2520_FIFOP_PIN); } while (0)

/*
 * Enables/disables CC2520 access to the SPI bus (not the bus).
 * (Chip Select)
 */

 /* ENABLE CSn (active low) */
#define CC2520_SPI_ENABLE()     do{ UCB1CTL1 &= ~UCSWRST;  clock_delay(5); P4OUT &= ~BIT1;clock_delay(5);}while(0)
 /* DISABLE CSn (active low) */
#define CC2520_SPI_DISABLE()    do{clock_delay(5);UCB1CTL1 |= UCSWRST;clock_delay(1); P4OUT |= BIT1;clock_delay(5);}while(0)
#define CC2520_SPI_IS_ENABLED() ((CC2520_CSN_PORT(OUT) & BV(CC2520_CSN_PIN)) != BV(CC2520_CSN_PIN))









#endif /* __PLATFORM_CONF_H__ */
