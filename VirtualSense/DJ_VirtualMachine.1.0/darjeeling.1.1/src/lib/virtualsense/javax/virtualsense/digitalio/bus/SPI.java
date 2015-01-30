/*
 *	SPI.java
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

package javax.virtualsense.digitalio.bus;

/**
 * Controls the SPI communication bus on the board.
 * 
 * @author Matteo Dromedari
 *
 */
public class SPI
{
	public static final short CE0 = 0;
	public static final short CE1 = 1;
	public static final short CE2 = 2;

	private short chipEnable;
	
	/**
	 * Init SPI comunication to specified chip
	 * @param ce Chip enable line to use
	 */
	public SPI(short ce){
		this.chipEnable = ce;
		_init(this.chipEnable);
	}
	
	
	/**
	 * Enables the SPI bus 
	 */
	public void enable(){
		_enable();
	}
	
	/**
	 * Disables the SPI bus 
	 */
	public void disable(){
		_disable();
	}
	
	/**
	 * Clear selected Chip enable line 
	 */
	public void clearCE(){
		_setCE(this.chipEnable);
	}
	
	/**
	 * Set selected Chip enable line 
	 */
	public void setCE(){
		_setCE(this.chipEnable);
	}
	
	
	/**
	 * Writes a byte on the SPI bus 
	 * @param value the byte to be written
	 * @return true if comunication if communication succeeds.
	 */
	public static boolean write(byte value){
		return _write(value);
	}
	
	/**
	 * Reads a byte from the SPI bus 
	 * @return the read byte.
	 */
	public static byte read(){
		return _read();
	}
	
	private static native void _init(short ce);
	private static native void _enable();
	private static native void _disable();
	private static native void _setCE(short ce);
	private static native void _clearCE(short ce);
	private static native byte _read();
	private static native boolean _write(byte value);

}
