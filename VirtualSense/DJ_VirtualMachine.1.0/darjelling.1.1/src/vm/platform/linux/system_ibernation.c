/*
 *	VirtualSense.java
 *
 *  Copyright (c) 2011 DiSBeF, University of Urbino.
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
 * The VirtualSense ibernation system.
 *
 * @author Emanuele Lattanzi
 *
 */

#include "stdlib.h"
#include "stdio.h"

#include "config.h"
#include "common/debug.h"
#include "pointerwidth.h"

// save the heap on the disk
void save_heap(void *heap,
			   /*void *app_mem,
			   void *virtual_sense_mem,
			   void *darjeeling_mem,
			   void *base_mem,*/
			   uint16_t left_p,
			   uint16_t right_p,
			   uint16_t panic_exe_p,
			   uint8_t ref_stack)
{

	FILE *heapFile;

	heapFile = fopen("virtualSensHeap","w");

	if (!heapFile)
		DEBUG_LOG("Fatal: could not create a file \n");


	fwrite(&left_p, sizeof(uint16_t), 1, heapFile);
	fwrite(&right_p, sizeof(uint16_t), 1, heapFile);
	fwrite(&panic_exe_p, sizeof(uint16_t), 1, heapFile);
	fwrite(&ref_stack, sizeof(uint8_t), 1, heapFile);
	// write the entire thing as one block
	fwrite(heap, 1, MEMSIZE, heapFile);

	/*fwrite(app_mem, 1, MAX_DI_SIZE, heapFile);
	fwrite(virtual_sense_mem, 1, MAX_DI_SIZE, heapFile);
	fwrite(darjeeling_mem, 1, MAX_DI_SIZE, heapFile);
	fwrite(base_mem, 1, MAX_DI_SIZE, heapFile);*/

	// close file
	fclose(heapFile);
	return;

}

// load the heap from the disk
uint8_t load_machine(void *heap/*,
					 void *app_mem,
					 void *virtual_sense_mem,
					 void *darjeeling_mem,
					 void *base_mem*/)
{

	FILE *heapFile;
	uint16_t left_p, right_p, panic_exe_p = 0;
	uint8_t ref_stack = 0;

	heapFile = fopen("virtualSensHeap","rb");

	if (!heapFile){
		DEBUG_LOG("info: savedHeap not found restarting virtual machine \n");
		return 0;
	}

	fread(&left_p, sizeof(uint16_t), 1, heapFile);
	fread(&right_p, sizeof(uint16_t), 1, heapFile);
	fread(&panic_exe_p, sizeof(uint16_t), 1, heapFile);
	fread(&ref_stack, sizeof(uint8_t), 1, heapFile);
	// read the entire thing as one block
	fread(heap,1,MEMSIZE,heapFile);

	/*fread(app_mem, 1, MAX_DI_SIZE, heapFile);
	fread(virtual_sense_mem, 1, MAX_DI_SIZE, heapFile);
	fread(darjeeling_mem, 1, MAX_DI_SIZE, heapFile);
	fread(base_mem, 1, MAX_DI_SIZE, heapFile);*/

	dj_mem_set_left_pointer(left_p);
	dj_mem_set_right_pointer(right_p);
	dj_mem_set_panic_exception_object_pointer(panic_exe_p);
	dj_mem_set_ref_stack(ref_stack);
	// close file
	fclose(heapFile);
	DEBUG_LOG("info: Heap initilized from file \n");
	return 1;

}


