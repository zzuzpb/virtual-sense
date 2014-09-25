/*
 *      javax_virtualsense_sensosors_ADC.c
 *
 *  Copyright (c) 2011 DiSBeF, University of Urbino.
 *
 *      This file is part of VirtualSense.
 *
 *      VirtualSense is free software: you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation, either version 3 of the License, or
 *      (at your option) any later version.
 *
 *      VirtualSense is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *
 *      You should have received a copy of the GNU General Public License
 *      along with VirtualSense.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * ADC access methods.
 *
 * @author Emanuele Lattanzi
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// generated at infusion time
#include "base_definitions.h"
#include "platform-conf.h"
#include "common/execution/execution.h"
#include "common/heap/heap.h"
#include "dev/adc.h"

//#include "common/djtimer.h"

// short javax.virtualsense.sensors.getNrADCs()
//void javax_virtualsense_sensors_ADC_short_getNrADCs()
//{
//       dj_exec_stackPushShort(NUM_CHANNEL);
//}


// short javax.virtualsense.sensors.ADC.readIntRef(short channel short ref);
void javax_virtualsense_sensors_ADC_short_readIntRef_short_short()
{
        uint16_t ref = dj_exec_stackPopShort();
        uint16_t channel = dj_exec_stackPopShort();

        uint16_t value = read_channel_intref(channel, ref);
        dj_exec_stackPushShort(value);
}


// short javax.virtualsense.sensors.ADC.readExtRef(short channel short vrefN, short vrefP);
void javax_virtualsense_sensors_ADC_short_readExtRef_short_short_short()
{
        uint16_t vref_p = dj_exec_stackPopShort();
        uint16_t vref_n = dj_exec_stackPopShort();
        uint16_t channel = dj_exec_stackPopShort();

        uint16_t value = read_channel_extref(channel, vref_n, vref_p);
        dj_exec_stackPushShort(value);
}


// short javax.virtualsense.sensors.ADC.readVccLevel();
void javax_virtualsense_sensors_ADC_short_readVccLevel()
{
        uint16_t value = read_channel_intref(11, 2);
        dj_exec_stackPushShort(value);
}


