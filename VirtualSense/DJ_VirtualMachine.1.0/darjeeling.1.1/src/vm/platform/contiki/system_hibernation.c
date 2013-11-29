/*
 *	system_hibernation.c
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
 * The VirtualSense hibernation system.
 *
 * @author Emanuele Lattanzi
 *
 */


#include "config.h"
#include "dev/board.h"
#include "common/debug.h"
#include "pointerwidth.h"
#include "dev/pcf2123_spi.h"
#include "dev/eeprom.h"
#include "dev/leds.h"
#include "dev/digitalio.h"
#include "clock.h"


void heap_dump(void *heap);
void enable_wakeup_from_interrupt(void);
void prepare_for_LPM4_5(void);
void enter_LPM4_5(void);


uint8_t save_heap(void *heap,
			   uint16_t left_p,
			   uint16_t right_p,
			   uint16_t panic_exe_p,
			   uint8_t ref_stack)
{
	eeprom_addr_t mem_pointer = SYSTEM_EEPROM_FS_BASE;

   	int writed = 0;
	uint8_t ret = 1;
	unsigned char buf[2];
	//heap_dump(heap);


	/* erase memory before write it. It is needed each time */
	//far_rom_erase_block(mem_pointer, HEAPSIZE+9);

	/* writing header for validating hibernation
	 * TODO: check if the actual hibernated heap is realted to
	 * the actual application */
	buf[0] = 0xCA;
	buf[1] = 0xFF;
	eeprom_write(mem_pointer, buf, 2);
	//data20_write_word(mem_pointer, 0xCAFF);
	mem_pointer+=2;
	buf[0] = (left_p >> 8);
	buf[1] = (left_p & 0xFF);

	eeprom_write(mem_pointer, buf, 2);

	//data20_write_word(mem_pointer,left_p);
	mem_pointer+=2;

	buf[0] = (right_p >> 8);
	buf[1] = (right_p & 0xFF);

	eeprom_write(mem_pointer, buf, 2);
	//data20_write_word(mem_pointer,right_p);
	mem_pointer+=2;

	buf[0] = (panic_exe_p >> 8);
	buf[1] = (panic_exe_p & 0xFF);

	eeprom_write(mem_pointer, buf, 2);
	//data20_write_word(mem_pointer,panic_exe_p);
	mem_pointer+=2;

	buf[0] = ref_stack;

	eeprom_write(mem_pointer, buf, 1);
	//data20_write_char(mem_pointer,ref_stack);
	mem_pointer+=1;


	eeprom_write(mem_pointer, heap, HEAPSIZE);
	//data20_write_block(mem_pointer, HEAPSIZE, heap);
	mem_pointer+=HEAPSIZE;

	buf[0] = (1 /* TODO to implement get_clock_divider() */ >> 8);
	buf[1] = (1 /* TODO to implement get_clock_divider() */ & 0xFF);
	eeprom_write(mem_pointer, buf, 2);
	//data20_write_word(mem_pointer, get_clock_divider());
	mem_pointer+=2;


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

	eeprom_addr_t mem_pointer = SYSTEM_EEPROM_FS_BASE; //TODO: EEPROM defines addresses as shor problem
	uint16_t left_p, right_p, panic_exe_p = 0, clock_divider = 1;
	uint8_t ref_stack = 0;
	int readed = 0;
	unsigned int validity = 0;
	unsigned char buf[2];

	/* check if the memory contains a valid heap
	 * hibernation
	 */

	eeprom_read(mem_pointer, buf, 2);
	//printf("b1 %x b2 %x\n", buf[0], buf[1]);
	validity = (buf[0] << 8) + buf[1];
	//validity = data20_read_word(mem_pointer);
	mem_pointer+=2;
	//printf("Validity %x\n", validity);
	if(validity != 0xCAFF) /* not vaild heap found */
		return 0;

	eeprom_read(mem_pointer, buf, 2);
	//left_p = data20_read_word(mem_pointer);
	left_p = (buf[0] << 8) + buf[1];
	mem_pointer+=2;
    //printf("1\n");
	eeprom_read(mem_pointer, buf, 2);
	//right_p = data20_read_word(mem_pointer);
	right_p = (buf[0] << 8) + buf[1];
	mem_pointer+=2;
	//printf("2\n");

	eeprom_read(mem_pointer, buf, 2);
	//panic_exe_p = data20_read_word(mem_pointer);
	panic_exe_p = (buf[0] << 8) + buf[1];
	mem_pointer+=2;
	//printf("3\n");

	eeprom_read(mem_pointer, buf, 1);
	//ref_stack = data20_read_char(mem_pointer);
	ref_stack = buf[0];
	mem_pointer+=1;
	//printf("4\n");

	// read the entire thing as one block
	//heap-=9; //TODO: non so perch�
	eeprom_read(mem_pointer, heap, HEAPSIZE);
	//data20_read_block(mem_pointer, HEAPSIZE, heap);
	mem_pointer+=HEAPSIZE;
	//printf("5\n");

	eeprom_read(mem_pointer, buf, 2);
	//clock_divider = data20_read_word(mem_pointer);
	clock_divider = (buf[0] << 8) + buf[1];
	mem_pointer+=2;
	//printf("6\n");

	dj_mem_set_left_pointer(left_p);
	dj_mem_set_right_pointer(right_p);
	dj_mem_set_panic_exception_object_pointer(panic_exe_p);
	dj_mem_set_ref_stack(ref_stack);
	// TODO to implement clock_slow_down(clock_divider);
	//printf("7\n");

	DEBUG_LOG("info: Heap initilized from file \n");

	//printf("8\n");

		// invalidate hibernation info
	mem_pointer = SYSTEM_EEPROM_FS_BASE;
	buf[0] = 0x00;
	buf[1] = 0x00;
	eeprom_write(mem_pointer, buf, 2);
	//data20_write_word(mem_pointer, 0x0000);
	//heap_dump(heap);
	//printf("MAchine loaded\n");
	return 1;
}





