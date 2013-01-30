/*
 *	Packet.java
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
 * Defines the format of packets exchanged from nodes.
 *
 * @author Lattanzi
 */
public class Packet 
{
     private  byte data[];
     private short s_ID;
     private short r_ID;
     
     /**
      * Creates a packet from specified data.
      * @params data to be packaged. 
      */
     public Packet(byte data[]){
         this.data = data;         
     }
     
     /**
      * Creates a packet specifing data, sender id, receiver id.
      * @param data to be packaged.
      * @param s sender identifier.
      * @param r receiver identifier.
      */
     public Packet(byte data[], short s, short r){
         this.data = data;  
         this.s_ID = s;
         this.r_ID = r;
     }
     
     /**
      * Returns content of packet.
      * @returns data part of packet.
      */
     public byte[] getData(){
         return this.data;
     }
     
     /**
      * Returns sender identifier.
      * @returns sender of packet.
      */
     public short getSender(){
         return this.s_ID;
     }
     
     /**
      * Returns receiver identifier.
      * @returns receiver of packet.
      */
     public short getReceiver(){
         return this.r_ID;
     }
     
     /**
      * Sect sender identifier of current packet.
      * @param new packet sender.
      */
     protected void setSender(short sender){
    	 this.s_ID = sender;
     }
     
     /**
      * Sect receiver identifier of current packet.
      * @param new packet receiver.
      */
     protected void setReceiver(short receiver){
    	 this.r_ID = receiver;
     }
}
