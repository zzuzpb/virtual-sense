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

public class AlarmMultiUser
{
    public static void motemain()
    {
    	int th = 40;
    	int samplingInterval = 500;
    	Network.init(); 
    	short nodeId = VirtualSense.getNodeId();
    	
        while(true)
        {
            int value = Light.getValue();
            if(value >= th){
            	// send the avg using null protocol (broadcast)	            
	            byte data[] = new byte[6];
	    		data[0] = (byte)(nodeId>>8); // this is node id
	          	data[1] = (byte)(nodeId & 0xff); // this node id
	          	data[2] = (byte)(value >> 24); // this node id
	          	data[3] = (byte)(value >> 16); // this node id
	          	data[4] = (byte)(value >> 8); // this node id
	          	data[5] = (byte)(value & 0xff); // this node id
	          	
	    		Packet p = new Packet(data);
	    		Network.send(p);
	    		VirtualSense.printTime();
	            System.out.println(" -- SENDER packet sent");
	            Leds.setLed(0, true);
            }else {
            	Leds.setLed(0, false);
            }
	        Thread.sleep(samplingInterval);
	        System.out.print("Light = ");
	        System.out.println(value);	          
        }
    }
}
