/*
 *	Blink.java
 * 
 *	Copyright (c) 2012 DiSBeF, University of Urbino.
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
 * Simple Blink application.
 * 
 * @author Emanuele Lattanzi
 *
 */
import javax.virtualsense.powermanagement.PowerManager;
import javax.virtualsense.actuators.Leds;
import javax.virtualsense.VirtualSense;
import java.lang.Runtime;

public class AudioDSPMultiUser
{
    public static void motemain()
    {
    	AudioDSP d = new AudioDSP();
         
        while(true)
        {
        	
        	
        	boolean state = false;
        	
        	if(state){
        		Leds.setLed(Leds.LED0, state);
        		d.enable(true);
        		System.out.println("Enable AudioDSP layer");
        	}else{
        		Leds.setLed(Leds.LED0, state);
        		d.enable(false);
        		System.out.println("Disable AudioDSP layer");
        	}
        	state = !state;
        	Thread.sleep(5000);
        }
       
    }
}