/*
 *	MinPathProtocol.java
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
 * Simple Radio Test Application application.
 * 
 * @author Emanuele Lattanzi
 *
 */
import javax.virtualsense.network.*;
import javax.virtualsense.actuators.Leds;
import javax.virtualsense.radio.Radio;
import javax.virtualsense.VirtualSense;


public class MinPathProtocol extends Protocol{
		
	private byte minHops = (byte)127;	
	private byte epoch = (byte)-127;
	short nodeId = VirtualSense.getNodeId();
	private static byte data[] = new byte [10]; //to fix a GC problem
	
	protected void packetHandler(Packet received){
		 data = received.getData();
		
		 if(data[0]==0){// INTEREST MESSAGE 
			 Leds.setLed(2,true);			
			if(data[2] > epoch){ // new epoch start -- reset routing table
				 epoch = data[2];
				 super.bestPath = -1;
				 minHops = (byte)127;
				 //System.out.println("New epoch start");
			}
			if(data[1] < this.minHops){
				 VirtualSense.printTime();
				 System.out.println(" Routing updated ");
				 this.minHops = data[1];
				 super.bestPath = Radio.getSenderId();
				 
				 //System.out.println(super.bestPath);
				 // in this case we need to forward the interest
				 // and increment the hop counter
				 data[1]+=1;
				 Packet forward = new Packet(data);
				 Thread.sleep(50);
				 super.sendBroadcast(forward);				
			}	
			Leds.setLed(2,false);
		 }
		 if (data[0] == 1){ //DATA			 
			 if(data[1] == 1){// the packet should be forwarded to the sink 1.. we are a router
				 Leds.setLed(4,true);
				 VirtualSense.printTime();
				 //System.out.println(" Forward packet to the sink");
				 Thread.sleep(50);
				 super.send(received);
				 Leds.setLed(4,false);
			 }
			 else{ // data is for the node. We are the receiver
				 Leds.setLed(5,true);
				 VirtualSense.printTime();
				 //System.out.println(" Data is for us");
				 super.notifyReceiver();
				 Leds.setLed(5,false);
			 }
		 }
	    	
	 }
}
