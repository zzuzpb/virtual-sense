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

#define ON_RAISING			0
#define ON_FALLING			1
#define INTERRUP_PORT	 	P2OUT

#define INT_PORT_DIR 		P2DIR
#define INT_PORT_REN		P2REN
#define INT_PORT_IE			P2IE
#define INT_PORT_IES 		P2IES
#define INT_PORT_IFG		P2IFG

#define P0BIT				BIT0
#define P1BIT				BIT3
#define P2BIT				BIT5
#define P3BIT				BIT6
#define P4BIT				BIT7
#define PRTCBIT			    BIT4

#define P0					0
#define P1					1
#define P2					2
#define P3					3
#define P4					4
#define PRTC			    5




void init_digitalio_interface(void ( *callback)(uint8_t port));
void init_interrupt(uint8_t on_falling, uint16_t port);


#endif /* __DIGITALIO_H__ */
