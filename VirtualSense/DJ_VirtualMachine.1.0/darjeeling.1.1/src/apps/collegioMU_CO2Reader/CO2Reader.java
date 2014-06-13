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

import javax.virtualsense.network.Network;
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
public class CO2Reader
{
	static short nodeId = -1;
	     
    public static void motemain() {   		
        boolean state = true;
        short index = 0;
        nodeId = VirtualSense.getNodeId();
        Network myNetwork = Network.getInstance();
        
        Leds.setLed(0, false); 
        Leds.setLed(1, false); 
        Leds.setLed(2, false);
        
        while(true) {  
        	System.out.print("CO2 - nodeId: ");
    		System.out.println(nodeId);
    		
    		DataMsg data = new DataMsg();
    		data.counter = index++;
    		data.sender_id = nodeId;
    		data.route = 0;
    		data.co2 = readCO2();
    		
    		System.out.print("CO2 - co2 level: ");
	   		System.out.println(data.co2);
    		
	   		Leds.setLed(0, state);        		
    		myNetwork.send(data);
    		VirtualSense.printTime();
            System.out.println("CO2 - packet sent");    		
    		state =! state;
    		
    		// Sleep period
			Thread.sleep(35000);
			Thread.sleep(35000);	
    	}
    } 
    
    private static short readCO2() {
    	byte ret1, ret2, ret3, ret4;
    	short co2 = 0;    	
    	short counter = 0;
    	
    	do {
	    	 I2C.enable();
	    	 
	    	 I2C.start();
	    	 
	    	 I2C.write((byte)0xD0);     	 
	    	 I2C.write((byte)0x22); 
	    	 I2C.write((byte)0x00);
	    	 I2C.write((byte)0x08);
	    	 I2C.write((byte)0x2A);
	    	 
	    	 I2C.stop();
	    	 
	    	 Thread.sleep(50);
	    	 
	    	 
	    	 I2C.start();
	    	 
	    	 I2C.write((byte)0xD1);
	    	 ret1 = I2C.read(true);
	
	    	 if(ret1 == 33) {
	        	 ret2 = I2C.read(true);
	        	 ret3 = I2C.read(true);
	        	 co2 = (short)(ret2 << 8);
	        	 co2 += (short)ret3;
	        	 
	        	 ret4 = I2C.read(false);      	 
	        	 
	        	 
	        	 /*System.out.print("ret1: ");
	        	 System.out.println(ret1);
	        	 
	        	 System.out.print("ret2: ");
	        	 System.out.println(ret2);
	        	 System.out.print("ret3: ");
	        	 System.out.println(ret3);
	        	 System.out.println("");
	        	 
	        	 System.out.print("CO2 level: ");
	        	 System.out.println(co2);*/
	    	 }
	    	 
	    	 I2C.stop();
	    	 I2C.disable();
	    	 Thread.sleep(1000);
	    	 counter++;
    	} while(co2 == 0 && counter < 5);
    	
    	return co2;
    }
}
    
    

