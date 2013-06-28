/*
 *	javax_virtualsense_network_Packet.c
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
 * Packet serialization native methods.
 *
 * @author Emanuele Lattanzi
 *
 */
#include <stdint.h>

#include "common/execution/execution.h"
#include "common/types.h"
#include "common/array.h"
#include "common/darjeeling.h"
#include "common/heap/heap.h"

#include "base_definitions.h"

#define DEBUG 0
#if DEBUG
#include <stdio.h>
#define PRINTF(...) printf(__VA_ARGS__)
#define PRINTDEBUG(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#define PRINTDEBUG(...)
#endif



dj_thread * receiver_thread;
//static struct abc_conn uc;
static struct broadcast_conn broadcast;
static struct unicast_conn unicast;

static inline heap_chunk* getChunk(void *ptr)
{
	return (heap_chunk*)((size_t)ptr-sizeof(heap_chunk));
}

inline dj_global_id dj_vm_resolveRuntimeClass(dj_vm *vm, runtime_id_t id)
{
	dj_global_id ret;
	dj_infusion *infusion = vm->infusions;
	runtime_id_t base = 0;

	// TODO: optimize this! (binary search?)
	// TODO: test for multiple loaded infusions
	while (infusion!=NULL)
	{
		base = infusion->class_base;
		PRINTF("Comparing with infusion %d having base id %d\n",dj_vm_getInfusionId(dj_exec_getVM(), infusion), infusion->class_base);
		if ((id>=base)&&(id<base + dj_di_parentElement_getListSize(infusion->classList)))
		{
			ret.infusion = infusion;
			ret.entity_id = id - base;

			return ret;
		}
		infusion = infusion->next;
	}

	// TODO raise error, class not found
	printf("error: class not found: %d\n", id);
    // dead code to make compiler happy
    ret.entity_id=255;
    ret.infusion=NULL;
    return ret;
}

javax_virtualsense_network_Packet_byte___toByteArray(){
	 uint8_t i = 0;
	 dj_object *obj = REF_TO_VOIDP(dj_exec_stackPopRef());
	 heap_chunk * chk = getChunk(obj);
	 dj_int_array * ret;
	 uint16_t length = chk->size;
	 PRINTF("obj id is %d and array size is %d \n", chk->id, chk->size);
	 ret = dj_int_array_create(T_BYTE, length);
	 // data.bytes conatains | chunk struct | chk->size -sizeof(heap_chunk) bytes |
	 memcpy(ret->data.bytes, (char*)chk, length);
	 for(i = 0; i < ret->array.length; i++)
		 PRINTF("%x-", ret->data.bytes[i]);
	 PRINTF("\n");
	 dj_exec_stackPushRef(VOIDP_TO_REF(ret));

}
void javax_virtualsense_network_Packet_javax_virtualsense_network_Packet_createPacket_byte__(){
	uint8_t i = 0;
	dj_object* ret = NULL;
	dj_int_array * byteArray = REF_TO_VOIDP(dj_exec_stackPopRef());
	// create an object or allocate the chunk as is ?
	//heap_chunk * chk = (heap_chunk *)byteArray;
	PRINTF("array size is %d\n", byteArray->array.length);


	// resolve runtime class
	dj_global_id refClass = dj_vm_resolveRuntimeClass(dj_exec_getVM(), byteArray->data.bytes[4]);
	if(refClass.infusion != NULL){
		ret = (dj_object*)dj_mem_alloc((byteArray->array.length - sizeof(heap_chunk)), byteArray->data.bytes[4]);

		heap_chunk  * chk = getChunk(ret);
		PRINTF("BEFORE W Created object id %d s %d\n", chk->id, chk->size);
		memcpy((char*)ret, (char*)(byteArray->data.bytes+sizeof(heap_chunk)), byteArray->array.length- sizeof(heap_chunk));

		chk = getChunk(ret);
		PRINTF("Created object id %d s %d\n", chk->id, chk->size);

		for(i = 0; i < byteArray->array.length; i++)
			PRINTF("%x-", byteArray->data.bytes[i]);
		PRINTF("\n");
	}else {
		PRINTF("Packet not created\n");
	}
	dj_exec_stackPushRef(VOIDP_TO_REF(ret));
	// crete a new object from array
}
