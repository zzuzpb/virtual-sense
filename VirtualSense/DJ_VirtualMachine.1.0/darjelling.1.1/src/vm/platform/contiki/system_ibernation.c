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

#include <msp430.h>
#include <legacymsp430.h>

#include "config.h"
#include "common/debug.h"
#include "pointerwidth.h"

#define FAR_MEM_BASE 0x10000

void data20_write_char(unsigned long int address, unsigned char value);
void data20_write_word(unsigned long int address, unsigned int value);
void data20_write_block(unsigned long int address, unsigned int size, void *src_address);
unsigned char data20_read_char(unsigned long int address);
unsigned int data20_read_word(unsigned long int address);
void data20_read_block(unsigned long int address, unsigned int size, void *src_address);

uint8_t save_heap(void *heap,
			   uint16_t left_p,
			   uint16_t right_p,
			   uint16_t panic_exe_p,
			   uint8_t ref_stack)
{
	unsigned long int mem_pointer = FAR_MEM_BASE;

   	int writed = 0;
	uint8_t ret = 1;

	/* writing header for validating ibernation
	 * TODO: check if the actual ibernated heap is realted to
	 * the actual application */
	data20_write_word(mem_pointer, 0xCAFF);
	mem_pointer+=2;
	data20_write_word(mem_pointer,left_p);
	mem_pointer+=2;
	data20_write_word(mem_pointer,right_p);
	mem_pointer+=2;
	data20_write_word(mem_pointer,panic_exe_p);
	mem_pointer+=2;
	data20_write_char(mem_pointer,ref_stack);
	mem_pointer+=1;
	data20_write_block(mem_pointer, HEAPSIZE, heap);
	mem_pointer+=HEAPSIZE;
	data20_write_block(mem_pointer, HEAPSIZE, heap);
	DEBUG_LOG("End of save heap. mem_pointer = %ld\n", mem_pointer);

	/*writed = 1;
	if(writed != HEAPSIZE){
		DEBUG_LOG("ERROR WRITING HEAP FILE\n");
		ret = 0;
	}*/
	return ret;

}
//TODO:check if the file is actual (i.e. if the file is related to the actual app)

uint8_t load_machine(void *heap)
{

	unsigned long int mem_pointer = FAR_MEM_BASE;
	uint16_t left_p, right_p, panic_exe_p = 0;
	uint8_t ref_stack = 0;
	int readed = 0;
	unsigned int validity = 0;

	/* check if the memory contains a valid heap
	 * ibernation
	 */
	validity = data20_read_word(mem_pointer);
	mem_pointer+=2;
	if(validity != 0xCAFF) /* not vaild heap found */
		return 0;
	left_p = data20_read_word(mem_pointer);
	mem_pointer+=2;
	right_p = data20_read_word(mem_pointer);
	mem_pointer+=2;
	panic_exe_p = data20_read_word(mem_pointer);
	mem_pointer+=2;
	ref_stack = data20_read_char(mem_pointer);
	mem_pointer+=1;
	// read the entire thing as one block
	data20_read_block(mem_pointer, HEAPSIZE, heap);
	mem_pointer+=HEAPSIZE;

	dj_mem_set_left_pointer(left_p);
	dj_mem_set_right_pointer(right_p);
	dj_mem_set_panic_exception_object_pointer(panic_exe_p);
	dj_mem_set_ref_stack(ref_stack);

	DEBUG_LOG("info: Heap initilized from file \n");

	// invalidate ibernation info
	mem_pointer = FAR_MEM_BASE;
	data20_write_word(mem_pointer, 0x0000);
	return 1;
}



void data20_write_char(unsigned long int address, unsigned char value){
	 FCTL3 = 0x0A500; /* Lock = 0 */
	 FCTL1 = 0x0A540; /* WRT = 1 */
	 asm volatile("dint				\n\t" \
		 		  "nop 				\n\t" \
		 		  "movx.a %0, R15	\n\t" \
		 	      "movx.b %1, @R15	\n\t" \
		 	      "eint"::"m"(address),"r"(value));
	 FCTL1 = 0x0A500; /* WRT = 0 */
	 FCTL3 = 0x0A510; /* Lock = 1 */
}

void data20_write_word(unsigned long int address, unsigned int value){
	 FCTL3 = 0x0A500; /* Lock = 0 */
	 FCTL1 = 0x0A540; /* WRT = 1 */
	 asm volatile("dint				\n\t" \
		 		  "nop 				\n\t" \
		 		  "movx.a %0, R15	\n\t" \
		 	      "movx.w %1, @R15	\n\t" \
		 	      "eint"::"m"(address),"r"(value));
	 FCTL1 = 0x0A500; /* WRT = 0 */
	 FCTL3 = 0x0A510; /* Lock = 1 */
}

void data20_write_block(unsigned long int address, unsigned int size, void *src_address){
	 unsigned long int ad = address;
	 unsigned int counter = 0;
	 unsigned char value = 0;

	 FCTL3 = 0x0A500; /* Lock = 0 */
	 FCTL1 = 0x0A540; /* WRT = 1 */
	 while(counter < size){
		 value = *((char *)src_address);
		 asm volatile("dint				\n\t" \
					  "nop 				\n\t" \
					  "movx.a %0, R15	\n\t" \
					  "movx.b %1, @R15	\n\t" \
					  "eint"::"m"(ad),"r"(value));
		 ad++;
		 src_address++;
		 counter++;
	 }
	 FCTL1 = 0x0A500; /* WRT = 0 */
	 FCTL3 = 0x0A510; /* Lock = 1 */
}

unsigned char data20_read_char(unsigned long int address){
	 unsigned char result = 0;
	 asm volatile("dint				\n\t" \
	 			  "nop 				\n\t" \
	 			  "movx.a %1, R15	\n\t" \
	 			  "movx.b 	@R15, %0\n\t" \
	 			  "eint":"=r"(result):"m"(address));
	 return result;
}

unsigned int data20_read_word(unsigned long int address){
	 unsigned int result = 0;
	 asm volatile("dint				\n\t" \
	 			  "nop 				\n\t" \
	 			  "movx.a %1, R15	\n\t" \
	 			  "movx.w 	@R15, %0\n\t" \
	 			  "eint":"=r"(result):"m"(address));
	 return result;
}

void data20_read_block(unsigned long int address, unsigned int size, void *src_address){
	 unsigned long int ad = address;
	 unsigned int counter = 0;
	 unsigned char result = 0;

	 while(counter > size){
		 asm volatile("dint				\n\t" \
					  "nop 				\n\t" \
					  "movx.a %1, R15	\n\t" \
					  "movx.b 	@R15, %0\n\t" \
					  "eint":"=r"(result):"m"(ad));
		 (*(char *)src_address) = result;
		 counter++;
		 src_address++;
		 ad++;
	 }
}


