/*
 *	gateway-shell.h
 *
 *  Copyright (c) 2012 DiSBeF, University of Urbino.
 *
 *	This file is part of VirtualSense.
 *
 *	VirtualSense is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	VirtualSense is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with VirtualSense.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * the command shell on the virtualsense gateway.
 *
 * @author Emanuele Lattanzi
 *
 */

#include "contiki.h"
#include "virtualsense-gateway-shell.h"

#include "dev/watchdog.h"

#include "net/rime.h"
#include "dev/leds.h"

#include "node-id.h"
#include <string.h>

/*---------------------------------------------------------------------------*/
PROCESS(shell_nodeid_process, "nodeid");
SHELL_COMMAND(nodeid_command,
	      "nodeid",
	      "nodeid: set node ID",
	      &shell_nodeid_process);

PROCESS(shell_apps_process, "apps");
SHELL_COMMAND(apps_command,
	      "apps",
	      "apps: <id> <command> manage installed applications\n command can be (LOAD, START, STOP, UNLOAD)",
	      &shell_apps_process);
/*---------------------------------------------------------------------------*/

PROCESS_THREAD(shell_apps_process, ev, data)
{
  const char *newptr;
  char *command;
  char buf[60];
  PROCESS_BEGIN();

  uint16_t txpower = shell_strtolong(data, &newptr);
  if(newptr == data) {
     snprintf(buf, sizeof(buf), "apps: An application id is required");
  }else {
  command = strchr(data, ' ');
  if(strncmp (command," LOAD",5)== 0){
	  snprintf(buf, sizeof(buf), "sendig a LOAD command for application %u\n",txpower);
  }else if (strncmp (command," START",5)== 0){
	  snprintf(buf, sizeof(buf), "sendig a START command for application %u\n",txpower);
  }else if (strncmp (command," STOP",5)== 0){
	  snprintf(buf, sizeof(buf), "sendig a STOP command for application %u\n",txpower);
  }else if (strncmp (command," UNLOAD",5)== 0){
	  snprintf(buf, sizeof(buf), "sendig a UNLOAD command for application %u\n",txpower);
  }else {
	  snprintf(buf, sizeof(buf), "apps: Command not found %s", command);
  }

  }
  shell_output_str(&apps_command, "", buf);

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(shell_nodeid_process, ev, data)
{
  uint16_t nodeid;
  char buf[20];
  const char *newptr;
  PROCESS_BEGIN();

  nodeid = shell_strtolong(data, &newptr);
  
  /* If no node ID was given on the command line, we print out the
     current channel. Else we burn the new node ID. */
  if(newptr == data) {
    nodeid = node_id;
  } else {
    nodeid = shell_strtolong(data, &newptr);
    watchdog_stop();
    leds_on(0);
    node_id_burn(nodeid);
    leds_on(1);
    node_id_restore();
    leds_off(0 + 1);
    watchdog_start();
  }

  snprintf(buf, sizeof(buf), "%d", nodeid);
  shell_output_str(&nodeid_command, "Node ID: ", buf);

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
void
shell_gateway_init(void)
{
  shell_register_command(&apps_command);
  shell_register_command(&nodeid_command);

}
/*---------------------------------------------------------------------------*/
