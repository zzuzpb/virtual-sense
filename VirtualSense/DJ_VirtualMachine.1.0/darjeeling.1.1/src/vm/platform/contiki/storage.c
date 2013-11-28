/*
 *	storage.c
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
 * Storage definitions.
 *
 * @author Matteo Dromedari
 *
 */



#include "storage.h"
#include "contiki.h"
#include "dev/board.h"
#include "eeprom.h"
#include "base_definitions.h"
#include "common/array.h"

#include "common/heap/heap.h"
#include "common/execution/execution.h"
#include "common/panic.h"





/*---------------------------------------------------------------------------*/

/* Operation on variable */
short write_var(unsigned char *name, int name_len, unsigned char *var, int var_len)
{
	eeprom_addr_t mem_pointer = APPS_EEPROM_FS_BASE;

	eeprom_init();
	eeprom_write(mem_pointer, name, name_len);
	eeprom_write((mem_pointer + name_len), var, var_len);

	return 0;
}

int16_t read_short(unsigned char *name, int name_len)
{
	eeprom_addr_t mem_pointer = APPS_EEPROM_FS_BASE;
	int i;

	eeprom_init();
	unsigned char n[name_len];	
	eeprom_read(mem_pointer, n, name_len);
	printf("C - nome letto: ");
	for(i = 0; i < name_len; i++)
	{
		printf("%c", n[i]);
	}
	printf(" ");

	unsigned char v[sizeof(int16_t)];
	eeprom_read(mem_pointer + name_len, v, sizeof(int16_t));

	int16_t *ret = &v;

	return *ret;
}

int32_t read_int(unsigned char *name, int name_len)
{
	eeprom_addr_t mem_pointer = APPS_EEPROM_FS_BASE;
	int i;

	eeprom_init();
	unsigned char n[name_len];	
	eeprom_read(mem_pointer, n, name_len);
	printf("C - nome letto: ");
	for(i = 0; i < name_len; i++)
	{
		printf("%c", n[i]);
	}
	printf(" ");

	unsigned char v[sizeof(int32_t)];
	eeprom_read(mem_pointer + name_len, v, sizeof(int32_t));

	int32_t *ret = &v; 

	return *ret;
}

char* read_String(unsigned char *name, int name_len)
{
	eeprom_addr_t mem_pointer = APPS_EEPROM_FS_BASE;
	int i;
	char *str;

	eeprom_init();
	unsigned char n[name_len];	
	eeprom_read(mem_pointer, n, name_len);
	printf("C - nome letto: ");
	for(i = 0; i < name_len; i++)
	{
		printf("%c", n[i]);
	}
	printf(" ");

	unsigned char v[7];
	eeprom_read(mem_pointer + name_len, v, 6);
	v[6] = '\0';

	str = dj_mem_alloc(strlen(v) + 1, dj_vm_getSysLibClassRuntimeId(dj_exec_getVM(), BASE_CDEF_java_lang_String));

	if(str != NULL)
	{
		strcpy(str, v);
	}

	return str;
}




//short write_var(char name[], short s);
//short write_var(char name[], int i);
//uint8_t write_var(char name[], long l);
//short write_var(char name[], uint8_t b[]);
//uint8_t *read_var(char name[]);


/* Operation on file */
//short write_file(char name[], uint8_t b);
//uint8_t read_file(char name[]);



/*---------------------------------------------------------------------------*/

