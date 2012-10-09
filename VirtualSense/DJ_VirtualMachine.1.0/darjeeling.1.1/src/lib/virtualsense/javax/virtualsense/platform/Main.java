/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package javax.virtualsense.platform;

/**
 *
 * @author Lattanzi
 */
public class Main {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
       short c_type = -1;
       while(true){
            //wait for a new command (stop  ID) (start ID) (load ID) 
            CommandManager.waitForMessage();
            c_type = CommandManager.getCommandType();
            switch(c_type){
	            case Command.LOAD:
	            	System.out.println("LOADING A NEW TASK");
	                TaskManager.loadTask(CommandManager.getExecutionContextID());
	                break;
	            case Command.START:
	              	System.out.println("STARTING A NEW TASK");
	                TaskManager.startTask(CommandManager.getExecutionContextID());
	                break;
	            case Command.STOP:
	              	System.out.println("STOPPING TASK");
	                TaskManager.stopTask(CommandManager.getExecutionContextID());
	                break;
	            case Command.UNLOAD:
	              	System.out.println("UNLOADING TASK");
	                TaskManager.unloadTask(CommandManager.getExecutionContextID());
	                break;
	            default:
	              	System.out.println("UNKNOWN COMMAND");
	        }
            Thread.sleep(100);
        }
    }
}
