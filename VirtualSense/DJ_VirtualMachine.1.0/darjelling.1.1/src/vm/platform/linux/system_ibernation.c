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

// save the heap on the disk
void save_machine(void *heap)
{

	FILE *heapFile;

	heapFile = fopen("virtualSensHeap","w");

	if (!heapFile)
		DEBUG_LOG("Fatal: could not create a file \n");


	// write the entire thing as one block
	fwrite(heap, 1, MEMSIZE, heapFile);

	// close file
	fclose(heapFile);
	return;

}

// load the heap from the disk
void load_machine(void *heap)
{

	FILE *heapFile;

	heapFile = fopen("virtualSensHeap","rb");

	if (!heapFile){
		DEBUG_LOG("info: savedHeap not found restarting virtual machine \n");
		return;
	}

	// read the entire thing as one block
	fread(heap,1,MEMSIZE,heapFile);

	// close file
	fclose(heapFile);
	DEBUG_LOG("info: Heap initilized from file \n");
	return;

}


