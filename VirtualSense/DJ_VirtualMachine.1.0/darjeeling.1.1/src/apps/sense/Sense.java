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
import javax.virtualsense.actuators.Leds;
import javax.virtualsense.sensors.*;
import javax.darjeeling.Darjeeling;

public class Sense
{
    public static void main(String args[])
    {
        while(true)
        {
            
	            Leds.setLed(0,true);
	            Thread.sleep(1000);
	            System.out.print("Temp = ");
	            System.out.println(Temperature.getValue());
	            System.out.print("TempBoard = ");
	            System.out.println(Temperature.getBoardValue());
	            System.out.print("Humidity = ");
	            System.out.println(Humidity.getValue());
	            System.out.print("Light = ");
	            System.out.println(Light.getValue());
	            
	           
	            System.gc();

            
        }
    }
}
