
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



/**
 * Simple Radio Test Application application.
 * 
 * @author Emanuele Lattanzi
 *
 */
public class Node
{
	static short nodeId = -1;
	     
    public static void motemain() {  
    	boolean state = true;
    	short index = 0;
        short value = 0;
        char label[] = {'-','-','-','i','n'};        //in
        char label2[] = {'-','-','o','u','t'};        //out
        nodeId = VirtualSense.getNodeId();
        Network myNetwork = new Network(Protocol.MINPATH);
        
        Leds.setLed(0, false); 
        Leds.setLed(1, false); 
        Leds.setLed(2, false);
        
        PeopleCounter people = new PeopleCounter();
        people.start();
        
        // Enable bluetooth serial port
		DigitalPin enBTH = new DigitalPin(false, DigitalPin.DIO3);
		enBTH.write(false);
        
        while(true) {
        	System.out.print("PC - nodeId: ");
    		System.out.println(nodeId);
    		/*
    		DataMsg data = new DataMsg(nodeId, index++);
    		
    		value = people.in;
    		
    		data.value = value;
    		data.label = label;
    		
    		System.out.print("PC - people in: ");
	   		System.out.println(data.value);
	   		
	   		Leds.setLed(0, state);        		
    		myNetwork.send(data);
    		VirtualSense.printTime();
            System.out.println("PC - packet sent");    		
    		
            
            data = new DataMsg(nodeId, index);
            
			value = people.out; 			
			data.value = value;
    		data.label = label2;
    		
			System.out.print("PC - people out: ");
	   		System.out.println(data.value);
	   		
	   		Leds.setLed(0, state);        		
    		myNetwork.send(data);
    		VirtualSense.printTime();
            System.out.println("PC - packet sent");    
    		
	   		state =! state;*/
    		
    		// Sleep period
			Thread.sleep(35000);
			Thread.sleep(35000);
        }
    }
    
}
