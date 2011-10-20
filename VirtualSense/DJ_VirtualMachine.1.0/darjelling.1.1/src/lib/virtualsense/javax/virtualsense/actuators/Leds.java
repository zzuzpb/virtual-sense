/*
 *	Leds.java
 * 
 *  Copyright (c) 2011 DiSBeF, University of Urbino.
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

package javax.virtualsense.actuators ;

/**
 * Controls the leds on the board.
 * 
 * @author Emanuele Lattanzi
 *
 */
public class Leds
{
	public static final byte GREEN	=0;
	public static final byte RED	=1;	
	public static final byte YELLOW	=2;
    public static final byte ORANGE	=3;
    
    
    
	/**
	 * Controls a led on the  board.
	 * @param led the led to set (0, 1, 2, 3)
	 * @param state the state of the led. (true=on, false=off)
	 */
    public static native void setLed(int led, boolean state);

}
