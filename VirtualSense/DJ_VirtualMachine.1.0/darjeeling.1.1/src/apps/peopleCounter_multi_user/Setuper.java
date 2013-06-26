
import javax.virtualsense.actuators.Leds;
import javax.virtualsense.VirtualSense;
import javax.virtualsense.concurrent.*;
import javax.virtualsense.digitalio.*;

public class Setuper extends Thread
{
	private InterruptPin setupPin;
	private Semaphore sem;
	private boolean setup;
	
    public Setuper(short pin)
    {
    	this.setupPin = new InterruptPin(false, pin);
    	this.sem = new Semaphore((short)0);
    	this.setup = false;
    }
  
    public void run()
    {	
    	while(true)
    	{   
    		this.setupPin.waitForInterrupt();
	    		
	    	this.setup = !this.setup;
    	}
    }
    
    public boolean getState()
    {
    	return this.setup;
    }
    
}