/*
 *  radio-driver.h
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
 * radio driver for cc2520 RF module.
 *
 * @author Emanuele Lattanzi
 *
 */

#ifndef RADIO_DRIVER_H_
#define RADIO_DRIVER_H_

#include "sys/process.h"

/* Driver state */
typedef enum {
        ON, OFF
} radio_driver_state_t;

extern const struct radio_driver radio_driver;

PROCESS_NAME(radio_driver_process);
extern void isolateMCU(void);

#endif /*RADIO_DRIVER_H_*/
