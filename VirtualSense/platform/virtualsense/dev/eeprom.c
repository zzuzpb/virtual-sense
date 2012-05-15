
#include <msp430.h>
#include "dev/eeprom.h"
#include "platform-conf.h"



#ifdef PLATFORM_HAS_EEPROM
#include "eeprom_24AA512.h"
#else
#include "null_eeprom.h"
#endif

#ifdef PLATFORM_HAS_EEPROM
static const unsigned char nullb[PAGE_SIZE] = {0};
#endif


void eeprom_write(eeprom_addr_t addr, unsigned char *buf, int size){
#ifdef PLATFORM_HAS_EEPROM
	write_sequential_24AA512(EEPROM_ADDRESS, (uint16_t)(addr+BASE_EEPROM), (uint16_t)size, buf);
#else
	unsigned long int current_address = BASE_EEPROM + (unsigned long int)addr;
	//far_rom_erase_block(current_address, (unsigned int) size);
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

	read_sequential_24AA512(EEPROM_ADDRESS, (uint16_t)(addr+BASE_EEPROM), size, buf);
#else
	unsigned long int current_address = BASE_EEPROM + (unsigned long int)addr;
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
	i2c_eeprom_24AA512_init();
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
