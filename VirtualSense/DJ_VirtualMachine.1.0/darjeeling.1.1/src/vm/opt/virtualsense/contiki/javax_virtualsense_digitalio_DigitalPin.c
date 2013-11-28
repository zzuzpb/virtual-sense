/*
 *	javax_virtualsense_digitalio_InterruptPin.c
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
 * DigitalIO InterruptPin native methods.
 *
 * @author Emanuele Lattanzi
 *
 */
#include <stdint.h>

#include "common/execution/execution.h"
#include "common/array.h"
#include "common/darjeeling.h"
#include "node-id.h"

#include "contiki.h"
#include "power-interface.h"

#include "dev/digitalio.h"
#include "dev/board.h"


#include "base_definitions.h"

void javax_virtualsense_digitalio_DigitalPin_void__createDigitalPin_boolean_short(){
	uint16_t port = dj_exec_stackPopShort();
	uint16_t onInput = dj_exec_stackPopShort();
	init_dio((uint8_t)onInput, port);
}

void javax_virtualsense_digitalio_DigitalPin_void__write_short_boolean(){
	uint16_t value = dj_exec_stackPopShort();
	uint16_t port = dj_exec_stackPopShort();
	write_pin(port, value);

}
void javax_virtualsense_digitalio_DigitalPin_boolean__read_short(){
	uint16_t port = dj_exec_stackPopShort();
	uint8_t value = read_pin(port);
	dj_exec_stackPushShort((uint16_t)value);
}
