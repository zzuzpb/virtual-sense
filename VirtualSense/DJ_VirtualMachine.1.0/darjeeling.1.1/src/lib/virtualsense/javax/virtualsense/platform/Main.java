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
    	/*Thread my = new Th();
    	my.start();*/
    
       while(true){
            //wait for a new command (stop  ID) (start ID) (load ID) 
            Command c = CommandManager.getCommand();
            short c_type = c.type;
            switch(c_type){
                case Command.LOAD:
                	System.out.println("LOADING A NEW TASK");
                    TaskManager.loadTask(c.executionContextID);
                    break;
                case Command.START:
                	System.out.println("STARTING A NEW TASK");
                    TaskManager.startTask(c.executionContextID);
                    break;
                case Command.STOP:
                	System.out.println("STOPPING TASK");
                    TaskManager.stopTask(c.executionContextID);
                    break;
                default:
                	System.out.println("UNKNOWN COMMAND");
            }
            Thread.sleep(100);
            System.out.println("++++");
        }
    }
}
