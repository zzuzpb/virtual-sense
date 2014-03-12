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



/**
 * Simple Radio Test Application application.
 * 
 * @author Emanuele Lattanzi
 *
 */
public class Node
{	     
    public static void motemain()
    {   
    	// Set led 0 on
        Leds.setLed(0, true);
        
        System.out.print("nodeId: ");
		System.out.println(VirtualSense.getNodeId());
		
		DigitalPin alarmAND = new DigitalPin(true, DigitalPin.DIO1);
		DigitalPin alarmOR = new DigitalPin(true, DigitalPin.DIO0);
		
    	while(true)
    	{
    		short r = ReaderCO2.read();
    		boolean aAND = alarmAND.read();
    		boolean aOR = alarmOR.read();
    		
    		System.out.print(r); System.out.print(" - AND: ");
    		System.out.print((aAND)?"on ":"off "); System.out.print("- OR: ");
    		System.out.println((aOR)?"on":"off");
	        
    		//Thread.sleep(500);
	  
    	}
    } 
}


    /*
    public static void sender(short nodeId, Network myNetwork)
    {
    	short i = 0;
    	boolean state = true;
    	short index = 0;
    	PeopleCounter people = new PeopleCounter();
    	//RurrentReader cr = new RurrentReader();
    	
    	if(nodeId > 9 && nodeId < 13) 
    	{
    		people.start();
    		
    		// Enable bluetooth serial port
    		DigitalPin enBTH = new DigitalPin(false, DigitalPin.DIO3);
    		enBTH.write(false);
    	}
    	
    	while(true)
    	{  	
    		System.out.print("nodeId: ");
    		System.out.println(nodeId);
    		
    		DataMsg data = new DataMsg();
    		data.counter = index++;
    		data.sender_id = nodeId;
    		data.route = 0;
			data.noise = NoiseReader.read();    			
			data.co2 = ReaderCO2.read();
			
			// Read temperature, pressure and light level
			if(nodeId == 7 || nodeId == 6 || nodeId ==  5 || nodeId ==  8 || nodeId == 3)
			{
				// Node 3 and 5 have temperature on LM19
				if(nodeId == 5 || nodeId == 3)
				{
					data.temp = getLM19Temperature();
				}
				else
				{
					while(data.temp <= 0 || data.temp >= 9999)
					{
						data.temp = Temperature.getValue();
						System.out.print("Rt");
					}
					while((data.pressure <= 700 || data.pressure >= 1200)) // Pressure on node 3 and 5 is not running very well!
					{
						data.pressure = Pressure.getValue();
						System.out.print("Rp");
					}
				}
				
				data.light = Light.getValue();
				System.out.println("");
			}
			
			System.out.print(" ---- temp: ");
	   		System.out.println(data.temp);
	   		System.out.print(" ---- pressue: ");
	   		System.out.println(data.pressure);
	   		System.out.print(" ---- light: ");
	   		System.out.println(data.light);
	   		
	   		
			if(nodeId > 9 && nodeId < 13){
				data.in = people.in;
				data.out = people.out;
			}
			
			if((nodeId != 3) && (nodeId != 4) && (nodeId != 5) && (nodeId != 8)){
    			data.noise = 0;
    			data.co2 = 0;   
    		}
			else
    		{
    			System.out.print(" ---- MEDIA SOUND: ");
    	   		System.out.println(data.noise);
    	   		System.out.print(" ---- CO2: ");
    	   		System.out.println(data.co2);
    		}
			
			if(nodeId == 13)
			{
	    		int sumCurrent = 0;
	    		
	    		for(short c = 0; c < 180; c++)
	    		{
	    			sumCurrent += getCurrentLevel();
	    		}		
	    		data.temp = (short)(sumCurrent / 180);
				
				System.out.print(" ---- Avg Current level: ");
				System.out.println(data.temp);
			}
			
    		Leds.setLed(0, state);        		
    		myNetwork.send(data);
    		VirtualSense.printTime();
            System.out.println(" -- SENDER packet sent");    		
    		state =! state; 		
    		
    		// Sleep period for all without 13
    		if(nodeId != 13)
    		{
	    		if(nodeId != 5 && nodeId != 11)
	    		{
	    			Thread.sleep(35000);
	    			Thread.sleep(35000);
	    		}
	    		else
	    		{
	    			System.out.println(".");
	    			Thread.sleep(35000);
	        		Thread.sleep(35000);
	    		}
    		}
    	}         
    }
    
    private static short getCurrentLevel()
    {
    	short maxCurrentLevel = 0;
		
		for(short i = 0; i < 50; i++)
		{
			short read = ADC.readExtRef(ADC.CHANNEL0, (short)0, (short)3300);
			
			if(read >= (short)2600)
			{
				//System.out.print("+");
				read = (short)(read - 2600);
			}
			else
			{
				read = (short)(2600 - read);
				//System.out.print("-");
			}	
			
			//System.out.println(read);
			if(read > maxCurrentLevel)
				maxCurrentLevel = read;
			
			Thread.sleep(10);
		}
		
		return maxCurrentLevel;
    }
    
    private static short getLM19Temperature()
    {
    	int sum = 0;
    	
    	for(short c = 0; c < 10; c++)
    	{
    		sum += (int)ADC.readIntRef(ADC.CHANNEL0, ADC.REF2_5V);
    	}
    			
    	return (short)(sum / 10); 
    }
}*/
