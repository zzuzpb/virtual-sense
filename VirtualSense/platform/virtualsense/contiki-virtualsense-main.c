/*
 * Copyright (c) 2006, Swedish Institute of Computer Science
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * @(#)$Id: contiki-sky-main.c,v 1.87 2011/01/09 21:03:42 adamdunkels Exp $
 */


#include <stdio.h>
#include <string.h>
#include <msp430.h>
#include <legacymsp430.h>

#include "contiki.h"
#include "contiki-conf.h"
#include "platform-conf.h"

#include "radio_driver.h"
#include "power-interface.h"

#include "dev/cc2520ll.h"
#ifdef PLATFORM_HAS_EUI48
#include "dev/EUI-48_24AA025E48.h"
#endif
#include "dev/eeprom.h"
#ifdef PLATFORM_HAS_RTC_PCF2123
#include "dev/pcf2123_spi.h"
#endif
#include "dev/barometer_MPL115A2.h"
#include "dev/adc.h"
#include "dev/leds.h"
#include "dev/serial-line.h"
#include "uart.h"
#include "dev/watchdog.h"
#include "lib/random.h"
#include "net/netstack.h"
#include "net/mac/frame802154.h"



#include "net/rime.h"

#include "node-id.h"
#include "sys/autostart.h"
#include "sys/profile.h"

#if DCOSYNCH_CONF_ENABLED
static struct timer mgt_timer;
#endif
extern int msp430_dco_required;
struct process * processes;



void init_platform(void);

/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
void uip_log(char *msg) { puts(msg); }
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
static void
set_rime_addr(void)
{
  rimeaddr_t addr;
  int i;

  memset(&addr, 0, sizeof(rimeaddr_t));
#ifdef PLATFORM_HAS_EUI48
  if(node_id == 0) {
    for(i = 0; i < sizeof(rimeaddr_t); ++i) {
      addr.u8[i] = EUI_48[7 - i];
      printf("Setting addr %x-", EUI_48[7-i]);
      //node_id = addr.u8[0] + addr.u8[1]>>8;
    }
  } else {
    addr.u8[0] = node_id & 0xff;
    addr.u8[1] = node_id >> 8;
  }
#else
  if(node_id == 0) {
     for(i = 0; i < sizeof(rimeaddr_t); ++i) {
       addr.u8[i] = i+0xa;
     }
   } else {
	 addr.u8[0] = node_id & 0xff;
	 addr.u8[1] = node_id >> 8;
   }
#endif
  rimeaddr_set_node_addr(&addr);
  printf("Rime address ");
  for(i = 0; i < sizeof(addr.u8) - 1; i++) {
    printf("%d.", addr.u8[i]);
  }
  printf("%d\n", addr.u8[i]);
}
/*---------------------------------------------------------------------------*/
#if 0
static void
print_processes(struct process * const processes[])
{
  /*  const struct process * const * p = processes;*/
  //printf("Starting");
  while(*processes != NULL) {
    printf(" '%s'", (*processes)->name);
    processes++;
  }
  printf("\n");
}
#endif
/*--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
int
main(int argc, char **argv)
{
  /*
   * Initalize hardware.
   */
  watchdog_init();
  init_ports();


  setVCoreValue(VCORE_16MHZ);
  setSystemClock(SYSCLK_16MHZ);
  uartInit(SYSCLK_16MHZ);
  clock_init();

  //clock_slow_down(50);

  leds_init();
  leds_off(LEDS_ALL);
  rtimer_init();
  eeprom_init();
  EUI_init();
  adc_init();
  barometer_MPL115A2_init();


                    // Initialize I2C module




  /*
   * Hardware initialization done!
   */



/* Restore node id if such has been stored in external mem */
//while(node_id == 0)
	node_id_restore();
//node_id = 2;



/* if wakeup from hibernation do not init (i.e. reset) the RTC */
if(SYSRSTIV == SYSRSTIV_LPM5WU){
	  /* interrupt service routine for button on P2.0 and external RTC (pcf2123) on P2.2*/
#ifdef  PLATFORM_HAS_RTC_PCF2123
	  	RTC_clear_interrupt();
	  	RTC_disable_all_interrupts();
#endif
	  	P2IFG &= ~(BIT4);                          // P2.0 and P2.2 IFG cleared
	  	/* if system was wake-up by RTC interrupt we need to
	  	 * clear interrupt flag and disable all interrupt on the RTC in order to reduce power
	  	 * consumption
	  	 */
}else {
#ifdef PLATFORM_HAS_RTC_PCF2123
	  RTC_init();
#endif
}



#ifdef PLATFORM_HAS_DS2411
  random_init(ds2411_id[0] + node_id);
#endif


  /*
   * Initialize Contiki and our processes.
   */
  process_init();
  process_start(&etimer_process, NULL);
  ctimer_init();
  init_platform();
  set_rime_addr();

  uart_set_input(serial_line_input_byte);
  serial_line_init();

  printf(CONTIKI_VERSION_STRING "up. ");

