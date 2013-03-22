/*
 *  eeprom_24AA1025.c
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
 * The eeprom implementation by 24AA1025.
 *
 * @author Emanuele Lattanzi
 *
 */


#include <msp430.h>
#include "platform-conf.h"
#include "dev/i2c.h"
#include "dev/leds.h"
#include "EUI-48_24AA025E48.h"

uint8_t EUI_is_idle(uint8_t dev_address){
	 i2c_start();
	 return i2c_write((dev_address <<1));
}

 void EUI_init(){
	 uint16_t i = 0;
	 P4DIR |= BIT5;
	 EUI_POWER_UP();
	 i2c_init();
	 i2c_enable();
	 printf("i2c enabled\n");
	 while(! EUI_is_idle(EUI_DEVICE))
	 		 __delay_cycles(50);

	 i2c_start();
	 //printf("i2c start\n");

	 i2c_write((EUI_DEVICE <<1));
	 i2c_write((EUI_ADDRESS ));
	 //i2c_write((EUI_ADDRESS & 0xff));
	 //printf("READ i2c sent address %ud on dev_address\n", EUI_ADDRESS);

	 i2c_start();
	 i2c_write(((EUI_DEVICE<<1) | 1));
	 for(i = 0; i < 3; i++)
		 EUI_48[i] = i2c_read(1); // ACK MEANS SEQ READ
	 EUI_48[3] = 0xFF;
	 EUI_48[4] = 0xFE;
	 for(i = 5; i < 7; i++)
			 EUI_48[i] = i2c_read(1); // ACK MEANS SEQ READ
	 EUI_48[i] = i2c_read(0); // NO ACK MEANS SEQ READ STOP

	 i2c_stop();
	 i2c_disable();
	 EUI_POWER_DOWN();
}


