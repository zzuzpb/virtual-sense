/*
 *	Command.java
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

/**
 *	Defines a generic command. 
 *
 * @author Lattanzi
 */
public class Command {
    public static final short LOAD = 0;
    public static final short START = 1;
    public static final short STOP = 2;
    public static final short UNLOAD = 3;
    public static final short PSAU = 4;
    private  static short type;
    private  static short executionContextID;
    
    /**
     * Sect the type of current command.
     * @param type of command (LOAD=0, START=1, STOP=2, UNLOAD=3, PSAU=4).
     */
    public void setType(short t){
    	type = t;
    }
    
    /**
     * Set the identifier of execution context.
     * @param execution context identifier.
     */
    public void setExecutionContextID(short e){
    	executionContextID = e;
    }
    
    /**
     * Returns type of current command.
     * @preturn command type (LOAD=0, START=1, STOP=2, UNLOAD=3, PSAU=4)
     */
    public short getType(){
    	return type;
    }
    
    /**
     * Returns the identifier execution context of current command.
     * @param identifier execution context.
     */
    public short getExecutionContextID(){
    	return executionContextID;
    }
}
