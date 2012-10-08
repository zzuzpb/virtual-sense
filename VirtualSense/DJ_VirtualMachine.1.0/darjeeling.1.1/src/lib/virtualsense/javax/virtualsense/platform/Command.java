/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package javax.virtualsense.platform;

/**
 *
 * @author Lattanzi
 */
class Command {
    protected static final short LOAD = 0;
    protected static final short START = 1;
    protected static final short STOP = 2;
    private  static short type;
    private  static short executionContextID;
    
    public void setType(short t){
    	type = t;
    }
    public void setExecutionContextID(short e){
    	executionContextID = e;
    }
    
    public short getType(){
    	return type;
    }
    public short getExecutionContextID(){
    	return executionContextID;
    }
}
