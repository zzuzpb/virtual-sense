/*
 *  pcf2321_spi.h
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
 * The SPI communication system with PCF2123 RTC.
 *
 * @author Emanuele Lattanzi
 *
 */


#include <msp430.h>
//#include <legacymsp430.h>

#include "contiki-conf.h"


#define SS_ACTIVE				P3OUT &= 0xBF					///1011 1111												///0100 0000
#define SS_INACTIVE				P3OUT |= 0x40
#define CE_ACTIVE				P2OUT |= BIT1
#define CE_INACTIVE       		P2OUT &= ~BIT1

#define PCF2123_REG_CTRL1		(0x00)	/* Control Register 1 */
#define PCF2123_REG_CTRL2		(0x01)	/* Control Register 2 */
#define PCF2123_REG_SC			(0x02)	/* datetime */
#define PCF2123_REG_MN			(0x03)
#define PCF2123_REG_HR			(0x04)
#define PCF2123_REG_DM			(0x05)
#define PCF2123_REG_DW			(0x06)
#define PCF2123_REG_MO			(0x07)
#define PCF2123_REG_YR			(0x08)
#define PCF2123_REG_MN_ALARM	(0x09)
#define PCF2123_REG_HR_ALARM	(0xA0)
#define PCF2123_REG_DA_ALARM	(0xB0)

#define PCF2123_SUBADDR		(1 << 4)
#define PCF2123_WRITE		(0x10)
#define PCF2123_READ		(0x90)

#define PCF2123_RESET		(0x58)
#define PCF2123_SC_INT		(0x40)
#define PCF2123_MI_INT		(0x80)
#define PCF2123_AL_INT		(0x02)

#define bcd2bin(val) ((val)&15) + ((val)>>4)*10
#define bin2bcd(val) (((val)/10)<<4) + (val)%10


void RTC_spi_init(void);
void RTC_init(void);
void RTC_clear_interrupt(void);
void RTC_disable_all_interrupts(void);

void RTC_schedule_interrupt_at_minutes(uint8_t minutes);
void RTC_schedule_interrupt_at_hours(uint8_t hours);
void RTC_schedule_interrupt_at_days(uint8_t days);


uint8_t RTC_read_register(uint8_t aReg);
void RTC_write_register(uint8_t aReg, uint8_t aValue);

uint8_t RTC_get_year(void);
uint8_t RTC_get_month(void);
uint8_t RTC_get_day_of_month(void);
uint8_t RTC_get_day_of_week(void);
uint8_t RTC_get_hours(void);
uint8_t RTC_get_minutes(void);
uint8_t RTC_get_seconds(void);
