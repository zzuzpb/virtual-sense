
import javax.virtualsense.actuators.Leds;
import javax.virtualsense.powermanagement.PowerManager;

public class MultiThreadBlinkMultiUser
{
	static int temp = 0;
	public static void motemain()
    {
    	Th thread1 = new Th(temp, 1000, (short)1);
    	Th thread2 = new Th(temp, 500, (short)2);
    	
    	thread1.start();
    	thread2.start();
    
    	   	
    	while(true){
    		Leds.setLed(1,false);
    		Leds.setLed(2,false);
    		temp-=4;
    		Thread.sleep(2000);
    		System.out.print("tmp: ");
    		System.out.println(temp);
    		
            
    	}
    }
}
