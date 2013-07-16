import javax.virtualsense.digitalio.bus.*;

public class CO2Reader{
	
public static short read(){
    	byte ret1, ret2, ret3, ret4;
    	short co2 = 0;    	
    	
    	I2C.enable();
    	 
    	I2C.start();
    	 
    	I2C.write((byte)0xD0);     	 
    	I2C.write((byte)0x22); 
    	I2C.write((byte)0x00);
    	I2C.write((byte)0x08);
    	I2C.write((byte)0x2A);
    	 
    	I2C.stop();
    	 
    	Thread.sleep(50);
    	 
    	
    	I2C.start();
    	 
    	I2C.write((byte)0xD1);
    	ret1 = I2C.read(true);

    	if(ret1 == 33)
    	{
    		ret2 = I2C.read(true);
         	ret3 = I2C.read(true);
        	co2 = (short)(ret2 << 8);
        	co2 += (short)ret3;
        	 
        	ret4 = I2C.read(false);
        	System.out.print("CO2 level: ");
        	System.out.println(co2);
    	 }    	 
    	 I2C.stop();
    	 I2C.disable();
    	 return co2;
    }
}