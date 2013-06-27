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

#include "contiki.h"
#include "power-interface.h"

#include "dev/leds.h"
#include "platform-conf.h"


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
	dj_int_array * byteArray = REF_TO_VOIDP(dj_exec_stackPopRef());
	// create an object or allocate the chunk as is ?
	//heap_chunk * chk = (heap_chunk *)byteArray;
	PRINTF("array size is %d\n", byteArray->array.length);

	dj_object* ret = (dj_object*)dj_mem_alloc((byteArray->array.length - sizeof(heap_chunk)), byteArray->data.bytes[4]);
	//dj_object* ret2 = (dj_object*)dj_mem_alloc(10, byteArray->data.bytes[4]);

	heap_chunk  * chk = getChunk(ret);
	PRINTF("BEFORE W Created object id %d s %d\n", chk->id, chk->size);
	memcpy((char*)ret, (char*)(byteArray->data.bytes+sizeof(heap_chunk)), byteArray->array.length- sizeof(heap_chunk));

	chk = getChunk(ret);
	PRINTF("Created object id %d s %d\n", chk->id, chk->size);

	for(i = 0; i < byteArray->array.length; i++)
		PRINTF("%x-", byteArray->data.bytes[i]);
	PRINTF("\n");

	dj_exec_stackPushRef(VOIDP_TO_REF(ret));
	// crete a new object from array
}
#if 0
void javax_virtualsense_radio_Radio_void__waitForMessage()
{
		// wait for radio
		lock_RF(); // lock radio module to prevent rf shutdown
		lock_MAC();// lock mac layer to prevent duty cycle shutdown;
        receiver_thread = dj_exec_getCurrentThread();
        receiver_thread->status = THREADSTATUS_BLOCKED_FOR_IO;
        dj_exec_breakExecution();
}

// byte[] javax.virtualsense.radio.Radio._readBytes()
void javax_virtualsense_radio_Radio_byte____readBytes()
{
        dj_int_array * ret;
        int length = packetbuf_datalen();

        ret = dj_int_array_create(T_BYTE, length);

        // copy data from the rimebuf to the return array
#ifdef PLATFORM_HAS_RF
        packetbuf_copyto(ret->data.bytes);
#endif

        //packetbuf_clear();
        dj_exec_stackPushRef(VOIDP_TO_REF(ret));
}

// void javax.virtualsense.radio.Radio._init()
void javax_virtualsense_radio_Radio_void__init()
{
#ifdef PLATFORM_HAS_RF
	//uc = network_init();
	broadcast	= broadcast_network_init();
	unicast 	= unicast_network_init();
#endif
}

// byte javax.virtualsense.radio.Radio._getNumMessages()
/*void javax_virtualsense_radio_Radio_byte__getNumMessages()
{

}*/

// void javax.virtualsense.radio.Radio.setChannel(short)
/*void javax_virtualsense_radio_Radio_void_setChannel_short()
{
        int16_t channel = dj_exec_stackPopShort();
        // not implemented
}*/

// short javax.virtualsense.radio.Radio.getMaxMessageLength()
/*void javax_virtualsense_radio_Radio_short_getMaxMessageLength()
{

}*/

// void javax.virtualsense.radio.Radio._broadcast(byte[])
void javax_virtualsense_radio_Radio_void__broadcast_byte__()
{

#ifdef PLATFORM_HAS_RF
        rimeaddr_t addr;
#endif

        dj_int_array * byteArray = dj_exec_stackPopRef();

        // check null
        if (byteArray==nullref)
                dj_exec_createAndThrow(BASE_CDEF_java_lang_NullPointerException);
#ifdef PLATFORM_HAS_RF
        // copy bytes to the rime buffer
    packetbuf_copyfrom(byteArray->data.bytes, byteArray->array.length);
    packetbuf_set_datalen(byteArray->array.length);
    // abc
    lock_RF();
    broadcast_send(&broadcast);
    release_RF();
#endif
}


// boolean javax.virtualsense.radio.Radio._send(short, byte[])
void javax_virtualsense_radio_Radio_boolean__send_short_byte__()
{
#ifdef PLATFORM_HAS_RF
        rimeaddr_t addr;
#endif

        dj_int_array * byteArray = REF_TO_VOIDP(dj_exec_stackPopRef());
        int16_t id = dj_exec_stackPopShort();
        addr.u8[0] = id & 0xff;
        addr.u8[1] = id >> 8;


        // check null
        if (byteArray==nullref)
                dj_exec_createAndThrow(BASE_CDEF_java_lang_NullPointerException);
#ifdef PLATFORM_HAS_RF
        // copy bytes to the rime buffer
    packetbuf_copyfrom(byteArray->data.bytes, byteArray->array.length);
    packetbuf_set_datalen(byteArray->array.length);

    // abc
    lock_RF();
    unicast_send(&unicast, &addr);
    release_RF();
#endif
}
// short javax.virtualsense.radio.Radio.getDestId()
void javax_virtualsense_radio_Radio_short_getDestId(){
	// Destination should be broadcast or node id because otherwise
	// contiki network stack does not call the callback
	dj_exec_stackPushShort(packetbuf_attr(PACKETBUF_ADDR_RECEIVER));
}

void javax_virtualsense_radio_Radio_short_getSenderId(){
	dj_exec_stackPushShort(packetbuf_attr(PACKETBUF_ADDR_SENDER));
}
#endif
