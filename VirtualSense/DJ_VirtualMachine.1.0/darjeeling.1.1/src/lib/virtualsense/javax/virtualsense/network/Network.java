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


/**
 * Manages network functionalities of virtualsense as: manage a network protocol, send and receive packets from other nodes.
 * 
 * @author Lattanzi
 *
 */
public class Network {

    private static Protocol protocol = null;

    
    private Network()
    {
    	// don't let people instantiate this class
    }

    /**
     * Creates a network using a null comunication protocol that forwards all received packets at application layer.
     */
    public static void init(){
        protocol = new NullProtocol();
        protocol.start();
        Thread.yield();
    }

    /**
     * Creates a network using specified protocol.
     * @param protocol to be initialized.
     */
    public static void init(Protocol p){
            protocol = p;
            protocol.start();
            Thread.yield();

    }

    /**
     * Sends a packet to an another node following the rules of protocol.
     * @param packet to be sent.
     */
    public static void send(Packet packet){
            protocol.send(packet);
    }

    /**
     * Waits to receive a packet from an another network node. 
     * @param recived packet.
     */
    public static Packet receive(){
        return protocol.receive();
    }  
      
   
}
