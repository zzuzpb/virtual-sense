/*
 *	I2C.java
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
 * Controls the I2C communication bus on the board.
 * 
 * @author Emanuele Lattanzi
 *
 */
public class I2C
{
	
	/**
	 * Enables the I2C bus 
	 */
	public static void enable(){
		_enable();
	}
	
	/**
	 * Disables the I2C bus 
	 */
	public static void disable(){
		_disable();
	}
	
	/**
	 * Starts a new I2C  communication session 
	 */
	public static void start(){
		_start();
	}
	
	/**
	 * Stops the commuincation session 
	 */
	public static void stop(){
		_stop();
	}
	
	/**
	 * Writes a byte on the I2C bus 
	 * @param value the byte to be written
	 * @return true if we received an ACK.
	 */
	public static boolean write(byte value){
		return _write(value);
	}
	
	/**
	 * Reads a byte from the I2C bus 
	 * @param sendAck send an Ack after the read
	 * @return the read byte.
	 */
	public static byte read(boolean sendAck){
		return _read(sendAck);
	}
	
	
	private static native void _enable();
	private static native void _disable();
	private static native void _start();
	private static native void _stop();
	private static native byte _read(boolean sendAck);
	private static native boolean _write(byte value);

}
