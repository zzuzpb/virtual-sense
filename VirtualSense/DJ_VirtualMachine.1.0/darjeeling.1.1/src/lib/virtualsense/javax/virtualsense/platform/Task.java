/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package javax.virtualsense.platform;

import javax.darjeeling.vm.Infusion;
import javax.darjeeling.vm.InfusionUnloadDependencyException;

/**
 *
 * @author Lattanzi
 */
public class Task {
	protected short executionContextID;
	protected short infusionID;
	
	// creates a new Thread using the task and returns the thread id
	private static native short _loadExecutionContext(short executionContextID);
	
	// start the Task , calls the 'motemain' method on the Task
	private static native void _start(short infusionID, short executionContextID);
	
	private static native void _stop(short infusionID);
	
	protected Task(short executionContextID){
		this.executionContextID = executionContextID;
		this.infusionID = _loadExecutionContext(executionContextID);
	}
	
    protected void startExecution(){ 
    	// creates thread and runs it
    	_start(this.infusionID, this.executionContextID);
    }
    protected void stopExecution(){
    	//kill all threads belonging to the infusion
    	_stop(this.infusionID);    	
    }  
    protected void unload(){
    	// kill threads and unload the infusion
    	Infusion toRemove = Infusion.getInfusion(this.infusionID);
    	try{
    		toRemove.unload();
    	}catch(InfusionUnloadDependencyException ex){
    		System.out.println("Problem unloading infusion task");
    	}

    }
    
}
