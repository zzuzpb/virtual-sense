/*
 *	Bench.java
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
 * Energy benchmarks for state characterization.
 * 
 * @author Emanuele Lattanzi
 *
 */

import javax.virtualsense.powermanagement.PowerManager;
import javax.virtualsense.actuators.Leds;

public class BenchMultiUser
{
    public static void motemain()
    {
        /* slow down the system clock 
         * (normally it is configured at 10 ms)
         * to reduce power consumption 
         * leaves the CPU in the LPM3 state */        
        //PowerManager.setSystemClockMillis(100);
        while(true)
        {
        	 Thread.sleep(3000); 
            /* need to go in the LPM3 mode in order to identify the 
        	 * power consumed for scheduler activation
        	 * then resume from LPM3 using the timer interrupt and 
        	 * immediatly ibernate the machine. For this bench the systemIbernation needs to 
        	 * put the MCU in the LPM3 instead of the LPM4.5 or LPM4 */
        	//System.out.println("Going in deep sleep mode");
        	//PowerManager.deepSleep(1);
        	System.out.println("Now ibernate");
	        PowerManager.systemHibernation(3);
	        System.out.println("Ibernation done");
	        //Thread.sleep(3000);      
            
        }
    }
}
