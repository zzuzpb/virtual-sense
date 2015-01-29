/*
 *  SI7020.c
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
 * The SI7020 temperature humidity sensor driver.
 *
 * @author Matteo Dromedari
 *
 */


#include "board.h"

#include "i2c.h"
#include "SI7020.h"


void SI7020_POWER_UP(){
	GPIOPinWrite(SI7020_POWER_PORT, SI7020_POWER_PIN, SI7020_POWER_PIN);
}

void SI7020_POWER_DOWN(){
	GPIOPinWrite(SI7020_POWER_PORT, SI7020_POWER_PIN, SI7020_POWER_PIN & 0x00);
}

void init_SI7020(){

	// Configure eeprom power pin and shut down it
	GPIOPinTypeGPIOOutput(SI7020_POWER_PORT, SI7020_POWER_PIN);
	SI7020_POWER_DOWN();

	// Init i2c bus
	i2c_init();
}

uint8_t isIdle_SI7020(){

	uint32_t isIdle = 0x00;

	I2CMasterSlaveAddrSet(SI7020_ADDRESS, true);
	I2CMasterControl(I2C_MASTER_CMD_BURST_SEND_START);
	while(I2CMasterBusy());
	isIdle = I2CMasterErr();
	I2CMasterControl(I2C_MASTER_CMD_BURST_SEND_ERROR_STOP);
	while(I2CMasterBusy());

	return (uint8_t)isIdle;
}

void waitForIdle_SI7020(){

	printf("SI7020: wait for sensor...");
	while(isIdle_SI7020()){
		printf(".");
	}
	printf("sensor up!\n");
}

uint16_t read_temp_SI7020(void){
	uint16_t temp = 0;

	// Power sensor and wait is up
	SI7020_POWER_UP();
	waitForIdle_SI7020();

	// Set I2Cmodule in write mode to SI7020 address
	I2CMasterSlaveAddrSet(SI7020_ADDRESS, false);

	// Write command for start temperature conversion
	I2CMasterDataPut(SI7020_READ_TEMP_NOHOLD);
	I2CMasterControl(I2C_MASTER_CMD_SINGLE_SEND);
	while(I2CMasterBusy());
	if(I2CMasterErr() != I2C_MASTER_ERR_NONE){
		printf("SI7020: error on write command (temp read)\n");
	}

	// Set I2Cmodule in read mode to SI7020 address
	I2CMasterSlaveAddrSet(SI7020_ADDRESS, true);

	// Wait for temperature conversion
	do{
		I2CMasterControl(I2C_MASTER_CMD_BURST_RECEIVE_START);
		while(I2CMasterBusy());
	}
	while(I2CMasterErr() != I2C_MASTER_ERR_NONE);

	// Read converted temperature value from SI7020
	uint16_t read = ((uint16_t)I2CMasterDataGet()) << 8;

	I2CMasterControl(I2C_MASTER_CMD_BURST_RECEIVE_FINISH);
	while(I2CMasterBusy());
	if(I2CMasterErr() == I2C_MASTER_ERR_NONE){
		read += (uint16_t)I2CMasterDataGet();
	}else{
		printf("SI7020: error on read temp (read last byte)\n");
	}

	// Conversion returned value in celsius degrees
	temp = (uint16_t)((((uint32_t)read * 21965)/ 81920)-4685);	// °C = ((175,72*16bit_value)/65536)-46,85

	// Power down sensor
	SI7020_POWER_DOWN();

	return temp;
}



uint16_t read_humid_SI7020(void){
	uint16_t humid = 0;

	// Power sensor and wait is up
	SI7020_POWER_UP();
	waitForIdle_SI7020();

	// Set I2Cmodule in write mode to SI7020 address
	I2CMasterSlaveAddrSet(SI7020_ADDRESS, false);

	// Write command for start humidity conversion
	I2CMasterDataPut(SI7020_READ_HUMID_NOHOLD);
	I2CMasterControl(I2C_MASTER_CMD_SINGLE_SEND);
	while(I2CMasterBusy());
	if(I2CMasterErr() != I2C_MASTER_ERR_NONE){
		printf("SI7020: error on write command (humid read)\n");
	}

	// Set I2Cmodule in read mode to SI7020 address
	I2CMasterSlaveAddrSet(SI7020_ADDRESS, true);

	// Wait for humidity conversion
	do{
		I2CMasterControl(I2C_MASTER_CMD_BURST_RECEIVE_START);
		while(I2CMasterBusy());
	}
	while(I2CMasterErr() != I2C_MASTER_ERR_NONE);

	// Read converted humidity value from SI7020
	uint16_t read = ((uint16_t)I2CMasterDataGet()) << 8;

	I2CMasterControl(I2C_MASTER_CMD_BURST_RECEIVE_FINISH);
	while(I2CMasterBusy());
	if(I2CMasterErr() == I2C_MASTER_ERR_NONE){
		read += (uint16_t)I2CMasterDataGet();
	}else{
		printf("SI7020: error on read hum (read last byte)\n");
	}

	// Conversion returned value in %RH
	humid = (uint16_t)((((uint32_t)read * 125)/ 65536)-6);		// %RH = ((16bit_value*125)/65536)-6

	// Power down sensor
	SI7020_POWER_DOWN();

	return humid;
}


