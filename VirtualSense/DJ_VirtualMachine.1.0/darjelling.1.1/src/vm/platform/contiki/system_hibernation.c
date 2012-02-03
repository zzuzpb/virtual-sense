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

#include "stdlib.h"
#include "stdio.h"

#include <msp430.h>
#include <legacymsp430.h>

#include "config.h"
#include "common/debug.h"
#include "pointerwidth.h"
#include "dev/pcf2123_spi.h"
#include "dev/leds.h"
#include "clock.h"

#define FAR_MEM_BASE 0x20000 // second rom bank of 64K or 23K in 5418a


void data20_write_char(unsigned long int address, unsigned char value);
void data20_write_word(unsigned long int address, unsigned int value);
void data20_write_block(unsigned long int address, unsigned int size, void *src_address);
unsigned char data20_read_char(unsigned long int address);
unsigned int data20_read_word(unsigned long int address);
void data20_read_block(unsigned long int address, unsigned int size, void *dest_address);
void heap_dump(void *heap);
void far_rom_erase_block(unsigned long int address, unsigned int size);
void enable_wakeup_from_interrupt(void);
void prepare_for_LPM4_5(void);
void enter_LPM4_5(void);


uint8_t save_heap(void *heap,
			   uint16_t left_p,
			   uint16_t right_p,
			   uint16_t panic_exe_p,
			   uint8_t ref_stack)
{
	unsigned long int mem_pointer = FAR_MEM_BASE;

   	int writed = 0;
	uint8_t ret = 1;

	/* erase memory before write it. It is needed each time */
	far_rom_erase_block(mem_pointer, HEAPSIZE+9);

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
	data20_write_word(mem_pointer, get_clock_divider());
	mem_pointer+=2;
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
	uint16_t left_p, right_p, panic_exe_p = 0, clock_divider = 1;
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
	heap-=9; //TODO: non so perchè
	data20_read_block(mem_pointer, HEAPSIZE, heap);
	mem_pointer+=HEAPSIZE;
	clock_divider = data20_read_word(mem_pointer);
	mem_pointer+=2;

	dj_mem_set_left_pointer(left_p);
	dj_mem_set_right_pointer(right_p);
	dj_mem_set_panic_exception_object_pointer(panic_exe_p);
	dj_mem_set_ref_stack(ref_stack);
	clock_slow_down(clock_divider);

	DEBUG_LOG("info: Heap initilized from file \n");

		// invalidate hibernation info
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

void data20_read_block(unsigned long int address, unsigned int size, void *dest_address){
	 unsigned long int ad = address;
	 unsigned int counter = 0;
	 unsigned char result = 0;
	 char *heap = (char *)dest_address;

	 while(counter < size){
		 asm volatile("dint				\n\t" \
					  "nop 				\n\t" \
					  "movx.a %1, R15	\n\t" \
					  "movx.b 	@R15, %0\n\t" \
					  "nop				\n\t"\
					  "nop				\n\t"\
					  "eint":"=r"(result):"m"(ad));

		 //printf("res %x\n", result);
		 *heap = result;
		 counter++;
		 heap++;
		 ad++;
	 }
}

void far_rom_erase_block(unsigned long int address, unsigned int size){
	 unsigned long int ad = address;
	 unsigned int counter = 0;

	 FCTL3 = 0x0A500; /* Lock = 0 */
	 FCTL1 = 0x0A502; /* ERASE = 1 */
	 while(counter < size){
		 asm volatile("dint				\n\t" \
					  "nop 				\n\t" \
					  "movx.a %0, R15	\n\t" \
					  "movx.b #0, @R15	\n\t" \
					  "eint"::"m"(ad));
		 ad++;
		 counter++;
	 }
	 FCTL1 = 0x0A500; /* ERASE = 0 */
	 FCTL3 = 0x0A510; /* Lock = 1 */
}

void enable_wakeup_from_interrupt(void){
	dint();
	P2DIR &= ~(BIT0+BIT2);
	P2REN |= BIT0+BIT2;			                   // Disable P2.0 internal resistance
	P2OUT |= BIT0+BIT2;                            // Set P2.0 and P2.2 as pull-Up resistance
	P2IE  |= BIT0+BIT2;                            // P2.0 and P2.2 interrupt enabled
	P2IES |= BIT0+BIT2;                            // P2.0 and P2.2 Hi/Lo edge
#ifdef PLATFORM_HAS_RTC_PCF2123
	RTC_clear_interrupt();
#endif
	P2IFG &= ~(BIT0+BIT2);                         // P2.0 and P2.2 IFG cleared
	eint();
}
void prepare_for_LPM4_5(void){
#ifdef PLATFORM_HAS_RTC_PCF2123
		RTC_spi_shutdown();
#endif

		//Tie unused ports
		PBOUT = 0x0000;
		PBDIR = 0x0000;                           // Enalbe OUTPUT driver
		PBREN = 0xFFFF;

		P2DIR &= ~(BIT3+BIT4+BIT5+BIT6+BIT7);
		P2REN |= BIT3+BIT4+BIT5+BIT6+BIT7;			                   // Disable P2.0 internal resistance
		P2OUT &= ~(BIT3+BIT4+BIT5+BIT6+BIT7);

		PCOUT = 0x0000;
		PCDIR = 0x0000;
		PCREN = 0xFFFF;                           // Enalbe Pull down

		PDOUT = 0x0000;
		PDDIR = 0x0000;
		PDREN = 0xFFFF;                           // Enalbe Pull down

		PEOUT = 0x0000;
		PEDIR = 0x0000;
		PEREN = 0xFFFF;                           // Enalbe Pull down

		PFOUT = 0x0000;
		PFDIR = 0x0000;
		PFREN = 0xFFFF;                           // Enalbe Pull down


		PJOUT = 0x0000;
		PJDIR = 0x0000;
		PJREN = 0xFFFF;


}
void enter_LPM4_5(void){
	/* stop timer A that is the contiki clock in order to
	 * allow entering on LPM4.5*/
		TA0CTL = MC_0;
		/* set the PMMREGOFF and then go to LPM4 */
		PMMCTL0_H = PMMPW_H;                      // open PMM
		PMMCTL0_L |= PMMREGOFF;                   // set Flag to enter LPM4.5 with LPM4 request
		PMMCTL0_H = PMMPW_H;                      // open PMM
		PM5CTL0 = 0x00;                       	  // Clear LOCKIO and enable ports
		PMMCTL0_H = 0x00;                         // close PMM

		/* LPM4 request */
		_BIS_SR(GIE | SCG0 | SCG1 | CPUOFF | OSCOFF);
}

interrupt(PORT2_VECTOR)
     irq_p2(void)
{
	/* interrupt service routine for button on P2.0 and external RTC (pcf2123) on P2.2*/
#ifdef PLATFORM_HAS_RTC_PCF2123
	RTC_clear_interrupt();
	RTC_disable_all_interrupts();
#endif
	P2IFG &= ~(BIT0+BIT2);                          // P2.0 and P2.2 IFG cleared
	LPM4_EXIT;
}

//#ifdef DARJEELING_DEBUG
#if 0
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


