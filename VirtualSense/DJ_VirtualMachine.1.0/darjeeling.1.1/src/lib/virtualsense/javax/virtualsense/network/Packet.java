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
 *
 * @author Lattanzi
 */
public class Packet {
     private  byte data[];
     private  short sender;
     private short receiver;
     
     public Packet(byte data[]){
         this.data = data;         
     }
     
     public byte[] getData(){
         return this.data;
     }
     
     public short getSender(){
         return this.sender;
     }
     public short getReceiver(){
         return this.receiver;
     }
     protected void setSender(short sender){
    	 this.sender = sender;
     }
     protected void setReceiver(short receiver){
    	 this.receiver = receiver;
     }
}
