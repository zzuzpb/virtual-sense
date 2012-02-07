/*
 *  eeprom_i2c.h
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
 * The I2C communication system with 24AA512 EEPROM.
 *
 * @author Emanuele Lattanzi
 *
 */


#include <msp430.h>

#include "contiki-conf.h"
#define STATUS_OK 	1
#define STATUS_ERR	0
#define	NACK_ERR	0
#define baseAddress24AA512 0xA2


void eeprom_i2C_init(void);
uint8_t eeprom_read_byte_from_address(uint16_t address, uint8_t *status);
uint8_t eeprom_read_bytes_from_address(uint16_t address, uint8_t num_bytes, uint8_t buff[]);
uint8_t eeprom_write_byte_at_address(uint8_t data, uint16_t address);
uint8_t eeprom_write_bytes_at_address(uint16_t address, uint8_t num_bytes, uint8_t buff[]);

