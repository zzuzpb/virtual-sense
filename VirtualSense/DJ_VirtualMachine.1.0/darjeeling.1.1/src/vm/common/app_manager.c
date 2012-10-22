/*
 *	app_manager.c
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
 * The VirtualSense application manager headers.
 *
 * @author Emanuele Lattanzi
 *
 */
#include "common/types.h"
#include "common/app_manager.h"

static unsigned char app_table[TABLE_ENTRIES*ENTRY_SIZE];

dj_di_pointer app_manager_getDiFilePointer(int16_t execution_context_id){
	dj_di_pointer file_pointer = 0;
	uint16_t i = 0;
	struct virtualsense_execution_context *context;
	for(i=0; i < TABLE_ENTRIES*ENTRY_SIZE; i+=ENTRY_SIZE){ // for debug
		context = (struct virtualsense_execution_context *)(app_table+i);
		DEBUG_LOG("Entry: name %s\n", context->name);
		DEBUG_LOG("Entry: context_id %d\n", context->execution_context_id);
		DEBUG_LOG("Entry: di_pointer %d\n", context->di_file_pointer);
		DEBUG_LOG("Entry: loaded %x\n", context->loaded);
		DEBUG_LOG("Entry: running %x\n", context->running);

		if(context->execution_context_id == execution_context_id){
			file_pointer = context->di_file_pointer;
			break;
		}
	}
	return file_pointer;
	//return arch_getDiFilePointer(execution_context_id);
}
void app_manager_loadApplicationTable(){
	arch_loadApplicationTable(app_table);
}
void app_manager_saveApplicationTable(){
	arch_saveApplicationTable(app_table);
}
char *app_manager_getApplicationTable(){
	return (char *)app_table;
}

char *app_manager_getName(int16_t execution_context_id){
	char *ret = nullref;
	uint16_t i = 0;
	struct virtualsense_execution_context *context;
	for(i=0; i < TABLE_ENTRIES*ENTRY_SIZE; i+=ENTRY_SIZE){
		context = (struct virtualsense_execution_context *)(app_table+i);
		if(context->execution_context_id == execution_context_id){
			ret = (char *)context->name;
			break;
		}
	}
	return ret;
}
uint8_t app_manager_isLoaded(int16_t execution_context_id){
	uint8_t ret = 0;
	uint16_t i = 0;
	struct virtualsense_execution_context *context;
	for(i=0; i < TABLE_ENTRIES*ENTRY_SIZE; i+=ENTRY_SIZE){
		context = (struct virtualsense_execution_context *)(app_table+i);
		if(context->execution_context_id == execution_context_id){
			ret = context->loaded;
			break;
		}
	}
	return ret;
}
uint8_t app_manager_isRunning(int16_t execution_context_id){
	uint8_t ret = 0;
	uint16_t i = 0;
	struct virtualsense_execution_context *context;
	for(i=0; i < TABLE_ENTRIES*ENTRY_SIZE; i+=ENTRY_SIZE){
		context = (struct virtualsense_execution_context *)(app_table+i);
		printf("Looking for running %d == %d\n", execution_context_id, context->execution_context_id);
		if(context->execution_context_id == execution_context_id){
			ret = context->running;
			printf("FOUND running\n");
			break;
		}
	}
	return ret;
}



