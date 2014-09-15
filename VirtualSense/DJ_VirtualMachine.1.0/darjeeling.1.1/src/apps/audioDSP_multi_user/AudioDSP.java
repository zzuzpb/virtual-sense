import javax.virtualsense.digitalio.DigitalPin;
import javax.virtualsense.actuators.Leds;


/**
 * Driver for VirtualSense AudioDSP layer.
 * 
 * @author Emanuele Lattanzi
 *
 */
public class AudioDSP{
	// Pin for manage AudioDSP layer
	public DigitalPin dspEn;
	public DigitalPin codecEn;
	
	public void AudioDSP(){
		dspEn = new DigitalPin(false, DigitalPin.DIO3);
		codecEn = new DigitalPin(false, DigitalPin.DIO2);
		
		// Disable AudioDSP layer
		dspEn.write(true);
		codecEn.write(false);
	}
	
	public void enable(boolean state){
		if(state){
			// Enable AudioDSP layer
			dspEn.write(false);
			codecEn.write(true);
		}else{
			// Disable AudioDSP layer
			dspEn.write(true);
			codecEn.write(false);
		}
	}
}