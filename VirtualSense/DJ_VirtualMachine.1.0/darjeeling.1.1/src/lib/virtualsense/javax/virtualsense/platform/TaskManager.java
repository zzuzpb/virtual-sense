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
    protected static Task loadTask(short infusionID){
        // create task using a native method and push reference on the stack
    	System.out.print("Load a new app!!!!  id ");
    	System.out.println(infusionID);
        Task loaded = Task.createTask(infusionID);
        tasks.add(loaded);
        System.out.println("App loaded successfully");
        return loaded;   
    }
    
    protected static void startTask(short infusionID){
    	System.out.println("Now we need to start a Task!!!!!");
        Iterator ite = tasks.iterator();
        Task t = null;
        while(ite.hasNext()){
        	System.out.print("Looking for the right task ");
            t = (Task) ite.next();
            System.out.println(t.getInfusionID());
            if(t.getInfusionID() == infusionID)
                break;
        }
        if(t != null){
            t.startExecution();
            System.out.println("New App started ");
        }
    }
    
    protected static void stopTask(short infusionID){
        Iterator ite = tasks.iterator();
        Task t = null;
        while(ite.hasNext()){
            t = (Task) ite.next();
            if(t.getInfusionID() == infusionID)
                break;
        }
        if(t != null)
            t.stopExecution();
    }
}
