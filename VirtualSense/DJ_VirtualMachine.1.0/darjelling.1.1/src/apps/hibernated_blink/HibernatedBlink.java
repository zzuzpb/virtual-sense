/*
 *	Blink.java
 * 
 *	Copyright (c) 2011 DiSBeF, University of Urbino.
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

import javax.virtualsense.powermanagement.PowerManager;
import javax.virtualsense.actuators.Leds;

public class HibernatedBlink
{
    public static void main(String args[])
    {
        boolean state=true;
        boolean hibernated = false;
        int a = 0;
        
        Leds.setLed(1,false);
        Leds.setLed(2,false);
        Leds.setLed(3,false);
        Leds.setLed(4,false);
        Leds.setLed(5,false);
        Leds.setLed(6,false);
                
        /* slow down the system clock 
         * (normally it is configured at 10 ms)
         * to reduce power consumption 
         * leaves the CPU in the LPM3 state */        
        PowerManager.setSystemClockMillis(1000);
        System.gc();
        while(true)
        {
            for (short i=0; i<7; i++)
            {
                        //Leds.setLed(i,state);
            			//Leds.setLed(0,false);
                        /* invoke gc and write mem status */
            	 		System.out.print("System standby at ");
            	 		System.out.println(System.currentTimeMillis());
            	 		PowerManager.standby();
            	 		System.out.print("System resumed from standby at ");
            	 		System.out.println(System.currentTimeMillis());
            	 		System.out.println("#######################");
            	 		
                        System.out.print("System deepSleep at ");
                        System.out.println(System.currentTimeMillis());
                        PowerManager.deepSleep(1);
                    	System.out.print("System resumed from deepSleep at ");
                    	System.out.println(System.currentTimeMillis());
                    	System.out.println("#######################");
                    	
                        System.out.print("System hibernation at ");
                        System.out.println(System.currentTimeMillis());
                    	PowerManager.systemHibernation(1);
                    	System.out.print("System resumed from hibernation at ");
                    	System.out.println(System.currentTimeMillis());
                    	System.out.println("#######################");
                       /* if(i == 4 && !hibernated){
                        	hibernated = true;
                        	
                        	//PowerManager.deepSleep();
                            
                        }
                        else
                        	Thread.sleep(1000);*/
                        System.out.print("i value ");                           
                        System.out.println(i);
            }
            state=!state;
            
        }
    }
}
