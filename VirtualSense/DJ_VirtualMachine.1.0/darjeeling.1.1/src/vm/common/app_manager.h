/*
 *	app_storage.h
 *
*  Copyright (c) 2012 DiSBeF, University of Urbino.
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
 * The VirtualSense application manager functions.
 *
 * @author Emanuele Lattanzi
 *
 */
#include "common/types.h"
#include "common/array.h"
#include "common/panic.h"
#include "common/debug.h"
#include "common/vmthread.h"

void app_manager_setPlatformThreadID(int16_t id);
void app_manager_wakeUpPlatformThread(uint8_t command_id, uint8_t infusion_id);
int8_t app_manager_getCommandID(void);
int8_t app_manager_getExecutionContextID(void);

dj_di_pointer app_manager_getApplicationPointer(int16_t unique_id);
dj_di_pointer arch_getApplicationPointer(int16_t unique_id);

