/*
 *  spi_sw.c
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
 * The emulated SPI software communication system.
 *
 * @author Matteo Dromedari
 *
 */

#include "contiki.h"
#include "spi_sw.h"


void spi_sw_delay(uint8_t delay){
	uint8_t i = 0x00;
	uint8_t j = 0x00;

	for(i = 0; i < delay; i++)
		for(j = 0; j < 0xFF; j++)
			asm("   NOP");
}

void spi_sw_init(void) {
	GPIO_SOFTWARE_CONTROL(SPI_CE_PORT, (1 << SPI_CE_PIN));
	GPIO_SET_OUTPUT(SPI_CE_PORT, (1 << SPI_CE_PIN));
	GPIO_CLR_PIN(SPI_CE_PORT, (1 << SPI_CE_PIN));

	GPIO_SOFTWARE_CONTROL(SPI_MISO_PORT, (1 << SPI_MISO_PIN));
	GPIO_SET_INPUT(SPI_MISO_PORT, (1 << SPI_MISO_PIN));

	GPIO_SOFTWARE_CONTROL(SPI_MOSI_PORT, (1 << SPI_MOSI_PIN));
	GPIO_SET_OUTPUT(SPI_MOSI_PORT, (1 << SPI_MOSI_PIN));
	GPIO_CLR_PIN(SPI_MOSI_PORT, (1 << SPI_MOSI_PIN));

	GPIO_SOFTWARE_CONTROL(SPI_CLK_PORT, (1 << SPI_CLK_PIN));
	GPIO_SET_OUTPUT(SPI_CLK_PORT, (1 << SPI_CLK_PIN));
	GPIO_CLR_PIN(SPI_CLK_PORT, (1 << SPI_CLK_PIN));
}


void spi_sw_write(uint8_t write){
	SPI_CLK_CLR();
	SPI_MOSI_CLR();

	uint8_t i = 0x00;
	for(i = 0; i < 8; i++){
									// Prepare to clock out the Address byte
	    if(write & 0x80){         	// Check for a 1
	    	SPI_MOSI_SET();
	    }							// and set the MOSI line appropriately
	    else{
	    	SPI_MOSI_CLR();
	    }
	    spi_sw_delay(0x0F);

	    SPI_CLK_SET();
	    write <<= 1; 				// Rotate to get the next bit
	    spi_sw_delay(0x0F);			// Toggle the clock line

	    SPI_CLK_CLR();
	}
}


uint8_t spi_sw_read(void){
	uint8_t ret = 0x00;
	uint8_t read = 0x00;
	uint8_t i = 0x00;

	for (i = 0; i < 8; i++){
		ret <<=1;                         	// Rotate the data
		spi_sw_delay(0x0F);

		SPI_CLK_SET();
		read = SPI_MISO_READ();
		if(read & (1 << SPI_MISO_PIN))
			ret += 0x01;
		spi_sw_delay(0x0F);
											// Raise the clock to clock the data out of the MAX7456
		SPI_CLK_CLR();

											// Raise the clock to clock the data out of the MAX7456
	                           				// Read the data bit
											// Drop the clock ready for the next bit
	}                                    	// and loop back

	return ret;
}
