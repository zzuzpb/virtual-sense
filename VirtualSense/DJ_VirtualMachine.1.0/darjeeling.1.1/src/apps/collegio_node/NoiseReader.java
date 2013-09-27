import javax.virtualsense.actuators.Leds;
import javax.virtualsense.VirtualSense;
import javax.virtualsense.sensors.ADC;
import javax.virtualsense.actuators.Leds;


public class NoiseReader
{
	static int read = 0;
	static int sum = 0;
	static int avgLev = 1080;
	static short nodeId = VirtualSense.getNodeId();
	
	public static short read()
    {
		read = 0;
		sum = 0;
		
    	for(int i = 0; i < 200; i++)
    	{
    		read = (int)ADC.read((nodeId == 8)?ADC.CHANNEL6:ADC.CHANNEL7, ADC.REF2_5V);
    		
    		sum += (read >= avgLev)?read - avgLev:avgLev - read;
    		
    		Thread.sleep(10);
    	}
   		
    	return (short) (sum / 200);	
    }    	
}