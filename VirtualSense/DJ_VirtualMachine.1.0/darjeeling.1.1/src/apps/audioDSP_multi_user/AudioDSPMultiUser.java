/*
 *	Blink.java
 * 
 *	Copyright (c) 2012 DiSBeF, University of Urbino.
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
 * Simple Blink application.
 * 
 * @author Emanuele Lattanzi
 *
 */
import javax.virtualsense.powermanagement.PowerManager;
import javax.virtualsense.actuators.Leds;
import javax.virtualsense.VirtualSense;
import java.lang.Runtime;
import javax.virtualsense.digitalio.DigitalPin;
import javax.virtualsense.digitalio.bus.*;

public class AudioDSPMultiUser
{
    public static void motemain()
    {
    	//AudioDSP d = new AudioDSP();
    	DigitalPin dspEn = new DigitalPin(false, DigitalPin.DIO3);
    	DigitalPin codecEn = new DigitalPin(false, DigitalPin.DIO2);
         
        while(true)
        {   
        	dspEn.write(true);
    		codecEn.write(false);
    		
			int temp = VirtualSense.getSecond() + 3;
			while(VirtualSense.getSecond() < temp)
				Thread.sleep(500);     	
        	
        	// Enable dsp
    		dspEn.write(false);
			codecEn.write(true);
			Leds.setLed(Leds.LED0, true);
			
			// Wait for dsp init
			temp = VirtualSense.getSecond() + 8;
			while(VirtualSense.getSecond() < temp)
				Thread.sleep(500); 
					
			// Write time on uart
			Leds.setLed(Leds.LED1, true);
			char datetime[] = {0x0F, 0x09, 0x0E, 0x0C, 0x1A, 0x00};
			for(int i = 0; i<datetime.length; i++)
				UART.write(datetime[i]);
			
			// Read -ACI-
			short aci1, aci2, aci3;
			
    		// Wait for aci1
    		String read = "";
    		do{
    			read = UART.readline();
    		}while(!read.equals("ACI"));	
    		// Read aci1
    		aci1 = UART.read();
    		
    		aci2 = UART.read();
    		
    		aci3 = UART.read();
    			
    		System.out.print("Read ACI1: ");
    		System.out.println(aci1);
    	
    		System.out.print("Read ACI2: ");
    		System.out.println(aci2);
    		
    		System.out.print("Read ACI3: ");
    		System.out.println(aci3);
    		Leds.setLed(Leds.LED2, true);

    		
    		Thread.sleep(1000);
    		Leds.setLed(Leds.LED0, false);
    		Leds.setLed(Leds.LED1, false);
    		Leds.setLed(Leds.LED2, false);
        }
    }
}