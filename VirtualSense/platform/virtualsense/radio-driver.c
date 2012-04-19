/*
 * This file implements the radio driver needed for the 6LP-GW
 */
#include "radio_driver.h"
/*
 * We include the "contiki-net.h" file to get all the network functions.
 */
#include "contiki-net.h"
/*
 * And "cc2520ll.h" file to get all the lower level driver functions.
 */
#include "dev/cc2520ll.h"

/*
 * And "pgw_fwd.h", which holds the interface data structures.
 */
//#include "net/p-gw/pgw_fwd.h"

static int init(void);
static int send(const void *payload, unsigned short payload_len);
static int read(void *buf, unsigned short buf_len);
static int pending_packet(void);
static int on(void);
static int off(void);

/* The driver state */
static radio_driver_state_t radio_state = OFF;



#if 0

/*---------------------------------------------------------------------------*/
/*
 * This is the poll handler function in the process below. This poll handler
 * function checks for incoming packets and forwards them to the right
 * interface or delivers them to the TCP/IP stack.
 */
static void
pollhandler(void)
{
	printf("Invoke pollhandler\n");
        if (cc2520ll_pending_packet()) {

                //LELE //incoming_if = IEEE_802_15_4;

                packetbuf_clear();
    packetbuf_set_datalen(read(packetbuf_dataptr(), PACKETBUF_SIZE));
                /*
           * Forward the packet to the upper level in the stack
           */
        //LELE //NESTACK_MAC_RADIO.input();
        }
  /*
   * Now we'll make sure that the poll handler is executed repeatedly.
   * We do this by calling process_poll() with this process as its
   * argument.
   */
  process_poll(&radio_driver_process);
}
/*---------------------------------------------------------------------------*/
/*
 * Finally, we define the process that does the work.
 */
PROCESS_THREAD(radio_driver_process, ev, data)
{
        /*
         * This process has a poll handler, so we declare it here. Note that
         * the PROCESS_POLLHANDLER() macro must come before the PROCESS_BEGIN()
   * macro.
   */
        PROCESS_POLLHANDLER(pollhandler());

        /*
         * This process has an exit handler, so we declare it here. Note that
   * the PROCESS_EXITHANDLER() macro must come before the PROCESS_BEGIN()
   * macro.
   */
        PROCESS_EXITHANDLER(_NOP());

  /*
   * The process begins here.
   */
  PROCESS_BEGIN();

  /*
   * Now we'll make sure that the poll handler is executed initially. We do
   * this by calling process_poll() with this process as its argument.
   */
  process_poll(&radio_driver_process);

        /*
   * And we wait for the process to exit.
   */
        PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_EXIT);

  /*
   * Here ends the process.
   */
  PROCESS_END();
}
#endif

/*---------------------------------------------------------------------------*/
static int
init()
{
	//printf("Invoke init\n");
        if (cc2520ll_init() == FAILED) {
                return 0;
        } else {
                on();
                return 1;
        }
}

/* These functions are not used; They are defined only for compliance with
 * struct radio_driver defined in radio.h*/
int
prepare (const void *payload, unsigned short payload_len)
{
	//printf("Invoke prepare\n");
	return cc2520ll_prepare(payload, payload_len);
}

/*---------------------------------------------------------------------------*/
static int
send(const void *payload, unsigned short payload_len)
{
	//prepare (payload, payload_len);
	//printf("Invoke send\n");
        if (radio_state == ON) {
        	  cc2520ll_prepare(payload, payload_len);
        	  return cc2520ll_transmit();
                //return cc2520ll_packetSend(payload, payload_len);
        }
        return 0;
}

static int
read(void *buf, unsigned short buf_len)
{
	//printf("Invoke read\n");
        if (radio_state == ON) {
                /* substract CRC length */
                return cc2520ll_packetReceive(buf, buf_len) - 2;
        } else {
                return 0;
        }
}

static int
pending_packet()
{
	//printf("Invoke pending_packet\n");
        if (radio_state == ON) {
                return cc2520ll_pending_packet();
        } else {
                return 0;
        }
}

static int
on()
{
	//printf("Invoke on \n");
	if(radio_state == ON)
		return 1;
	//TODO: implements lock ?
	cc2520ll_receiveOn();
    radio_state = ON;
    return 1;
}

static int
off()
{
	if(radio_state == OFF)
		return 1;

	//TODO: implements lock ?
	//printf("Invoke off \n");
	//cc2520ll_receiveOff(); // NON necessario perchè automaticamente passa a off se la trasmissione
							 // viene iniziata con STXONCCA e torna a on alla fine della trasmissione.
    radio_state = OFF;
    return 1;
}



int
transmit(unsigned short transmit_len)
{
	//printf("Invoke transmit\n");
	printf("start transmit: %d \n", clock_time());
	u16_t res = 0;
	if(radio_state == OFF)
		on();
	res = cc2520ll_transmit();
	printf("end transmit: %d \n", clock_time());
	return res;
}

int
channel_clear(void)
{
	//printf("Invoke channel_clear\n");
	return cc2520ll_channel_clear();}
int
receiving_packet(void)
{
	u16_t v = cc2520ll_rxtx_packet();
	//printf("Invoke receiving_packet v=%x\n", v);
	return cc2520ll_rxtx_packet();
}

const struct radio_driver cc2520_driver =
  {
        init,
        prepare,
        transmit,
    send,
                read,
                channel_clear,
                receiving_packet,
                pending_packet,
                on,
                off
  };

/*---------------------------------------------------------------------------*/
