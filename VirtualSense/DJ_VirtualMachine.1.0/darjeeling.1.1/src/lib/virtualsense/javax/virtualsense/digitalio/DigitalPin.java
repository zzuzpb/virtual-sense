/*
 *	DigitalPin.java
 * 
 *  Copyright (c) 2013 DiSBeF, University of Urbino.
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

package javax.virtualsense.digitalio ;

/**
 * Controls the digital I/O pin on the board.
 * 
 * @author Emanuele Lattanzi
 *
 */
public class DigitalPin
{
	public static final short DIO0 = 0;
	public static final short DIO1 = 1;
	public static final short DIO2 = 2;
	public static final short DIO3 = 3;
	
	private boolean onInput;
	private short 	port;
	/**
	 * Creates a new digital pin on input direction or on output direction on a specified port 
	 * @param onInput the pin direction configuration (true=input direction, false=output direction)
	 * @param port the digital io port number ()
	 */
	public DigitalPin(boolean onInput, short port){
		this.onInput 	= onInput;
		this.port 		= port;
		_createDigitalPin(onInput, port);
	}
	
	/**
	 * Writes to a digital pin a boolean value 
	 * @param hight the pin value (true=high, false=low)
	 */
	public void write(boolean high){
		_write(this.port, high);
	}
	
	/**
	 * Reads a boolean value from a digital pin 
	 * @return the pin value as boolean (true=high, false=low)
	 */
	public boolean read(){
		return _read(this.port);
	}
	
	private static native void _createDigitalPin(boolean onInput, short port);
	private static native void _write(short port, boolean high);
	private static native boolean _read(short port);

}
