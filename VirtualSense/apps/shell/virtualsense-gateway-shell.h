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
#ifndef __SHELL_GATEWAY_H__
#define __SHELL_GATEWAY_H__

#include "net/rime.h"
#include "contiki.h"
#include "shell.h"

void shell_gateway_init(struct broadcast_conn *c);

#endif /* __SHELL_GATEWAY_H__ */
