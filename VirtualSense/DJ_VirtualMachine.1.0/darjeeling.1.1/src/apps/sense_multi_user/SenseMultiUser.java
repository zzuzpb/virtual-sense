/*
 *	Blink.java
 * 
 *	Copyright (c) 2008 CSIRO, Delft University of Technology.
 * 
 *	This file is part of Darjeeling.
 * 
 *	Darjeeling is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	Darjeeling is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 * 
 *	You should have received a copy of the GNU General Public License
 *	along with Darjeeling.  If not, see <http://www.gnu.org/licenses/>.
 */
import javax.virtualsense.actuators.Leds;

import javax.virtualsense.sensors.*;
import javax.virtualsense.VirtualSense;
import javax.darjeeling.Darjeeling;

public class SenseMultiUser
{
    public static void motemain()
    {
        while(true)
        {

        	System.out.print("ADC0: ");
        	System.out.print(ADC.readIntRef(ADC.CHANNEL0, ADC.REF2_5V));
        	System.out.println("mV");
        	Thread.sleep(500);

        	System.out.print("ADC1: ");
        	System.out.print(ADC.readIntRef(ADC.CHANNEL1, ADC.REF2_5V));
        	System.out.println("mV");
        	Thread.sleep(500);
        	
        	System.out.print("ADC2: ");
        	System.out.print(ADC.readIntRef(ADC.CHANNEL2, ADC.REF2_5V));
        	System.out.println("mV");
        	Thread.sleep(500);
        	
        	System.out.print("Battery voltage: ");
        	System.out.print(ADC.readVccLevel());
        	System.out.println("mV");
        	Thread.sleep(500);  	
        	
        	System.out.print("Temperature: ");
        	System.out.print(Temperature.getValue());
        	System.out.println("mC");
        	Thread.sleep(500);
        	
        	System.out.print("Board temperature: ");
        	System.out.print(Temperature.getBoardValue());
        	System.out.println("mC");
        	Thread.sleep(500);
        	
        	System.out.print("Pressure: ");
        	System.out.print(Pressure.getValue());
        	System.out.println("hPa");
        	Thread.sleep(500);
        	
        	System.out.print("Light intensity: ");
        	System.out.print(Light.getValue());
        	System.out.println("lx");
        	
            System.out.print("Free mem: ");
            System.out.println(Runtime.freeMemory());
            System.out.println("");
            Thread.sleep(1000);
        	
        }
    }
}
