/*
 *	DigitalIOMultiUser.java
 * 
 *	Copyright (c) 2013 DiSBeF, University of Urbino.
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
 * Simple Interrupt receiver application.
 * 
 * @author Emanuele Lattanzi
 *
 */
import javax.virtualsense.powermanagement.PowerManager;
import javax.virtualsense.digitalio.InterruptPin;
import javax.virtualsense.digitalio.DigitalPin;
import javax.virtualsense.actuators.Leds;
import java.lang.Runtime;

public class DigitalIOMultiUser
{
	static int  temp = 0;
    public static void motemain()
    {
        
    	 
         InterruptPin io = new InterruptPin(false, (short)8);//InterruptPin.INT4);
         //DigitalPin dio = new DigitalPin(false, DigitalPin.DIO2);
         while(true)
         {                     	
     		//Leds.setLed(0,true); 
     		//Leds.setLed(1, false);
        	System.out.println("Wait for interrupt on PC4...");
     		io.waitForInterrupt();
     		//dio.write(true);
     		
     		//Leds.setLed(1, true);
     		//Leds.setLed(0,false);    
     		
     		//io.waitForInterrupt();
     		//dio.write(false);
     		
            System.out.print("Received interrupt! - Free mem: ");
            System.out.println(Runtime.freeMemory());      
            
         }
       
    }
}
