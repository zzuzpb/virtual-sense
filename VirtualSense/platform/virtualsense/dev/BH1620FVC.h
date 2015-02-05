/*
 *  BH1620FVC.h
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

#define BH1620FVC_POWER_PORT		GPIO_C_BASE	//GPIO_D_BASE
#define BH1620FVC_POWER_PIN			GPIO_PIN_0	//GPIO_PIN_5

#define BH1620FVC_GC1_PORT			GPIO_C_BASE	//GPIO_D_BASE
#define BH1620FVC_GC1_PIN			GPIO_PIN_0	//GPIO_PIN_5

#define BH1620FVC_GC2_PORT			GPIO_C_BASE	//GPIO_D_BASE
#define BH1620FVC_GC2_PIN			GPIO_PIN_0	//GPIO_PIN_5

#define BH1620FVC_LGAIN				0
#define BH1620FVC_MGAIN				1
#define BH1620FVC_HGAIN				2

void init_BH1620FVC(void);
void set_gain_BH1620FVC(uint8_t);
uint16_t read_BH1620FVC(void);


