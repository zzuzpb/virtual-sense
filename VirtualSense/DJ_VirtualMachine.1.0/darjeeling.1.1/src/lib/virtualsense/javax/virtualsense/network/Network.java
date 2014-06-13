/*
 *	Network.java
 * 
 *      Copyright (c) 2012 DiSBeF, University of Urbino.
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

package javax.virtualsense.network;

import javax.virtualsense.concurrent.*;
/**
 * Manages network functionalities of virtualsense as: manage a network protocol, send and receive packets from other nodes.
 * 
 * @author Lattanzi
 *
 */
public class Network {
	
	private static Network instance = null;
	
	
    private Network(Protocol protocol) {
    	Dispatcher.registerNullProtocol(new NullProtocoll());
    	Dispatcher.registerProtocol(protocol);
    	init();
    }
    
    private Network() {
    	Dispatcher.registerNullProtocol(new NullProtocoll());
    	init();
    }

    public static Network getInstance() {
    	System.out.print("get insatnce");
    	if (instance == null) {
    		instance = new Network();
    	}
    	
    	return instance;
    }
    
    public static Network getInstance(Protocol newProtocol) {
    	System.out.print("get instance protocollo");
    	if (instance == null) {
    		instance = new Network(newProtocol);
    	}
    	else {
        	Dispatcher.registerProtocol(newProtocol);
        	init();
    	}
    	
    	return instance;
    }
    
    
    /**
     * Creates a network using a null comunication protocol that forwards all received packets at application layer.
     */
    private static void init(){
        Dispatcher.launch(); // if already running does nothing 
        Thread.yield();
    }

    /**
     * Sends a packet to an another node following the rules of protocol.
     * @param packet to be sent.
     */
    public void send(Packet packet) {
    	send(packet, (short)0);
    }
    
    public void send(Packet packet, short protocolId) {
    	Dispatcher.send(packet, protocolId);
    }

    /**
     * Waits to receive a packet from an another network node. 
     * @param received packet.
     */
    public Packet receive() {
        return Dispatcher.receive(myProtocol);
    }  
      
   
}
