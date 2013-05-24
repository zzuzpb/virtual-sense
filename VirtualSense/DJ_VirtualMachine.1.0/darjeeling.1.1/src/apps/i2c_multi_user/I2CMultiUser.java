/*
 *	I2CMultiUser.java
 * 
 *	Copyright (c) 2013 DiSBeF, University of Urbino.
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
 * Simple I2C test application to read data from embedded eeprom.
 * 
 * @author Emanuele Lattanzi
 *
 */
import javax.virtualsense.powermanagement.PowerManager;
import javax.virtualsense.digitalio.bus.I2C;
import javax.virtualsense.actuators.Leds;
import java.lang.Runtime;

public class I2CMultiUser
{
	
    public static void motemain()
    {
    	 byte ret = 0;
    	 byte eepromAddress = (byte)0xA6;
    	 short address = 3;
 
    	 I2C.enable();
    	 I2C.start();

    	 I2C.write((eepromAddress));
    	 I2C.write(((byte)(address >> 8)));
    	 I2C.write(((byte)(address & 0xff)));

    	 I2C.start();
    	 I2C.write((byte)((eepromAddress) | 1));
    	 ret = I2C.read(false);

    	 I2C.stop();
    	 I2C.disable();
    	 System.out.print("Read id from eeprom ");
    	 System.out.println(ret);
        
       
    }
}
