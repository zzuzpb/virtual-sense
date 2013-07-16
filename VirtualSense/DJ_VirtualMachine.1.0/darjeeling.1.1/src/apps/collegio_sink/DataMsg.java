
import javax.virtualsense.network.UnicastPacket;

public class DataMsg extends UnicastPacket {
	public short sender_id; 
	public short noise;
	public short co2;
}