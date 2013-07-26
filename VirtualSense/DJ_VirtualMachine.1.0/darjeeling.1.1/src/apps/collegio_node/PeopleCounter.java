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
	public static int state = 0;
	public static Semaphore sem = new Semaphore((short)0);
	public short in = 0;
	public short out = 0;
	
    public void run()
    { 
    	
    	Leds.setLed(Leds.LED0, false);
    	Leds.setLed(Leds.LED1, false);
    	Leds.setLed(Leds.LED2, false);
    	
    	int length = 0;
    	
    	PhotoCell p3 = new PhotoCell(InterruptPin.INT3);
    	PhotoCell p4 = new PhotoCell(InterruptPin.INT4);
    	
    	p3.start();
    	p4.start();
    	
    	while(true)
    	{ 		
    		
    		System.out.println("Main wait on semaphore.");
    		sem.acquire();
    		
    		Leds.setLed(Leds.LED0, false);
    		System.out.println("Main unlock.");
    		
    		if(state == 3)
    		{
    			length = p4.getTime() - p3.getTime();
    			System.out.print("Main prima p3, durata:");System.out.println(length);
    			Leds.setLed(Leds.LED0, true);
    			this.in++;
    		}
    		else
    		{
    			length = p3.getTime() - p4.getTime();
    			System.out.print("Main prima p4, durata:");System.out.println(length);
    			Leds.setLed(Leds.LED1, true);
    			this.out++;
    		}
    		Leds.setLed(Leds.LED0, false);
    		Leds.setLed(Leds.LED1, false);
    		state = 0;
    	}
    		
    }
    
}