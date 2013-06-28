/*
 *	InterestSender.java
 * 
 *	Copyright (c) 2013 DiSBeF, University of Urbino.
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
 * Simple Radio Test Application .
 * 
 * @author Emanuele Lattanzi
 *
 */


import javax.virtualsense.network.Network;
import javax.virtualsense.network.Packet;
import javax.virtualsense.VirtualSense;


public class InterestSender extends Thread
{
	     
	private Network myNetwork = null;
	
    public InterestSender(Network network){
    	this.myNetwork = network;
    }
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
			this.myNetwork.send(intMsg);
			System.out.println(" interest sent....");
		}   
    }
}
