/*
 *	javax_virtualsense_bus_digitalio_SPI.c
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
 * SPI bus native methods.
 *
 * @author Matteo Dromedari
 *
 */

#include "common/execution/execution.h"
#include "common/array.h"
#include "common/darjeeling.h"
#include "node-id.h"

#include "contiki.h"
#include "power-interface.h"

#include "dev/ssi.h"
#include "dev/board.h"


#include "base_definitions.h"



void javax_virtualsense_digitalio_bus_SPI_void__init_short(){
	uint16_t ce = dj_exec_stackPopShort();

	switch(ce){
		case CE0:
			ssi0_spi_init(SSI0_SPI_CE0_PORT, SSI0_SPI_CE0_PIN);
			break;
		case CE1:
			ssi0_spi_init(SSI0_SPI_CE1_PORT, SSI0_SPI_CE1_PIN);
			break;
		case CE2:
			ssi0_spi_init(SSI0_SPI_CE2_PORT, SSI0_SPI_CE2_PIN);
			break;
		default:
			break;
	}
}
void javax_virtualsense_digitalio_bus_SPI_void__enable(){
	ssi0_spi_enable();
}
void javax_virtualsense_digitalio_bus_SPI_void__disable(){
	ssi0_spi_disable();
}
void javax_virtualsense_digitalio_bus_SPI_void__setCE_short(){
	uint16_t ce = dj_exec_stackPopShort();

	switch(ce){
		case CE0:
			ssi0_spi_ce_set(SSI0_SPI_CE0_PORT, SSI0_SPI_CE0_PIN);
			break;
		case CE1:
			ssi0_spi_ce_set(SSI0_SPI_CE1_PORT, SSI0_SPI_CE1_PIN);
			break;
		case CE2:
			ssi0_spi_ce_set(SSI0_SPI_CE2_PORT, SSI0_SPI_CE2_PIN);
			break;
		default:
			break;
	}
}
void javax_virtualsense_digitalio_bus_SPI_void__clearCE_short(){
	uint16_t ce = dj_exec_stackPopShort();

	switch(ce){
		case CE0:
			ssi0_spi_ce_clr(SSI0_SPI_CE0_PORT, SSI0_SPI_CE0_PIN);
			break;
		case CE1:
			ssi0_spi_ce_clr(SSI0_SPI_CE1_PORT, SSI0_SPI_CE1_PIN);
			break;
		case CE2:
			ssi0_spi_ce_clr(SSI0_SPI_CE2_PORT, SSI0_SPI_CE2_PIN);
			break;
		default:
			break;
	}
}
void javax_virtualsense_digitalio_bus_SPI_byte__read(){
	uint16_t ret = (uint16_t)ssi0_spi_read();
	dj_exec_stackPushShort(ret);
}
void javax_virtualsense_digitalio_bus_SPI_boolean__write_byte(){
	uint16_t byte = dj_exec_stackPopShort();
	ssi0_spi_write((uint8_t)byte);
	//uint16_t ret = (uint16_t)ssi0_spi_write((uint8_t)byte);	// TODO MET Add return value for function read and write spi
	dj_exec_stackPushShort(0x0001);
}
