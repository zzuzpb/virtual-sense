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

#include "contiki-conf.h"
#include "board.h"
#include "gpio.h"

#define SPI_CE_SET()	GPIO_SET_PIN(SPI_CE_PORT, (1 << SPI_CE_PIN));
#define SPI_CE_CLR()	GPIO_CLR_PIN(SPI_CE_PORT, (1 << SPI_CE_PIN));

#define SPI_CLK_SET()	GPIO_SET_PIN(SPI_CLK_PORT, (1 << SPI_CLK_PIN));
#define SPI_CLK_CLR()	GPIO_CLR_PIN(SPI_CLK_PORT, (1 << SPI_CLK_PIN));

#define SPI_MOSI_SET()	GPIO_SET_PIN(SPI_MOSI_PORT, (1 << SPI_MOSI_PIN));
#define SPI_MOSI_CLR() 	GPIO_CLR_PIN(SPI_MOSI_PORT, (1 << SPI_MOSI_PIN));

#define SPI_MISO_READ()	GPIO_READ_PIN(SPI_MISO_PORT, (1 << SPI_MISO_PIN));

void spi_delay(uint8_t);
void spi_init(void);
void spi_write(uint8_t);
uint8_t spi_read(void);

void ce_set(void);
void ce_clr(void);
