/*
 *  power-interface.c
 *
 *  Copyright (c) 2012 DiSBeF, University of Urbino.
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
 * 	power interface of the power manageable modules
 *
 * @author Emanuele Lattanzi
 *
 */

#ifndef __POWER_INTERFACE_H__
#define __POWER_INTERFACE_H__



#define GET_LOCK(lock) lock++
#define RELEASE_LOCK(lock) lock--

/* shutdown functions */
void shutdown_RF(void);
void shutdown_RTC(void);
void shutdown_ADC(void);
void shutdown_UART(void);
void shutdown_MAC(void);
void shutdown_SPI(void);

/*wakeup functions */
uint8_t wakeup_RF(void);
uint8_t wakeup_RTC(void);
uint8_t wakeup_ADC(void);
uint8_t wakeup_UART(void);
uint8_t wakeup_MAC(void);
uint8_t wakeup_SPI(void);

/* lock functions */
void lock_RF(void);
void lock_RTC(void);
void lock_ADC(void);
void lock_UART(void);
void lock_MAC(void);
void lock_SPI(void);

void release_RF(void);
void release_RTC(void);
void release_ADC(void);
void release_UART(void);
void release_MAC(void);
void release_SPI(void);

/* test functions */
uint8_t is_locked_RF(void);
uint8_t is_locked_RTC(void);
uint8_t is_locked_ADC(void);
uint8_t is_locked_UART(void);
uint8_t is_locked_MAC(void);
uint8_t is_locked_SPI(void);


#endif /* __POWER_INTERFACE_H__ */
