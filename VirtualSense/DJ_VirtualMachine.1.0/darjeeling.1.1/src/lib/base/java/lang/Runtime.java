/*
 *	Runtime.java
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


public class Runtime 
{

	private static Runtime currentRuntime = null;//new Runtime();

	/**
	 * Returns the runtime object associated with the current Java application.
	 * Most of the methods of class <code>Runtime</code> are instance methods
	 * and must be invoked with respect to the current runtime object.
	 * 
	 * @return the <code>Runtime</code> object associated with the current Java
	 *         application.
	 */
	public static Runtime getRuntime() {
		return null; //currentRuntime;
	}

	/** Don't let anyone else instantiate this class */
	private Runtime() {
	}

	/**
	 * Terminates the currently running Java application. This method never
	 * returns normally.
	 * <p>
	 * The argument serves as a status code; by convention, a nonzero status
	 * code indicates abnormal termination.
	 * 
	 * @param status
	 *            exit status.
	 * @since JDK1.0
	 */
	public static native void exit(int status);

	/**
	 * Returns the amount of free memory in the system. Calling the
	 * <code>gc</code> method may result in increasing the value returned by
	 * <code>freeMemory.</code>
	 * 
	 * @return an approximation to the total amount of memory currently
	 *         available for future allocated objects, measured in bytes.
	 */
	public static native int freeMemory();

	/**
	 * Returns the total amount of memory in the Java Virtual Machine. The value
	 * returned by this method may vary over time, depending on the host
	 * environment.
	 * <p>
	 * Note that the amount of memory required to hold an object of any given
	 * type may be implementation-dependent.
	 * 
	 * @return the total amount of memory currently available for current and
	 *         future objects, measured in bytes.
	 */
	public static native int totalMemory();

	

}