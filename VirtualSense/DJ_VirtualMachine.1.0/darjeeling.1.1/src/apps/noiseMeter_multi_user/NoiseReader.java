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
public class NoiseReader
{
	private final int AVGLEVEL = 1080;
	
	/**
	 * Measure current noise level.
	 * @return Current noise level.
	 */
	
	public short read()
    {
		int read = 0;
		int sum = 0;
		
    	for(int i = 0; i < 200; i++)
    	{
    		read = (int)ADC.readIntRef(ADC.CHANNEL6, ADC.REF2_5V);
    		
    		sum += (read >= AVGLEVEL)?read - AVGLEVEL:AVGLEVEL - read;
    		
    		Thread.sleep(10);
    	}
   		
    	return (short) (sum / 200);	
    }
}



