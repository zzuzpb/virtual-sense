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
import javax.virtualsense.network.Network;
import javax.virtualsense.network.Packet;

public class MonitorMultiUser
{
    public static void motemain()
    {
    	int avg = 0;
    	int samples = 50;
    	int samplingInterval = 200;
    	boolean state = true;
    	
        while(true)
        {
            
	            avg = 0;
	            int t0 = System.currentTimeMillis();
	            for(int i = 0; i < samples; i++){
	            	Leds.setLed(4,state);
	            	//avg+=Temperature.getValue();
	            	avg+=i;
	            	Thread.sleep(samplingInterval);
	            	state = !state;
	            }
	            avg = avg / samples;
	    		VirtualSense.printTime();
	    		int t1 = System.currentTimeMillis();
	            System.out.println(" -- monitor done (1)");	            
	            System.out.println(t1-t0);
	            Thread.sleep(100);
	             
        }
    }
}
