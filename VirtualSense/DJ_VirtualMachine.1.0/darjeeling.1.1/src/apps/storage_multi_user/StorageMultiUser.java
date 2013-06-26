/*
 *	Storage.java
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

import javax.virtualsense.powermanagement.PowerManager;
import javax.virtualsense.storage.Storage;
import java.lang.Runtime;

/**
 * Storage application.
 * 
 * @author Matteo Dromedari
 *
 */
public class StorageMultiUser
{
	public static void motemain()
    {
		while(true)
		{
			/*int i = 23;
			
			System.out.print("write integer: ");
			System.out.print(i);
			System.out.println("...");
			Storage.writeVar("i", i);
			
			System.out.print("read integer: ");
			int r = Storage.readInt("i");
			System.out.print(r);
			System.out.println(".");
			*/
			short s = 3;
			
			System.out.print("write short: ");
			System.out.print(s);
			System.out.println("...");
			Storage.writeVar("s", s);
			
			System.out.print("read shorts: ");
			short x = Storage.readShort("s");
			System.out.print(x);
			System.out.println(".");
			
			String a = "matteo";
			
			System.out.print("write String: ");
			System.out.print(a);
			System.out.println("...");
			Storage.writeVar("a", a);
			
			System.out.print("read string: ");
			String g = Storage.readString("a");
			System.out.print(g);
			System.out.println(".");
			
			
			
			Thread.sleep(2000);
		}
    }
}