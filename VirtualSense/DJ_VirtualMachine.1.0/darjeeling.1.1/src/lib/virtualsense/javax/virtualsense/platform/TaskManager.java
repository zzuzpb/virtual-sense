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
	
    protected static void loadTask(short infusionID){
        // create task using a native method and push reference on the stack
    	System.out.print("----- Load a new app!!!!  id ");
    	System.out.println(infusionID);
        Task.createTask(infusionID);
        System.out.println("----- App loaded successfully"); 
    }
    
    protected static void startTask(short infusionID){
    	System.out.print("----- Now we need to start the  Task -> ");
    	System.out.println(infusionID);
        Task.startExecution(infusionID);
        System.out.println("----- New App started ");
    }
    
    protected static void stopTask(short infusionID){
    	System.out.print("----- Now we need to stop the  Task -> ");
    	System.out.println(infusionID);
        Task.stopExecution(infusionID);
        System.out.println("----- App stopped ");
    }
}
