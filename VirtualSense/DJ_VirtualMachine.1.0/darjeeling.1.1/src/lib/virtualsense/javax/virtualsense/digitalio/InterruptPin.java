/*
 *	InterruptedPin.java
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
 * Manages the interrupt pin of the board.
 * 
 * @author Emanuele Lattanzi
 *
 */
public class InterruptPin
{
	private boolean onFalling;
	private short port;
	public static final short INT0 = 0;
	public static final short INT1 = 1;
	public static final short INT2 = 2;
	public static final short INT3 = 3;
	public static final short INT4 = 4;
	
	/**
	 * Creates a new interrupt pin on Falling edge or on Rising edge to a specified port 
	 * @param onFalling the interrupt pin configuration (true=falling signal, false=rising signal)
	 * @param port the interrupt port number (0,1,2,3,4)
	 */
	public InterruptPin(boolean onFalling, short port){
		this.onFalling = onFalling;
		this.port = port;
		_createInterruptPin(onFalling, port);
	}
	
	/**
	 * Suspends the calling thread until the interrupt occurs 	 
	 */
	public void waitForInterrupt(){
		_waitForInterrupt(this.port);
	}	
	
	/**
	 * Check if the interrupt is falling 	
	 * @return the onFalling paramter 
	 */
	public boolean isFalling(){
		return this.onFalling;
	}
	
	/**
	 * Get the port number 	
	 * @return the port paramter 
	 */
	public short getPort(){
		return this.port;
	}
	
    public static native void _createInterruptPin(boolean onFalling, short port);
    
    public static native void _waitForInterrupt(short port);

}
