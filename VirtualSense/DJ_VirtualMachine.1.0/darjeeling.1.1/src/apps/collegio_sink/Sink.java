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
import javax.virtualsense.platform.InfoMsg;
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
        Network myNetwork = new Network(new MinPathProtocolSink()); // 
        //InterestSender sender = new InterestSender(myNetwork);
    	//sender.start();
    	/*
    	 * TODO: Need a deallocation methods to unregister protocols 
    	 * on the Network dispatcher. For now commands are not safe.
    	SerialReceiver serialReceiver = new SerialReceiver(myNetwork);
    	serialReceiver.start();
    	*/
        sink(myNetwork);        
            
    }
    public static void sink(Network network){
    	System.out.println("SINK!!!");
    	Network myNetwork = network;
    	
  		System.out.println("Receiver thread!!!");
  	    while(true){
        	Packet p = myNetwork.receive();
        	System.out.println("<packet>");
        	System.out.print("   >time: ");
        	System.out.println(System.currentTimeMillis());
        	System.out.print("   >router: ");
        	System.out.println(p.getSender());        	
        	if(p instanceof DataMsg){
        		DataMsg d = (DataMsg)p;
        		System.out.print("   >sender: ");
        		System.out.println(d.sender_id);
        		System.out.print("   >counter: ");
        		System.out.println(d.counter);
        		System.out.print("   >route: ");
        		System.out.println(d.route);
        		System.out.print("   >noise: ");
        		System.out.println(d.noise);
        		System.out.print("   >co2: ");
        		System.out.println(d.co2);
        		System.out.print("   >in: ");
        		System.out.println(d.in);
        		System.out.print("   >out: ");
        		System.out.println(d.out);
        		System.out.print("   >temp: ");
        		System.out.println(d.temp);
        		System.out.print("   >humid: ");
        		System.out.println(d.humid);
        		System.out.print("   >light: ");
        		System.out.println(d.light);
        		
        	}else if(p instanceof InfoMsg){
        		InfoMsg d = (InfoMsg)p;
        		System.out.println(d.nodeID);
        		System.out.println(d.executionContextID); 
        		System.out.print(d.loaded?"t":"f");
        		System.out.print(d.running?"t":"f");
        	}/*else {
        		System.out.println("UNKNOWN");
        	}*/
        	System.out.println("</packet>");
      		//System.out.println("");
  	    }
    }
}
