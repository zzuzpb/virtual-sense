import javax.virtualsense.actuators.Leds;
import javax.virtualsense.VirtualSense;
import javax.virtualsense.sensors.ADC;
import javax.virtualsense.actuators.Leds;


public class NoiseReader
{
	static int read = 0;
	static int sum = 0;
	static int h = 0;
	
	public static short read()
    {
       		
		read = 0;
		sum = 0;
		h = 0;
		
    	for(int i = 0; i < 50; i++){
    		read = (int)ADC.readIntRef(ADC.CHANNEL6, ADC.REF2_5V);
    		//System.out.print("letto: ");System.out.println(read); 
    		if(read > 0)
    		{    			   			
    			sum += read;        		
        	    h++;
    		}
    		Thread.sleep(100);
    	}
   		/*System.out.print(" ---- MEDIA SOUND: ");
   		System.out.println(sum = (sum / 20));  */
   		
    	return (short) (sum/h);	
    	}    	
}