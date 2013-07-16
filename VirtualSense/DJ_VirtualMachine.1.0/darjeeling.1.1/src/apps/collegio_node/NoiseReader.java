import javax.virtualsense.actuators.Leds;
import javax.virtualsense.VirtualSense;
import javax.virtualsense.sensors.ADC;
import javax.virtualsense.actuators.Leds;


public class NoiseReader
{
	public static short read()
    {
    	int read = 0;
    	int sum = 0;
    		
    	for(int i = 0; i < 20; ){
    		read = (int)ADC.read(ADC.CHANNEL0, ADC.REF2_5V);
        	
    		if(read > 0)
    		{
    			System.out.print("letto: ");System.out.println(read);
    			i++;
    			sum += read;
        		
        	        	Thread.sleep(200);
    		}
    	}
   		System.out.print(" ---- MEDIA: ");
   		System.out.println(sum = (sum / 20));  
   		
    	return (short) sum;	
    	}    	
}