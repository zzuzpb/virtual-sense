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
#include "platform-conf.h"


#include "base_definitions.h"

javax_virtualsense_digitalio_DigitalPin_void__createDigitalPin_boolean_short(){

}
javax_virtualsense_digitalio_DigitalPin_void__write_short_boolean(){

}
javax_virtualsense_digitalio_DigitalPin_boolean__read_short(){

}
/*void javax_virtualsense_digitalio_InterruptPin_void__createInterruptPin_boolean_short(){
	uint16_t falling = dj_exec_stackPopShort();
	uint16_t port = dj_exec_stackPopShort();
	init_interrupt((uint8_t)falling, port);
}

void javax_virtualsense_digitalio_InterruptPin_void__waitForInterrupt_short(){
	uint16_t port = dj_exec_stackPopShort();
	dj_thread *thread = dj_exec_getCurrentThread();
	thread->sem_id = port;
	thread->status = THREADSTATUS_BLOCKED_FOR_IO;
	dj_exec_breakExecution();
}
*/
