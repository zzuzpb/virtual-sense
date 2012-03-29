/*
 *	System.java
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
package java.lang;
import java.io.PrintStream;
import java.io.OutputStream;


public class System 
{
	public static PrintStream out = new PrintStream();
	
	// no instance allowed
	private System() {};
	
	
    public static native int currentTimeMillis();
    
    public static native void arraycopy(Object src, int src_position, Object dst, int dst_position, int length);

    /**
	 * Runs the garbage collector. Calling this method suggests that the Java
	 * Virtual Machine expend effort toward recycling unused objects in order to
	 * make the memory they currently occupy available for quick reuse. When
	 * control returns from the method call, the Java Virtual Machine has made
	 * its best effort to recycle all discarded objects.
	 * <p>
	 * The name <code>gc</code> stands for "garbage collector". The Java Virtual
	 * Machine performs this recycling process automatically as needed, in a
	 * separate thread, even if the <code>gc</code> method is not invoked
	 * explicitly.
	 * <p>
	 * The method {@link System#gc()} is hte conventional and convenient means
	 * of invoking this method.
	 */
	public static native void gc();
    


}
