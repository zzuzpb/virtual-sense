
/*
 *      javax_radio_Radio.c
 *
 *      Copyright (c) 2008 CSIRO, Delft University of Technology.
 *
 *      This file is part of Darjeeling.
 *
 *      Darjeeling is free software: you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation, either version 3 of the License, or
 *      (at your option) any later version.
 *
 *      Darjeeling is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *
 *      You should have received a copy of the GNU General Public License
 *      along with Darjeeling.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <stdint.h>

#include "common/execution/execution.h"
#include "common/array.h"


#include "contiki.h"
#ifdef PLATFORM_HAS_RF
#include "net/rime.h"
#endif
#include "dev/leds.h"
#include "platform-conf.h"


#include "base_definitions.h"

static struct abc_conn uc;
static short receiveThreadId = -1;
static struct process *actual_process;
dj_thread * receiver_thread;

#ifdef PLATFORM_HAS_RF
static void recv_uc(struct abc_conn *c, rimeaddr_t *from)
{
		// wake-up thread that is blocked for I/O.
		// to select which thread is waiting for actual message we need port.
		if(receiver_thread != nullref){
			receiver_thread->status = THREADSTATUS_RUNNING;
			printf("Thread id %d wake-up for packet with timestamp %ld\n", receiver_thread->id,  receiver_thread->scheduleTime);
		}
        process_poll(actual_process);
}
#endif
#ifdef PLATFORM_HAS_RF
static const struct abc_callbacks abc_callbacks = {recv_uc};
#endif

void javax_virtualsense_radio_Radio_void__waitForMessage()
{
		//initialize the current process pointer
		actual_process = PROCESS_CURRENT();
		// wait for radio
        receiver_thread = dj_exec_getCurrentThread();
        receiveThreadId = receiver_thread->id;
        receiver_thread->status = THREADSTATUS_BLOCKED_FOR_IO;
        dj_exec_breakExecution();
}

// byte[] javax.virtualsense.radio.Radio._readBytes()
void javax_virtualsense_radio_Radio_byte____readBytes()
{
        dj_int_array * ret;
        int length = packetbuf_datalen(); //rimebuf_datalen() prima versione

        ret = dj_int_array_create(T_BYTE, length);

        // copy data from the rimebuf to the return array
#ifdef PLATFORM_HAS_RF
        packetbuf_copyto(ret->data.bytes);
#endif

        dj_exec_stackPushRef(VOIDP_TO_REF(ret));
}

// void javax.virtualsense.radio.Radio._init()
void javax_virtualsense_radio_Radio_void__init()
{
#ifdef PLATFORM_HAS_RF
        abc_open(&uc, 9345, &abc_callbacks);
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
    leds_on(LEDS_1);
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
    abc_send(&uc);
#endif

    leds_off(LEDS_1);
}


// boolean javax.virtualsense.radio.Radio._send(short, byte[])
void javax_virtualsense_radio_Radio_boolean__send_short_byte__()
{
    leds_on(LEDS_1);
#ifdef PLATFORM_HAS_RF
        rimeaddr_t addr;
#endif

        dj_int_array * byteArray = REF_TO_VOIDP(dj_exec_stackPopRef());
        int16_t id = dj_exec_stackPopShort();

        // check null
        if (byteArray==nullref)
                dj_exec_createAndThrow(BASE_CDEF_java_lang_NullPointerException);
#ifdef PLATFORM_HAS_RF
        // copy bytes to the rime buffer
    packetbuf_copyfrom(byteArray->data.bytes, byteArray->array.length);
    packetbuf_set_datalen(byteArray->array.length);

    // abc
    abc_send(&uc);
#endif

    leds_off(LEDS_1);
}
