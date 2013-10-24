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
#include "platform-conf.h"

PROCESS(digitalio_driver_process, "DIGITALIO_process");
static uint8_t flag = 10;
static void (*registered_call_back)(uint8_t port);

void init_digitalio_interface(void ( *callback)(uint8_t port)){
	registered_call_back = callback;
	process_start(&digitalio_driver_process, NULL);
}

/*---------------------------------------------------------------------------*/
void init_interrupt(uint8_t on_falling, uint16_t port){
	dint();
	switch(port){
	case INT_P0:
		 INT_PORT_DIR  &= ~INT_P0BIT;
		 INT_PORT_OUT  &= ~INT_P0BIT;
		 INT_PORT_REN  |=  INT_P0BIT;
		 INT_PORT_IE   |=  INT_P0BIT;
		 on_falling<1?(INT_PORT_IES &= ~INT_P0BIT):(INT_PORT_IES |= INT_P0BIT);
		 INT_PORT_IFG = 0; // to clean all interrupt flags; needed when initializing more then one port
		break;
	case INT_P1:
		 INT_PORT_DIR  &= ~INT_P1BIT;
		 INT_PORT_OUT  &= ~INT_P1BIT;
		 INT_PORT_REN  |=  INT_P1BIT;
		 INT_PORT_IE   |=  INT_P1BIT;
		 on_falling<1?(INT_PORT_IES &= ~INT_P1BIT):(INT_PORT_IES |= INT_P1BIT);
		 INT_PORT_IFG = 0; // to clean all interrupt flags; needed when initializing more then one port
		break;
	case INT_P2:
		 INT_PORT_DIR  &= ~INT_P2BIT;
		 INT_PORT_OUT  &= ~INT_P2BIT;
		 INT_PORT_REN  |=  INT_P2BIT;
		 INT_PORT_IE   |=  INT_P2BIT;
		 on_falling<1?(INT_PORT_IES &= ~INT_P2BIT):(INT_PORT_IES |= INT_P2BIT);
		 INT_PORT_IFG = 0; // to clean all interrupt flags; needed when initializing more then one port
		break;
	case INT_P3:
		 INT_PORT_DIR  &= ~INT_P3BIT;
		 INT_PORT_OUT  &= ~INT_P3BIT;
		 INT_PORT_REN  |=  INT_P3BIT;
		 INT_PORT_IE   |=  INT_P3BIT;
		 on_falling<1?(INT_PORT_IES &= ~INT_P3BIT):(INT_PORT_IES |= INT_P3BIT);
		 INT_PORT_IFG = 0; // to clean all interrupt flags; needed when initializing more then one port
		break;
	case INT_P4:
		 INT_PORT_DIR  &= ~INT_P4BIT;
		 INT_PORT_OUT  &= ~INT_P4BIT;
		 INT_PORT_REN  |=  INT_P4BIT;
		 INT_PORT_IE   |=  INT_P4BIT;
		 on_falling<1?(INT_PORT_IES &= ~INT_P4BIT):(INT_PORT_IES |= INT_P4BIT);
		 INT_PORT_IFG = 0; // to clean all interrupt flags; needed when initializing more then one port
		break;
	case INT_PRTC:
		INT_PORT_DIR  &= ~INT_PRTCBIT;
		INT_PORT_OUT  &= ~INT_PRTCBIT;
		INT_PORT_REN  |=  INT_PRTCBIT;
		INT_PORT_IE   |=  INT_PRTCBIT;
		on_falling<1?(INT_PORT_IES &= ~INT_PRTCBIT):(INT_PORT_IES |= INT_PRTCBIT);
		INT_PORT_IFG = 0; // to clean all interrupt flags; needed when initializing more then one port
		break;
	default:
		break;
	}
	eint();
}

/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
PROCESS_THREAD(digitalio_driver_process, ev, data)
{
  int len;
  PROCESS_BEGIN();


  //printf("DIGITALIO_process: started\n"); // ------------------- LINK BUG

  while(1) {
    PROCESS_YIELD_UNTIL(ev == PROCESS_EVENT_POLL);
    //printf("DIGITALIO_process: calling callback\n");
    //printf("");
    registered_call_back(flag);
  }
  PROCESS_END();
}

