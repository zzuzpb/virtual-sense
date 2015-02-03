/*  eeprom_24AA512.c
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


#include "board.h"

#include "i2c.h"
#include "eeprom_24AA512.h"


#define DEBUG 1
#if DEBUG
#include <stdio.h>
#define PRINTF(...) printf(__VA_ARGS__)
#define PRINTDEBUG(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#define PRINTDEBUG(...)
#endif

void EEPROM_POWER_UP(){
	GPIOPinWrite(EEPROM_POWER_PORT, EEPROM_POWER_PIN, EEPROM_POWER_PIN);
}

void EEPROM_POWER_DOWN(){
	GPIOPinWrite(EEPROM_POWER_PORT, EEPROM_POWER_PIN, EEPROM_POWER_PIN & 0x00);
}

void init_24AA512(){

	// Configure eeprom power pin and shut down it
	GPIOPinTypeGPIOOutput(EEPROM_POWER_PORT, EEPROM_POWER_PIN);
	EEPROM_POWER_UP();
	//EEPROM_POWER_DOWN();

	// Init i2c bus
	i2c_init();
}

uint8_t isIdle_24AA512(){

	uint32_t isIdle = 0x00;

	I2CMasterSlaveAddrSet(EEPROM_ADDRESS, false);
	I2CMasterControl(I2C_MASTER_CMD_BURST_SEND_START);
	while(I2CMasterBusy());
	isIdle = I2CMasterErr();
	//I2CMasterControl(I2C_MASTER_CMD_BURST_SEND_ERROR_STOP);
	//while(I2CMasterBusy());

	return (uint8_t)isIdle;
}

void waitForIdle_24AA512(){

	PRINTF("24AA512: wait for eeprom...");
	while(isIdle_24AA512()){
		PRINTF(".");
	}
	PRINTF("eeprom up!\n");
}

uint8_t read_24AA512(uint8_t *ui8Data, uint16_t ui16BaseAdr, uint8_t ui8ByteToRead){

	uint8_t readBytes = 0;
	uint8_t j = 0;
	uint32_t adrHi = (uint32_t)(ui16BaseAdr >> 8);
	uint32_t adrLo = (uint32_t)(ui16BaseAdr & 0x00FF);

	// Power eeprom and wait is up
	EEPROM_POWER_UP();
	waitForIdle_24AA512();

	// Set I2Cmodule in write mode to eeprom address
	I2CMasterSlaveAddrSet(EEPROM_ADDRESS, false);

	// Write the base address where start read eeprom
	I2CMasterDataPut(adrHi);
	I2CMasterControl(I2C_MASTER_CMD_BURST_SEND_START);
	while(I2CMasterBusy());
	if(I2CMasterErr() != I2C_MASTER_ERR_NONE){
		PRINTF("24AA512: error on read (write adrHi)\n");
	}

	I2CMasterDataPut(adrLo);
	I2CMasterControl(I2C_MASTER_CMD_BURST_SEND_CONT);
	while(I2CMasterBusy());
	if(I2CMasterErr() != I2C_MASTER_ERR_NONE){
		PRINTF("24AA512: error on read (write adrLo)\n");
	}

	// Set I2Cmodule in read mode to eeprom address
	I2CMasterSlaveAddrSet(EEPROM_ADDRESS, true);

	if(ui8ByteToRead == 1){
		// Read a single byte from eeprom
		I2CMasterControl(I2C_MASTER_CMD_SINGLE_RECEIVE);
		while(I2CMasterBusy());
		if(I2CMasterErr() == I2C_MASTER_ERR_NONE){
			*ui8Data = I2CMasterDataGet();
			readBytes = 1;
		}else{
			PRINTF("24AA512: error on read (read single byte)\n");
		}
	}else{
		// Read a burst of byte
		I2CMasterControl(I2C_MASTER_CMD_BURST_RECEIVE_START);
		while(I2CMasterBusy());
		if(I2CMasterErr() == I2C_MASTER_ERR_NONE){
			ui8Data[0] = I2CMasterDataGet();
			readBytes = 1;
		}else{
			PRINTF("24AA512: error on read (read first byte)\n");
		}

		for(j = readBytes; j < (ui8ByteToRead-1); j++){
			I2CMasterControl(I2C_MASTER_CMD_BURST_RECEIVE_CONT);
			while(I2CMasterBusy());
			if(I2CMasterErr() == I2C_MASTER_ERR_NONE){
				ui8Data[readBytes] = I2CMasterDataGet();
				readBytes++;
			}else{
				PRINTF("24AA512: error on read (read byte %d)\n", readBytes);
			}
		}

		I2CMasterControl(I2C_MASTER_CMD_BURST_RECEIVE_FINISH);
		while(I2CMasterBusy());
		if(I2CMasterErr() == I2C_MASTER_ERR_NONE){
			ui8Data[readBytes] = I2CMasterDataGet();
			readBytes++;
		}else{
			PRINTF("24AA512: error on read (read last byte)\n");
		}
	}

	// Power down eeprom
	EEPROM_POWER_DOWN();

	return readBytes;
}

uint8_t write_24AA512(uint8_t *ui8Data, uint16_t ui16BaseAdr, uint8_t ui8BToWrite){

	uint8_t writeBytes = 0;
	uint8_t j = 0;
	uint32_t adrHi = (uint32_t)(ui16BaseAdr >> 8);
	uint32_t adrLo = (uint32_t)(ui16BaseAdr & 0x00FF);

	// Power eeprom and wait is up
	EEPROM_POWER_UP();
	waitForIdle_24AA512();

	// Set I2Cmodule in write mode to eeprom address
	I2CMasterSlaveAddrSet(EEPROM_ADDRESS, false);

	// Write the base address where start write eeprom
	I2CMasterDataPut(adrHi);
	I2CMasterControl(I2C_MASTER_CMD_BURST_SEND_START);
	while(I2CMasterBusy());
	if(I2CMasterErr() != I2C_MASTER_ERR_NONE){
		PRINTF("24AA512: error on write (write adrHi)\n");
	}

	I2CMasterDataPut(adrLo);
	I2CMasterControl(I2C_MASTER_CMD_BURST_SEND_CONT);
	while(I2CMasterBusy());
	if(I2CMasterErr() != I2C_MASTER_ERR_NONE){
		PRINTF("24AA512: error on write (write adrLo)\n");
	}

	// Write ui8BToWrite-1 byte on eeprom
	for(j = 0; j < ui8BToWrite-1; j++){
		I2CMasterDataPut(ui8Data[j]);
		I2CMasterControl(I2C_MASTER_CMD_BURST_SEND_CONT);
		while(I2CMasterBusy());
		if(I2CMasterErr() == I2C_MASTER_ERR_NONE){
			writeBytes++;
		}else{
			PRINTF("24AA512: error on write (write byte %d)\n", writeBytes);
		}
	}

	// Write the last byte on eeprom
	I2CMasterDataPut(ui8Data[ui8BToWrite-1]);
	I2CMasterControl(I2C_MASTER_CMD_BURST_SEND_FINISH);
	while(I2CMasterBusy());
	if(I2CMasterErr() == I2C_MASTER_ERR_NONE){
		writeBytes++;
	}else{
		PRINTF("24AA512: error on write (write last byte)\n");
	}

	// Wait eeprom writing time for power down
	waitForIdle_24AA512();
	EEPROM_POWER_DOWN();

	return writeBytes;
}

void test_24AA512(){
	 uint8_t read[128];
	 uint8_t write[128];
	 uint8_t i = 0x00;

	  for(i = 0x00; i < 128; i++){
		  write[i] = i;
	  }

	  printf("test 128 eeprom write: %d\n", write_24AA512(write, 0x0000, 128));
	  printf("test 128 eeprom read: %d\n", read_24AA512(read, 0x0000, 128));

	  printf("eeprom:\n");
	  for(i = 0x00; i < 128; i++){
		  printf("[%d]: %x - %s\n", i, read[i], (read[i] == write[i])?"OK":"FAIL");
	  }
}



