/*
 *      EmbeddedLoaderTask.java
 * 
 *      Copyright (c) 2008 CSIRO, Delft University of Technology.
 * 
 *      This file is part of Darjeeling.
 * 
 *      Darjeeling is free software: you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation, either version 3 of the License, or
 *      (at your option) any later version.
 *
 *      Darjeeling is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 * 
 *      You should have received a copy of the GNU General Public License
 *      along with Darjeeling.  If not, see <http://www.gnu.org/licenses/>.
 */
package org.csiro.darjeeling.embeddedloader.ant;

import java.io.FileOutputStream;
import java.io.IOException;
import java.io.PrintWriter;

import org.apache.tools.ant.BuildException;
import org.apache.tools.ant.Task;

public class EmbeddedLoaderTask extends Task
{
        private String cfile;
        private String burnfile;
        private String burnhfile;
        private String hfile;
        private String infusions;
        private String nativeinfusions;
        private String run;
        private boolean avr;
        
        public void execute()
        {
                // make sure properties are set
                if (cfile==null)
                        throw new BuildException("Property 'cfile' not set");
                if (burnfile==null)
                    	throw new BuildException("Property 'burnfile' not set");
                if (burnhfile==null)
                		throw new BuildException("Property 'burnhfile' not set");
                if (hfile==null)
                        throw new BuildException("Property 'hfile' not set");
                if (infusions==null)
                        throw new BuildException("Property 'infusions' not set");
                if (nativeinfusions==null)
                        throw new BuildException("Property 'nativeinfusions' not set");
                if (run==null)
                        throw new BuildException("Property 'run' not set");

                // write loader C file
                try {
                        FileOutputStream fout = new FileOutputStream(cfile);
                        PrintWriter writer = new PrintWriter(fout);
                        writeCFile(writer );
                        writer.flush();
                        writer.close();
                        fout.close();
                } catch (IOException ioex) {
                        throw new org.apache.tools.ant.BuildException("IO error while writing: " + cfile);
                }
                
                // write burn C file
                try {
                        FileOutputStream fout = new FileOutputStream(burnfile);
                        PrintWriter writer = new PrintWriter(fout);
                        writeBurnFile(writer );
                        writer.flush();
                        writer.close();
                        fout.close();
                } catch (IOException ioex) {
                        throw new org.apache.tools.ant.BuildException("IO error while writing: " + burnfile);
                }
                // write burner H file
                try {
                        FileOutputStream fout = new FileOutputStream(burnhfile);
                        PrintWriter writer = new PrintWriter(fout);
                        writeBurnHFile(writer );
                        writer.flush();
                        writer.close();
                        fout.close();
                } catch (IOException ioex) {
                        throw new org.apache.tools.ant.BuildException("IO error while writing: " + hfile);
                }
                
                // write loader H file
                try {
                        FileOutputStream fout = new FileOutputStream(hfile);
                        PrintWriter writer = new PrintWriter(fout);
                        writeHFile(writer );
                        writer.flush();
                        writer.close();
                        fout.close();
                } catch (IOException ioex) {
                        throw new org.apache.tools.ant.BuildException("IO error while writing: " + hfile);
                }
                
        }
        
        private static String[] split(String str)
        {
                String trim;
                
                // trim the string
                trim = str.trim();

                if (trim.length()==0)
                        return new String[0];

                // split on comma, remove any whitespace before and after the comma
                return trim.split("\\s*,\\s*");
                                
        }
        
        private static boolean isInList(String[] list, String str)
        {
                for (String nat : list) if (nat.equals(str)) return true;
                return false;
        }
        
