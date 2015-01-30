/*
 *  ssi.h
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

#ifndef SSI_H
#define SSI_H

#include "contiki-conf.h"
#include "board.h"

void ssi0_spi_init(uint32_t, uint8_t);
void ssi0_spi_enable();
void ssi0_spi_disable();


void ssi0_spi_write(uint8_t);
uint8_t ssi0_spi_read(void);

void ssi0_spi_ce_set(uint32_t, uint8_t);
void ssi0_spi_ce_clr(uint32_t, uint8_t);


#endif /* SSI_H */

