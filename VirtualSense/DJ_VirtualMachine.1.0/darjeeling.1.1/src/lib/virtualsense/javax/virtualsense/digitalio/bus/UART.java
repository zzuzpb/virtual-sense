/*
 *	UART.java
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

package javax.virtualsense.digitalio.bus ;

/**
 * Controls the UART communication bus on the board.
 * 
 * @author Emanuele Lattanzi
 *
 */
public class UART
{
	
	private UART(){
		// this class can not be instantiated 
	}
	
	/**
	 * Write a string to the default UART bus 
	 * @param s the String to be writed  
	 */
	public static void write(String s){
		System.out.print(s);
	}
	
	/**
	 * Reads a String from the default UART bus  
	 * @return the read String.
	 */
	public static String readline(){
		_waitForMessage();
		return _getMessage();
	}	
	
	/**
	 * Write a char to the default UART bus  
	 * @param c the char to be writed  
	 */
	public static native void write(char c);
	
	/**
	 * Reads a char from the default UART bus  
	 * @return the read char.
	 */
	public static short read(){
		_waitForMessage();
		return _getChar();
	}
	
	
	private static native void _waitForMessage();	
	private static native String _getMessage();
	private static native short _getChar();

}
