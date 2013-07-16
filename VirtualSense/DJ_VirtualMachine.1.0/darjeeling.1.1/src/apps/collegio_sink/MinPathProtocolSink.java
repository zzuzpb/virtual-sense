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
import javax.virtualsense.platform.InfoMsg;
import javax.virtualsense.network.*;
import javax.virtualsense.actuators.Leds;
import javax.virtualsense.radio.Radio;
import javax.virtualsense.VirtualSense;


public class MinPathProtocolSink extends Protocol{
		
	private short minHops = 127;	
	private short epoch = -1;
	short nodeId = VirtualSense.getNodeId();
	
	public MinPathProtocolSink(){
		super();
	}
	
	protected void packetHandler(Packet received){
		 if(received instanceof InterestMsg){// INTEREST MESSAGE 
			 System.out.println(" received interest ");
			 //NOTINGH TO DO; IS A PONG INTEREST
		 }
		 if (received instanceof DataMsg){ //DATA	
			 super.notifyReceiver();		 
		 }	 
		 if (received instanceof InfoMsg){
			 super.notifyReceiver();	
		 }
	 }
}
