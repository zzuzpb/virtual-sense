/*
 *  BH1620FVC.c
 *
 *  Copyright (c) 2013 DiSBeF, University of Urbino.
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
 * The BH1620FVC light sensor driver.
 *
 * @author Matteo Dromedari
 *
 */


#include "board.h"

#include "lib/_gpio.h"
#include "dev/adc.h"
#include "BH1620FVC.h"


#define DEBUG 1
#if DEBUG
#include <stdio.h>
#define PRINTF(...) printf(__VA_ARGS__)
#define PRINTDEBUG(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#define PRINTDEBUG(...)
#endif

void BH1620FVC_POWER_UP(){
	//GPIOPinWrite(BH1620FVC_POWER_PORT, BH1620FVC_POWER_PIN, BH1620FVC_POWER_PIN);
}

void BH1620FVC_POWER_DOWN(){
	//GPIOPinWrite(BH1620FVC_POWER_PORT, BH1620FVC_POWER_PIN, BH1620FVC_POWER_PIN & 0x00);
}


void init_BH1620FVC(){

	// Configure BH1620FVC power pin, gain control pins and shut down it
	/*GPIOPinTypeGPIOOutput(BH1620FVC_POWER_PORT, BH1620FVC_POWER_PIN);
	GPIOPinTypeGPIOOutput(BH1620FVC_GC1_PORT, BH1620FVC_GC1_PIN);
	GPIOPinTypeGPIOOutput(BH1620FVC_GC2_PORT, BH1620FVC_GC2_PIN);
	BH1620FVC_POWER_DOWN();*/

	// Init ADC
	adc_init();
}


void set_gain_BH1620FVC(uint8_t gain){

	/*switch(gain){
		case BH1620FVC_LGAIN:{
			GPIOPinWrite(BH1620FVC_GC1_PORT, BH1620FVC_GC1_PIN, BH1620FVC_GC1_PIN);
			GPIOPinWrite(BH1620FVC_GC2_PORT, BH1620FVC_GC2_PIN, BH1620FVC_GC2_PIN);
			}
			break;
		case BH1620FVC_MGAIN:{
			GPIOPinWrite(BH1620FVC_GC1_PORT, BH1620FVC_GC1_PIN, BH1620FVC_GC1_PIN & 0x00);
			GPIOPinWrite(BH1620FVC_GC2_PORT, BH1620FVC_GC2_PIN, BH1620FVC_GC2_PIN);
			}
			break;
		case BH1620FVC_HGAIN:{
			GPIOPinWrite(BH1620FVC_GC1_PORT, BH1620FVC_GC1_PIN, BH1620FVC_GC1_PIN);
			GPIOPinWrite(BH1620FVC_GC2_PORT, BH1620FVC_GC2_PIN, BH1620FVC_GC2_PIN & 0x00);
			}
			break;
		default:
			break;
	}*/
}


uint16_t read_BH1620FVC(void){

	BH1620FVC_POWER_UP();

	//Bh1620fvc spec => M Gain (GC2:1  GC1:0) => Viout = 0.057*10^(-6)* lux*R1
	// R1 = 2.2k
	// mViout = 0.057*10^(-3)* lux*2200
	// lux = (mViout/2200)*57

	uint16_t read = (uint16_t)((uint32_t)(adc_read(ADCLIGHT_CH, SOCADC_REF_INTERNAL) * ADC_INT_REF) / 2047);

	BH1620FVC_POWER_DOWN();

	// Read ADC light channel and convert mV in lx (Lux = lm * m^2)
	return (read * 570) / 2200;
}








