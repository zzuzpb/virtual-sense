
#include <msp430.h>
#include "dev/eeprom.h"
#include "dev/i2c_eeprom_24AA512.h"
#include "dev/leds.h"
#include "eeprom_24AA512.h"



static const unsigned char nullb[PAGE_SIZE] = {0};
unsigned char eeprom_address;

void eeprom_write(eeprom_addr_t addr, unsigned char *buf, int size){
	write_sequential_24AA512(EEPROM_ADDRESS, (uint16_t)addr, (uint16_t)size, buf);
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
	read_sequential_24AA512(EEPROM_ADDRESS, (uint16_t)addr, size, buf);
}



/**
 * Initialize the EEPROM module
 *
 * This function initializes the EEPROM module and is called from the
 * bootup code.
 *
 */
void eeprom_init(void){
}


void
eeprom_erase(unsigned int current_address, unsigned char size)
{
  eeprom_write(current_address, (unsigned char *)nullb, size);
}

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
	 while(! is_idle_24AA512(dev_address))
	 		 __delay_cycles(50);

	 i2c_start();

	 i2c_write((dev_address <<1));
	 i2c_write((address >> 8));
	 i2c_write((address & 0xff));

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
	 uint16_t first_page_offset = PAGE_SIZE - (address % PAGE_SIZE);
	 uint16_t i = 0;
	 uint16_t current_address = address;
	 uint16_t num_bytes = size;
	 uint16_t temp_num = 0;
	 uint16_t bytes_writed = 0;

	 // write enable
	 WRITE_ENABLE();
 	 i2c_enable();
 	 while(! is_idle_24AA512(dev_address))
 		 __delay_cycles(50);



 	 i2c_start();
 	 i2c_write((dev_address<<1));
 	 i2c_write((current_address >> 8));
 	 i2c_write((current_address & 0xff));

 	 while(i < first_page_offset){ // writing in the first page
 		 i2c_write(data[i]);
 		 i++;
 		 bytes_writed++;
 	 }
 	 i2c_stop();

 	// write other pages
 	current_address+=first_page_offset;
 	num_bytes-=first_page_offset;

 	while(num_bytes > 0){
 		if(num_bytes > PAGE_SIZE)
 			temp_num = PAGE_SIZE;
 		else
 			temp_num = num_bytes;
 		// write a new page
 		 while(! is_idle_24AA512(dev_address))
 		 	 		 __delay_cycles(50);
 		i2c_start();
 		i2c_write((dev_address<<1));
 		i2c_write((current_address >> 8));
 		i2c_write((current_address & 0xff));
 		for(i=0; i < temp_num; i++){
 			i2c_write(data[bytes_writed]);
 			bytes_writed++;
 		}
 		i2c_stop();
 		current_address+=temp_num;
 		num_bytes-=temp_num;
 	}


 	 i2c_disable();
 	WRITE_PROTECT();

  }

uint8_t is_idle_24AA512(uint8_t dev_address){
	 i2c_start();
	 return i2c_write((dev_address <<1));
}
