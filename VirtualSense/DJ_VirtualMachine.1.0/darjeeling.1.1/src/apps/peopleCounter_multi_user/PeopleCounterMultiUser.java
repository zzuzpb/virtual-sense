/*
 *	PeopleCounterMultiUser.java
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


public class PeopleCounterMultiUser
{
	private static final int MIN_LENGTH = 100;
	
	private PhotoCell pC3;
	private PhotoCell pC4;
	
	private short direction;
	private int eventLen;
	
	private Semaphore lock;
	private Semaphore mutex;
	
	
	public PeopleCounterMultiUser()
	{    	
    	this.pC3 = new PhotoCell(InterruptPin.INT3, this);
    	this.pC4 = new PhotoCell(InterruptPin.INT4, this);
    	
    	this.direction = 0;
    	this.eventLen = 0;
    	
    	this.lock = new Semaphore((short)0);
    	this.mutex = new Semaphore((short)1);
    	
    	this.pC3.start();
    	this.pC4.start();
	}
	
	public void execute()
	{	
		while(true)
    	{
    		// Wait for event
			System.out.println("wait for events...");
    		this.lock.acquire();
    		System.out.println("main unlock");
    		
    		if(this.eventLen > this.MIN_LENGTH)
    		{
    			// Write on serial port the event
    			System.out.print(((this.direction == 3)?"IN ":"OUT "));
    			System.out.print("length: ");
    			System.out.println(this.eventLen);
    		}
    		else
    		{
    			System.out.print("Dopped event! length: ");
    			System.out.println(this.eventLen);
    		}
    		
    		// Remove all event
    		this.direction = 0;
    		this.eventLen = 0;
    		
    		// Unlock photocells for next event
    		this.pC3.resume();
    		this.pC4.resume();
    	}
	}
    
    
    public void notifyCellEvent(short portId)
    {
    	System.out.println("cell boh rilascia");
    	this.mutex.acquire();
    	System.out.println("passa il lock");
    	
    	if(this.direction == 0)
    	{
    		this.direction = portId;
    		this.eventLen = System.currentTimeMillis();
    	}
    	else
    	{
    		this.eventLen = System.currentTimeMillis() - this.eventLen;
    		this.lock.release();
    	}
    	
    	this.mutex.release();
    }
    
    
    public static void motemain()
    { 
    	System.out.println("Thread main run.");
    	
    	PeopleCounterMultiUser counter = new PeopleCounterMultiUser();
    	counter.execute();
    }
    
}