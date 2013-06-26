/*
 *	storage.h
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
 * Storage definition.
 *
 * @author Matteo Dromedari
 *
 */
#include <msp430.h>
#include "common/array.h"
#include "common/darjeeling.h"

#ifndef __STORAGE_H__
#define __STORAGE_H__

#define BASE_PAGE_TABLE APPS_EEPROM_FS_BASE // 8320

#define BASE_MEM 14000

#define MEM_END 63999


typedef struct pt_entry
{
	unsigned char pid;
	unsigned char name[8];
	int16_t adr_base;
	int16_t adr_end;
} pt_entry_t;



short write_var(unsigned char *name, int name_len, unsigned char *var, int var_len);
int16_t read_short(unsigned char *name, int name_len);
int32_t read_int(unsigned char *name, int name_len);
char* read_String(unsigned char *name, int name_len);


/* Operation on file */
//short write_file(char *, uint8_t);
//uint8_t read_file(char *);




#endif /* __STORAGE_H__ */
