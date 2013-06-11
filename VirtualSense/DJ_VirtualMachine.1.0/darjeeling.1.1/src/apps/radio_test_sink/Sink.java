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


public class Sink
{
	     
    public static void motemain()
    {   	
    	 /* slow down the system clock 
         * (normally it is configured at 10 ms)
         * to reduce power consumption 
         * leaves the CPU in the LPM3 state */        
        PowerManager.setSystemClockMillis(50);	
        short nodeId = VirtualSense.getNodeId();
        Network.init(); // null protocol will forward all packets to application level
        sink();
        
            
    }
    public static void sink(){
    	System.out.println("SINK!!!");
    	
  		new Thread(){ // The interest sender thread
  	        	public void run(){
  	        		
  	        		System.out.println("Starting interest thread!!!");
  	        		short i = 0;
  	        		while(true){
  	        			Thread.sleep(60000);
  	        			InterestMsg intMsg = new InterestMsg();
  	        			intMsg.epoch = i;
  	        			intMsg.hops = 0;
  	        			intMsg.nodeID = VirtualSense.getNodeId();
  	        			i++;
  	        			Network.send(intMsg);
  	        			VirtualSense.printTime();
  	        			System.out.println(" INTEREST");
  	        		}           
  	        	}
  	        }.start();   	
  	      Thread.yield();
  		
  	        System.out.println("Receiver thread!!!");
  	        while(true){
  	        	Packet p = Network.receive();
  	        	VirtualSense.printTime();
  	        	System.out.println("");
  	        	System.out.print("Packet received from ");
  	        	System.out.println(p.getSender());
  	        	if(p instanceof DataMsg){
  	        		System.out.println("DATA");
  	        		DataMsg d = (DataMsg)p;
  	        		System.out.print("counter: ");
  	        		System.out.println(d.counter);
  	        	}/*else if(p instanceof InterestMsg){
  	        		System.out.println("INTERSEST");
  	        		InterestMsg d = (InterestMsg)p;
  	        		System.out.print("epoch: ");
  	        		System.out.println(d.epoch);
  	        	}else {
  	        		System.out.println("UNKNOWN");
  	        	}*/
          		System.out.println("");
  	        }
    }
}
