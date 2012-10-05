/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package javax.virtualsense.platform;

/**
 *
 * @author Lattanzi
 */
class Command {
    protected static final short LOAD = 0;
    protected static final short START = 1;
    protected static final short STOP = 2;
    short type;
    short executionContextID;
    
    protected Command(short _type, short _executionContextID){
        this.type = _type;
        this.executionContextID = _executionContextID;
    }
}
