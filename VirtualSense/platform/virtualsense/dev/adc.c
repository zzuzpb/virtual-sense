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

uint16_t read_adc12(uint32_t);



void 
adc_init()
{
	//Enable A/D channel inputs
	ADC_PORT_DIR &=~(ADC_CH0_BIT + ADC_CH1_BIT + ADC_CH2_BIT + ADC_CH3_BIT + ADC_CH4_BIT + ADC_CH5_BIT + ADC_CH6_BIT);
	ADC_PORT_OUT &=~(ADC_CH0_BIT + ADC_CH1_BIT + ADC_CH2_BIT + ADC_CH3_BIT + ADC_CH4_BIT + ADC_CH5_BIT + ADC_CH6_BIT);
	ADC_PORT_SEL |= ADC_CH0_BIT + ADC_CH1_BIT + ADC_CH2_BIT + ADC_CH3_BIT + ADC_CH4_BIT + ADC_CH5_BIT + ADC_CH6_BIT;

	P2DIR |= BIT7 | BIT6;								// Set ACLK and ADCLK as input
	P5DIR |= BIT6 | BIT7;								// Set P5.6 and P5.7 as input

  	P5OUT &= ~(BIT7 | BIT6);							// Set VREF+ and VREF-
  	P5OUT |= BIT7; 										// H-GAIN MODE GC1 1 and GC2 0
}
/*---------------------------------------------------------------------------*/



uint16_t read_channel_intref(short channel, short ref_sel)
{
	uint16_t ret = 0;
	uint32_t ref = ADC_INTREF_2_5;			// If value of reference is wrong ref default is 2.5V
	uint16_t ref_v = 2500;


	switch(ref_sel)
	{
		case 0:		// Ref 1.5V
		  ref = ADC_INTREF_1_5;
		  ref_v = 1500;
		  break;

		case 1:		// Ref 2V
		  ref = ADC_INTREF_2;
		  ref_v = 2000;
		  break;

		case 2:		// Ref 2.5V
		  ref = ADC_INTREF_2_5;
		  ref_v = 2500;
		  break;
	}

	// Turn on Ref module, set and use refuse reference for sampling
	REFCTL0 = REFMSTR + REFON + ref;
	ADC12MCTL0 = ADC_INTREF;

  	switch(channel)
	{
  		case 0:
		  ret = read_adc12(ADC_CH_0);
  	  	  break;

		case 1:
		  ret = read_adc12(ADC_CH_1);
  	  	  break;

		case 2:
		  ret = read_adc12(ADC_CH_2);
  	  	  break;

		case 3:
		  ret = read_adc12(ADC_CH_3);
  	  	  break;

		case 4:
		  ret = read_adc12(ADC_CH_4);
  	  	  break;

		case 5:
		  ret = read_adc12(ADC_CH_5);
  	  	  break;

		case 6:
		  ret = read_adc12(ADC_CH_6);
  	  	  break;

		case 10:
		  ret = read_adc12(ADC_CH_INTEMP);
  	  	  break;

		case 11:
		  ret = (read_adc12(ADC_CH_VBAT) * 2);
  	  	  break;
	}

  	return ret;
}
/*---------------------------------------------------------------------------*/



uint16_t read_channel_extref(short channel, short ref_n, short ref_p)
{
	// use reference from Ref module
		ADC12MCTL0 = ADC_EXTREF;


}
/*---------------------------------------------------------------------------*/



uint16_t read_adc12(uint8_t ch)
{	
	uint16_t ret = 0;
  	ADC12CTL0 = ADC12SHT0_8 + ADC12ON;		    // Turn on ADC and set sample time
	ADC12CTL1 = ADC12SHP;                     	// Enable sample timer
	ADC12MCTL0 += ch;  							// Select ADC channel
	
	ADC12IE = 0x001; 							// Enable interrupt on MEM0
	__delay_cycles(75);
	ADC12CTL0 |= ADC12ENC;                    	// Enable conversions

	// Sampling and conversion start
	ADC12CTL0 |= ADC12SC;                   		
	__bis_SR_register(LPM4_bits + GIE);     	// LPM4 with interrupts enabled
	__no_operation();
	  
	// On wakeup read value and turn off ADC and Ref module to save power
	//printf("reading long %d\n", &reading);
	//printf("reading uint %d", &(uint16_t)reading);
	ret = (uint16_t)((reading * (unsigned long)2500) / 4096);
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
	  case 12: break;              	             // Vector 12:  ADC12IFG3
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
