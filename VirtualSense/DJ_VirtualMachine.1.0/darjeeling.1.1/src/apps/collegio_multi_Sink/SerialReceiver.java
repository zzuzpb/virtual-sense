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

import java.lang.Integer;
import javax.virtualsense.network.Network;
import javax.virtualsense.network.Packet;
import javax.virtualsense.VirtualSense;
import javax.virtualsense.digitalio.bus.UART;
import javax.virtualsense.platform.CommandMsg;
import javax.virtualsense.platform.Command;


public class SerialReceiver extends Thread
{
	     
	private Network myNetwork = null;
	
    public SerialReceiver(Network network){
    	this.myNetwork = network;
    }
    public void run(){  	        		
  		System.out.println("Starting serial receiver thread!!!");
  		short i = 1;
  		while(true){
  			CommandMsg cmdMsg = new CommandMsg();	
  			cmdMsg.type = -1;
  			String r = UART.readline();
  			System.out.println(r);
  			String arr[] = r.split(' ');
  			
  			if(arr != null){
	  			if(arr.length >= 3 && arr[0].equals("APPS")){
	  				System.out.println(arr[1]);
	  				if(arr[1].equals("LOAD"))
	  					cmdMsg.type = Command.LOAD;
	  				if(arr[1].equals("START"))
	  					cmdMsg.type = Command.START;
	  				if(arr[1].equals("STOP"))
	  					cmdMsg.type = Command.STOP;
	  				if(arr[1].equals("UNLOAD"))
	  					cmdMsg.type = Command.UNLOAD;
	  				if(arr[1].equals("PSAU"))
	  					cmdMsg.type = Command.PSAU;
	  				if(cmdMsg.type != -1){
	  					cmdMsg.id = i; 
	  					cmdMsg.executionContextID = (short)Integer.parseInt(arr[2]);
	  					this.myNetwork.send(cmdMsg);
	  					System.out.println(" command sent....");
	  					System.out.println(cmdMsg.type);
	  					System.out.println(cmdMsg.executionContextID);
	  					i++;
	  				}
	  			}  
  			}
		}   
    }
}
