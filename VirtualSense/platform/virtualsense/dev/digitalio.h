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
#include <msp430.h>

#ifndef __DIGITALIO_H__
#define __DIGITALIO_H__

/* interrupt port definitions */
#define ON_RAISING			0
#define ON_FALLING			1

#define INT_PORT_DIR 			P2DIR
#define INT_PORT_REN			P2REN
#define INT_PORT_IE				P2IE
#define INT_PORT_IES 			P2IES
#define INT_PORT_IFG			P2IFG

#define INT_P0BIT				BIT0
#define INT_P1BIT				BIT3
#define INT_P2BIT				BIT5
#define INT_P3BIT				BIT6
#define INT_P4BIT				BIT7
#define INT_PRTCBIT			    BIT4

#define INT_P0					0
#define INT_P1					1
#define INT_P2					2
#define INT_P3					3
#define INT_P4					4
#define INT_PRTC			    5

/* digital I/O port definitions */
#define DIO_PORT_0_DIR 			P6DIR
#define DIO_PORT_1_DIR 			P6DIR
#define DIO_PORT_2_DIR 			P6DIR
#define DIO_PORT_3_DIR 			P6DIR

#define DIO_PORT_0_IN 			P6IN
#define DIO_PORT_1_IN 			P6IN
#define DIO_PORT_2_IN 			P6IN
#define DIO_PORT_3_IN 			P6IN

#define DIO_PORT_0_OUT 			P6OUT
#define DIO_PORT_1_OUT 			P6OUT
#define DIO_PORT_2_OUT 			P6OUT
#define DIO_PORT_3_OUT 			P6OUT

#define DIO_P0BIT				BIT0
#define DIO_P1BIT				BIT2
#define DIO_P2BIT				BIT3
#define DIO_P3BIT				BIT4

#define DIO_P0					0
#define DIO_P1					1
#define DIO_P2					2
#define DIO_P3					3



void init_digitalio_interface(void ( *callback)(uint8_t port));
void init_interrupt(uint8_t on_falling, uint16_t port);
void init_dio(uint8_t direction, uint16_t port);

uint8_t read_pin(uint16_t port);
void write_pin(uint16_t port, uint8_t value);



#endif /* __DIGITALIO_H__ */
