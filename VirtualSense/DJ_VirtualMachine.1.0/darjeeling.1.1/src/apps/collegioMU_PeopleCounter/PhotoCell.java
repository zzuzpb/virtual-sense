
import javax.virtualsense.actuators.Leds;
import javax.virtualsense.VirtualSense;
import javax.virtualsense.concurrent.*;
import javax.virtualsense.digitalio.*;
import java.util.*;


public class PhotoCell extends Thread
{
	//private PeopleCounterMultiUser counter;
	private short pin;
	private InterruptPin cell;	
	private int time;
	
    public PhotoCell(short c)//, PeopleCounterMultiUser counter)
    {
    	this.pin = c;
    	this.time = 0;
    }
  
    public void run()
    {
    	this.cell = new InterruptPin(true, this.pin);

    	while(true)
    	{
    		this.cell.waitForInterrupt();
    		//System.out.print(this.pin);System.out.println(" wakeup ----");
    		this.time = System.currentTimeMillis();
    		
    		if(this.pin == 3)
    		{
    			Leds.setLed(Leds.LED0, true);
    		}
    		else
    		{
    			Leds.setLed(Leds.LED1, true);
    		}
    			
    			
    		
    		Leds.setLed(Leds.LED2, false);
    		
			if(PeopleCounter.state == 0)	
				PeopleCounter.state = this.pin;
			else if(PeopleCounter.state != this.pin)
				PeopleCounter.sem.release();
			
    	}
    	
    	
    }
    
    public int getTime()
    {
    	return this.time;
    }
}