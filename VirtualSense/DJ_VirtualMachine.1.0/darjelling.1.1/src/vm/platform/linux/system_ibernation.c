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
uint8_t save_heap(void *heap,
			   uint16_t left_p,
			   uint16_t right_p,
			   uint16_t panic_exe_p,
			   uint8_t ref_stack)
{

	FILE *heapFile;
	int writed = 0;
	uint8_t ret = 1;

	heapFile = fopen("virtualSensHeap","wb");

	if (!heapFile){
		DEBUG_LOG("Fatal: could not create a file \n");
		return 0;
	}


	fwrite(&left_p, sizeof(uint16_t), 1, heapFile);
	fwrite(&right_p, sizeof(uint16_t), 1, heapFile);
	fwrite(&panic_exe_p, sizeof(uint16_t), 1, heapFile);
	fwrite(&ref_stack, sizeof(uint8_t), 1, heapFile);
	// write the entire thing as one block
	writed = fwrite(heap, 1, MEMSIZE, heapFile);
	if(writed != MEMSIZE){
		DEBUG_LOG("ERROR WRITING HEAP FILE\n");
		remove("virtualSensHeap");
		ret = 0;
	}

	// close file
	fclose(heapFile);
	return ret;

}
//TODO:check if the file is actual (i.e. if the file is related to the actual app)

// load the heap from the disk
uint8_t load_machine(void *heap)
{

	FILE *heapFile;
	uint16_t left_p, right_p, panic_exe_p = 0;
	uint8_t ref_stack = 0;
	int readed = 0;

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
	readed = fread(heap,1,MEMSIZE,heapFile);
	if(readed != MEMSIZE){
			printf("ERROR READING HEAP FILE\n");
			remove("virtualSensHeap");
			return 0;
	}

	dj_mem_set_left_pointer(left_p);
	dj_mem_set_right_pointer(right_p);
	dj_mem_set_panic_exception_object_pointer(panic_exe_p);
	dj_mem_set_ref_stack(ref_stack);
	// close file
	fclose(heapFile);
	DEBUG_LOG("info: Heap initilized from file \n");
	// invalidate ibernation info
	remove("virtualSensHeap");
	return 1;

}


