/*
 *  barometer_MPL115A2.h
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

#define DEVICE_ADDRESS	0xC0

#define READ 	0x01
#define WRITE 	0x00

#define BAR_POWER_UP()	{} //TODO to implement P2OUT  |= BIT7;
#define BAR_POWER_DOWN() {} //TODO to implement P2OUT &= ~BIT7;


#define START_CONVERSION_COMMAND	0x12
#define READ_PRESSURE_MSB_COMMAND   	0x00
#define READ_PRESSURE_LSB_COMMAND   	0x01
#define READ_TEMPERATURE_MSB_COMMAND   	0x02
#define READ_TEMPERATURE_LSB_COMMAND   	0x03
#define READ_COEFFICIENTS_COMMAND   	0x04



uint16_t read_temperature_barometer_MPL115A2();
uint16_t read_pressure_barometer_MPL115A2();

