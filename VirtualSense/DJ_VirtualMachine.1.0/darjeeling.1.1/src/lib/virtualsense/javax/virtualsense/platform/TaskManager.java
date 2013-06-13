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
	//private static ArrayList<Task> tasks = new ArrayList<Task>((short)20);
	
	private static Task tasks[] = new Task[20];
	private static int index = 0;
	
	protected static void initDefaultTasks(){
		short tn = Task.getDefaultTasksNumber();
		for(short i = 0; i < tn; i++){
			short executionContextID = Task.getDefaultExecutionContextID(i);
			if(Task.defaultNeedToLoad(i))
				loadTask(executionContextID);
			if(Task.defaultNeedToStart(i))
				startTask(executionContextID);
		}
	}
	
    protected static void loadTask(short executionContextID){
        // create task using a native method and push reference on the stack
    	System.out.print("----- Load a new app!!!!  id ");
    	System.out.println(executionContextID);
        addTask(new Task(executionContextID));    
        Thread.yield(); // needed to allow deferred infusion initialization
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
    protected static void sendInfoTask(short executionContextID){
    	
    	Task toSendInfo = getTask(executionContextID);
    	if(toSendInfo != null)
    		toSendInfo.sendInfo();  
    	else Task.sendGlobalInfo();
    }
    
    protected static Task getTask(short id){
    	Task toGet = null;
    	for(int i = 0; i < index; i++){
    		if(tasks[i].executionContextID == id){
    			toGet = tasks[i];
    			break;
    		}
    	}
    	return toGet;    
    } 
    
    protected static void addTask(Task t){
    	tasks[index] = t;
    	index++;    
    }
   
}
