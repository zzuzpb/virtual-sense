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


import javax.virtualsense.network.Network;
import javax.virtualsense.network.Packet;
import javax.virtualsense.actuators.Leds;
import javax.virtualsense.powermanagement.PowerManager;
import javax.virtualsense.VirtualSense;


public class Node
{
	     
    public static void motemain()
    {   	
    	 /* slow down the system clock 
         * (normally it is configured at 10 ms)
         * to reduce power consumption 
         * leaves the CPU in the LPM3 state */        
        PowerManager.setSystemClockMillis(50);	
        short nodeId = VirtualSense.getNodeId();
        Network myNetwork = new Network(new MinPathProtocol()); 
        sender(nodeId, myNetwork);
       
            
    }    
    public static void sender(short nodeId, Network myNetwork){
    	short i = 0;
    	boolean state = true;
    	while(true)
    	{    
    		Thread.sleep(10000);
    		DataMsg data = new DataMsg();
    		data.counter = i++;
    		Leds.setLed(0, state);        		
    		myNetwork.send(data);
    		VirtualSense.printTime();
            System.out.println(" -- SENDER packet sent");    		
    		state =! state;    	        		
    	}          
    }
}
