
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
    		System.out.print(this.pin);System.out.println(" wakeup");

    		if(PeopleCounter.state == 0)
    		{
    			PeopleCounter.state = this.pin;
    		}
    		else if(PeopleCounter.state != this.pin)
    		{
    			PeopleCounter.sem.release();
    		}
    		
    		this.time = System.currentTimeMillis();
    	}
    	
    	
    }
    
    public int getTime()
    {
    	return this.time;
    }
}