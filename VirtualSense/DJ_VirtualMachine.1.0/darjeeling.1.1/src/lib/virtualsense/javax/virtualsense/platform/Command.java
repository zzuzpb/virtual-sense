/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package javax.virtualsense.platform;

/**
 *	Defines a generic command. 
 *
 * @author Lattanzi
 */
class Command {
    protected static final short LOAD = 0;
    protected static final short START = 1;
    protected static final short STOP = 2;
    protected static final short UNLOAD = 3;
    protected static final short PSAU = 4;
    private  static short type;
    private  static short executionContextID;
    
    /**
     * Sect the type of current command.
     * @param type of command (LOAD=0, START=1, STOP=2, UNLOAD=3, PSAU=4).
     */
    public void setType(short t){
    	type = t;
    }
    
    /**
     * Set the identifier of execution context.
     * @param execution context identifier.
     */
    public void setExecutionContextID(short e){
    	executionContextID = e;
    }
    
    /**
     * Returns type of current command.
     * @preturn command type (LOAD=0, START=1, STOP=2, UNLOAD=3, PSAU=4)
     */
    public short getType(){
    	return type;
    }
    
    /**
     * Returns the identifier execution context of current command.
     * @param identifier execution context.
     */
    public short getExecutionContextID(){
    	return executionContextID;
    }
}
