/*
 *  spi_sw.c
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
 * The emulated SPI software communication system.
 *
 * @author Matteo Dromedari
 *
 */

#include "contiki.h"
#include "spi_sw.h"

#include <stdbool.h>
#include <stdint.h>
#include "lib/hw_memmap.h"
#include "lib/hw_ioc.h"
#include "lib/_gpio.h"
#include "lib/_ioc.h"
#include "lib/_ssi.h"
#include "lib/_sys_ctrl.h"


#define EXAMPLE_PIN_SSI_CLK             GPIO_PIN_3
#define EXAMPLE_PIN_SSI_FSS             GPIO_PIN_5
#define EXAMPLE_PIN_SSI_RX              GPIO_PIN_1
#define EXAMPLE_PIN_SSI_TX              GPIO_PIN_2
#define EXAMPLE_GPIO_SSI_BASE           GPIO_B_BASE


void spi_delay(uint8_t delay){
	uint8_t i = 0x00;
	uint8_t j = 0x00;

	for(i = 0; i < delay; i++)
		for(j = 0; j < 0xFF; j++)
			asm("   NOP");
}

void spi_init(void) {/*
	GPIO_SOFTWARE_CONTROL(SPI_CE_PORT, (1 << SPI_CE_PIN));
	GPIO_SET_OUTPUT(SPI_CE_PORT, (1 << SPI_CE_PIN));
	GPIO_CLR_PIN(SPI_CE_PORT, (1 << SPI_CE_PIN));

	GPIO_SOFTWARE_CONTROL(SPI_MISO_PORT, (1 << SPI_MISO_PIN));
	GPIO_SET_INPUT(SPI_MISO_PORT, (1 << SPI_MISO_PIN));

	GPIO_SOFTWARE_CONTROL(SPI_MOSI_PORT, (1 << SPI_MOSI_PIN));
	GPIO_SET_OUTPUT(SPI_MOSI_PORT, (1 << SPI_MOSI_PIN));
	GPIO_CLR_PIN(SPI_MOSI_PORT, (1 << SPI_MOSI_PIN));

	GPIO_SOFTWARE_CONTROL(SPI_CLK_PORT, (1 << SPI_CLK_PIN));
	GPIO_SET_OUTPUT(SPI_CLK_PORT, (1 << SPI_CLK_PIN));
	GPIO_CLR_PIN(SPI_CLK_PORT, (1 << SPI_CLK_PIN));*/

	uint32_t pui32DataRx = 0;

	//SysCtrlClockSet(false, false, SYS_CTRL_SYSDIV_500KHZ);
	SysCtrlIOClockSet(SysCtrlClockGet());
	//
	// The SSI0 peripheral must be enabled for use.
	//
	SysCtrlPeripheralEnable(SYS_CTRL_PERIPH_SSI0);

	//
	// Disable SSI function before configuring module
	//
	SSIDisable(SSI0_BASE);

	//
	// Set IO clock as SSI clock source
	//
	SSIClockSourceSet(SSI0_BASE, SSI_CLOCK_PIOSC);

	IOCPinConfigPeriphOutput(EXAMPLE_GPIO_SSI_BASE, EXAMPLE_PIN_SSI_CLK,
							 IOC_MUX_OUT_SEL_SSI0_CLKOUT);

	//IOCPinConfigPeriphOutput(EXAMPLE_GPIO_SSI_BASE, EXAMPLE_PIN_SSI_FSS,
	//						 IOC_MUX_OUT_SEL_SSI0_FSSOUT);

	GPIOPinTypeGPIOOutput(EXAMPLE_GPIO_SSI_BASE, EXAMPLE_PIN_SSI_FSS);
	GPIOPinWrite(EXAMPLE_GPIO_SSI_BASE, EXAMPLE_PIN_SSI_FSS, EXAMPLE_PIN_SSI_FSS & 0x00);


	IOCPinConfigPeriphOutput(EXAMPLE_GPIO_SSI_BASE, EXAMPLE_PIN_SSI_TX,
							 IOC_MUX_OUT_SEL_SSI0_TXD);

	IOCPinConfigPeriphInput(EXAMPLE_GPIO_SSI_BASE, EXAMPLE_PIN_SSI_RX,
							IOC_SSIRXD_SSI0);

	GPIOPinTypeSSI(EXAMPLE_GPIO_SSI_BASE, EXAMPLE_PIN_SSI_CLK |
				   /*EXAMPLE_PIN_SSI_FSS |*/ EXAMPLE_PIN_SSI_RX |
				   EXAMPLE_PIN_SSI_TX);
	//GPIOPinTypeSSI(EXAMPLE_GPIO_SSI_BASE, EXAMPLE_PIN_SSI_CLK |
		//		   EXAMPLE_PIN_SSI_RX | EXAMPLE_PIN_SSI_TX);

	//printf("clk: %d\n",SysCtrlClockGet());

    SSIConfigSetExpClk(SSI0_BASE, SysCtrlIOClockGet(), SSI_FRF_MOTO_MODE_0,
                       SSI_MODE_MASTER, SysCtrlClockGet()/1000, 8);

    //SSIDMAEnable(SSI0_BASE, SSI_DMA_RX);

    SSIEnable(SSI0_BASE);

    //uint32_t pui32DataRx = 0;

    while(SSIDataGetNonBlocking(SSI0_BASE, &pui32DataRx))
   	{
   	}

    //SSIDataGet(SSI0_BASE, &pui32DataRx);
    //SSIDataGet(SSI0_BASE, &pui32DataRx);
    //while(SSIDataGetNonBlocking(SSI0_BASE, &pui32DataRx))
    //{
    //}

    //SSIDisable(SSI0_BASE);
}


