/*
 * Storage.java
 * 
 * Copyright (c) 2008-2010 CSIRO, Delft University of Technology.
 * 
 * This file is part of Darjeeling.
 * 
 * Darjeeling is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Darjeeling is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with Darjeeling.  If not, see <http://www.gnu.org/licenses/>.
 */
 
 
package javax.virtualsense.storage;

import java.lang.String;

/**
 * 
 * API for store on flash memory.
 * 
 * @author Matteo Dromedari
 *
 */
public class Storage
{
	/**
	 * Write variable on flash memory.
	 * @param name Name of variable to write;
	 * @param var Variable to write. 
	 * @return 0 - Successful write;
	 * 		  -1 - Write failed.
	 */
	public static short writeVar(String name, String str)
	{
		return _writeVar(name, name.length(), str, str.length());	
	}
	
	public static short writeVar(String name, short s)
	{
		return _writeVar(name, name.length(), s);
	}
	
	public static short writeVar(String name, int var)
	{
		return _writeVar(name, name.length(), var);
	}
	
	private static native short _writeVar(String name, int nameLen, String str, int strLen);
	private static native short _writeVar(String name, int nameLen, short s);
	private static native short _writeVar(String name, int nameLen, int var);
	
	
	/**
	 * Read to flash memory specified variable.
	 * @param name Name of variable to read;
	 * @return specified variable in byte array.
	 */
	public static int readInt(String name)
	{
		return _readInt(name, name.length());
	}
	
	public static short readShort(String name)
	{
		return _readShort(name, name.length());
	}
	
	public static String readString(String name)
	{
		return _readString(name, name.length());
	}
	
	private static native int _readInt(String name, int nameLen);
	private static native short _readShort(String name, int nameLen);
	private static native String _readString(String name, int nameLen);
	
	
	/**
	 * Write byte onto specified file. Reserves a free memory space for save a file. At the first invocation the memory is reserved
	 * and the first byte is write, the next bytes are added in append.	
	 * @param name File name;
	 * @param b Byte to write;
	 * @return 0 - Successful write;
	 * 		  -1 - Write failed.
	 */
	
	/**
	 * Read a byte from specified file.
	 * @param name Name of file.
	 * @return read byte.
	 */
}