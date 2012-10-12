
import javax.virtualsense.actuators.Leds;
import javax.virtualsense.powermanagement.PowerManager;

public class Th extends Thread
{
	int myTemp;
	int myTime;
	short myLed;
	
    public Th(int temp, int time, short led){
    	this.myTemp = temp;
    	this.myTime = time;
    	this.myLed = led;
    }
  
    public void run(){
    	boolean state = true;
    	while(true)
    	{    				
    		Leds.setLed(this.myLed, state);
    		this.myTemp++;
    	    Thread.sleep(this.myTime);    
    	   	state =! state;
    	   	System.out.print("my led is : ");
    		System.out.println(this.myLed);
    	}
    }
}