void spi_write(uint8_t write){/*
	SPI_CLK_CLR();
	SPI_MOSI_CLR();

	uint8_t i = 0x00;
	for(i = 0; i < 8; i++){
																// Prepare to clock out the Address byte
	    if(write & 0x80){                                      	// Check for a 1
	    	SPI_MOSI_SET();
	    }														// and set the MOSI line appropriately
	    else{
	    	SPI_MOSI_CLR();
	    }
	    spi_delay(0x0F);

	    SPI_CLK_SET();
	    write <<= 1; 											// Rotate to get the next bit
	    spi_delay(0x0F);												// Toggle the clock line

	    SPI_CLK_CLR();
	}*/
	/*uint32_t clkIO = SysCtrlIOClockGet();
	uint32_t clkSYS = SysCtrlClockGet();


	SysCtrlClockSet(false, false, SYS_CTRL_SYSDIV_500KHZ);
	SysCtrlIOClockSet(SYS_CTRL_SYSDIV_500KHZ);

	SSIConfigSetExpClk(SSI0_BASE, SysCtrlIOClockGet(), SSI_FRF_MOTO_MODE_0,
	                       SSI_MODE_MASTER, SysCtrlClockGet()/8, 8);

	SSIEnable(SSI0_BASE);*/

	//GPIOPinWrite(EXAMPLE_GPIO_SSI_BASE, EXAMPLE_PIN_SSI_FSS, EXAMPLE_PIN_SSI_FSS);
	uint32_t pui32DataTx = 0;

	pui32DataTx = (uint32_t)write;

	SSIDataPut(SSI0_BASE, pui32DataTx);

    while(SSIBusy(SSI0_BASE))
    {
    }

	//while(SSIBusy(SSI0_BASE))
	//{
	//}
	//GPIOPinWrite(EXAMPLE_GPIO_SSI_BASE, EXAMPLE_PIN_SSI_FSS, EXAMPLE_PIN_SSI_FSS & 0x00);

	//SSIDisable(SSI0_BASE);

	//SysCtrlClockSet(false, false, clkSYS);
	//SysCtrlIOClockSet(clkIO);
}

void ce_set(void){
	GPIOPinWrite(EXAMPLE_GPIO_SSI_BASE, EXAMPLE_PIN_SSI_FSS, EXAMPLE_PIN_SSI_FSS);
}

void ce_clr(void){
	GPIOPinWrite(EXAMPLE_GPIO_SSI_BASE, EXAMPLE_PIN_SSI_FSS, EXAMPLE_PIN_SSI_FSS & 0x00);
}

uint8_t spi_read(void){/*
	uint8_t ret = 0x00;
	uint8_t read = 0x00;
	uint8_t i = 0x00;

	for (i = 0; i < 8; i++){
		ret <<=1;                                               // Rotate the data
		spi_delay(0x0F);

		SPI_CLK_SET();
		read = SPI_MISO_READ();
		if(read & (1 << SPI_MISO_PIN))
			ret += 0x01;
		spi_delay(0x0F);
																// Raise the clock to clock the data out of the MAX7456
		SPI_CLK_CLR();

																// Raise the clock to clock the data out of the MAX7456
	                                        					// Read the data bit
																// Drop the clock ready for the next bit
	}                                                           // and loop back

	return ret;*/
	/*uint32_t clkIO = SysCtrlIOClockGet();
	uint32_t clkSYS = SysCtrlClockGet();


	SysCtrlClockSet(false, false, SYS_CTRL_SYSDIV_500KHZ);
	SysCtrlIOClockSet(SYS_CTRL_SYSDIV_500KHZ);

	SSIConfigSetExpClk(SSI0_BASE, SysCtrlIOClockGet(), SSI_FRF_MOTO_MODE_0,
						   SSI_MODE_MASTER, SysCtrlClockGet()/8, 8);

	SSIEnable(SSI0_BASE);*/

	//GPIOPinWrite(EXAMPLE_GPIO_SSI_BASE, EXAMPLE_PIN_SSI_FSS, EXAMPLE_PIN_SSI_FSS);

	//printf("leggo\n");

	// Empty receive FIFO
	uint32_t pui32DataRx = 0;
	while(SSIDataGetNonBlocking(SSI0_BASE, &pui32DataRx))
	{
	}

	// Write 0 for generate clock for read
	pui32DataRx = 0;
	SSIDataPut(SSI0_BASE, pui32DataRx);

	while(SSIBusy(SSI0_BASE))
	{
	}

	// Get read byte
	SSIDataGet(SSI0_BASE, &pui32DataRx);
	pui32DataRx &= 0x00FF;

	return (uint8_t)pui32DataRx;
}
