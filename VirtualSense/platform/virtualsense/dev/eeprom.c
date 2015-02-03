/*
 *  eeprom.c
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
 * The eeprom wrapper. Uses 24AA1025 or null_eeprom.
 *
 * @author Emanuele Lattanzi
 *
 */


#include "dev/eeprom.h"
#include "dev/board.h"



#ifdef PLATFORM_HAS_EEPROM
#include "eeprom_24AA1025.h"
#else
#include "null_eeprom.h"
#endif

#ifdef PLATFORM_HAS_EEPROM
static const unsigned char nullb[PAGE_SIZE] = {0};
#endif


void eeprom_write(eeprom_addr_t addr, unsigned char *buf, int size){
#ifdef PLATFORM_HAS_EEPROM
	write_sequential_24AA1025(EEPROM_ADDRESS, (uint16_t)(addr+BASE_EEPROM), (uint16_t)size, buf);
#else
	unsigned long int current_address = BASE_EEPROM + (unsigned long int)addr;
	//far_rom_erase_block(current_address, (unsigned int) size);
	/*unsigned int i = 0;
	for (i = 0; i < size; i++){
			data20_write_char(current_address, buf[i]);
			current_address++;
			printf("%x ", buf[i]);
		}
		printf("\n");
		*/
	data20_write_block(current_address, (unsigned int) size, (void *)buf);
#endif
}


/**
 * Read data from the EEPROM.
 *
 * This function reads a number of bytes from the specified address in
 * EEPROM and into a buffer in memory.
 *
 * \param addr The address in EEPROM from which the data should be read.
 *
 * \param buf A pointer to the buffer to which the data should be stored.
 *
 * \param size The number of bytes to read.
 *
 *
 */
void eeprom_read(eeprom_addr_t addr, unsigned char *buf, int size){
#ifdef PLATFORM_HAS_EEPROM

	read_sequential_24AA1025(EEPROM_ADDRESS, (uint16_t)(addr+BASE_EEPROM), size, buf);
#else
	unsigned long int current_address = BASE_EEPROM + (unsigned long int)addr;
	printf("read %ld size %d\n", current_address, size);
	data20_read_block(current_address, (unsigned int) size, (void *)buf);
#endif
}



/**
 * Initialize the EEPROM module
 *
 * This function initializes the EEPROM module and is called from the
 * bootup code.
 *
 */
void eeprom_init(void){
#ifdef PLATFORM_HAS_EEPROM
	///i2c_init();
#endif
}


void
eeprom_erase(unsigned int current_address, unsigned char size)
{
#ifdef PLATFORM_HAS_EEPROM
  eeprom_write(current_address, (unsigned char *)nullb, size);
#else
  unsigned long int address = BASE_EEPROM + (unsigned long int)current_address;
  far_rom_erase_block(address, (unsigned int) size);
#endif
}
