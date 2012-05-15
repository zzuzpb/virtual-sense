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
static void on(void);
static void off(void);
//static void isolateMCU(void);

/* The driver state */
static radio_driver_state_t radio_state = OFF;
static uint8_t receive_on;
static uint8_t locked, lock_on, lock_off;




/*---------------------------------------------------------------------------*/
#define GET_LOCK() locked++
static void RELEASE_LOCK(void) {
  if(locked == 1) {
    if(lock_on) {
    	//printf("Was lock_on\n");
      on();
      lock_on = 0;
    }
    if(lock_off) {
      off();
      lock_off = 0;
    }
  }
  locked--;
}


/*---------------------------------------------------------------------------*/
static int
init()
{
	    if (cc2520ll_init(0) == FAILED)
                return 0;
        else{
        		on();
                return 1;
        }
}

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
	int res = 0;
	GET_LOCK();
	cc2520ll_prepare(payload, payload_len);
	RELEASE_LOCK();
	GET_LOCK();
	res =  cc2520ll_transmit();
	RELEASE_LOCK();

	return res;
}

static int
read(void *buf, unsigned short buf_len)
{
	 int res = 0;
	 GET_LOCK();
	 res =  cc2520ll_packetReceive(buf, buf_len) - 2;
	 RELEASE_LOCK();
	 return res;
}

static int
pending_packet()
{
	return cc2520ll_pending_packet();
}

static void
on()
{
	cc2520ll_receiveOn();
	receive_on = 1;
}

static void
off()
{
	cc2520ll_receiveOff();
	receive_on = 0;
}


/*---------------------------------------------------------------------------*/
int
cc2520_off(void)
{
  /* Don't do anything if we are already turned off. */
  if(receive_on == 0) {
	  //printf("DONT CALL OFF WAS OFF\n");
    return 1;
  }

  /* If we are called when the driver is locked, we indicate that the
     radio should be turned off when the lock is unlocked. */
  if(locked) {
    /*    printf("Off when locked (%d)\n", locked);*/
    lock_off = 1;
    //printf("DONT CALL OFF WAS LOCKED SCHEDULE OFF\n");
    return 1;
  }

  GET_LOCK();
  /* If we are currently receiving a packet (indicated by SFD == 1),
     we don't actually switch the radio off now, but signal that the
     driver should switch off the radio once the packet has been
     received and processed, by setting the 'lock_off' variable. */
  if(cc2520ll_rxtx_packet()) {
	  //printf("DONT CALL OFF WAS LOCKED SCHEDULE OFF BUSY\n");
    lock_off = 1;
  } else {
	  //printf("CALL OFF WAS ON\n");
    off();
  }
  RELEASE_LOCK();
  return 1;
}
/*---------------------------------------------------------------------------*/
int
cc2520_on(void)
{
  if(receive_on) {
	  //printf("DONT CALL ON WAS ON\n");
    return 1;
  }
  if(locked) {
	  //printf("DONT CALL ON WAS LOCKED\n");
    lock_on = 1;
    return 1;
  }

  GET_LOCK();
  //printf("CALL ON WAS OFF\n");
  on();
  RELEASE_LOCK();
  return 1;
}
/*---------------------------------------------------------------------------*/


int
transmit(unsigned short transmit_len)
{
	cc2520_on();
	//printf("start transmit: %u \n", RTIMER_NOW());
	u16_t res = 0;
	GET_LOCK();
	//printf("Lock acquired\n");
	//printf("start transmit2: %u \n", RTIMER_NOW());
	res = cc2520ll_transmit();
	//printf("end transmit2: %u \n", RTIMER_NOW());
	RELEASE_LOCK();
	//cc2520_off();
	return res;
}

int
channel_clear(void)
{
	  int cca;
	  int radio_was_off = 0;
	  /* If the radio is locked by an underlying thread (because we are
	     being invoked through an interrupt), we preted that the coast is
	     clear (i.e., no packet is currently being transmitted by a
	     neighbor). */
	  if(locked) {
	    return 1;
	  }

	  GET_LOCK();
	  if(!receive_on) {
	    radio_was_off = 1;
	    cc2520_on();
	  }
	  /* Make sure that the radio really got turned on. */
	  if(!receive_on) {
	    RELEASE_LOCK();
	    if(radio_was_off) {
	      cc2520_off();
	    }
	    return 1;
	  }
	  cca = cc2520ll_channel_clear();
	  if(radio_was_off) {
	    cc2520_off();
	  }
	  RELEASE_LOCK();
	  return cca;
}
int
receiving_packet(void)
{
	//u16_t v = cc2520ll_rxtx_packet();
	//printf("Invoke receiving_packet \n");
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
        cc2520_on,
        cc2520_off
  };

/*---------------------------------------------------------------------------*/
