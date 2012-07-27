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


public class RadioTest
{
    public static void main(String args[])
    {
   	
        /* slow down the system clock 
         * (normally it is configured at 10 ms)
         * to reduce power consumption 
         * leaves the CPU in the LPM3 state */        
        PowerManager.setSystemClockMillis(50);	
        short nodeId = VirtualSense.getNodeId();
       
        
        
        if (nodeId == 1){ // I'am the sink 
        	Network.init(); // null protocol will forward all packets to application level
        	sink();
        }
        else{ // I'am the sender
        	 Network.init(new MinPathProtocol()); 
        	sender(nodeId);
        }
            
    }
    public static void sink(){
    	System.out.println("I'am the SINK!!!");
    	
  		new Thread(){ // The interest sender thread
  	        	public void run(){
  	        		
  	        		System.out.println("Starting interest thread!!!");
  	        		byte i = -126;
  	        		while(true){
  	        			Thread.sleep(15000);
  	        			byte d[] = new byte[3];
  	        			d[0] = 0; //MinPathProtocol.INTEREST;
  	        			d[1] = 0; // num hop
  	        			d[2] = i; // epoch
  	        			i++;
  	        			Packet p = new Packet(d);
  	        			Network.send(p);
  	        			VirtualSense.printTime();
  	        			System.out.println(" INTEREST SENT!!!");
  	        		}           
  	        	}
  	        }.start();   	
  	      Thread.yield();
  		
  	        System.out.println("Starting receiver thread!!!");
  	        while(true){
  	        	Packet p = Network.receive();
  	        	VirtualSense.printTime();
  	        	System.out.print(" SINK -- Packet received from ");
  	        	System.out.println(javax.virtualsense.radio.Radio.getSenderId());                
  	        	byte data[] = p.getData();   
          		for(int i = 0; i< data.length; i++){
          			Leds.setLed(1,true);
          			System.out.print("-");
          			System.out.print(data[i]);
          			Leds.setLed(1,false);
          		}
          		System.out.println("");
          		System.out.println("");
  	        }
    }
    
    public static void sender(short nodeId){
    	byte i = -127;
    	boolean state = true;
    	while(true)
    	{    
    		Thread.sleep(10000);
    		byte data[] = new byte[5];
    		data[0] = 1; //MinPathProtocol.DATA;
    		data[1] = 1; // packet should be forwarded to the sink
    		data[2] = i; // this is the data
    		data[3] = (byte)(nodeId>>8); // this is node id
          	data[4] = (byte)(nodeId & 0xff); // this node id
    		i++;        		                     
    		Leds.setLed(0,state);        		
    		Packet p = new Packet(data);
    		Network.send(p);
    		VirtualSense.printTime();
            System.out.println(" -- SENDER packet sent");
    		
    		state =! state;    	        		
    	}          
    }
}
