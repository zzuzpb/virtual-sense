/*
 *	Temperature.java
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
package javax.virtualsense.sensors ;

/**
 * Reads temperature from the sensor.
 * 
 * @author Emanuele Lattanzi
 *
 */
public class Temperature
{
	private static final short TEMP_CHANNEL = 0;
	private static final short BOARD_TEMP_CHANNEL = 2;
   
	
	/**
	 * Reads temperature value from the external sensor.	 
	 * @return measured temperature C.
	 */
	public static int getValue(){
		int temp = 0;
		short vValue = ADC.read(TEMP_CHANNEL);
		temp = (vValue*1000-1857000) /(-11670);
		return temp;
	}
	
	/**
	 * Reads temperature value from the MCU sensor.	  
	 * @return measured temperature C.
	 */
	public static int getBoardValue(){
		int temp = 0;
		short vValue = ADC.read(BOARD_TEMP_CHANNEL);
		temp = ((vValue - 1857) * 666) / 4096;
		return temp;
	}
}
