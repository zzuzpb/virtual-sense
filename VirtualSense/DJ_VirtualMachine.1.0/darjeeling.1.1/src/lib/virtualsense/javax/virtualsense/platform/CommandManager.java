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


class CommandManager {
	private static native void _waitForMessage();
	private static native short _readCommandID();
	private static native short _readExecutionContextID();
	
    protected static Command getCommand(){
        //should wait for a command on a semaphore
        // return a command as soon as it is received on the native buffer
    	_waitForMessage();
        return new Command(_readCommandID(),_readExecutionContextID());
    }
}
