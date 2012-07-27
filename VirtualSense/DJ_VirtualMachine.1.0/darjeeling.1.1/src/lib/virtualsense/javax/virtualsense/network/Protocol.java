/*
 *	Protocol.java
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

import javax.virtualsense.concurrent.Semaphore;
import javax.virtualsense.radio.Radio;

/**
 *
 * @author Lattanzi
 */

public abstract class Protocol {
    protected static short bestPath = -1;
    private static boolean running = true;
    private static Packet actualPacket = null;
    private Semaphore packet = new Semaphore((short)0);
    
      
    // invoke Radio.send(short, data) with bestPath as dest
    protected void send(Packet p){
        if(bestPath >= 0){
        	//System.out.println("SEND UNICAST ");
        	//System.out.println(bestPath);
        	//Radio.broadcast(p.getData()); //Introduced to remove timeout for conference publication
            Radio.send(bestPath, p.getData());     
        }else{
            Radio.broadcast(p.getData());
            //System.out.println("SEND BROADCAST");
            
        }
    }
    protected void sendBroadcast(Packet p){
    	Radio.broadcast(p.getData());
    }
    protected Packet receive(){        
       packet.acquire();        
       return actualPacket;
    }
    
    protected void notifyReceiver(){
        // should wake-up the receiver calling thread!
        packet.release();
    }
    
    protected void packetHandler(Packet p){
    	
    }
    	
    
    
    protected void start(){    	
        new Thread(){
        	public void run(){
        		short s_id = -1;
        		short r_id = -1;
        		Radio.init();
        		//System.out.println(Thread.currentThread().getId());
        		while(running){
        			byte d[] = Radio.receive();
        			s_id = Radio.getSenderId();        			
        			r_id = Radio.getDestId();  
        			Packet p = new Packet(d, s_id, r_id);
        			actualPacket = p;
        			packetHandler(actualPacket);
        		}           
        	}
        }.start();
    }
    
    protected void stop(){
        running = false;
    }
    
    
}
