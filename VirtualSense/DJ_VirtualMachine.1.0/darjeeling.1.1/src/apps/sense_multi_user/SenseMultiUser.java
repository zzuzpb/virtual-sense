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
        	System.out.print("ptR-int4: ");
        	System.out.println(ADC.readIntRef(ADC.CHANNEL2, ADC.REF2_5V));
        	
        	System.out.print("ptT-int3: ");
        	System.out.println(ADC.readIntRef(ADC.CHANNEL3, ADC.REF2_5V));
        	
        	System.out.print("soglia: ");
        	System.out.println(ADC.readIntRef(ADC.CHANNEL4, ADC.REF2_5V));
        	
        	
        	Thread.sleep(500);
        	/*System.out.print("letto: ");
        	System.out.println(read);
        	Thread.sleep(500);
        	short read = ADC.read(ADC.CHANNEL3, ADC.REF2_5V);
        	Thread.sleep(500);
        	System.out.print("letto: ");
        	System.out.println(read);
        	Thread.sleep(500);
        	short read = ADC.read(ADC.CHANNEL4, ADC.REF2_5V);
        	Thread.sleep(500);
        	System.out.print("letto: ");
        	System.out.println(read);
        	Thread.sleep(500);*/
        	
        	/*
        	System.out.print("ADC0: ");
        	System.out.print(ADC.read(ADC.CHANNEL0, ADC.REF2_5V));
        	System.out.println("mV");
        	Thread.sleep(1000);

        	System.out.print("ADC1: ");
        	System.out.print(ADC.read(ADC.CHANNEL1, ADC.REF2_5V));
        	System.out.println("mV");
        	Thread.sleep(1000);
        	
        	System.out.print("ADC2: ");
        	System.out.print(ADC.read(ADC.CHANNEL2, ADC.REF2_5V));
        	System.out.println("mV");
        	Thread.sleep(1000);
        	
        	System.out.print("Ext. temp: ");
        	System.out.print(Temperature.getValue());
        	System.out.println("°C");
        	Thread.sleep(1000);
        	
        	System.out.print("Ext. pressure: ");
        	System.out.print(Pressure.getValue());
        	System.out.println("bar");
        	Thread.sleep(1000);
        	
        	System.out.print("Light intensity: ");
        	System.out.print(Light.getValue());
        	System.out.println("lx");
        	Thread.sleep(1000);
        	
        	System.out.print("Battery voltage: ");
        	System.out.print(ADC.read(ADC.BATTERY_CHANNEL, ADC.REF2V));
        	System.out.println("mV");
        	Thread.sleep(1000);*/
        	
        	//int avg = 0;
        	//short sum = 0;
        	//short i = 0;
        	
        	/*
        	for(i = 0; i < 4; i++)
        	{
        		System.out.print("Livello adc");System.out.print(i);
        		System.out.print(" ref: ");System.out.print(ADC.REF1_5V);System.out.print("> ");
            	System.out.println(ADC.read((short)i, ADC.REF1_5V));
        		
        		System.out.print("Livello adc");System.out.print(i);
        		System.out.print(" ref: ");System.out.print(ADC.REF2V);System.out.print("> ");
            	System.out.println(ADC.read((short)i, ADC.REF2V));
        		
            	System.out.print("Livello adc");System.out.print(i);
        		System.out.print(" ref: ");System.out.print(ADC.REF2_5V);System.out.print("> ");
            	System.out.println(ADC.read((short)i, ADC.REF2_5V));
            	
            	Thread.sleep(1000);
        	}*/
        	
        	/*System.out.print("Livello adc: ");
        	System.out.println(ADC.read((short)11, ADC.REF2V));
        	Thread.sleep(1000);*/
        	
        	/*System.out.print("Temp core: ");
        	System.out.println(Temperature.getBoardValue());
        	Thread.sleep(1000);*/
        	
        	/*System.out.print("Light: ");
        	System.out.println(Light.getValue());
        	Thread.sleep(1000);*/
        }
    }
}
