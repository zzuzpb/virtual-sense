
import javax.virtualsense.actuators.Leds;
import javax.virtualsense.powermanagement.PowerManager;

public class Th extends Thread
{
	Buffer myTemp;
	int myTime;
	short myLed;
	String buff = "";
	
    public Th(Buffer temp, int time, short led){
    	this.myTemp = temp;
    	this.myTime = time;
    	this.myLed = led;
    }
  
    public void run(){
    	boolean state = true;
    	while(true)
    	{    				
    		Leds.setLed(this.myLed, state);
    		this.myTemp.temp++;
    	    Thread.sleep(this.myTime);    
    	   	state =! state;
    	   	this.buff = " Lattanzi Emanuele via Luigi Falasconi 25/F";
    	   	/*System.out.print("my led is : ");
    		System.out.println(this.myLed);*/
    	}
    }
}