/*---------------------------------------------------------------------------*/
interrupt(PORT2_VECTOR)
     irq_p2(void)
{
	if(INT_PORT_IFG & INT_P0BIT){
		flag = INT_P0;
		process_poll(&digitalio_driver_process);
		INT_PORT_IFG &= ~INT_P0BIT;
	}else if(INT_PORT_IFG & INT_P1BIT){
		flag = INT_P1;
		process_poll(&digitalio_driver_process);
		INT_PORT_IFG &= ~INT_P1BIT;
	}else if(INT_PORT_IFG & INT_P2BIT){
		flag = INT_P2;
		process_poll(&digitalio_driver_process);
		INT_PORT_IFG &= ~INT_P2BIT;
	}else if(INT_PORT_IFG & INT_P3BIT){
		flag = INT_P3;
		process_poll(&digitalio_driver_process);
		INT_PORT_IFG &= ~INT_P3BIT;
	}else if(INT_PORT_IFG & INT_P4BIT){
		flag = INT_P4;
		process_poll(&digitalio_driver_process);
		INT_PORT_IFG &= ~INT_P4BIT;
	}else if(INT_PORT_IFG & INT_PRTCBIT){
		flag = INT_PRTCBIT;
#ifdef PLATFORM_HAS_RTC_PCF2123
		RTC_clear_interrupt();
		RTC_disable_all_interrupts();
#endif
		LPM4_EXIT;
		INT_PORT_IFG &= ~INT_PRTCBIT;
	}
}


/*---------------------------------------------------------------------------*/
void init_dio(uint8_t direction, uint16_t port){
	switch(port){
	case DIO_P0:
		direction>0?(DIO_PORT_0_DIR  &= ~DIO_P0BIT):(DIO_PORT_0_DIR  |= DIO_P0BIT);
		break;
	case DIO_P1:
		direction>0?(DIO_PORT_1_DIR  &= ~DIO_P1BIT):(DIO_PORT_1_DIR  |= DIO_P1BIT);
		break;
	case DIO_P2:
		direction>0?(DIO_PORT_2_DIR  &= ~DIO_P2BIT):(DIO_PORT_2_DIR  |= DIO_P2BIT);
		break;
	case DIO_P3:
		direction>0?(DIO_PORT_3_DIR  &= ~DIO_P3BIT):(DIO_PORT_3_DIR  |= DIO_P3BIT);
		break;
	default:
		break;
	}
}

/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/

uint8_t read_pin(uint16_t port){
	uint8_t ret = 0;
	switch(port){
		case DIO_P0:
			ret = (DIO_PORT_0_IN & DIO_P0BIT);
			break;
		case DIO_P1:
			ret = (DIO_PORT_1_IN & DIO_P1BIT);
			break;
		case DIO_P2:
			ret = (DIO_PORT_2_IN & DIO_P2BIT);
			break;
		case DIO_P3:
			ret = (DIO_PORT_3_IN & DIO_P3BIT);
			break;
		default:
			break;
		}
	return ret;
}

/*---------------------------------------------------------------------------*/
void write_pin(uint16_t port, uint8_t value){
	switch(port){
		case DIO_P0:
			value > 0 ? (DIO_PORT_0_OUT |= DIO_P0BIT):(DIO_PORT_0_OUT &= ~DIO_P0BIT);
			break;
		case DIO_P1:
			value > 0 ? (DIO_PORT_1_OUT |= DIO_P1BIT):(DIO_PORT_1_OUT &= ~DIO_P1BIT);
			break;
		case DIO_P2:
			value > 0 ? (DIO_PORT_2_OUT |= DIO_P2BIT):(DIO_PORT_2_OUT &= ~DIO_P2BIT);
			break;
		case DIO_P3:
			value > 0 ? (DIO_PORT_3_OUT |= DIO_P3BIT):(DIO_PORT_3_OUT &= ~DIO_P3BIT);
			break;
		default:
			break;
		}
}
