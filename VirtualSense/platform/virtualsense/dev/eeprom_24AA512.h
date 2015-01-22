/*
 *  eeprom_24AA512.h
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
 * @author Matteo Dromedari
 *
 */

#define EEPROM_POWER_PORT		GPIO_D_BASE
#define EEPROM_POWER_PIN		GPIO_PIN_4

#define EEPROM_ADDRESS 			0x57

#define	EEPROM_PAGE_SIZE   		128

void init_24AA512(void);
uint8_t read_24AA512(uint8_t *, uint16_t, uint8_t);
uint8_t write_24AA512(uint8_t *, uint16_t, uint8_t);
void test_24AA512();



