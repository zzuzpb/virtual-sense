/*
 * Noise.java
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

/**
 * Simple noise measure Test Application application.
 * 
 * @author Matteo Dromedari
 *
 */
import javax.virtualsense.sensors.*;

/**
 * 
 * API for noise meter.
 * 
 * @author Matteo Dromedari
 *
 */
public class CurrentReader
{
	private final short ZEROLEVEL = 2540;
	
	/**
	 * Measure current level.
	 * @return Current level.
	 */
	public short getCurrentLevel()
	{
		short maxCurrentLevel = 0;
		
		for(short i = 0; i < 50; i++)
		{
			short read = ADC.readExtRef(ADC.CHANNEL0, (short)0, (short)3300);
			
			if(read >= ZEROLEVEL)
			{
				//System.out.print("+");
				read = (short)(read - ZEROLEVEL);
			}
			else
			{
				read = (short)(ZEROLEVEL - read);
				//System.out.print("-");
			}	
			
			//System.out.println(read);
			if(read > maxCurrentLevel)
				maxCurrentLevel = read;
			
			Thread.sleep(10);
		}
		
		return maxCurrentLevel;
	}
}



