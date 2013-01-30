/*
 *	Radio.java
 * 
 *  Copyright (c) 2011 DiSBeF, University of Urbino.
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
package javax.virtualsense.radio;

/**
 * Manages the radio interface layer of virtualsense nodes.
 * 
 * @author Lattanzi
 *
 */
public class Radio
{
	
	private static native void _waitForMessage();
	private static native byte[] _readBytes();
	private static native void _init();
	public static native short getDestId();
	public static native short getSenderId();
	//private static native byte _getNumMessages();
	//public static native void setChannel(short channel);
	//public static native short getMaxMessageLength();

	private static Object sendLock, receiveLock;
	
	private Radio()
	{
		// don't let people instantiate this class
	}
	
	/**
	 * Inizialize radio interfaces on the node. 
	 */
	public static void init()
	{
		_init();
		sendLock = new Object();
		receiveLock = new Object();
	}
	
	/**
	 * Returns lock for send any data.
	 * 
	 * @return sender lock.
	 */
	public static Object getSendLock()
	{
		return sendLock;
	}
	
	/*public static byte getNumMessages()
	{
		return _getNumMessages();
	}*/
	
	/**
	 * Return data received from an another node.
	 * 
	 * @return data under form of byte received.
	 */
	public static byte[] receive()
	{
		if (receiveLock==null){
			throw new RadioNotInitialisedException();
		}
		
		// allow only one thread to wait for radio messages
		synchronized(receiveLock)
		{
			_waitForMessage();
			return _readBytes();
		}		
	}
	
	private static native void _broadcast(byte[] data);
	
	/**
	 * Send specified data in broadcast mode. 
	 * 
	 * @param data under form of byte to send.
	 */
	public static void broadcast(byte[] data)
	{
		if (sendLock==null)
			throw new RadioNotInitialisedException();
		
		if (data==null)
			throw new NullPointerException();

		// allow only one thread to send at the same time
		synchronized (sendLock)
		{
			// broadcast the message
			_broadcast(data);
		}
	}
		
	private static native boolean _send(short receiverId, byte[] data);

	/**
	 * Sends data to specified node identified by id.  
	 * 
	 * @param data data to be send.
	 * @param receiverId identifier of receiver.
	 * @return true if packet is sent false otherwise.
	 */
	public static boolean send(short receiverId, byte[] data)
	{
		if (sendLock==null)
			throw new RadioNotInitialisedException();

		if (data==null)
			throw new NullPointerException();
		
		// allow only one thread to send at the same time
		synchronized (sendLock)
		{
			// broadcast the message
			return _send(receiverId, data);
		}
	}
}
