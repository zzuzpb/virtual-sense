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
   
	// creates a new Thread using the task and returns the thread id
	private static native short _createTask(short infusionID);
	
	// start the Task , calls the 'motemain' method on the Task
	private static native void _start(short id);
	
	protected static void createTask(short infusionID){
		_createTask(infusionID);
	}
    protected static void startExecution(short infusionID){ 
    	_start(infusionID);
    }
    protected static void stopExecution(short infusionID){
    	System.out.println("Stopping task");
    	Infusion toRemove = Infusion.getInfusion((short)(infusionID+2));
    	try{
    		toRemove.unload();
    	}catch(InfusionUnloadDependencyException ex){
    		System.out.println("Problem stopping task");
    	}

    }     
    
}
