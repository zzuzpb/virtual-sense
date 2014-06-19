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
 * @author Matteo Dromedari
 *
 */
import javax.virtualsense.powermanagement.PowerManager;
import javax.virtualsense.actuators.Leds;
import javax.virtualsense.VirtualSense;
import java.lang.Runtime;

public class BurnNodeId
{
	private static final short NODEID = 4;
	
    public static void motemain()
    {
    	Leds.setLed(0, false); 
        Leds.setLed(1, false); 
        Leds.setLed(2, false);
        System.out.print("Burn node id...");
        
        Leds.setLed(0, true);
    	short nodeId = VirtualSense.getNodeId();
    	
    	Leds.setLed(1, true);
    	if(nodeId != NODEID) {
    		VirtualSense.setNodeId(NODEID);
    	
    		Leds.setLed(2, true);
	    	System.out.print("New node id is: ");
	    	System.out.println(VirtualSense.getNodeId());
    	}else {
    		System.out.println("Never to do.");
    	}
    }
    
}