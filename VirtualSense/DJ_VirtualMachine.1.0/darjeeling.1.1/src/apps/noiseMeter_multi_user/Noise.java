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
public class Noise
{
	private final short AVGLEVEL = 1100;
	
	/**
	 * Measure current noise level.
	 * @return Current noise level.
	 */
	public int getNoiseLevel()
	{
		int currentLevel = 0;
		int sum = 0;
		
		for(short i = 0; i < 100; i++)
		{
			currentLevel = (int)ADC.read(ADC.CHANNEL0, ADC.REF2_5V);
			
			if(currentLevel >= AVGLEVEL)
			{
				sum += (currentLevel - AVGLEVEL);
			}
			else
			{
				sum += (AVGLEVEL - currentLevel);
			}
			Thread.sleep(10);
		}
		
		return (sum / 100);
	}
}



