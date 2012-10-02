/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package javax.virtualsense.platform;

/**
 *
 * @author Lattanzi
 */
interface ExecutionContext {
    short getInfusionID();
    void  setInfusionID(short ID);
    void startExecution();
    void stopExecution();
    /*boolean load();
    boolean remove();*/
    
}
