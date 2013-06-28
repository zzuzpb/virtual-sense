/*
 *  serial-input.c
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
 * 	serial input shell
 *
 * @author Emanuele Lattanzi
 *
 */


#include "contiki.h"

#include "dev/serial-line.h"

#include <stdio.h>
#include <string.h>

static void (*registered_call_back)(char *line, int line_len);

/*---------------------------------------------------------------------------*/
PROCESS(serial_input_process, "VirtualSense serial shell");
/*---------------------------------------------------------------------------*/

/*-------------------------------- -------------------------------------------*/
void
shell_prompt(char *str)
{
  printf("%s\r\n", str);
}
/*---------------------------------------------------------------------------*/
void
shell_exit(void)
{
}
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(serial_input_process, ev, data)
{
  PROCESS_BEGIN();

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(ev == serial_line_event_message && data != NULL);
    registered_call_back(data, strlen(data));
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
void serial_input_init(void( *callback)(char *line, int line_len))
{
  registered_call_back = callback;
  process_start(&serial_input_process, NULL);
}
/*---------------------------------------------------------------------------*/
/** @} */
