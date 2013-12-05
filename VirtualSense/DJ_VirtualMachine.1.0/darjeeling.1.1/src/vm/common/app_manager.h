/*
 *	app_manager.h
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
//#include "dev/board.h"

#ifndef __APP_MANAGER_H__
#define __APP_MANAGER_H__

#define APP_CONTEXT_NAME_SIZE 10
#define TABLE_ENTRIES	FLASH_SEGMENT_SIZE/sizeof(struct virtualsense_execution_context)
#define ENTRY_SIZE 		sizeof(struct virtualsense_execution_context)

struct virtualsense_execution_context {
  char name[APP_CONTEXT_NAME_SIZE];
  int16_t 			execution_context_id;
  dj_di_pointer 	di_file_pointer;
  uint8_t			loaded;
  uint8_t			running;
  // a reference to the loaded infusion index can be useful??
};

void app_manager_loadApplicationTable(void);
void app_manager_saveApplicationTable(void);
char *app_manager_getApplicationTable(void);



dj_di_pointer app_manager_getDiFilePointer(int16_t execution_context_id);
char *app_manager_getName(int16_t execution_context_id);
uint8_t app_manager_isLoaded(int16_t execution_context_id);
uint8_t app_manager_isRunning(int16_t execution_context_id);


dj_di_pointer arch_getDiFilePointer(int16_t execution_context_id); // LELE non servitrà più in seguito
void arch_loadApplicationTable(void *table);
void arch_saveApplicationTable(void *table);

#endif
