/*
 *  barometer_MPL115A2.c
 *
 *  Copyright (c) 2013 DiSBeF, University of Urbino.
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
 * The MPL115A2 barometer/temperature sensor driver.
 *
 * @author Emanuele Lattanzi
 *
 */


#include <msp430.h>
#include "platform-conf.h"
#include "dev/i2c.h"
#include "dev/leds.h"
#include "barometer_MPL115A2.h"

 void barometer_MPL115A2_init(){
	 P2DIR |= BIT7;
	 // read coefficient ??
 }

 uint16_t read_pressure_barometer_MPL115A2(){
	 uint16_t ret = 0;
	 	 uint8_t res = 0;
	 	 BAR_POWER_UP();
	 	 i2c_enable();

	 	 i2c_start();
	 	 i2c_write((DEVICE_ADDRESS | WRITE));
	 	 i2c_write((START_CONVERSION_COMMAND));
	 	 i2c_write((0x01)); // trovato su un forum di uno spagnolo
	 	 i2c_stop();
	 	 //delay 1.6 ms
	 	 __delay_cycles(1662500);

	 	 i2c_start();
	 	 i2c_write((DEVICE_ADDRESS | WRITE));
	 	 i2c_write((READ_PRESSURE_MSB_COMMAND));

	 	 i2c_start();
	 	 i2c_write((DEVICE_ADDRESS | READ));

	 	 res = i2c_read(1);
	 	 //printf("temp MSB %x\n", res);
	 	 ret = (res << 8);
	 	 res = i2c_read(0);
	 	 ret += res;
	 	 ret = ret >> 6;

	 	 i2c_stop();
	 	 i2c_disable();
	 	 BAR_POWER_DOWN();
	 	 return ret;
}

 uint16_t read_temperature_barometer_MPL115A2(){
 	 uint16_t ret = 0;
 	 uint8_t res = 0;
 	 float t;
 	 BAR_POWER_UP();
 	 i2c_enable();

 	 i2c_start();
 	 i2c_write((DEVICE_ADDRESS | WRITE));
 	 i2c_write((START_CONVERSION_COMMAND));
 	 i2c_write((0x01)); // trovato su un forum di uno spagnolo
 	 i2c_stop();
 	 //delay 1.6 ms
 	 __delay_cycles(1662500);

 	 i2c_start();
 	 i2c_write((DEVICE_ADDRESS | WRITE));
 	 i2c_write((READ_TEMPERATURE_MSB_COMMAND));

 	 i2c_start();
 	 i2c_write((DEVICE_ADDRESS | READ));

 	 res = i2c_read(1);
 	 //printf("temp MSB %x\n", res);
 	 ret = (res << 8);
 	 res = i2c_read(0);
 	 ret += res;
 	 ret = ret >> 6;

 	 i2c_stop();
 	 i2c_disable();
 	 BAR_POWER_DOWN();


 	  t = ((float) ret - 498.0F) / -5.35F +25.0F;           // C
 	  ret = (uint16_t)(t*100);
 	 return ret;
 }



