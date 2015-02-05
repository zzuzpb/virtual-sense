/*
 *	adc.h
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

#ifndef ADC_H
#define ADC_H


#include <stdint.h>

#include "lib/_adc.h"

void adc_init(void);

uint16_t adc_read(uint32_t, uint32_t);


#endif /* __ADC_H__ */
