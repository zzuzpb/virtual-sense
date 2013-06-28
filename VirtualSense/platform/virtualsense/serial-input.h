/*
 *  serial-input.h
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
#ifndef __SERIAL_SHELL_H__
#define __SERIAL_SHELL_H__

#include "contiki.h"

void serial_input_init(void ( *callback)(char *line, int line_len));

#endif /* __SERIAL_SHELL_H__ */
