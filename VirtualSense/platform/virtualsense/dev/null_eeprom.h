/*
 *  null_eeprom.h
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
 * The null_eeprom that emulates eeprom by means of CPU far flash.
 *
 * @author Emanuele Lattanzi
 *
 */


void data20_write_char(unsigned long int address, unsigned char value);
void data20_write_word(unsigned long int address, unsigned int value);
void data20_write_block(unsigned long int address, unsigned int size, void *src_address);
unsigned char data20_read_char(unsigned long int address);
unsigned int data20_read_word(unsigned long int address);
void data20_read_block(unsigned long int address, unsigned int size, void *dest_address);
