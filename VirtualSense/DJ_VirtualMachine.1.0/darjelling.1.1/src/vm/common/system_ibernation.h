/*
 *	VirtualSense.java
 *
 *  Copyright (c) 2011 DiSBeF, University of Urbino.
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
 * The VirtualSense ibernation system headers.
 *
 * @author Emanuele Lattanzi
 *
 */

#include "pointerwidth.h"

// save the heap on the disk
uint8_t save_heap(void *heap,
			   uint16_t left_p,
			   uint16_t right_p,
			   uint16_t panic_exe_p,
			   uint8_t ref_stack);

// load the heap from the disk
uint8_t load_machine(void *heap);
