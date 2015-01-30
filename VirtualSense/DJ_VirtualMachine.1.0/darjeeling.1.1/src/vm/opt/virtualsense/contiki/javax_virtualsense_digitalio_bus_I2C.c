/*
 *	javax_virtualsense_bus_digitalio_I2C.c
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
 * I2C bus native methods.
 *
 * @author Emanuele Lattanzi
 *
 */

#include "common/execution/execution.h"
#include "common/array.h"
#include "common/darjeeling.h"
#include "node-id.h"

#include "contiki.h"
#include "power-interface.h"

#include "dev/i2c.h"
#include "dev/board.h"


#include "base_definitions.h"



void javax_virtualsense_digitalio_bus_I2C_void__enable(){
	i2c_enable();
}
void javax_virtualsense_digitalio_bus_I2C_void__disable(){
	i2c_disable();
}
void javax_virtualsense_digitalio_bus_I2C_byte____read_byte(){
	uint16_t ack_send = dj_exec_stackPopShort();
	uint16_t ret = i2c_read((int)ack_send);
	dj_exec_stackPushShort(ret);
}
void javax_virtualsense_digitalio_bus_I2C_boolean__write_byte_byte__(){
	uint16_t byte = dj_exec_stackPopShort();
	uint16_t ret = i2c_write((unsigned)byte);
	dj_exec_stackPushShort(ret);
}

