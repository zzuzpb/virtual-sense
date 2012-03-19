/*
 *	RadioTest.java
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
 * Simple Radio Test Application application.
 * 
 * @author Emanuele Lattanzi
 *
 */


import javax.virtualsense.radio.Radio;
import javax.virtualsense.actuators.Leds;
import javax.virtualsense.powermanagement.PowerManager;

public class RadioTest
{
    public static void main(String args[])
    {
                     
    	byte data[] = new byte[20];
    	for(int i = 0; i< data.length; i++)
    		data[i] = (byte)i;
    	
        /* slow down the system clock 
         * (normally it is configured at 10 ms)
         * to reduce power consumption 
         * leaves the CPU in the LPM3 state */        
        PowerManager.setSystemClockMillis(500);
        Radio.init();
        while(true)
        {
                  	
    		Leds.setLed(0,false);                
            Radio.send((short)11, data);
            Leds.setLed(0,true); 
            /*for(int i = 0; i< data.length; i++)
            	System.out.print(data[i]);
            System.out.println("");   */
            Thread.sleep(1000);
        }
    }
}
