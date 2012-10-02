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
        Task loaded = new Task();
        tasks.add(loaded);
        return loaded;
    }
    
    protected static void startTask(short infusionID){
        Iterator ite = tasks.iterator();
        Task t = null;
        while(ite.hasNext()){
            t = (Task) ite.next();
            if(t.getInfusionID() == infusionID)
                break;
        }
        if(t != null)
            t.startExecution();
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
