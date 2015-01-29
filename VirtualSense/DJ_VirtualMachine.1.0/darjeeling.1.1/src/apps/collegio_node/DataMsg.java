
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
	public short humid;
	public short light;
	
}