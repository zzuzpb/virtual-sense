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


#include "common/execution/execution.h"
#include "common/array.h"
#include "common/darjeeling.h"

#include "contiki.h"
#include "power-interface.h"

#include "serial-input.h"
#include "dev/board.h"


#include "base_definitions.h"

void javax_virtualsense_digitalio_bus_UART_void__waitForMessage(){
	serial_input_open();
	dj_thread *thread = dj_exec_getCurrentThread();
	thread->status = THREADSTATUS_BLOCKED_FOR_IO;
	dj_exec_breakExecution();


}
void javax_virtualsense_digitalio_bus_UART_java_lang_String__getMessage(){
	// allocate the new string
	int len = get_serial_buffer_len();
	char *buffer = get_serial_buffer();
	char *ret = (char *)dj_mem_alloc(len + 1, dj_vm_getSysLibClassRuntimeId(dj_exec_getVM(), BASE_CDEF_java_lang_String));
	if (ret==NULL)
	{
	   	dj_exec_createAndThrow(BASE_CDEF_java_lang_OutOfMemoryError);
	   	return;
	}
	memcpy(ret, buffer, len);
	*(ret+len+1) = 0;
	dj_exec_stackPushRef(VOIDP_TO_REF(ret));
}


 /*

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
*/
