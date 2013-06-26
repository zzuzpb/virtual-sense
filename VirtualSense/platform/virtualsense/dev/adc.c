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
#include "platform-conf.h"
#include "eeprom.h"



volatile unsigned long reading = 0;

uint16_t read_ch(uint32_t, short);


/*---------------------------------------------------------------------------*/


void 
adc_init()
{
	P6DIR &=~(BIT0+BIT1+BIT2+BIT3+BIT4+BIT6+BIT7);
	P6OUT &=~(BIT0+BIT1+BIT2+BIT3+BIT4+BIT6+BIT7);
	P6SEL |= BIT0+BIT1+BIT2+BIT3+BIT4+BIT6+BIT7; 		//Enable A/D channel inputs

	P2DIR |= BIT7 | BIT6;					// Set ACLK and ADCLK as input
	P5DIR |= BIT6 | BIT7;					// Set P5.6 and P5.7 as input

  	P5OUT &= ~(BIT7 | BIT6);	// Set VREF+ and VREF-
  	P5OUT |= BIT7; 			// H-GAIN MODE GC1 1 and GC2 0
}
/*---------------------------------------------------------------------------*/



uint16_t read_adc_channel(int channel, short ref)
{
	uint16_t ret = 0;

  	switch(channel)
	{
  		case CHANNEL_0:
		  ret = read_ch(ADC12INCH_0, ref);
  	  	  break;

		case CHANNEL_1:
		  ret = read_ch(ADC12INCH_1, ref);
  	  	  break;

		case CHANNEL_2:
		  ret = read_ch(ADC12INCH_2, ref);
  	  	  break;

		case CHANNEL_3:
		  ret = read_ch(ADC12INCH_3, ref);
  	  	  break;

		case CHANNEL_4:
		  ret = read_ch(ADC12INCH_4, ref);
  	  	  break;

		case CHANNEL_6:
		  ret = read_ch(ADC12INCH_6, ref);
  	  	  break;

		case CHANNEL_7:
		  ret = read_ch(ADC12INCH_7, ref);
  	  	  break;

		case CHANNEL_VREFP:
		  ret = read_ch(ADC12INCH_8, ref);
  	  	  break;

		case CHANNEL_VREFN:
		  ret = read_ch(ADC12INCH_9, ref);
  	  	  break;

		case BOARD_TEMP_CHANNEL:
		  ret = read_ch(ADC12INCH_10, ref);
  	  	  break;

		case VBAT_CHANNEL:
		  ret = (read_ch(ADC12INCH_11, ref) * 2);
  	  	  break;
	}

  	return ret;
}
/*---------------------------------------------------------------------------*/



uint16_t read_ch(uint32_t ch, short f_ref)
{	
	uint32_t ref = REFVSEL0 + REFVSEL1;
	uint16_t v_ref = 2500;
	uint16_t ret = 0;	
	
	//printf("ch = %x o ", ch);

	// If value of reference is wrong ref default is 2.5V
	switch(f_ref)
	{
		case 0:
		  // Ref 1.5V
		  //printf("ref 1.5V");
		  ref &= (~REFVSEL0) + (~REFVSEL1);
		  v_ref = 1500;
		  break;

		case 1:
		  // Ref 2V
		  //printf("ref 2V");
		  ref &= ~REFVSEL1;
		  v_ref = 2000;
		  break;

		case 2:
		  // Ref 2.5V
		  //printf("ref 2.5V");
		  v_ref = 2500;
		  break;
	}


  	ADC12CTL0 = ADC12SHT0_8 + ADC12ON;		    	// Turn on ADC and set sample time	
	REFCTL0 = REFMSTR + REFON + ref;			// Turn on Ref module and set ref
	ADC12CTL1 = ADC12SHP;                     		// Enable sample timer
	ADC12MCTL0 = ADC12SREF_1 + ch;  			// Select ADC channel use reference from Ref module
	
	ADC12IE = 0x001; 					// Enable interrupt on MEM0
	__delay_cycles(75);
	ADC12CTL0 |= ADC12ENC;                    		// Enable conversions

	// Sampling and conversion start
	ADC12CTL0 |= ADC12SC;                   		
	__bis_SR_register(LPM4_bits + GIE);     		// LPM4 with interrupts enabled
	__no_operation();
	  
	// On wakeup read value and turn off ADC and Ref module to save power
	ret = (uint16_t)((reading * (unsigned long)v_ref) / 4096);
	reading = 0;

	ADC12CTL0 &= ~ADC12ON; 
	REFCTL0 &= ~REFON;
	ADC12CTL0 &= ~ADC12ENC;

	return ret;
}
/*---------------------------------------------------------------------------*/



interrupt(ADC12_VECTOR)
ADC12ISR (void)
{
	 switch(ADC12IV)
	  {
	  case  0: break;                           // Vector  0:  No interrupt
	  case  2: break;                           // Vector  2:  ADC overflow
	  case  4: break;                           // Vector  4:  ADC timing overflow
	  case  6:                                  // Vector  6:  ADC12IFG0
	    reading = ADC12MEM0;                    // Move results, IFG is cleared
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
