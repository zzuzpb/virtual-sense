/*
 *	javax_virtualsense_storage_Storage.c
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
 * Storage access methods.
 *
 * @author Matteo Dromedari
 *
 */

#include <stdint.h>
#include <string.h>
#include <stdio.h>

// generated at infusion time
#include "base_definitions.h"

#include "common/execution/execution.h"
#include "common/array.h"
#include "common/darjeeling.h"
#include "common/heap/heap.h"
#include "storage.h"


//#include "common/djtimer.h"


// ---------------------------------------- WRITE -----------------------------------------

//private static native short _writeVar(String name, int nameLen, short s)
void javax_virtualsense_storage_Storage_short__writeVar_java_lang_String_int_short()
{
	int16_t s = dj_exec_stackPopShort();
	unsigned char *s_p = &s;
	int32_t name_len = dj_exec_stackPopInt();
	unsigned char *name = REF_TO_VOIDP(dj_exec_stackPopRef());

	dj_exec_stackPushShort(write_var(name, name_len, s_p, sizeof(int16_t)));
}


// public static native short _writeVar(String name, int nameLen, int var)
void javax_virtualsense_storage_Storage_short__writeVar_java_lang_String_int_int()
{
	int32_t var = dj_exec_stackPopInt();
	unsigned char *var_p = &var;
	int32_t name_len = dj_exec_stackPopInt();
	unsigned char *name = REF_TO_VOIDP(dj_exec_stackPopRef());
	
	dj_exec_stackPushShort(write_var(name, name_len, var_p, sizeof(int32_t)));
}


//private static native short _writeVar(String name, int nameLen, String str, int strLen)
void javax_virtualsense_storage_Storage_short__writeVar_java_lang_String_int_java_lang_String_int()
{
	int32_t str_len = dj_exec_stackPopInt();
	int32_t name_len = dj_exec_stackPopInt();
	unsigned char *str = REF_TO_VOIDP(dj_exec_stackPopRef());
	unsigned char *name = REF_TO_VOIDP(dj_exec_stackPopRef());

	dj_exec_stackPushShort(write_var(name, name_len, str, str_len));
}



// ---------------------------------------- READ -----------------------------------------

//private static native short _readShort(String name, int nameLen)
void javax_virtualsense_storage_Storage_short__readShort_java_lang_String_int()
{
	unsigned char *name = REF_TO_VOIDP(dj_exec_stackPopRef());
	int32_t name_len = dj_exec_stackPopInt();

	dj_exec_stackPushShort(read_short(name, name_len));
}


// public static native int _readInt(String name, String nameLen)
void javax_virtualsense_storage_Storage_int__readInt_java_lang_String_int()
{
	unsigned char *name = REF_TO_VOIDP(dj_exec_stackPopRef());
	int32_t name_len = dj_exec_stackPopInt();

	dj_exec_stackPushInt(read_int(name, name_len));
}


//private static native String _readString(String name, int nameLen)
void javax_virtualsense_storage_Storage_java_lang_String__readString_java_lang_String_int()
{
	unsigned char *name = REF_TO_VOIDP(dj_exec_stackPopRef());
	int32_t name_len = dj_exec_stackPopInt();
	char *ret;	
	
	ret = read_String(name, name_len);
	
	if(ret == NULL)
	{
	    	dj_exec_createAndThrow(BASE_CDEF_java_lang_OutOfMemoryError);
	    	return;
	}

	dj_exec_stackPushRef(VOIDP_TO_REF(ret));
}



/*
// short javax.virtualsense.storage.Storage.writeFile(String name, byte b)
void javax_virtualsense_storage_Storage_short_writeFile_String_byte()
{
	uint8_t b = dj_exec_stackPopShort();
	char *name =  REF_TO_VOIDP(dj_exec_stackPopRef());
	short ret = write_file(name, b);
	dj_exec_stackPushShort(ret);
}

// byte javax.virtualsense.storage.Storage.readFile(String name)
void javax_virtualsense_storage_Storage_byte_readFile_String()
{
	char *name =  REF_TO_VOIDP(dj_exec_stackPopRef());
	uint8_t b = read_file(name);
	dj_exec_stackPushShort(b);
}*/
