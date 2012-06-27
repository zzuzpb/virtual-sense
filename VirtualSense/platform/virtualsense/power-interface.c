/*
 *  power-interface.h
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
 * 	power interface of the power manageable modules
 *
 * @author Emanuele Lattanzi
 *
 */

#include "contiki-conf.h"
#include "power-interface.h"
#include "net/netstack.h"

static uint8_t RF_locked = 0, RTC_locked = 0, ADC_locked = 0, UART_locked = 0, MAC_locked = 0, SPI_locked = 0;
void lock_RF() {RF_locked++;}
void lock_RTC() {RTC_locked++;}
void lock_ADC() {ADC_locked++;}
void  lock_UART() {UART_locked++;}

void  lock_MAC() {
	if(MAC_locked <= 0)
		wakeup_MAC(); //accensione legata all'acquisizione del lock
	MAC_locked++;
}

void  lock_SPI() {SPI_locked++;}

void release_RF() {RF_locked--;}
void release_RTC() {RTC_locked--;}
void release_ADC() {ADC_locked--;}
void release_UART() {UART_locked--;}
void release_MAC() {MAC_locked--;}
void release_SPI() {SPI_locked--;}


/* shutdown functions */
void shutdown_RF(void){
	NETSTACK_RADIO.off();
}
void shutdown_RTC(void){
}
void shutdown_ADC(void){
}
void shutdown_UART(void){
}
void shutdown_MAC(void){
	 NETSTACK_RDC.off(0);
	 NETSTACK_MAC.off(0);
}
void shutdown_SPI(void){
	spi_UCB1_shutdown();
}

/*wakeup functions */
uint8_t wakeup_RF(void){
	return NETSTACK_RADIO.on();
}

uint8_t wakeup_RTC(void){
	return 1;
}
uint8_t wakeup_ADC(void){
	return 1;
}
uint8_t wakeup_UART(void) {
	return 1;
}
uint8_t wakeup_MAC(void){
	NETSTACK_RDC.on();
	NETSTACK_MAC.on();
	return 1;
}

uint8_t wakeup_SPI(void){
	return 1;
}

/* test functions */
uint8_t is_locked_RF(void){
	return RF_locked >0?1:0;
}
uint8_t is_locked_RTC(void){
	return RTC_locked >0?1:0;
}
uint8_t is_locked_ADC(void){
	return ADC_locked >0?1:0;
}
uint8_t is_locked_UART(void){
	return UART_locked >0?1:0;
}
uint8_t is_locked_MAC(void){
	return MAC_locked >0?1:0;
}
uint8_t is_locked_SPI(void){
	return SPI_locked >0?1:0;
}
