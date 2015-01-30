/*
 *  ssi.c
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
 * The SSI communication system.
 *
 * @author Matteo Dromedari
 *
 */

#include <stdbool.h>
#include <stdint.h>

#include "contiki.h"
#include "ssi.h"
#include "board.h"

#include "lib/hw_memmap.h"
#include "lib/hw_ioc.h"
#include "lib/_gpio.h"
#include "lib/_ioc.h"
#include "lib/_ssi.h"
#include "lib/_sys_ctrl.h"



void ssi0_spi_init(uint32_t ui32CEPort, uint8_t ui8CEPins) {
    //
    // Set IO clock to the same as system clock
    //
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

	IOCPinConfigPeriphOutput(SSI0_SPI_PORT, SSI0_SPI_CLK_PIN,
							 IOC_MUX_OUT_SEL_SSI0_CLKOUT);

	//IOCPinConfigPeriphOutput(EXAMPLE_GPIO_SSI_BASE, EXAMPLE_PIN_SSI_FSS,
	//						 IOC_MUX_OUT_SEL_SSI0_FSSOUT);

	GPIOPinTypeGPIOOutput(ui32CEPort, ui8CEPins);
	GPIOPinWrite(ui32CEPort, ui8CEPins, ui8CEPins & 0x00);


	IOCPinConfigPeriphOutput(SSI0_SPI_PORT, SSI0_SPI_MOSI_PIN,
							 IOC_MUX_OUT_SEL_SSI0_TXD);

	IOCPinConfigPeriphInput(SSI0_SPI_PORT, SSI0_SPI_MISO_PIN,
							IOC_SSIRXD_SSI0);

	GPIOPinTypeSSI(SSI0_SPI_PORT, SSI0_SPI_CLK_PIN |
				   /*EXAMPLE_PIN_SSI_FSS |*/ SSI0_SPI_MOSI_PIN |
				   SSI0_SPI_MISO_PIN);
	//GPIOPinTypeSSI(EXAMPLE_GPIO_SSI_BASE, EXAMPLE_PIN_SSI_CLK |
		//		   EXAMPLE_PIN_SSI_RX | EXAMPLE_PIN_SSI_TX);

	//printf("clk: %d\n",SysCtrlClockGet());

    SSIConfigSetExpClk(SSI0_BASE, SysCtrlIOClockGet(), SSI_FRF_MOTO_MODE_0,
                       SSI_MODE_MASTER, SysCtrlClockGet()/1000, 8);

    //SSIDMAEnable(SSI0_BASE, SSI_DMA_RX);

    SSIEnable(SSI0_BASE);

    uint32_t pui32DataRx = 0;
    while(SSIDataGetNonBlocking(SSI0_BASE, &pui32DataRx))
   	{
   	}
}


void ssi0_spi_enable(){

}


void ssi0_spi_disable(){

}


void ssi0_spi_write(uint8_t write){

	uint32_t pui32DataTx = 0;

	pui32DataTx = (uint32_t)write;

	SSIDataPut(SSI0_BASE, pui32DataTx);

    while(SSIBusy(SSI0_BASE))
    {
    }
}


void ssi0_spi_ce_set(uint32_t ui32CEPort, uint8_t ui8CEPins){
	GPIOPinWrite(ui32CEPort, ui8CEPins, ui8CEPins);
}


void ssi0_spi_ce_clr(uint32_t ui32CEPort, uint8_t ui8CEPins){
	GPIOPinWrite(ui32CEPort, ui8CEPins, ui8CEPins & 0x00);
}


uint8_t ssi0_spi_read(void){

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