#ifdef PLATFORM_HAS_RF
  NETSTACK_RADIO.init();
  NETSTACK_RDC.init();
  {
       uint8_t longaddr[8];
       uint16_t shortaddr;

       shortaddr = (rimeaddr_node_addr.u8[0] << 8) +
         rimeaddr_node_addr.u8[1];
       memset(longaddr, 0, sizeof(longaddr));
       rimeaddr_copy((rimeaddr_t *)&longaddr, &rimeaddr_node_addr);
       /*printf("MAC %02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x ",
              longaddr[0], longaddr[1], longaddr[2], longaddr[3],
              longaddr[4], longaddr[5], longaddr[6], longaddr[7]);*/
       cc2520ll_setPanId(0xABCD);
       //node_id = shortaddr;
     }
    cc2520ll_setChannel(RF_CHANNEL);
    NETSTACK_MAC.init();
    NETSTACK_NETWORK.init();

 /*printf("%s %s, channel check rate %lu Hz, channel %u\n",
         NETSTACK_MAC.name, NETSTACK_RDC.name,
         CLOCK_SECOND / (NETSTACK_RDC.channel_check_interval() == 0? 1:
                         NETSTACK_RDC.channel_check_interval()),
         RF_CHANNEL);*/
 	/*NETSTACK_MAC.off(0);
    NETSTACK_RDC.off(0);
    NETSTACK_RADIO.off();*/
#endif

#ifdef PLATFORM_HAS_EUI48
     printf("MAC %02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x\n",
	 EUI_48[7], EUI_48[6], EUI_48[5], EUI_48[4],
	 EUI_48[3], EUI_48[2], EUI_48[1], EUI_48[0]);
#endif

    if(node_id > 0) {
       printf("Node id  %u.\n", node_id);
     } else {
       printf("Node id is not set.\n");
     }


  watchdog_start(); //RESET THE DEVICE IF watchdog_periodic(); is not called in one second.
  // TODO: hibernation compatibility ?
  autostart_start(autostart_processes);
  /*
   * This is the scheduler loop.
   */

  /*watchdog_stop(); */


  while(1) {
    int r;
    clock_time_t t0 = RTIMER_NOW();//clock_time();
    do {
      /* Reset watchdog. */
     watchdog_periodic();
      r = process_run();
    } while(r > 0);


    /*
     * Idle processing.
     */
    int s = splhigh();		/* Disable interrupts. */
    if(process_nevents() != 0 || uart_active()) {
      splx(s);			/* Re-enable interrupts. */
    } else {
		  watchdog_stop();

		  /*if(!is_locked_RF())
			  shutdown_RF();*/
		  /*else
			  printf("RF Locked\n");*/

		  /*if(!is_locked_SPI())
			  shutdown_SPI();*/
		 /* else
			  printf("SPI Locked\n");*/
		 /* if(!is_locked_MAC())
			  shutdown_MAC();*/

		 /* else
			  printf("MAC Locked\n");*/


		    PMMCTL0_H = PMMPW_H; // PMM Password
		    SVSMHCTL &= ~(SVMHE+SVSHE); // Disable High side SVS
		    SVSMLCTL &= ~(SVMLE+SVSLE); // Disable Low side SVS
		    PMMCTL0_H = 0x00;                         // close PMM


		    /*P8OUT = 0x00;
		    P8DIR = 0x00; */
		    //LELE: clear request clock to allow LPM4 entering FOR DEBUG HERE.
		   /* UCSCTL8 &= ~(ACLKREQEN | MCLKREQEN | SMCLKREQEN | MODOSCREQEN);
		    UCSCTL6 |= (XT1OFF | XT2OFF);*/
		    //UCSCTL0 = 0x0000;
		    __bis_SR_register(LPM3_bits+GIE);             // Enter LPM3

		   /*_BIS_SR( GIE | SCG0 | SCG1 | CPUOFF );*/ /* LPM3 sleep. This

							statement will block
							until the CPU is
							woken up by an
							interrupt that sets
							the wake up flag. */

		  /* We get the current processing time for interrupts that was
		     done during the LPM and store it for next time around.  */
		/* printf("WAKE: %ld\n", clock_time());

		  processes = PROCESS_LIST();

		  while(processes != NULL) {
		      printf(" '%s'", processes->name);
		      processes = processes->next;
		    }
		    printf("\n");
		    */

		  /*printf(" -- Pending %d ", etimer_pending());
		  printf(" -- Next Ex %d\n", etimer_next_expiration_time());
		  */

#ifdef PLATFORM_HAS_RTC_PCF2123
		  //printf(" TIME %u:%u:%u\n", RTC_get_hours(),RTC_get_minutes(),RTC_get_seconds()) ;
#endif

		 // test reading temperature from barometer
		 //printf("temp %d\n",read_temperature_barometer_MPL115A2());
		 //printf("pressure %d\n",read_pressure_barometer_MPL115A2());
		 watchdog_start();
    }
  }

  return 0;
}
