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
		if(nodeId == 2)
			super.bestPath = (short)4;
		if(nodeId == 3)
			super.bestPath = (short)2;
		if(nodeId == 4)
			super.bestPath = (short)1;
		if(nodeId == 5)
			super.bestPath = (short)4;
		if(nodeId == 6)
			super.bestPath = (short)1;
		if(nodeId == 7)
			super.bestPath = (short)1;
		if(nodeId == 8)
			super.bestPath = (short)1;
		if(nodeId == 9)
			super.bestPath = (short)8;
		if(nodeId == 10)
			super.bestPath = (short)2;
		if(nodeId == 11)
			super.bestPath = (short)4;
		if(nodeId == 12)
			super.bestPath = (short)9;
		if(nodeId == 13)
			super.bestPath = (short)3;
		}
	
	protected void packetHandler(Packet received){
		
		 if(received instanceof InterestMsg){// INTEREST MESSAGE 
			 System.out.print(" received interest ");
			 System.out.println(received.getSender());
			 InterestMsg msg = (InterestMsg) received;
			 Leds.setLed(1,true);			
			if(msg.epoch > epoch){ // new epoch start -- reset routing table
			//if(msg.epoch > epoch && msg.hops > 0){ // new epoch start -- reset routing table
				 epoch = msg.epoch;
				 //super.bestPath = -1;  //TO set hand-made routing 
				 minHops = (byte)127;
			}
			if(msg.hops < this.minHops ){ 
			//if(msg.hops < this.minHops && msg.hops > 0){ //&& msg.hops > 0 to force route
				 VirtualSense.printTime();				 
				 this.minHops = msg.hops;
				 //super.bestPath = msg.nodeID; //TO set hand-made routing 
				 System.out.print(" Routing updated to ");
				 System.out.println(super.bestPath);
				 msg.hops+=1;				 
				 msg.nodeID = nodeId;
				 Thread.sleep(1700);
				 super.sendBroadcast(msg);
				 System.out.println(" Interest Forwarded");
				 Leds.setLed(1,false);
			}	
			//Leds.setLed(2,false);
		 }
		 if (received instanceof DataMsg){ //DATA	
			 DataMsg msg = (DataMsg)received;
			 Leds.setLed(2,true);
			 VirtualSense.printTime();
			 if(super.bestPath > 0){
				 System.out.print(" Forward packet to the sink ");
				 System.out.print(msg.sender_id);
				 System.out.print(" ");
				 System.out.println(received.getSender());
				 msg.route =(short)( msg.route + ((short)(0x01 << nodeId)));
				 Thread.sleep(1700);
				 super.send(msg);
				 Leds.setLed(1,false);
			 }else {
				 System.out.print("Dropped broadcast data!");
				 System.out.print(msg.sender_id);
				 System.out.print(" ");
				 System.out.println(received.getSender());
			 }
			 			 
		 }
	    	
	 }
	
	public static void motemain()
    {   	
        new Network(new MinPathProtocol());  
    } 
}
