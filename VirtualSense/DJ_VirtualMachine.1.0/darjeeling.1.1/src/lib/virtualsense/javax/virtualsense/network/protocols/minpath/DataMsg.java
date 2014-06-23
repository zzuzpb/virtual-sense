
package javax.virtualsense.network.protocols.minpath;

import javax.virtualsense.network.UnicastPacket;

public class DataMsg extends UnicastPacket {
	public static final short CO2 		= 0;
	public static final short LIGHT 	= 1;
	public static final short NOISE 	= 2;
	public static final short PEOPLEIN 	= 3;
	public static final short PEOPLEOUT = 4;
	public static final short POWER 	= 5;
	public static final short TEMP 		= 6;
	public static final short PRESSURE 	= 7;
	
	public short sender_id; 
	public short counter; 
	public short route;
	public short label;
	public short value;
	
	public DataMsg(short sender, short counter)
	{
		this.sender_id = sender;
		this.counter = counter;
		this.route = (short)0;
		this.label = (short)-1;
		this.value = (short)0;
		
		/*this.noise = (short)0;
		this.co2 = (short)0;
		this.in = (short)0;
		this.out = (short)0;
		this.temp = (short)0;
		this.pressure = (short)0;
		this.light = (short)0;*/
	}
	
}