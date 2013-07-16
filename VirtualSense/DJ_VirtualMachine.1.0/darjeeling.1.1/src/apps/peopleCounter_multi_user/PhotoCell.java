
import javax.virtualsense.actuators.Leds;
import javax.virtualsense.VirtualSense;
import javax.virtualsense.concurrent.*;
import javax.virtualsense.digitalio.*;

public class PhotoCell extends Thread
{
	private PeopleCounterMultiUser counter;
	private InterruptPin cell;
	private Semaphore sem;
	
	
    public PhotoCell(short cell, PeopleCounterMultiUser counter)
    {
    	this.counter = counter;
    	this.cell = new InterruptPin(true, cell);
    	
    	this.sem = new Semaphore((short)0);
    }
  
    public void run()
    {	 	
    	while(true)
    	{   
    		System.out.print("cell");
    		System.out.print(this.cell.getPort());
    		System.out.println("in wait...");
    		
    		// Wait for rising edge interrupt
    		this.cell.waitForInterrupt();
    		
    		System.out.print("cell");
    		System.out.print(this.cell.getPort());
    		System.out.println("unlock...");
    		
    		// When risign occours notify to counter and wait for the next event
    		this.counter.notifyCellEvent(this.cell.getPort());
    		this.sem.acquire();
    	}
    }
    
    public void resume()
    {
    	this.sem.release();
    }
}