void enable_wakeup_from_interrupt(void){
	init_interrupt(ON_RAISING, INT_PRTC);
#ifdef PLATFORM_HAS_RTC_PCF2123
	RTC_clear_interrupt();
#endif
}
void prepare_for_LPM4_5(void){

		//Tie unused ports


}
void enter_LPM4_5(void){
	/* stop timer A and B that are contiki clock ad MAC cicle clock in order to
	 * allow entering on LPM4.5*/
#if 0 //TODO to implement
#if 1
		UCSCTL8 &= ~(ACLKREQEN | MCLKREQEN | SMCLKREQEN | MODOSCREQEN);
		UCSCTL6 |= (XT1OFF | XT2OFF);
		UCSCTL0 = 0x0000;
		TA0CTL = MC_0;
		TB0CTL = MC_0;
		/* set the PMMREGOFF and then go to LPM4 */
		PMMCTL0_H = PMMPW_H;                      // open PMM
		PMMCTL0_L |= PMMREGOFF;                   // set Flag to enter LPM4.5 with LPM4 request
		PMMCTL0_H = PMMPW_H;                      // open PMM
		PM5CTL0 = 0x00;                       	  // Clear LOCKIO and enable ports
		PMMCTL0_H = 0x00;                         // close PMM
#endif
		/* LPM4 request */
		_BIS_SR(GIE | SCG0 | SCG1 | CPUOFF | OSCOFF);
#endif
}

//#ifdef DARJEELING_DEBUG
#if 1
void heap_dump(void *heap){
	unsigned int counter = 0;
	unsigned char inLine = 0;

	while(counter < HEAPSIZE){
		if(inLine == 0)
			printf("[%d] ", counter);
		printf("%x", *((char *)heap));
		heap++;
		counter++;
		inLine++;
		if(inLine == 16){
			inLine = 0;
			printf("\n");
		}
	}
	printf("\n");
}
#endif


