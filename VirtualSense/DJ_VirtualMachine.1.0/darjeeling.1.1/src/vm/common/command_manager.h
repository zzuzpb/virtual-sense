/*
 *  command-manager.h
 *
 *  Copyright (c) 2013 DiSBeF, University of Urbino.
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
 * 	command manager definitions
 *
 * @author Emanuele Lattanzi
 *
 */

#ifndef __COMMAND_MANAGER_H__
#define __COMMAND_MANAGER_H__


void command_manager_setPlatformThreadID(int16_t id);
void command_manager_wakeUpPlatformThread(uint8_t command_id, int16_t execution_context_id);

uint8_t command_manager_getCommandID(void);
int16_t command_manager_getExecutionContextID(void);
void command_manager_setCommandID(uint8_t command_id);
void command_manager_setExecutionContextID(int16_t execution_context_id);


// APPLICATIONS MANAGER

#define COMMAND_APPS_HEADER 		 "COMMAND"
#define COMMAND_APPS_PORT 				  9000 // this is the control port TODO: define it on the platform-conf
#define COMMAND_APPS_LOAD					 0
#define COMMAND_APPS_START 	 				 1
#define COMMAND_APPS_STOP 	 				 2
#define COMMAND_APPS_UNLOAD	 				 3
#define COMMAND_APPS_PSAU	 				 4

struct virtualsense_apps_command_msg {
  char header[sizeof(COMMAND_APPS_HEADER)];
  uint8_t id;
  int16_t app_id;
  uint8_t command;
};

#endif /* __COMMAND_MANAGER_H__ */