#if 0
#define I2C_THSENSOR_ADDRESS		0x40
#define I2C_THSENSOR_READ_TEMP		0xF3
#define I2C_THSENSOR_READ_HUMID		0xF5

#define I2C_READ    1
/** defines the data direction (writing to I2C device) in i2c_start(),i2c_rep_start() */
#define I2C_WRITE   0

#define CSL_I2C_DATA_SIZE        (64)
#define CSL_I2C_OWN_ADDR         (0x2F)
//#define CSL_I2C_SYS_CLK          (12.228)
//#define CSL_I2C_SYS_CLK          (40)
//#define CSL_I2C_SYS_CLK          (60)
//#define CSL_I2C_SYS_CLK          (75)
#define CSL_I2C_SYS_CLK          (40)
#define CSL_I2C_BUS_FREQ         (10)
#define CSL_I2C_EEPROM_ADDR		 (0x50)
#define CSL_I2C_CODEC_ADDR		 (0x18)


extern PSP_Handle    hi2c;


void ths_Delay(){
	int d,e;
	for (d=0; d<0x555; d++)
		for (e=0; e<0xAAA; e++)
			asm(" NOP ");
}


Uint16 THS_ReadTemp() {
	Uint16 startStop = ((CSL_I2C_START) | (CSL_I2C_STOP));
	CSL_Status  status;
	Uint16 ret = 0x00;
	CSL_I2cSetup     i2cSetup;
	CSL_I2cConfig    i2cConfig;

	status = I2C_init(CSL_I2C0);

	if(status != CSL_SOK) {
		debug_printf("I2C Init Failed!!\n");
	}

	/* Setup I2C module */
	i2cSetup.addrMode    = CSL_I2C_ADDR_7BIT;
	i2cSetup.bitCount    = CSL_I2C_BC_8BITS;
	i2cSetup.loopBack    = CSL_I2C_LOOPBACK_DISABLE;
	i2cSetup.freeMode    = CSL_I2C_FREEMODE_DISABLE;
	i2cSetup.repeatMode  = CSL_I2C_REPEATMODE_DISABLE;
	i2cSetup.ownAddr     = CSL_I2C_OWN_ADDR;
	i2cSetup.sysInputClk = CSL_I2C_SYS_CLK;
	i2cSetup.i2cBusFreq  = CSL_I2C_BUS_FREQ;

	status = I2C_setup(&i2cSetup);
	if(status != CSL_SOK) {
		debug_printf("I2C Setup Failed!!\n");
	}

	// Write command to read temperature on SI7020
	Uint16 b = I2C_THSENSOR_READ_TEMP;
	status = I2C_write(&b, 1, I2C_THSENSOR_ADDRESS, TRUE, startStop, CSL_I2C_MAX_TIMEOUT);
	//debug_printf("Read status temp %d\n", status);
	ths_Delay();

	// Poll sensor for temperature sample (max 5 times)
	Uint16 read[2];
	int i = 0;
	do{
		status = I2C_read(read, 2, I2C_THSENSOR_ADDRESS, NULL, 0, TRUE, startStop, CSL_I2C_MAX_TIMEOUT, FALSE);
		//debug_printf("Read status after temp %d\n", status);
		ths_Delay();
		i++;
	}while(status == -200 && i < 5);

	Uint16 r = (read[0] << 8) + read[1];
	if(status == 0 && r > 0x00){
		// Conversion returned value
		ret = (Uint16)((((long)r * 21965)/ 81920)-4685);	// °C = ((175,72*16bit_value)/65536)-46,85
	}

	return ret;
}




Uint16 THS_ReadHumid() {
	Uint16 startStop = ((CSL_I2C_START) | (CSL_I2C_STOP));
	CSL_Status  status;
	Uint16 ret = 0x00;
	CSL_I2cSetup     i2cSetup;
	CSL_I2cConfig    i2cConfig;

	status = I2C_init(CSL_I2C0);

	if(status != CSL_SOK) {
		debug_printf("I2C Init Failed!!\n");
	}

	/* Setup I2C module */
	i2cSetup.addrMode    = CSL_I2C_ADDR_7BIT;
	i2cSetup.bitCount    = CSL_I2C_BC_8BITS;
	i2cSetup.loopBack    = CSL_I2C_LOOPBACK_DISABLE;
	i2cSetup.freeMode    = CSL_I2C_FREEMODE_DISABLE;
	i2cSetup.repeatMode  = CSL_I2C_REPEATMODE_DISABLE;
	i2cSetup.ownAddr     = CSL_I2C_OWN_ADDR;
	i2cSetup.sysInputClk = CSL_I2C_SYS_CLK;
	i2cSetup.i2cBusFreq  = CSL_I2C_BUS_FREQ;

	status = I2C_setup(&i2cSetup);
	if(status != CSL_SOK) {
		debug_printf("I2C Setup Failed!!\n");
	}

	// Write command to read humidity on SI7020
	Uint16 b = I2C_THSENSOR_READ_HUMID;
	status = I2C_write(&b, 1, I2C_THSENSOR_ADDRESS, TRUE, startStop, CSL_I2C_MAX_TIMEOUT);
	//debug_printf("Read status humid %d\n", status);
	ths_Delay();

	// Poll sensor for humidity sample (max 5 times)
	Uint16 read[2];
	int i = 0;
	do{
		status = I2C_read(read, 2, I2C_THSENSOR_ADDRESS, NULL, 0, TRUE, startStop, CSL_I2C_MAX_TIMEOUT, FALSE);
		//debug_printf("Read status after humid %d\n", status);
		ths_Delay();
		i++;
	}while(status == -200 && i < 5);

	Uint16 r = (read[0] << 8) + read[1];
	if(status == 0 && r > 0x00){
		// Conversion returned value in %RH
		ret = (Uint16)((((long)r * 125)/ 65536)-6);		// %RH = ((16bit_value*125)/65536)-6
	}

	return ret;
}








