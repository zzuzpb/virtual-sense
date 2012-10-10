/*
 *	app_storage.c
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
 * The VirtualSense application storage system.
 *
 * @author Emanuele Lattanzi
 *
 */

#include <msp430.h>
#include "config.h"
#include "platform-conf.h"
#include "common/debug.h"
#include "common/types.h"
#include "pointerwidth.h"
#include "dev/null_eeprom.h"


dj_di_pointer arch_getApplicationPointer(int16_t unique_id);
dj_app_table_node app_storage_getAppNode(int16_t unique_id);


dj_di_pointer arch_getApplicationPointer(int16_t unique_id){
		dj_di_pointer di;
		dj_app_table_node node;
		node = app_storage_getAppNode(unique_id);
		return node.app_pointer;
}

dj_app_table_node app_storage_getAppNode(int16_t unique_id){
	dj_app_table_node node;
	node.app_id = unique_id;
	node.app_pointer = 0; // means not found
	uint16_t i = 0;

	// look into the table node.
	unsigned long int ad = APP_NODES_TABLE_BASE;
	unsigned char b1,b2,b3,b4;
	for(i = 0; i < FLASH_SEGMENT_SIZE; i+=6){
		b1 = data20_read_char(ad);
		b2 = data20_read_char(ad+1);
		DEBUG_LOG(" app_id 0x%x%x\n", b1,b2);
		node.app_id = (b1 << 8) + b2;
		//printf("Found app_id %d for i = %d\n", node.app_id, i);
		if(node.app_id == unique_id){
			b1 = data20_read_char(ad+2);
			b2 = data20_read_char(ad+3);
			DEBUG_LOG(" app_pointer 0x%x%x\n", b1,b2);
			node.app_pointer = ((b1 << 8) | b2);
			DEBUG_LOG("The p is %x\n", node.app_pointer);
			break;
		}else
			node.app_id = 0;
		ad+=4;
	}
	return node;
}


