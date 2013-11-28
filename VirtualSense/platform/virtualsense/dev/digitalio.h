/*
 *	digitalio.h
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
 * Digital I/O access definitions.
 *
 * @author Emanuele Lattanzi
 *
 */

#ifndef __DIGITALIO_H__
#define __DIGITALIO_H__

/* interrupt port definitions */
#define ON_RAISING			0
#define ON_FALLING			1

void init_digitalio_interface(void ( *callback)(uint8_t port));
void init_interrupt(uint8_t on_falling, uint16_t port);
void init_dio(uint8_t direction, uint16_t port);

uint8_t read_pin(uint16_t port);
void write_pin(uint16_t port, uint8_t value);



#endif /* __DIGITALIO_H__ */
