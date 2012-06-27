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


public class MinPathProtocol extends Protocol{
		
	private byte minHops = (byte)127;	
	private byte epoch = (byte)-127;
	
	public MinPathProtocol(){
		super.bestPath = 11; // for measure i need sending unicast
	}
	protected void packetHandler(Packet p){
		byte data[] = p.getData();
		 if(data[0]==0){// INTEREST MESSAGE 
			 /*System.out.print("INTEREST RECEIVED ");
			 System.out.print(data[1]);
			 System.out.print(" - ");
			 System.out.println(data[2]);*/
			if(data[2] > epoch){ // new epoch start -- reset routing table
				 epoch = data[2];
				 super.bestPath = -1;
				 minHops = (byte)127;
				 //System.out.println("New epoch start");
			}
			if(data[1] < this.minHops){
				//System.out.println("Routing updated");
				 this.minHops = data[1];
				 super.bestPath = 1;//p.getSender();
				 // in this case we need to forward the interest
				 // and increment the hop counter
				 data[1]+=1;
				 Packet forward = new Packet(data);
				 Thread.sleep(100);
				 super.sendBroadcast(forward);
			}			 
		 }else { //DATA
			 if(data[1] == 1){// the packet should be forwarded to the sink .. we are a router
				 //System.out.println("Forward packet to the sink");
				 super.send(p);
			 }
			 else{ // data is for the node. We are the receiver
				 System.out.println("Data is for us");
				 //super.notifyReceiver();
			 }
		 }
	    	
	 }
}