#if 0
void lcd_Init() {
	Uint16 startStop = ((CSL_I2C_START) | (CSL_I2C_STOP));
	CSL_Status  status;

	// Check if I2C display is just initialized
	Uint16 s = 0x00;
	status = I2C_write(&s, 1, I2C_DISPLAY_ADDRESS, TRUE, startStop, CSL_I2C_MAX_TIMEOUT);
	lcd_Delay();

	if(status == -200) {
		status = I2C_init(CSL_I2C0);

		if(status != CSL_SOK) {
			debug_printf("I2C Init Failed!!\n");
		}

		/* Setup I2C module */
		i2cSetup.addrMode    = CSL_I2C_ADDR_7BIT;
		i2cSetup.bitCount    = CSL_I2C_BC_8BITS;
		i2cSetup.loopBack    = CSL_I2C_LOOPBACK_DISABLE;
		i2cSetup.freeMode    = CSL_I2C_FREEMODE_DISABLE;
		i2cSetup.repeatMode  = CSL_I2C_REPEATMODE_DISABLE;
		i2cSetup.ownAddr     = CSL_I2C_OWN_ADDR;
		i2cSetup.sysInputClk = CSL_I2C_SYS_CLK;
		i2cSetup.i2cBusFreq  = CSL_I2C_BUS_FREQ;

		status = I2C_setup(&i2cSetup);
		if(status != CSL_SOK) {
			debug_printf("I2C Setup Failed!!\n");
		}

		Uint16 b = 0x00;
		status = I2C_write(&b, 1, 0x00, TRUE, startStop, CSL_I2C_MAX_TIMEOUT);
		lcd_Delay();

		Uint16 bToWrite[] = {0x00, 0x38, 0x39, 0x14, 0x74, 0x54, 0x6F, 0x0F, 0x01};
		status = I2C_write(bToWrite, 9, I2C_DISPLAY_ADDRESS, TRUE, startStop, CSL_I2C_MAX_TIMEOUT);
	}
	else {
		// Clear display
		Uint16 l[] = {0x00, 0x01};
		status = I2C_write(l, 2, I2C_DISPLAY_ADDRESS, TRUE, startStop, CSL_I2C_MAX_TIMEOUT);
	}
	lcd_Delay();
}


void lcd_Delay() {
	int d,e;

	for (d=0; d<0x555; d++)
		for (e=0; e<0xAAA; e++)
			asm(" NOP ");
}





/*************************************************************************
 Writes a string or char to the Midas MCCOG21605B6W-SPTLYI LCD display

 Input:   String to display as "Hello World!"

 Return:

*************************************************************************/
void LCD_Write(const char *format, ...){

	va_list aptr;
	int len;
	Int16 ret;

	va_start(aptr, format);
	len = vsprintf(buffer, format, aptr);

	Uint16 startStop = ((CSL_I2C_START) | (CSL_I2C_STOP));
	CSL_Status  status;

	lcd_Init();

	int t;
	Uint16 ustr[sizeof(buffer)+1];
	ustr[0] = 0x40;
	for(t = 0; t < len; t++)
	{
		ustr[t+1] = (Uint16)buffer[t];
	}

	status = I2C_write(ustr, (len <= 16)?(len+1):17, I2C_DISPLAY_ADDRESS, TRUE, startStop, CSL_I2C_MAX_TIMEOUT);
	lcd_Delay();

	if(len > 16) {
		Uint16 l[] = {0x00, 0xC0};
		status = I2C_write(l, 2, I2C_DISPLAY_ADDRESS, TRUE, startStop, CSL_I2C_MAX_TIMEOUT);
		lcd_Delay();

		ustr[16] = 0x40;
		status = I2C_write(ustr+16, len-15, I2C_DISPLAY_ADDRESS, TRUE, startStop, CSL_I2C_MAX_TIMEOUT);
		lcd_Delay();
	}

}

/*************************************************************************
 Clear the Midas MCCOG21605B6W-SPTLYI LCD display

 Input:

 Return:

*************************************************************************/
void LCD_Clear() {
	lcd_Init();
}
#endif
#endif
