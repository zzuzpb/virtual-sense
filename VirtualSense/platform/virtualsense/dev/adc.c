/*
 *	adc.c
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
 * ADC access definitions.
 *
 * @author Emanuele Lattanzi
 *
 */


#include <msp430.h>
#include "dev/adc.h"
#include "contiki.h"
/*---------------------------------------------------------------------------*/

volatile unsigned long reading = 0;

void 
adc_init()
{
	P6DIR &=~(TEMP_PIN+LIGHT_PIN+BIT2);
	P6OUT &=~(TEMP_PIN+LIGHT_PIN+BIT2);
	ADC_SEL_PORT |= TEMP_PIN+LIGHT_PIN /*+HUMIDITY_PIN*/;   //Enable A/D channel inputs

	P2DIR |= BIT7 | BIT6;
	P5DIR |= BIT6 | BIT7;

  	P5OUT &= ~(BIT7 | BIT6);
  	P5OUT |= BIT7; // H-GAIN MODE GC1 1 and GC2 0

}
/*---------------------------------------------------------------------------*/

uint16_t 
get_adc(int channel)
{
  uint16_t tmp = 0;
  uint8_t i =0;




  switch(channel){
  case LIGHT_CHANNEL:
	  LIGHT_POWER_UP();
	  //REFCTL0 |= REFMSTR + REFVSEL_3 + REFON;    // Enable internal 1.5V reference
	  ADC12CTL0 = ADC12SHT0_8 + ADC12ON;		    // Set sample time
	  ADC12CTL1 = ADC12SHP;                     // Enable sample timer
	  ADC12MCTL0 = ADC12INCH_1;  // ADC input ch A1 => external light sense
	  ADC12IE = 0x001; //enable interrupt on MEM0
	  __delay_cycles(75);
	  ADC12CTL0 |= ADC12ENC;                    // Enable conversions
	  for (i = 0; i< 100; i++){
	  ADC12CTL0 |= ADC12SC;                   // Sampling and conversion start
	  	  __bis_SR_register(LPM4_bits + GIE);     // LPM4 with interrupts enabled
	  	  __no_operation();
	  }
	  LIGHT_POWER_DOWN();
	  //TODO: shutdown adc and ref to save power.
  	  break;
  case TEMP_CHANNEL:
	  LIGHT_POWER_UP();
	  TEMP_POWER_UP();
	  //REFCTL0 |= REFMSTR + REFVSEL_3 + REFON;    // Enable internal 1.5V reference
	  ADC12CTL0 = ADC12SHT0_8 + ADC12ON;		    // Set sample time
	  ADC12CTL1 = ADC12SHP;                     // Enable sample timer
	  ADC12MCTL0 = ADC12INCH_0;  // ADC input ch A1 => external light sense
	  ADC12IE = 0x001; //enable interrupt on MEM0
	  __delay_cycles(75);
	  ADC12CTL0 |= ADC12ENC;                    // Enable conversions
	  for (i = 0; i< 100; i++){
		  ADC12CTL0 |= ADC12SC;                   // Sampling and conversion start
		  __bis_SR_register(LPM4_bits + GIE);     // LPM4 with interrupts enabled
		  __no_operation();
	  }
	  LIGHT_POWER_DOWN();
	  TEMP_POWER_DOWN();
	  //TODO: shutdown adc and ref to save power.
  	  break;
  case BOARD_TEMP_CHANNEL:
	  REFCTL0 |= REFMSTR + REFVSEL_0 + REFON;    // Enable internal 1.5V reference
	  ADC12CTL0 = ADC12SHT0_8 + ADC12ON;		    // Set sample time
	  ADC12CTL1 = ADC12SHP;                     // Enable sample timer
	  ADC12MCTL0 = ADC12SREF_1 + ADC12INCH_10;  // ADC input ch A10 => internal board temp
	  ADC12IE = 0x001; //enable interrupt on MEM0
	  __delay_cycles(75);
	  ADC12CTL0 |= ADC12ENC;                    // Enable conversions
	  for (i = 0; i< 100; i++){
		  ADC12CTL0 |= ADC12SC;                   // Sampling and conversion start
		  __bis_SR_register(LPM4_bits + GIE);     // LPM4 with interrupts enabled
		  __no_operation();
	  }
	  //TODO: shutdown adc and ref to save power.
	  REFCTL0 = 0;
	  break;
  }
  tmp = (uint16_t)((reading/100)-1005);
  //printf("Reading %u\n", tmp);
  reading = 0;
  return tmp;
}


interrupt(ADC12_VECTOR)
ADC12ISR (void)
{
	 switch(ADC12IV)
	  {
	  case  0: break;                           // Vector  0:  No interrupt
	  case  2: break;                           // Vector  2:  ADC overflow
	  case  4: break;                           // Vector  4:  ADC timing overflow
	  case  6:                                  // Vector  6:  ADC12IFG0
	    reading += ADC12MEM0;                       // Move results, IFG is cleared
	    //printf("R: %u\n", reading );
	    LPM4_EXIT;   // Exit active CPU
	    break;
	  case  8: break;                           // Vector  8:  ADC12IFG1
	  case 10: break;                           // Vector 10:  ADC12IFG2
	  case 12: break;                           // Vector 12:  ADC12IFG3
	  case 14: break;                           // Vector 14:  ADC12IFG4
	  case 16: break;                           // Vector 16:  ADC12IFG5
	  case 18: break;                           // Vector 18:  ADC12IFG6
	  case 20: break;                           // Vector 20:  ADC12IFG7
	  case 22: break;                           // Vector 22:  ADC12IFG8
	  case 24: break;                           // Vector 24:  ADC12IFG9
	  case 26: break;                           // Vector 26:  ADC12IFG10
	  case 28: break;                           // Vector 28:  ADC12IFG11
	  case 30: break;                           // Vector 30:  ADC12IFG12
	  case 32: break;                           // Vector 32:  ADC12IFG13
	  case 34: break;                           // Vector 34:  ADC12IFG14
	  default: break;
	  }

}
/*---------------------------------------------------------------------------*/
