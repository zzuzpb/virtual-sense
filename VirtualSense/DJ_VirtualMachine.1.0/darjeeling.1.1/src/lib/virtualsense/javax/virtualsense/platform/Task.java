/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package javax.virtualsense.platform;

/**
 *
 * @author Lattanzi
 */
public class Task extends Thread implements ExecutionContext{
    private short contextID;
        
    protected Task(){ // to deny the possibility to explicitely call the constructor
        
    }
    
    public void setInfusionID(short ID){
        this.contextID = ID;
    }
    public short getInfusionID(){
        return this.contextID;
    }
    
    public void startExecution(){
        super.start(); // invokes the start of the thread
    }
    public void stopExecution(){
        if(this.isAlive()){
            //need to preent the actual thread and all its children
            // a native methods is needed in order to preempt the
            // 
        }
    }
    
    public void run(){
        // this is the Thread run methods which calls the main of the task which 
        // has to be implemented by the programmers to create it own application.
        this.motemain();
    }
    
    public void motemain(){
    } // this is the user application entry point which need to be
                                     // overridden on the user task application.
    
    
}
