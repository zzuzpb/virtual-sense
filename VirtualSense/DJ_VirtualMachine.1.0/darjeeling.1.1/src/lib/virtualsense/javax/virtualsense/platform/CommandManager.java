/*
 *	CommandManager.java
 * 
 *  Copyright (c) 2012 DiSBeF, University of Urbino.
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
 *
 * @author Emanuele Lattanzi 
 */

package javax.virtualsense.platform;

/**
 * Provide the native message from the operativ system.
 * @author Lattanzi
 *
 */
class CommandManager {
	private static native void _waitForMessage();
	private static native short _readCommandID();
	private static native short _readExecutionContextID();
	
	/**
	 * Waits for the next command delivery from operative system.
	 */
	protected static void waitForMessage(){
		_waitForMessage();
	}
	
	/**
	 * Returns the type of current command delivery from operative system.
	 */
    protected static short getCommandType(){       
        return _readCommandID();
    }
    
    /**
     * Return the identifier of current context.
     */
    protected static short getExecutionContextID(){
    	return _readExecutionContextID();
    }
}
