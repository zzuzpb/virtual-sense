/*
 *	CO2Reader.java
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

import javax.virtualsense.network.*;
import javax.virtualsense.network.protocols.minpath.*;
import javax.virtualsense.sensors.*;
import javax.virtualsense.network.Packet;
import javax.virtualsense.actuators.Leds;
import javax.virtualsense.sensors.Temperature;
import javax.virtualsense.sensors.Pressure;
import javax.virtualsense.sensors.Light;
import javax.virtualsense.powermanagement.PowerManager;
import javax.virtualsense.VirtualSense;
import javax.virtualsense.digitalio.DigitalPin;
import javax.virtualsense.digitalio.bus.*;



/**
 * Collegio multi user C02Reader app
 * 
 * @author Emanuele Lattanzi
 *
 */
public class NoiseReader
{
	static short nodeId = -1;
	     
    public static void motemain() {   		
        boolean state = true;
        short index = 0;
        nodeId = VirtualSense.getNodeId();
        Network myNetwork = new Network(Protocol.MINPATH);
        
        Leds.setLed(0, false); 
        Leds.setLed(1, false); 
        Leds.setLed(2, false);
        
        while(true) {  
        	System.out.print("NOISE - nodeId: ");
    		System.out.println(nodeId);
    		
    		DataMsg data = new DataMsg();
    		data.counter = index++;
    		data.sender_id = nodeId;
    		data.route = 0;
    		data.noise = readNoise(nodeId);
    		
    		System.out.print("NOISE - noise level: ");
	   		System.out.println(data.light);
    		
	   		Leds.setLed(0, state);        		
    		myNetwork.send(data);
    		VirtualSense.printTime();
            System.out.println("NOISE - packet sent");    		
    		state =! state;
    		
    		// Sleep period
			Thread.sleep(35000);
			Thread.sleep(35000);	
    	}
    } 
    
    private static short readNoise(short nodeId)
    {
		int read = 0;
		int sum = 0;
		int avgLev = 1080;
		
    	for(int i = 0; i < 200; i++)
    	{
    		read = (int)ADC.readIntRef((nodeId == 8)?ADC.CHANNEL6:ADC.CHANNEL6, ADC.REF2_5V);
    		
    		sum += (read >= avgLev)?read - avgLev:avgLev - read;
    		
    		Thread.sleep(10);
    	}
   		
    	return (short) (sum / 200);	
    }
}
    
    

