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
		
	private short minHops = 127;	
	private short epoch = -1;
	short nodeId = VirtualSense.getNodeId();
	
	public MinPathProtocol(){
		super();
	}
	
	protected void packetHandler(Packet received){
		
		 if(received instanceof InterestMsg){// INTEREST MESSAGE 
			 System.out.println(" received interest ");
			 InterestMsg msg = (InterestMsg) received;
			 Leds.setLed(2,true);			
			if(msg.epoch > epoch){ // new epoch start -- reset routing table
				 epoch = msg.epoch;
				 super.bestPath = -1; 
				 minHops = (byte)127;
			}
			if(msg.hops < this.minHops){
				 VirtualSense.printTime();				 
				 this.minHops = msg.hops;
				 super.bestPath = msg.nodeID;
				 System.out.print(" Routing updated to ");
				 System.out.println(super.bestPath);
				 msg.hops+=1;				 
				 msg.nodeID = nodeId;
				 Thread.sleep(700);
				 super.sendBroadcast(msg);				
			}	
			Leds.setLed(2,false);
		 }
		 if (received instanceof DataMsg){ //DATA	
			 DataMsg msg = (DataMsg)received;
			 Leds.setLed(1,true);
			 VirtualSense.printTime();
			 System.out.println(" Forward packet to the sink");
			 Thread.sleep(700);
			 super.send(msg);
			 Leds.setLed(1,false);			 
		 }
	    	
	 }
}
