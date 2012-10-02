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
        while(true){
            //wait for a new command (stop  ID) (start ID) (load ID) 
            Command c = CommandManager.getCommand();
            short c_type = c.type;
            switch(c_type){
                case Command.LOAD:
                    TaskManager.loadTask(c.infusionID);
                    break;
                case Command.START:
                    TaskManager.startTask(c.infusionID);
                    break;
                case Command.STOP:
                    TaskManager.stopTask(c.infusionID);
                    break;
                default:
            }
            Thread.sleep(1000);
            System.out.println(".");
        }
    }
}
