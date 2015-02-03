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
   	 System.out.println("ritardi");
   	 System.out.println("ritardi");
	 System.out.println("ritardi");
	 System.out.println("ritardi");
	 
	 
	 	byte base[] = new byte[2];
	 	base[0] = (byte)0x00;
	 	base[1] = (byte)0x00;
	 	
    	 byte b = (byte)0x02;
    	 byte adr = (byte)0x57;
    	 byte write[] = new byte[130];
 
    	 write[0] = (byte)0x00;
 		 write[1] = (byte)0x00;
    	 System.out.println("java write");
    	 for(int i = 2; i < 130; i++){
    		 write[i] = b++;
    	 }
    	 
    	 I2C.enable();
    	 
    	 //I2C.write(adr, base);
    	 System.out.println("ritardi indirizzo");
    	 I2C.write(adr, write);
    	 
    	 System.out.println("ritardi");
    	 System.out.println("ritardi");
    	 System.out.println("ritardi");
    	 System.out.println("ritardi");
    	 
    	 I2C.write(adr, base);
    	 System.out.println("ritardi indirizzo");
    	 byte read[] = I2C.read(adr, (short)128);
    	 
    	 for(int j = 0; j < read.length; j++){
    		 System.out.print("java read[");System.out.print(j);System.out.print("]: ");System.out.print(read[j]);System.out.print(" test: ");System.out.println((write[j+2]==read[j])?"OK":"FAIL");
    	 }
    }
}
