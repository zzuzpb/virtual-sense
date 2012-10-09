/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package javax.virtualsense.platform;

import java.util.ArrayList;
import java.util.Iterator;

/**
 *
 * @author Lattanzi
 */
public class TaskManager {
	private static ArrayList<Task> tasks = new ArrayList<Task>();
	
    protected static void loadTask(short executionContextID){
        // create task using a native method and push reference on the stack
    	System.out.print("----- Load a new app!!!!  id ");
    	System.out.println(executionContextID);
        tasks.add(new Task(executionContextID));        
    }
    
    
    protected static void startTask(short executionContextID){
    	System.out.print("----- Starting a task ");
    	Task toStart = getTask(executionContextID);
    	if(toStart != null)
    		toStart.startExecution();        
    }
    
    protected static void stopTask(short executionContextID){
    	System.out.print("----- Stopping a task ");
    	Task toStop = getTask(executionContextID);
    	if(toStop != null)
    		toStop.stopExecution();        
    }
    
    protected static void unloadTask(short executionContextID){
    	System.out.print("----- Unloading task ");
    	Task toUnload = getTask(executionContextID);
    	if(toUnload != null)
    		toUnload.unload();        
    }
    
    protected static Task getTask(short id){
    	Task toGet = null;
    	Iterator it = tasks.iterator();
    	while(it.hasNext()){
    		toGet = (Task)it.next();
    		if(toGet.executionContextID == id)
    			break;
    	}
    	return toGet;
    }  
   
}
