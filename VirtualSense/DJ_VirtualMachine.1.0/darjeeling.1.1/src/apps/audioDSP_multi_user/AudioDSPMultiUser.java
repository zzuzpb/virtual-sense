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
    	dspEn.write(true);
		codecEn.write(false);
		//boolean state = false;
         
        while(true)
        {   
        	// Enable dsp
    		Leds.setLed(Leds.LED0, true);
    		dspEn.write(false);
			codecEn.write(true);
			
			Thread.sleep(5000);
			
			// Write time on uart
			char c = 0x00;
			c = 0x0F;
			UART.write(c);
    		c = 0x09;
    		UART.write(c);
    		c = 0x0E;
    		UART.write(c);
    		c = 0x0C;
    		UART.write(c);
    		c = 0x1A;
    		UART.write(c);
    		c = 0x00;
    		UART.write(c);
    		//c = '\n';
    		//System.out.print(c);

    		
    		// Wait for ACI
    		String read;
    		do{
    			read = UART.readline();
    		}while(read == "stop");
    		
    		
    		
 
        }
    }
}