        public void writeCFile(PrintWriter out)
        {
                if (avr)
                        out.println("#include <avr/pgmspace.h>");
                out.println("#include \"loader.h\"");
                out.println("#include \"common/parse_infusion.h\"");
                out.println("#include \"common/infusion.h\"");
                out.println("#include \"common/vmthread.h\"");
                out.println("// contiki inclusions");
        	 	out.println("#include \"platform-conf.h\"");
        	 	out.println("#include \"dev/eeprom.h\"");
        	 	out.println("// darjeeling inclusions");
        	 	out.println("#include \"config.h\"");
                out.println("");
                out.println("// infusion files");
                for (String infusion: split(infusions)) out.println("#include \"" + infusion + (avr?"_avr":"") + "_di_size.h\"");
				
                out.println("// native stubs for infusions");
                for (String infusion: split(nativeinfusions)) out.println("#include \"" + infusion + "_native.h\"");
                out.println("");
                out.println("// di files ram array");
                for (String infusion: split(infusions))
                	out.println("unsigned char " + infusion +"_ram["+infusion.toUpperCase()+"_DI_SIZE];");
				
                
                out.println("");
                out.println("void dj_loadEmbeddedInfusions(dj_vm * vm)");
                out.println("{");
                out.println("\tdj_infusion * infusion;");
                out.println("\tdj_thread * thread;");
                out.println("\tdj_global_id entryPoint;");
                out.println("\tuint16_t entryPointIndex;");
                
                out.println("\teeprom_addr_t address = SYSTEM_EEPROM_FS_BASE+HEAPSIZE+12;");
                out.println("");
                out.println("");
                
                for (String infusion: split(infusions)) 
                {
                
                	
                		out.println(String.format("\teeprom_read(address, %s, %s);",
                				infusion +"_ram", infusion.toUpperCase()+"_DI_SIZE"
                            ));
                		out.println("\taddress+=" + infusion.toUpperCase()+"_DI_SIZE;"); 
                		out.println("\t// " + infusion);
                        out.println(String.format("\tinfusion = %s(vm, (dj_di_pointer)%s_ram);",
                                        infusion.equals("base")?"dj_vm_loadSystemInfusion":"dj_vm_loadInfusion",
                                        infusion
                                        ));
                        
                        // if the infusion has native code, add the hook
                        if (isInList(split(nativeinfusions), infusion))
                                out.println(String.format("\tinfusion->native_handler = %s_native_handler;", infusion));
                        out.println("\tdj_vm_runClassInitialisers(vm, infusion);");
                        
                        // if the infusion should be run, create a thread and add it to the vm
                        if (isInList(split(run), infusion))
                        {
                                out.println("\tif ((entryPointIndex=dj_di_header_getEntryPoint(infusion->header))!=255)");
                                out.println("\t{");
                                out.println("\t\tentryPoint.infusion = infusion;");
                                out.println("\t\tentryPoint.entity_id = entryPointIndex;");
                                out.println("\t\tthread = dj_thread_create_and_run(entryPoint);");
                                out.println("\t\tdj_vm_addThread(vm, thread);");
                                out.println("\t}");                     
                        }
                        
                }
                out.println("");
                out.println("}");
                out.println("");
        }
        
        public void writeBurnFile(PrintWriter out)
        {
        		out.println("// contiki inclusions");
        	 	out.println("#include \"platform-conf.h\"");
        	 	out.println("#include \"dev/eeprom.h\"");
        	 	out.println("// darjeeling inclusions");
        	 	out.println("#include \"config.h\"");
        	 	out.println("// infusion files");
                for (String infusion: split(infusions)) out.println("#include \"" + infusion + (avr?"_avr":"") + "_di.h\"");

                out.println("");
                out.println("void dj_burnEmbeddedInfusions(void)");
                out.println("{");
                out.println("\teeprom_addr_t address = SYSTEM_EEPROM_FS_BASE+HEAPSIZE+12;");
                out.println("");
                
                for (String infusion: split(infusions)) 
                {
                        out.println("\t// " + infusion);
                        out.println(String.format("\teeprom_write(address, %s_di, %s);",
                                        infusion, infusion.toUpperCase()+"_DI_SIZE"
                                        ));
                        out.println("\taddress+=" + infusion.toUpperCase()+"_DI_SIZE;"); 
                }
                out.println("");
                out.println("}");
                out.println("");
        }
        public void writeBurnHFile(PrintWriter out)
        {
                out.println("#ifndef __burner__");
                out.println("#define __burner__");
                out.println("");
                out.println("void dj_burnEmbeddedInfusions(void);");
                out.println("");
                out.println("#endif");
                out.println("");
        }
        
        public void writeHFile(PrintWriter out)
        {
                out.println("#ifndef __loader__");
                out.println("#define __loader__");
                out.println("");
                out.println("#include \"common/vm.h\"");
                out.println("");
                out.println("void dj_loadEmbeddedInfusions(dj_vm * vm);");
                out.println("");
                out.println("#endif");
                out.println("");
        }
        
        public void setCfile(String cfile)
        {
                this.cfile = cfile;
        }
        public void setBurnFile(String burnfile)
        {
                this.burnfile = burnfile;
        }
        public void setBurnHFile(String burnhfile)
        {
                this.burnhfile = burnhfile;
        }
        
        public void setHfile(String hfile)
        {
                this.hfile = hfile;
        }
        
        public void setRun(String run)
        {
                this.run = run;
        }
        
        public void setInfusions(String infusions)
        {
                this.infusions = infusions;
        }
        
        public void setNativeinfusions(String nativeinfusions)
        {
                this.nativeinfusions = nativeinfusions;
        }
        
        public void setAvr(boolean avr)
        {
                this.avr = avr;
        }
        
}