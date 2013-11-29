/*
 *  null_eeprom.c
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
 * The null_eeprom that emulates eeprom by means of CPU far flash.
 *
 * @author Emanuele Lattanzi
 *
 */


#include "dev/board.h"
#include "null_eeprom.h"


 void data20_write_char(unsigned long int address, unsigned char value){
#if 0 //TODO to implement
 	 FCTL3 = 0x0A500; /* Lock = 0 */
 	 FCTL1 = 0x0A540; /* WRT = 1 */
 	 asm volatile("dint				\n\t" \
 		 		  "nop 				\n\t" \
 		 		  "movx.a %0, R15	\n\t" \
 		 	      "movx.b %1, @R15	\n\t" \
 		 	      "eint"::"m"(address),"r"(value));
 	 FCTL1 = 0x0A500; /* WRT = 0 */
 	 FCTL3 = 0x0A510; /* Lock = 1 */
#endif
 }

 void data20_write_word(unsigned long int address, unsigned int value){
#if 0 //TODO to implement
 	 FCTL3 = 0x0A500; /* Lock = 0 */
 	 FCTL1 = 0x0A540; /* WRT = 1 */
 	 asm volatile("dint				\n\t" \
 		 		  "nop 				\n\t" \
 		 		  "movx.a %0, R15	\n\t" \
 		 	      "movx.w %1, @R15	\n\t" \
 		 	      "eint"::"m"(address),"r"(value));
 	 FCTL1 = 0x0A500; /* WRT = 0 */
 	 FCTL3 = 0x0A510; /* Lock = 1 */
#endif
 }

 void data20_write_block(unsigned long int address, unsigned int size, void *src_address){
#if 0 //TODO to implement
 	 unsigned long int ad = address;
 	 unsigned int counter = 0;
 	 unsigned char value = 0;
 	 //printf("add %ld size %d\n",address,  size);
 	 FCTL3 = 0x0A500; /* Lock = 0 */
 	 FCTL1 = 0x0A540; /* WRT = 1 */
 	 while(counter < size){
 		 value = *((char *)src_address);
 		 //printf(".");
 		 asm volatile("dint				\n\t" \
 					  "nop 				\n\t" \
 					  "movx.a %0, R15	\n\t" \
 					  "movx.b %1, @R15	\n\t" \
 					  "nop 				\n\t" \
 					  "eint"::"m"(ad),"r"(value));
 		 ad++;
 		 src_address++;
 		 counter++;
 	 }
 	 FCTL1 = 0x0A500; /* WRT = 0 */
 	 FCTL3 = 0x0A510; /* Lock = 1 */
 	//printf("\n");
#endif
 }

 unsigned char data20_read_char(unsigned long int address){
 	 unsigned char result = 0;
#if 0 //TODO to implement
 	 asm volatile("dint				\n\t" \
 	 			  "nop 				\n\t" \
 	 			  "movx.a %1, R15	\n\t" \
 	 			  "movx.b 	@R15, %0\n\t" \
 	 			  "eint":"=r"(result):"m"(address));
#endif
 	 return result;
 }

 unsigned int data20_read_word(unsigned long int address){
 	 unsigned int result = 0;
#if 0 //TODO to implement
 	 asm volatile("dint				\n\t" \
 	 			  "nop 				\n\t" \
 	 			  "movx.a %1, R15	\n\t" \
 	 			  "movx.w 	@R15, %0\n\t" \
 	 			  "eint":"=r"(result):"m"(address));
#endif //TODO to implement
 	 return result;
 }

 void data20_read_block(unsigned long int address, unsigned int size, void *dest_address){
#if 0 //TODO to implement
 	 unsigned long int ad = address;
 	 unsigned int counter = 0;
 	 unsigned char result = 0;
 	 char *heap = (char *)dest_address;
 	//printf("read from %ld value %d\n",ad, size);
 	 while(counter < size){
 		 asm volatile("dint				\n\t" \
 					  "nop 				\n\t" \
 					  "movx.a %1, R15	\n\t" \
 					  "movx.b 	@R15, %0\n\t" \
 					  "nop				\n\t"\
 					  "nop				\n\t"\
 					  "eint":"=r"(result):"m"(ad));

 		 //printf("0x%x ",result);
 		 *heap = result;
 		 counter++;
 		 heap++;
 		 ad++;
 	 }
 	 //printf("\n");
#endif
 }

 void far_rom_erase_block(unsigned long int address, unsigned int size){
#if 0 //TODO to implement
 	 unsigned long int ad = address;
 	 unsigned int counter = 0;
 	 FCTL3 = 0x0A500; /* Lock = 0 */
 	 FCTL1 = 0x0A502; /* ERASE = 1 */
 	 //printf("erasing  %ld size %d\n",address,  size);
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
#endif //TODO to implement
 }

