/*
 * ADC.java
 * 
 * Copyright (c) 2008-2010 CSIRO, Delft University of Technology.
 * 
 * This file is part of Darjeeling.
 * 
 * Darjeeling is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Darjeeling is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with Darjeeling.  If not, see <http://www.gnu.org/licenses/>.
 */
 
 
package javax.virtualsense.sensors;

/**
 * 
 * API for ADC (analog-to-digital) converters.
 * 
 * @author Emanuele Lattanzi
 *
 */
public class ADC
{
        /**
         * Channels 
         */
        public static final short CHANNEL0      = 0;
        public static final short CHANNEL1      = 1;
        public static final short CHANNEL2      = 2;
        public static final short CHANNEL3      = 3;
        public static final short CHANNEL4      = 4;
        public static final short CHANNEL5      = 5;
        public static final short CHANNEL6      = 6;
        
        /**
         * Internal references
         */
        public static final short REF1_5V       = 0;    // The level to be read must be between 0 to 1500mV
        public static final short REF2V         = 1;    // The level to be read must be between 0 to 2000mV
        public static final short REF2_5V       = 2;    // The level to be read must be between 0 to 2500mV
        
        
        /**
         * Reads a value from an ADC channel, using internal referiments, measured between 0 and vref.
         * @param channel to read.
         * @param reference to use. 
         * @return actual level measured from selected channel in mV.
         */
        public static native short readIntRef(short channel, short ref);
        
        /**
         * Reads a value from an ADC channel, using as referiments the value of pin VREF- and VREF+ on CONN2.
         * @param channel to read.
         * @param vref- negative reference.
         * @param vref+ positive reference.
         * @return actual level measured from selected channel in mV, measured between VREF- and VREF+.
         */
        public static native short readExtRef(short channel, short vrefN, short vrefP);
        
        /**
         * Reads actual Vcc level.
         * @return actual Vcc level in mV.
         */
        public static native short readVccLevel();

}
