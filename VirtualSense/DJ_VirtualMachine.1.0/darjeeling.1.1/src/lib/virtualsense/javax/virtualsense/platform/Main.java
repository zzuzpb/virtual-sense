/*
 *	Main.java
 * 
 *      Copyright (c) 2013 DiSBeF, University of Urbino.
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
package javax.virtualsense.platform;

import javax.virtualsense.network.*;
import javax.virtualsense.network.Packet;
import javax.virtualsense.VirtualSense;
/**
 *
 * @author Lattanzi
 */
public class Main {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
    	
       TaskManager.initDefaultTasks();
       short c_type = -1;
       short executionContextID = -1;
       Network myCommandNetwork = new Network(new CommandProtocol()); 
       
       while(true){
    	   Packet p = myCommandNetwork.receive();
     	   VirtualSense.printTime();
     	   System.out.println("Command receive at main ");
     	   if(p instanceof CommandMsg){
     		  CommandMsg msg = (CommandMsg)p;
	     	   c_type 			  = msg.type;
	     	   executionContextID = msg.executionContextID;
	           switch(c_type)
	           {
		            case Command.LOAD:
		            	System.out.println("LOADING A NEW TASK");
		                TaskManager.loadTask(executionContextID);
		                break;
		            case Command.START:
		              	System.out.println("STARTING A NEW TASK");
		                TaskManager.startTask(executionContextID);
		                break;
		            case Command.STOP:
		              	System.out.println("STOPPING TASK");
		                TaskManager.stopTask(executionContextID);
		                //TODO: call a onDestroy() method to allows programmer to deallocate particular object 
		                // like protocol on the network dispatcher.
		                break;
		            case Command.UNLOAD:
		              	System.out.println("UNLOADING TASK");
		                TaskManager.unloadTask(executionContextID);
		                break;
		            case Command.PSAU:
		              	System.out.println("PRINTING INFO TASK");
		              	/*
		              	 * TODO: NOT STABLE. NEED MORE DEBUG
		                TaskManager.sendInfoTask(executionContextID,myCommandNetwork);
		                */
		                break;
		            default:
		              	System.out.println("UNKNOWN COMMAND");
		        }
	            Thread.sleep(100);
	        }
     	}
    }
}
