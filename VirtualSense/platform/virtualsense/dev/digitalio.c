/*
 *	digitalio.c
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
#include "dev/digitalio.h"
#include "contiki.h"
/*---------------------------------------------------------------------------*/
void init_interrupt(uint8_t on_falling, uint16_t port){
	switch(port){
	case 0:
		 INT_PORT_DIR  &= ~(P0);
		 INT_PORT_REN  |= P0;			               // Disable P2.6 internal resistance
		 INT_PORT_IE   |= P0;                           // P2.6 interrupt enabled
		 INT_PORT_IES  |= P0;                           // P2.6  Hi/Lo edge
		break;
	case 1:
		break;
	case 2:
		break;
	case 3:
		break;
	case 4:
		break;
	default:
		break;
	}
}

/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
