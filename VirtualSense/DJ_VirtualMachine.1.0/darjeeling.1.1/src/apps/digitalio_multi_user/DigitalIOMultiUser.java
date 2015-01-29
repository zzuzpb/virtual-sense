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
import javax.virtualsense.VirtualSense;

public class DigitalIOMultiUser
{
	static int  temp = 0;
    public static void motemain()
    {
        
    	 
         //InterruptPin io = new InterruptPin(true, InterruptPin.INT1, InterruptPin.PULL_UP);
         DigitalPin dio0 = new DigitalPin(true, DigitalPin.DIO0);
         DigitalPin dio1 = new DigitalPin(true, DigitalPin.DIO1);
         DigitalPin dio2 = new DigitalPin(true, DigitalPin.DIO2);
         DigitalPin dio3 = new DigitalPin(true, DigitalPin.DIO3);
         while(true)
         {                     	
     		//Leds.setLed(0,true); 
     		//Leds.setLed(1, false);
        	//System.out.println("Wait for interrupt on INT1...");
     		//io.waitForInterrupt();
        	 
        	System.out.print("DIO0 - ");
        	System.out.println(dio0.read()?"up":"down");
     		
        	System.out.print("DIO1 - ");
        	System.out.println(dio1.read()?"up":"down");
        	
        	System.out.print("DIO2 - ");
        	System.out.println(dio2.read()?"up":"down");
        	
        	System.out.print("DIO3 - ");
        	System.out.println(dio3.read()?"up":"down");
        	
        	
     		Thread.sleep(1000);

     		VirtualSense.printTime();
            System.out.print("Free mem: ");
            System.out.println(Runtime.freeMemory());
            
         }
       
    }
}
