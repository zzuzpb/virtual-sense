
import javax.virtualsense.actuators.Leds;
import javax.virtualsense.powermanagement.PowerManager;

public class MultiThreadBlinkMultiUser
{
	static int  temp = 0;
    public static void motemain()
    {
    	boolean ibernated = false;
    	short i = 0;
    	/* slow down the system clock 
         * (normally it is configured at 10 ms)
         * to reduce power consumption 
         * leaves the CPU in the LPM3 state */      
    	PowerManager.setSystemClockMillis(500); 
    	new Thread(){
    		public void run(){
    			boolean state = true;
    			int i = 0;
    			int x = 0;
    			while(true)
    	        {    				
    					Leds.setLed(0, state);
    					Leds.setLed(3, false);
    	            	temp++;
    	            	Thread.sleep(1000);    
    	            	state =! state;
    	            	i++;       	            	
    	        }
    		}
    	}.start(); 
    	
    	new Thread(){
    		public void run(){
    			boolean state = true;
    			while(true)
    	        {
    				Leds.setLed(1, state);
    				Leds.setLed(2, false);
    	            temp++;
    				Thread.sleep(1000); 
    				state =! state;

    	        }
    		}
    	}.start();
    	
    	while(true){
    		Leds.setLed(0,false);
    		Leds.setLed(1,false);
    		Leds.setLed(2,true);
    		Leds.setLed(3,true);
    		temp-=4;
    		i++;
    		Thread.sleep(2000);
    		if(!ibernated && i == 5){
    			ibernated = true;
    			PowerManager.systemHibernation();
    		}
    		System.out.print("****: tmp ");
    		System.out.println(temp);
    		
            
    	}
    }
}
