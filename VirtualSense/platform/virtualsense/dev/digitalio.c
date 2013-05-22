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
#include "dev/pcf2123_spi.h"
#include "contiki.h"

PROCESS(digitalio_driver_process, "DIGITALIO_process");
static uint8_t flag = 10;
static void (*registered_call_back)(uint8_t port);

void init_digitalio_interface(void ( *callback)(uint8_t port)){
	registered_call_back = callback;
	process_start(&digitalio_driver_process, NULL);
}

/*---------------------------------------------------------------------------*/
void init_interrupt(uint8_t on_falling, uint16_t port){
	switch(port){
	case P0:
		 INT_PORT_DIR  &= ~P0BIT;
		 INT_PORT_REN  |=  P0BIT;
		 INT_PORT_IE   |=  P0BIT;
		 on_falling<1?(INT_PORT_IES &= ~P0BIT):(INT_PORT_IES |= P0BIT);
		break;
	case P1:
		 INT_PORT_DIR  &= ~P1BIT;
		 INT_PORT_REN  |=  P1BIT;
		 INT_PORT_IE   |=  P1BIT;
		 on_falling<1?(INT_PORT_IES &= ~P1BIT):(INT_PORT_IES |= P1BIT);
		break;
	case P2:
		 INT_PORT_DIR  &= ~P2BIT;
		 INT_PORT_REN  |=  P2BIT;
		 INT_PORT_IE   |=  P2BIT;
		 on_falling<1?(INT_PORT_IES &= ~P2BIT):(INT_PORT_IES |= P2BIT);
		break;
	case P3:
		 INT_PORT_DIR  &= ~P3BIT;
		 INT_PORT_REN  |=  P3BIT;
		 INT_PORT_IE   |=  P3BIT;
		 on_falling<1?(INT_PORT_IES &= ~P3BIT):(INT_PORT_IES |= P3BIT);
		break;
	case P4:
		 INT_PORT_DIR  &= ~P4BIT;
		 INT_PORT_REN  |=  P4BIT;
		 INT_PORT_IE   |=  P4BIT;
		 on_falling<1?(INT_PORT_IES &= ~P4BIT):(INT_PORT_IES |= P4BIT);
		break;
	case PRTC:
		INT_PORT_DIR  &= ~PRTCBIT;
		INT_PORT_REN  |=  PRTCBIT;
		INT_PORT_IE   |=  PRTCBIT;
		on_falling<1?(INT_PORT_IES &= ~PRTCBIT):(INT_PORT_IES |= PRTCBIT);
		break;
	default:
		break;
	}
}

/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
PROCESS_THREAD(digitalio_driver_process, ev, data)
{
  int len;
  PROCESS_BEGIN();

  //printf("DIGITALIO_process: started\n");

  while(1) {
    PROCESS_YIELD_UNTIL(ev == PROCESS_EVENT_POLL);
    //printf("DIGITALIO_process: calling callback\n");
    registered_call_back(flag);
  }
  PROCESS_END();
}

/*---------------------------------------------------------------------------*/
interrupt(PORT2_VECTOR)
     irq_p2(void)
{
	if(INT_PORT_IFG & P0BIT){
		flag = P0;
		process_poll(&digitalio_driver_process);
		INT_PORT_IFG &= ~P0BIT;
	}else if(INT_PORT_IFG & P1BIT){
		flag = P1;
		process_poll(&digitalio_driver_process);
		INT_PORT_IFG &= ~P1BIT;
	}else if(INT_PORT_IFG & P2BIT){
		flag = P2;
		process_poll(&digitalio_driver_process);
		INT_PORT_IFG &= ~P2BIT;
	}else if(INT_PORT_IFG & P3BIT){
		flag = P3;
		process_poll(&digitalio_driver_process);
		INT_PORT_IFG &= ~P3BIT;
	}else if(INT_PORT_IFG & P4BIT){
		flag = P4;
		process_poll(&digitalio_driver_process);
		INT_PORT_IFG &= ~P4BIT;
	}else if(INT_PORT_IFG & PRTCBIT){
		flag = PRTCBIT;
#ifdef PLATFORM_HAS_RTC_PCF2123
		RTC_clear_interrupt();
		RTC_disable_all_interrupts();
#endif
		LPM4_EXIT;
		INT_PORT_IFG &= ~PRTCBIT;
	}
}

/*---------------------------------------------------------------------------*/
