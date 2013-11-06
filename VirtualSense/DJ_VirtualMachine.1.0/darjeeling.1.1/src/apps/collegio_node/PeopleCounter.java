/*
 *	PeopleCounter.java
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
 * People counter test application.
 * 
 * @author Matteo Dromedari
 *
 */
import javax.virtualsense.actuators.Leds;
import javax.virtualsense.VirtualSense;
import javax.virtualsense.concurrent.*;
import javax.virtualsense.digitalio.*;
import java.util.*;


public class PeopleCounter extends Thread
{
	private static final int MAX_LENGTH = 800;
	public static int state = 0;
	public static Semaphore sem = new Semaphore((short)0);
	public short in = 0;
	public short out = 0;
	short   nodeId = VirtualSense.getNodeId();
	
    public void run()
    {
    	int length = 0;
    	int dropped = 0;
    	Leds.setLed(Leds.LED0, false);
    	Leds.setLed(Leds.LED1, false);
    	Leds.setLed(Leds.LED2, false);
    	
    	
    	PhotoCell p4 = new PhotoCell(InterruptPin.INT4);
    	PhotoCell p3 = new PhotoCell(InterruptPin.INT3);
    	
    	p4.start();
    	p3.start();
    	
    	
    	while(true)
    	{ 			
    		//System.out.println("Main wait on semaphore.");
    		sem.acquire();
    		
    		//System.out.println("Main unlock.");
    		Leds.setLed(Leds.LED0, false);
			Leds.setLed(Leds.LED1, false);
    		length = p4.getTime() - p3.getTime();
    		if(length < 0)
    			length = -length;
    		if(length > MAX_LENGTH)
    		{
				dropped ++;
				//System.out.print("Drop -- ");System.out.println(dropped);
				Leds.setLed(Leds.LED2, true);
				// metto lo stato all'id opposto 
				state = state==InterruptPin.INT3?InterruptPin.INT4:InterruptPin.INT3;
			}
    		else 
    		{
				if(state == 3)
	    		{		
					Leds.setLed(Leds.LED0, false);
					Leds.setLed(Leds.LED1, false);
	    			if(nodeId == 10 || nodeId == 11)
	    			{
	    				this.in++;
	    				System.out.print("In event, length:");
	    			}
	    			else
	    			{
	    				this.out++;
	    				System.out.print("Out event, length");
	    			}
	    			System.out.println(length);
	    		}
	    		else
	    		{
	    			Leds.setLed(Leds.LED0, false);
					Leds.setLed(Leds.LED1, false);
	    			if(nodeId == 10 || nodeId == 11)
	    			{
	    				this.out++;
	    				System.out.print("Out event, length:");
	    			}
	    			else
	    			{
	    				this.in++;
	    				System.out.print("In event, length:");
	    			}
	    			System.out.println(length);
	    		}

	    		state = 0;
			}		
    	}
    		
    }
    
}