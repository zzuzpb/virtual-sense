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
/*---------------------------------------------------------------------------*/
void 
adc_init()
{
	ADC_SEL_PORT |= TEMP_PIN+LIGHT_PIN+HUMIDITY_PIN;   //Enable A/D channel inputs
	P7DIR = 0xff;
	P7OUT = 0x00;
	  /* Initialize ADC12_A */

  	P7OUT |= BIT2;
  	P7OUT &=~BIT3; // H-GAIN MODE GC1 1 and GC2 0

#if 1
  	  REFCTL0 |= REFMSTR + REFVSEL_0 + REFON;    // Enable internal 1.5V reference
  	  ADC12CTL0 = ADC12ON+ADC12MSC+ADC12SHT1_2; // Turn on ADC12, set sampling time
  	  ADC12CTL1 = ADC12SHP+ADC12CONSEQ_1;       // Use sampling timer, single sequence
  	  ADC12MCTL10 = ADC12SREF_1 + ADC12INCH_10;
	  ADC12MCTL13 = ADC12INCH_13;                 // ref+=AVcc, channel = A0 humidity
	  ADC12MCTL14 = ADC12INCH_14;                 // ref+=AVcc, channel = A1 light
	  ADC12MCTL15 = ADC12INCH_15+ADC12EOS;        // ref+=AVcc, channel = A2 temp

#endif

#if 0
	  ADC12CTL0 = ADC12ON+ADC12MSC+ADC12SHT1_8; // Turn on ADC12_A, extend sampling time
	                                             // to avoid overflow of results
	   ADC12CTL1 = ADC12SHP+ADC12CONSEQ_3;       // Use sampling timer, repeated sequence
	   ADC12MCTL13 = ADC12INCH_13;                 // ref+=AVcc, channel = A0
	   ADC12MCTL14 = ADC12INCH_14;                 // ref+=AVcc, channel = A1
	   ADC12MCTL15 = ADC12INCH_15+ADC12EOS;                 // ref+=AVcc, channel = A2
	   //ADC12MCTL3 = ADC12INCH_3+ADC12EOS;        // ref+=AVcc, channel = A3, end seq.
	   //ADC12IE = 0xff;                           // Enable ADC12IFG.3
	   ADC12CTL0 |= ADC12ENC;                    // Enable conversions
	   ADC12CTL0 |= ADC12SC;                     // Start conversion - software trigger
#endif
}
/*---------------------------------------------------------------------------*/

uint16_t 
get_adc(int channel)
{
  uint16_t reading;
  ADC12CTL0 |= ADC12ENC;                    // Enable conversions
  ADC12CTL0 |= ADC12SC;                   // Start convn - software trigger
  while(ADC12IV & ADC12IFG15);
  switch(channel){
  case TEMP_CHANNEL:
  	  reading = ADC12MEM15;
  	  break;
  case LIGHT_CHANNEL:
  	  reading = ADC12MEM14;
  	  break;
  case HUMIDITY_CHANNEL:
	  reading = ADC12MEM13;
	  break;
  case BOARD_TEMP_CHANNEL:
	  reading = ADC12MEM10;
	  break;
  }
  ADC12CTL0 &= ~ADC12ENC;		//stop conversion
  return reading;
}
/*---------------------------------------------------------------------------*/
