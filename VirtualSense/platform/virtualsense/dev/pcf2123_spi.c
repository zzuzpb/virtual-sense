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
#include "pcf2123_spi.h"
#include "contiki-conf.h"


unsigned char spi_busy = 0;
//static unsigned char spi_inited = 0;

/*
 * Initialize SPI bus.
 */
void
RTC_spi_init(void)
{

  /*if (spi_inited)
    return; */

  	/// P3.6 : /CS this signal is managed "by hand" not automatically by FMS
  	//P3DIR |= BIT6;
  	//P2DIR |= BIT1;
  	CE_INACTIVE;

  	/// set alternate port function
  	P5SEL |= BIT5 + BIT4; 				/// P5.5 (UCB1CLK) e P5.4 (UCB1MISO)
  	P3SEL |= BIT7;						///  P3.7 (UCB1MOSI)

  	UCB1CTL1 |= UCSWRST;			/// the state machine is in reset mode
  	/// msb, ,master, 4 wires, sync, ck inactive low
  	//UCB1CTL0 |= UCMSB + UCMST + UCMODE_1 + UCSYNC;

  	/// msb, ,master, 3 wires, sync, ck inactive low
  	UCB1CTL0 |= UCMSB + UCMST + UCSYNC;
  	///
  	UCB1CTL1 |= UCSSEL_2; 		/// clock from SMCLK
  	/// bit rate: SMCK / 2
  	UCB1BR0 = 0x2;						/// :2
  	UCB1BR1 = 0;

  	/// enable loopback
  	//UCB1STAT = UCLISTEN;

  	UCB1CTL1 &= ~UCSWRST;			/// initialize the state machine

  	/// wait until a tx operation end
  	while( UCB1STAT & UCBUSY );
  	/// is the same?
  	/// while (!(UCB1IFG & UCTXIFG));
  	//spi_inited = 1;

}

void RTC_spi_shutdown(void){

	  	P5SEL &= ~(BIT5 + BIT4); 				/// P5.5 (UCB1CLK) e P5.4 (UCB1MISO)
	  	P3SEL &= ~(BIT7);						///  P3.7 (UCB1MOSI)

}
uint8_t RTC_is_up(void){
	uint8_t res = 0;
	if(RTC_get_year() == 11)
		res = 1;
	return res;
}

/* initilize the RTC module */
void RTC_init(void){

	/* reset RTC */
	RTC_write_register(PCF2123_REG_CTRL1, PCF2123_RESET);

	//RTC_write_register(PCF2123_REG_CTRL2, PCF2123_MI_INT);
	// enable seconds interrupt as pulse DEMO

	//Set time after power up to Giulia Lattanzi's birth day Sunday 27/11/11 14:24
	RTC_write_register(PCF2123_REG_HR, bin2bcd(0x0E));
	RTC_write_register(PCF2123_REG_MN, bin2bcd(0x18));
	RTC_write_register(PCF2123_REG_SC, bin2bcd(0x00));

	RTC_write_register(PCF2123_REG_MO, bin2bcd(0x0B));
	RTC_write_register(PCF2123_REG_DM, bin2bcd(0x1B));
	RTC_write_register(PCF2123_REG_DW, bin2bcd(0x00));
	RTC_write_register(PCF2123_REG_YR, bin2bcd(0x0B));
}

void RTC_schedule_interrupt_at_minutes(uint8_t minutes){
	RTC_write_register(PCF2123_REG_CTRL2, PCF2123_AL_INT); // Enable alarm interrupt
	RTC_write_register(PCF2123_REG_MN_ALARM, bin2bcd(minutes)); //set alarm minutes
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
	return bcd2bin(RTC_read_register(PCF2123_REG_YR));
}

uint8_t RTC_get_month(void){
	return bcd2bin(RTC_read_register(PCF2123_REG_MO));
}

uint8_t RTC_get_day_of_month(void){
	return bcd2bin(RTC_read_register(PCF2123_REG_DM));
}

uint8_t RTC_get_day_of_week(void){
	return bcd2bin(RTC_read_register(PCF2123_REG_DW));
}

uint8_t RTC_get_hours(void){
	return bcd2bin(RTC_read_register(PCF2123_REG_HR));
}

uint8_t RTC_get_minutes(void){
	return bcd2bin(RTC_read_register(PCF2123_REG_MN));
}

uint8_t RTC_get_seconds(void){
	return bcd2bin(RTC_read_register(PCF2123_REG_SC));
}

/*
 *	transmit a byte
 */
void B1_tx(uint8_t val){
	/// wait until a tx operation end
	while( UCB1STAT & UCBUSY );
	UCB1TXBUF = val;
	/// wait until a tx operation end
	while( UCB1STAT & UCBUSY );

}

/*
 * read a byte
 */
uint8_t B1_rx(void){

	uint8_t read;
	/// wait until a tx operation end
	while( UCB1STAT & UCBUSY );
	UCB1TXBUF = 0x00;
	/// wait until a tx operation end
	while( UCB1STAT & UCBUSY );
	/// when write operation is finished, received data is ready.
	read = UCB1RXBUF;
	/// close connection
	return read;
}

uint8_t RTC_read_register(uint8_t aReg)
{
	uint8_t result;
	CE_ACTIVE;
	// Write "command"
	B1_tx(aReg|PCF2123_READ);
	// Read back "data"
	result = B1_rx();
	CE_INACTIVE;
	return result;
}

void RTC_write_register(uint8_t aReg, uint8_t aValue)
{
	CE_ACTIVE;
	B1_tx(aReg|PCF2123_WRITE);
	B1_tx(aValue);
	CE_INACTIVE;
}



