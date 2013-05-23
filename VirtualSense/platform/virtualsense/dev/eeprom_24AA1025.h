/*
 *  eeprom_24AA1025.h
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

#define EEPROM_ADDRESS	0x53

#define EE_POWER_UP()	P4OUT  |= BIT4;
#define EE_POWER_DOWN() P4OUT &= ~BIT4;

#define     PAGE_SIZE   128

void write_byte_24AA1025(uint8_t dev_address, uint16_t address, uint8_t data);
uint8_t read_byte_24AA1025(uint8_t dev_address, uint16_t address);
uint8_t is_idle_24AA1025(uint8_t dev_address);
void write_sequential_24AA1025(uint8_t dev_address, uint16_t address, uint16_t size, unsigned char * data);
void read_sequential_24AA1025(uint8_t dev_address, uint16_t address, uint16_t size, unsigned char * data);
