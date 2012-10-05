/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package javax.virtualsense.platform;

/**
 *
 * @author Lattanzi
 */
public class Task implements ExecutionContext{
   
	private short infusionID;
    
    // thread ID is a link to an internal thread
	private short id;

	// creates a new Thread using the task and returns the thread id
	private static native short _createThread(short infusionID, ExecutionContext r);
	
	// creates a new Task instance
	private static native Task _createTaskInstance(short infusionID);
	
	// start the Task , calls the 'motemain' method on the Task
	private native void _start(short id);

	// gets the status of a Task
	private native short _getStatus(short id);
 
	protected Task(){ //void constructor to quiet the compiler
		
	}
	
	protected static Task createTask(short infusionID){
		Task t = _createTaskInstance(infusionID);
		t.id = _createThread(infusionID, t);
    	t.infusionID = infusionID;
    	return t;
	}
        
    public void setInfusionID(short ID){
        this.infusionID = ID;
    }
    public short getInfusionID(){
        return this.infusionID;
    }
    
    public void startExecution(){
    	// check Task status
		short status = _getStatus(this.id);

		// state 0 == THREADSTATE_CREATED
		if (status != 0)
			throw new IllegalThreadStateException();
		// start
		_start(this.id);
    }
    public void stopExecution(){

    }    
    public void motemain(){
    	System.out.println("HELLO");
    }
     // this is the user application entry point which need to be
                                     // overridden on the user task application.
    
    
}
