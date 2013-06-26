/*
 *	CO2MeterMultiUser.java
 * 
 *	Copyright (c) 2012 DiSBeF, University of Urbino.
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

/**
 * CO2 sense application.
 * 
 * @author Matteo Dromedari
 *
 */
import javax.virtualsense.digitalio.bus.*;

public class CO2MeterMultiUser
{
	
	
	
    public static void motemain()
    {
    	byte ret1, ret2, ret3, ret4;
    	short co2; 
    	
    	
         while(true)
         {
        	 /*I2C.enable();
        	 I2C.start();
        	 I2C.write((byte)0x00);
        	 I2C.stop();
        	 I2C.disable();
        	 //Thread.sleep(20);*/
        	 
        	 I2C.enable();
        	 
        	 I2C.start();
        	 
        	 I2C.write((byte)0xD0);     	 
        	 I2C.write((byte)0x22); 
        	 I2C.write((byte)0x00);
        	 I2C.write((byte)0x08);
        	 I2C.write((byte)0x2A);
        	 
        	 I2C.stop();
        	 
        	 Thread.sleep(50);
        	 
        	 
        	 I2C.start();
        	 
        	 I2C.write((byte)0xD1);
        	 ret1 = I2C.read(true);

        	 if(ret1 == 33)
        	 {
	        	 ret2 = I2C.read(true);
	        	 ret3 = I2C.read(true);
	        	 co2 = (short)(ret2 << 8);
	        	 co2 += (short)ret3;
	        	 
	        	 ret4 = I2C.read(false);
	        	 
	        	 
	        	 
	        	 System.out.print("ret1: ");
	        	 System.out.println(ret1);
	        	 
	        	 System.out.print("ret2: ");
	        	 System.out.println(ret2);
	        	 System.out.print("ret3: ");
	        	 System.out.println(ret3);
	        	 System.out.println("");
	        	 
	        	 System.out.print("CO2 level: ");
	        	 System.out.println(co2);
        	 }
        	 
        	 I2C.stop();
        	 I2C.disable();
        	 
        	 
        	 Thread.sleep(1000);
        	 
        	 
         }
       
    }
}