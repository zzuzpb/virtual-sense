
import javax.virtualsense.network.UnicastPacket;

public class DataMsg extends UnicastPacket {
	public short sender_id; 
	public short counter; 
	public short route;
	public short noise;
	public short co2;
	public short in;
	public short out;
	public short temp;
	public short pressure;
	public short light;
	
	public DataMsg()
	{
		this.sender_id = (short)0;
		this.counter = (short)0;
		this.route = (short)0;
		this.noise = (short)0;
		this.co2 = (short)0;
		this.in = (short)0;
		this.out = (short)0;
		this.temp = (short)0;
		this.pressure = (short)0;
		this.light = (short)0;
	}
	
}