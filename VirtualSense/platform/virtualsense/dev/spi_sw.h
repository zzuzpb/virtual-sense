/*
 *  spi_sw.h
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

#ifndef SPI_SW_H
#define SPI_SW_H

#include "contiki-conf.h"
#include "board.h"
#include "gpio.h"

#define SPI_CE_PORT			GPIO_B_BASE
#define SPI_CE_PIN			5
#define SPI_MISO_PORT		GPIO_B_BASE
#define SPI_MISO_PIN		1
#define SPI_MOSI_PORT		GPIO_B_BASE
#define SPI_MOSI_PIN		2
#define SPI_CLK_PORT		GPIO_B_BASE
#define SPI_CLK_PIN			3

#define SPI_SW_CE_SET()		GPIO_SET_PIN(SPI_CE_PORT, (1 << SPI_CE_PIN));
#define SPI_SW_CE_CLR()		GPIO_CLR_PIN(SPI_CE_PORT, (1 << SPI_CE_PIN));

#define SPI_SW_CLK_SET()	GPIO_SET_PIN(SPI_CLK_PORT, (1 << SPI_CLK_PIN));
#define SPI_SW_CLK_CLR()	GPIO_CLR_PIN(SPI_CLK_PORT, (1 << SPI_CLK_PIN));

#define SPI_SW_MOSI_SET()	GPIO_SET_PIN(SPI_MOSI_PORT, (1 << SPI_MOSI_PIN));
#define SPI_SW_MOSI_CLR() 	GPIO_CLR_PIN(SPI_MOSI_PORT, (1 << SPI_MOSI_PIN));

#define SPI_SW_MISO_READ()	GPIO_READ_PIN(SPI_MISO_PORT, (1 << SPI_MISO_PIN));

void spi_sw_delay(uint8_t);

void spi_sw_init(void);
void spi_sw_write(uint8_t);
uint8_t spi_sw_read(void);


#endif /* SPI_SW_H */
