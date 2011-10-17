
import javax.darjeeling.actuators.Leds;
import javax.darjeeling.Darjeeling;
import javax.darjeeling.PowerManager;

public class MultiThreadBlink
{
	static int  temp = 0;
    public static void main(String args[])
    {
     
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
    		Thread.sleep(2000);
    		System.out.print("****: tmp ");
    		System.out.println(temp);
    		
            
    	}
    }
}
