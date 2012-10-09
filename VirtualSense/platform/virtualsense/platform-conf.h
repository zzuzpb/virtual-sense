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

//#ifndef __PLATFORM_CONF_H__
//#define __PLATFORM_CONF_H__

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
#define NETSTACK_CONF_RADIO   cc2520_driver
/* the frame filtering implementation on CC2520 */
#define WITH_FRAME_FILTERING
//#define WITH_CC2520_LPM2

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



/* Use the bank 2 of CPU (min 23K) flash for applications storage. starting at 0x20000*/
#define FLASH_SEGMENT_SIZE		512
#define APP_NODES_TABLE_BASE   (0x20000) // the first segment is used to store the app table
#define APPS_FLASH_FS_BASE   (APP_NODES_TABLE_BASE + 1 * FLASH_SEGMENT_SIZE)


//#endif /* __PLATFORM_CONF_H__ */
