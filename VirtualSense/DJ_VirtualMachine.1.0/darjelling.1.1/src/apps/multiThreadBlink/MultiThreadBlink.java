/*
 *	Blink.java
 * 
 *	Copyright (c) 2008 CSIRO, Delft University of Technology.
 * 
 *	This file is part of Darjeeling.
 * 
 *	Darjeeling is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	Darjeeling is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 * 
 *	You should have received a copy of the GNU General Public License
 *	along with Darjeeling.  If not, see <http://www.gnu.org/licenses/>.
 */
import javax.darjeeling.actuators.Leds;
import javax.darjeeling.Darjeeling;
import javax.darjeeling.PowerManager;

public class MultiThreadBlink
{
	static int  temp = 0;
    public static void main(String args[])
    {
     
    	PowerManager.setSystemClockMillis(500); 
    	new Thread(){
    		public void run(){
    			boolean state = true;
    			int i = 0;
    			int x = 0;
    			while(true)
    	        {    				
    					Leds.setLed(0, state);
    					Leds.setLed(3, false);
    	            	temp++;
    	            	Thread.sleep(1000);    
    	            	state =! state;
    	            	i++;     	            	
    	        }
    		}
    	}.start(); 
    	
    	new Thread(){
    		public void run(){
    			boolean state = true;
    			while(true)
    	        {
    				Leds.setLed(1, state);
    				Leds.setLed(2, false);
    	            temp++;
    				Thread.sleep(1000); 
    				state =! state;

    	        }
    		}
    	}.start();
    	
    	while(true){
    		Leds.setLed(0,false);
    		Leds.setLed(1,false);
    		Leds.setLed(2,true);
    		Leds.setLed(3,true);
    		temp-=4;
    		Thread.sleep(2000);
    		System.out.print("****: tmp ");
    		System.out.println(temp);
    		
            
    	}
    }
}
