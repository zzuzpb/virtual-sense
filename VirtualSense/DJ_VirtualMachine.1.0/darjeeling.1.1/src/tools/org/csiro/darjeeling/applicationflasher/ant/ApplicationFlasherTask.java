/*
 *  ApplicationFlasherTask.java
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
 * Applcation flasher task.
 * 
 * @author Emanuele Lattanzi
 *
 */
package org.csiro.darjeeling.applicationflasher.ant;

import java.io.FileOutputStream;
import java.io.IOException;
import java.io.PrintWriter;
import java.io.*;

import org.apache.tools.ant.BuildException;
import org.apache.tools.ant.Task;
import org.apache.tools.ant.*;

public class ApplicationFlasherTask extends Task
{
		private static final String PROGRAMMER = "mspdebug";
		private static final String OPTIONS = "-j";
		private static final String DRIVER = "tilib";
		
        private String flasherfile;
        private String ttyport;
        private String firmware;
        private String saddress;
        private String apps;
        private String appdir;
                
        public void execute()
        {
                // make sure properties are set
                if (flasherfile==null)
                        throw new BuildException("Property 'flasherfile' not set");
                if (ttyport==null)
                        throw new BuildException("Property 'ttyport' not set");
                if (firmware==null)
                        throw new BuildException("Property 'firmware' not set");
                if (apps==null)
                        throw new BuildException("Property 'apps' not set");
                if (saddress==null)
                        throw new BuildException("Property 'sddress' not set");
                if (appdir==null)
                    throw new BuildException("Property 'appdir' not set");

                // write loader C file
                try {
                        FileOutputStream fout = new FileOutputStream(flasherfile);
                        PrintWriter writer = new PrintWriter(fout);
                        writeFlasherFile(writer );
                        writer.flush();
                        writer.close();
                        fout.close();
                } catch (IOException ioex) {
                        throw new org.apache.tools.ant.BuildException("IO error while writing: " + flasherfile);
                }                
        }
        
        private static String[] split(String str)
        {
                String trim;
                
                // trim the string
                trim = str.trim();

                if (trim.length()==0)
                        return new String[0];

                // split on comma, remove any whitespace before and after the comma
                return trim.split("\\s*,\\s*");
                                
        }
        
        private static boolean isInList(String[] list, String str)
        {
                for (String nat : list) if (nat.equals(str)) return true;
                return false;
        }
        
        public void writeFlasherFile(PrintWriter out)
        {
        		int address = Integer.parseInt(saddress, 16);
        		int startingAddresses[] = new int[split(apps).length];
        		int i = 0;
        		String progApp = PROGRAMMER+" "+OPTIONS+" -d "+ttyport+" "+DRIVER+" ";
                out.println("#!/bin/bash");
                // the line needed to program the drjeeling firmware
                out.println(progApp+" 'prog "+firmware+"'");
                
                
                for (String app: split(apps)) 
                {
                	out.println("# this line will flash the application " + app);
                    log("Writing the a "+app+" application content",Project.MSG_INFO);
                    startingAddresses[i] = address;
                    i++;
                    try{
                    	  // Open the file that is the first 
                    	  // command line parameter
                    	  FileInputStream fstream = new FileInputStream(appdir+File.separator+app+"_di.hexd");
                    	  // Get the object of DataInputStream
                    	  DataInputStream in = new DataInputStream(fstream);
                    	  BufferedReader br = new BufferedReader(new InputStreamReader(in));
                    	  String strLine;
                    	  //Read File Line By Line
                    	  // read the first line containing the byte numbers
                    	  int total = Integer.parseInt(br.readLine());
                    	  while ((strLine = br.readLine()) != null)   {
                    	  // Print the content on the console
                    	  	out.println(progApp+" 'mw 0x"+Integer.toHexString(address)+" " +strLine+"'");
                    	  	if(total > 256){
                    	  		address+=256;
                    	  		total-=256;
                    	  	}else 
                    	  		address+=total;                        	  	
                    	  }
                    	  //Close the input stream
                    	  in.close();
                   	  }catch (Exception e){//Catch exception if any
                   	    	System.err.println("Error: " + e.getMessage());
                   	  }
                }
                out.println("# now write the application table [app_id][address]");
                String table = "";
                for(int h = 0; h < startingAddresses.length; h++){
                	byte id_0 = (byte)((char)(h+1) & 0xff);                	
                	byte id_1 = (byte)(((char)(h+1)>> 8) & 0xff);
                	System.out.println("id_0: "+id_0+" id_1:"+id_1);
                	byte ad_0 = (byte)((char)(startingAddresses[h]) & 0xff);
                	byte ad_1 = (byte)(((char)(startingAddresses[h])>> 8) & 0xff);
                	System.out.println("ad_0: "+ad_0+" ad_1:"+ad_1);
                	table+=""+Integer.toHexString(id_1)+" "+
                	purgeHex(Integer.toHexString(id_0))+" "+
                	purgeHex(Integer.toHexString(ad_1))+" "+
                	purgeHex(Integer.toHexString(ad_0))+" ";
                }
                out.println(progApp+" 'mw 0x20000 "+table+"'");
                log("Table writed: "+table,Project.MSG_INFO);
        }
        
        public void setFlasherfile(String ffile)
        {
                this.flasherfile = ffile;
        }
        
        public void setTtyport(String ttport)
        {
                this.ttyport = ttport;
        }
        
        public void setFirmware(String fware)
        {
                this.firmware = fware;
        }
        
        public void setSaddress(String sad)
        {
                this.saddress = sad;
        }    
        public void setApps(String ap)
        {
                this.apps = ap;
        } 
        public void setAppdir(String ap)
        {
                this.appdir = ap;
        } 
        public String purgeHex(String toPurge){
        	String ret = toPurge;
        	if (ret.length() > 2){
        		ret = ret.substring(ret.length()-2);
        		log("Purging "+toPurge+" into "+ret,Project.MSG_INFO);
        	}
        	return ret;
        }
}