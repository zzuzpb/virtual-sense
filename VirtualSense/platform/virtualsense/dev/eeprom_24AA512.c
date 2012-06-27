/*
 *  eeprom_24AA512.c
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
 * The eeprom implementation by 24AA512.
 *
 * @author Emanuele Lattanzi
 *
 */


#include <msp430.h>
#include "platform-conf.h"
#include "dev/i2c_eeprom_24AA512.h"
#include "dev/leds.h"
#include "eeprom_24AA512.h"

void write_byte_24AA512(uint8_t dev_address, uint16_t address, uint8_t data){
	 // write enable
	 WRITE_ENABLE();
	 i2c_enable();
	 while(! is_idle_24AA512(dev_address))
		 __delay_cycles(50);

	 i2c_start();

	 i2c_write((dev_address<<1));
	 i2c_write((address >> 8));
	 i2c_write((address & 0xff));
	 i2c_write(data);
	 // disable write

	 i2c_stop();
	 i2c_disable();
	 WRITE_PROTECT();
 }

 uint8_t read_byte_24AA512(uint8_t dev_address, uint16_t address){
	 uint8_t ret = 0;

	 i2c_enable();
	 while(! is_idle_24AA512(dev_address))
	 		 __delay_cycles(50);

	 i2c_start();

	 i2c_write((dev_address <<1));
	 i2c_write((address >> 8));
	 i2c_write((address & 0xff));

	 i2c_start();
	 i2c_write(((dev_address<<1) | 1));
	 ret = i2c_read(0);

	 i2c_stop();
	 i2c_disable();
	 return ret;
}


 void read_sequential_24AA512(uint8_t dev_address, uint16_t address, uint16_t size, unsigned char * data){
	 uint16_t i = 0;
	 i2c_enable();
	 //printf("i2c enabled\n");
	 while(! is_idle_24AA512(dev_address))
	 		 __delay_cycles(50);

	 i2c_start();
	 //printf("i2c start\n");

	 i2c_write((dev_address <<1));
	 i2c_write((address >> 8));
	 i2c_write((address & 0xff));
	 //printf("READ i2c sent address %ud on dev_address\n", address);

	 i2c_start();
	 i2c_write(((dev_address<<1) | 1));
	 for(i = 0; i < size-1; i++)
		 data[i] = i2c_read(1); // ACK MEANS SEQ READ
	 data[i] = i2c_read(0); // NO ACK MEANS SEQ READ STOP

	 i2c_stop();
	 i2c_disable();
}

 void write_sequential_24AA512(uint8_t dev_address, uint16_t address, uint16_t size, unsigned char * data){

	 // check page boundary
	 uint16_t bytes_left_on_this_page = PAGE_SIZE - (address % PAGE_SIZE);
	 uint16_t i = 0;
	 uint16_t current_address = address;
	 uint16_t num_bytes = size;
	 uint16_t temp_num = 0;
	 uint16_t bytes_writed = 0;

	 // write enable
	 WRITE_ENABLE();
 	 i2c_enable();


 	 while(bytes_writed < size){
 		while(! is_idle_24AA512(dev_address))
 			__delay_cycles(50);

 		i2c_start();
 		//printf("i2c started\n");

 		i2c_write((dev_address<<1));
 		i2c_write((current_address >> 8));
 		i2c_write((current_address & 0xff));
 		 while((i < bytes_left_on_this_page ) && (bytes_writed < size)){ // writing in the left page
 			 i2c_write(data[bytes_writed]);
 			 bytes_writed++;
 			 i++;
 			 //printf(".");
 		 }
 		 //printf("\n @ bytes_writed %u\n", bytes_writed);
 		 // pagina finita oppure byte da scrivere finiti
 		i2c_stop();
 	    // write other pages
 		current_address+=bytes_left_on_this_page;
 		bytes_left_on_this_page = PAGE_SIZE;
 		i=0;
 	 }
 	 i2c_disable();
 	 WRITE_PROTECT();

  }

uint8_t is_idle_24AA512(uint8_t dev_address){
	 i2c_start();
	 return i2c_write((dev_address <<1));
}
