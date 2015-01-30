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


#include "contiki.h"
#include "board.h"
#include "ssi.h"
#include "pcf2123_spi.h"
#include "digitalio.c"
#include "contiki-conf.h"
#include "power-interface.h"
#include "lib/_ioc.h"

#include <stdio.h>
#include <stdint.h>

//#include "lib/_cpu.h"



uint8_t RTC_is_up(void){ //TODO: trovare un modo pulito per farlo
	uint8_t res = 0;
	if(RTC_get_year() == 11)
		res = 1;
	return res;
}


/* initilize the RTC module */
void RTC_init(void){
	printf("RTC init\n");
	ssi0_spi_init(RTC_SPI_CE_PORT, RTC_SPI_CE_PIN);

	//RTC_write_register(PCF2123_REG_CTRL1, PCF2123_RESET);
	//RTC_write_register(PCF2123_REG_HR, bin2bcd(0x02));
	//uint8_t val = RTC_read_register(PCF2123_REG_HR);
	//printf("ora: %d\n", bcd2bin(val));
	//while(1);

	lock_SPI(); //NON ci sarebbe bisogno ma in questo modo impedisco che PM spenga
	// la SPI durante l'inizializzazione. Non servirebbe perch� ogni write acquisisce il lock
	// in questo modo ho performace maggiori
	uint8_t cout_value = RTC_read_register(PCF2123_REG_T_CLOKOUT);

	if(cout_value != 0x71){ // the rtc should be initialized because it has been just powerd-up

		/* reset RTC */
		RTC_write_register(PCF2123_REG_CTRL1, PCF2123_RESET);

		// setting low power mode by sourcing countdown timer with 1/60 Hz,
		// setting clockout frequency at 1Hz and disable countdown timer
		RTC_write_register(PCF2123_REG_T_CLOKOUT, PCF2123_COUT_F_1 | PCF2123_CDT_SF_1_64 | PCF2123_TIMER_DI);

		//RTC_write_register(PCF2123_REG_CTRL2, PCF2123_MI_INT);
		// enable seconds interrupt as pulse DEMO

		//Set time after power up to Giulia Lattanzi's birth day Sunday 27/11/11 14:24
		RTC_write_register(PCF2123_REG_HR, bin2bcd(0x0E));
		RTC_write_register(PCF2123_REG_MN, bin2bcd(0x18));
		RTC_write_register(PCF2123_REG_SC, bin2bcd(0x01));

		RTC_write_register(PCF2123_REG_MO, bin2bcd(0x0B));
		RTC_write_register(PCF2123_REG_DM, bin2bcd(0x1B));
		RTC_write_register(PCF2123_REG_DW, bin2bcd(0x00));
		RTC_write_register(PCF2123_REG_YR, bin2bcd(0x0B));

		uint8_t min = RTC_read_register(PCF2123_REG_MN);
		printf("RTC initialized with minutes %d\n", bcd2bin(min));
	}

	// Clear RTC internal interrupt
	RTC_clear_interrupt();
	release_SPI();
	// Enable falling edge interrupt on port PC7 with pull-up
	init_interrupt(ON_FALLING, RTC_INT_PIN, IOC_OVERRIDE_PUE);
}

void RTC_stop(void){
	RTC_write_register(PCF2123_REG_CTRL1, PCF2123_STOP);
}

void RTC_schedule_interrupt_at_minutes(uint8_t minutes){
	RTC_write_register(PCF2123_REG_CTRL2, PCF2123_AL_INT); // Enable alarm interrupt
	RTC_write_register(PCF2123_REG_MN_ALARM, bin2bcd(minutes)); //set alarm minutes
	release_SPI();
}
void RTC_schedule_interrupt_at_hours(uint8_t hours){
	RTC_write_register(PCF2123_REG_CTRL2, PCF2123_AL_INT); // Enable alarm interrupt
	RTC_write_register(PCF2123_REG_HR_ALARM, bin2bcd(hours)); //set alarm hours
}
void RTC_schedule_interrupt_at_days(uint8_t days){
	RTC_write_register(PCF2123_REG_CTRL2, PCF2123_AL_INT); // Enable alarm interrupt
	RTC_write_register(PCF2123_REG_DA_ALARM, bin2bcd(days)); //set alarm minutes
}


void RTC_clear_interrupt(void){
	uint8_t actual_value = RTC_read_register(PCF2123_REG_CTRL2);
	RTC_write_register(PCF2123_REG_CTRL2, (actual_value & 0xD3)); //clear BIT 5,3,2 --> 11010011*/
}
void RTC_disable_all_interrupts(void){
	RTC_write_register(PCF2123_REG_CTRL2, 0x00);
}
uint8_t RTC_get_year(void){
	uint8_t val = RTC_read_register(PCF2123_REG_YR);
	return bcd2bin(val);
}

uint8_t RTC_get_month(void){
	uint8_t val = RTC_read_register(PCF2123_REG_MO);
		return bcd2bin(val);
}

uint8_t RTC_get_day_of_month(void){
	uint8_t val = RTC_read_register(PCF2123_REG_DM);
		return bcd2bin(val);
}

uint8_t RTC_get_day_of_week(void){
	uint8_t val = RTC_read_register(PCF2123_REG_DW);
		return bcd2bin(val);
}

uint8_t RTC_get_hours(void){
	uint8_t val = RTC_read_register(PCF2123_REG_HR);
		return bcd2bin(val);
}

uint8_t RTC_get_minutes(void){
	uint8_t val = RTC_read_register(PCF2123_REG_MN);
		return bcd2bin(val);
}

uint8_t RTC_get_seconds(void){
	uint8_t val = RTC_read_register(PCF2123_REG_SC);
	uint8_t sec = bcd2bin(val);
	if (sec > 60)
		sec = sec -80;

	return sec; //bcd2bin(RTC_read_register(PCF2123_REG_SC));
}


uint8_t RTC_read_register(uint8_t aReg)
{
	uint8_t casted = 0;
	uint32_t result = 0;

	lock_SPI();
	ssi0_spi_ce_set(RTC_SPI_CE_PORT, RTC_SPI_CE_PIN);

	// Write "command"
	ssi0_spi_write(aReg|PCF2123_READ);

	// Read back "data"
	result = ssi0_spi_read();

	ssi0_spi_ce_clr(RTC_SPI_CE_PORT, RTC_SPI_CE_PIN);
	release_SPI();

	casted = (uint8_t)result;

	return casted;
}


void RTC_write_register(uint8_t aReg, uint8_t aValue)
{
	lock_SPI();
	ssi0_spi_ce_set(RTC_SPI_CE_PORT, RTC_SPI_CE_PIN);

	ssi0_spi_write(aReg|PCF2123_WRITE);
	ssi0_spi_write(aValue);

    ssi0_spi_ce_clr(RTC_SPI_CE_PORT, RTC_SPI_CE_PIN);
	release_SPI();
}
