/**
 *	PowerManager.java
 * 
 *	Copyright (c) 2011 DiSBeF, University of Urbino.
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
 *	along with Darjeeling.  If not, see <http://www.gnu.org/licenses/>.
 */
 
package javax.darjeeling ;
/**
 * Controls power manager of the board 
 * based on modified contiki operating system.
 * 
 * @author Emanuele Lattanzi
 *
 */
public class PowerManager
{
	public static native int getBatteryVoltage();
	public static native int getSolarVoltage();
	public static native int getSolarCurrent();
	
	/**
	 * Controls the system clock by programming it.
	 * @param number of millisecond between a clock interrupt
	 * @param state the state of the led. (true=on, false=off)
	 */
	public static native void setSystemClockMillis(int millis);
    public static native void slowDownClockByFactor(int factor);
}
