/*
 *	ArrayTest.java
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
package testvm.tests;

import javax.darjeeling.Darjeeling;

public class ArrayTest
{
	
	/**
	 * Tests the array implementation. Exercises the following opcodes:
	 * <p>
	 * BALOAD/BASTORE<br>
	 * SALOAD/SASTORE<br>
	 * IALOAD/IASTORE<br>
	 * NEWARRAY<br>
	 * ARRAYLENGTH
	 * <p>
	 * Also exercises the following non-array opcodes:<br>
	 * I2B/I2S (byte and short array tests)<br>
	 * ICONST_M1-ICONST5 (array initialiser tests)<br>
	 * DUP (array initialiser tests)<br>
	 * IFNULL<br>
	 */
	public static void test(int testBase)
	{
		boolean pass;
        byte    barr[];
        short   sarr[];
        int     iarr[];
        Object  oarr[];

        
		// test byte array new
		barr = new byte[100];
		Darjeeling.assertTrue(testBase+00, barr!=null);
		Darjeeling.assertTrue(testBase+01, barr.length == 100);

		// test byte array store/load
		pass = true;
		for (int i=0; i<barr.length; i++)
			barr[i] = (byte)i;
		for (int i=0; i<barr.length; i++)
			pass=pass && (barr[i]==i);
		Darjeeling.assertTrue(testBase+02, pass);

		// test byte array initialisation
		barr = new byte[] { -5, -4, -3, -2, -1, 0, 1, 2, 3, 4, 5 };
		Darjeeling.assertTrue(testBase+03, barr!=null);
		pass = true;
		for (int i=0; i<barr.length; i++)
			pass &= (barr[i]==i-5);
		Darjeeling.assertTrue(testBase+04, pass);

        // test default initialisation
        barr = new byte[5];
        Darjeeling.assertTrue(testBase+5, barr[3]==0);
        sarr = new short[5];
        Darjeeling.assertTrue(testBase+6, sarr[3]==0);
        iarr = new int[5];
        Darjeeling.assertTrue(testBase+7, iarr[3]==0);
        oarr = new Object[5];
        Darjeeling.assertTrue(testBase+8, oarr[3]==null);

		// test short array new
		sarr = new short[100];
		Darjeeling.assertTrue(testBase+10, sarr!=null);
		Darjeeling.assertTrue(testBase+11, sarr.length == 100);
		
		// test short array store/load
		pass = true;
		for (int i=0; i<sarr.length; i++)
			sarr[i] = (short)i;
		for (int i=0; i<sarr.length; i++)
			pass=pass && (sarr[i]==i);
		Darjeeling.assertTrue(testBase+12, pass);
		
		// test integer array new
		iarr = new int[100];
		Darjeeling.assertTrue(testBase+20, iarr!=null);
		Darjeeling.assertTrue(testBase+21, iarr.length == 100);
		
		// test integer array store/load
		pass = true;
		for (int i=0; i<iarr.length; i++)
			iarr[i] = i;
		for (int i=0; i<iarr.length; i++)
			pass=pass && (iarr[i]==i);
		Darjeeling.assertTrue(testBase+22, pass);
		
		// there was a bug in IASTORE that wasn't caught by the above tests,
		// this this is here to keep that bug from coming back :)
		int intTest[] = new int[1];
		intTest[0] = 0x67452301;
		Darjeeling.assertTrue(testBase+23, intTest[0] == 0x67452301);
		
		// char arrays weren't tested before :)
		char carr[] = new char[] { 'a', 'b', 'c', 'd' };
		Darjeeling.assertTrue(testBase+24, carr[0] == 'a');
		Darjeeling.assertTrue(testBase+25, carr[1] == 'b');
		Darjeeling.assertTrue(testBase+26, carr[2] == 'c');
		Darjeeling.assertTrue(testBase+27, carr[3] == 'd');
		
	}	

}
