/**
 * \file
 *         Tests related to clocks and timers
 * \author
 *         Zach Shelby <zach@sensinode.com>
 */

#include "contiki.h"
#include "sys/clock.h"
//#include "dev/bus.h"
#include "dev/leds.h"
#include <stdio.h> /* For printf() */
/*---------------------------------------------------------------------------*/
PROCESS(clock_test_process, "Freq test process");
AUTOSTART_PROCESSES(&clock_test_process);
/*---------------------------------------------------------------------------*/
unsigned long long task(void);
static volatile unsigned long long sum = 0;

PROCESS_THREAD(clock_test_process, ev, data)
{
  static struct etimer et;
  static clock_time_t count, start_count, end_count, diff;
  static unsigned long sec;
  static u8_t i;

  PROCESS_BEGIN();

  clock_slow_down(50);
  printf("Start Bench\n");
  i = 0;
  etimer_set(&et, CLOCK_SECOND*2);
  	 PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

	 setVCoreValue(VCORE_25MHZ);
	 setSystemClock(SYSCLK_25MHZ);
#ifdef PLATFORM_HAS_UART
	 uartInit(SYSCLK_25MHZ);
#endif
	 task();
	 etimer_set(&et, CLOCK_SECOND*2);
	 PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));


	 setVCoreValue(VCORE_20MHZ);
	 setSystemClock(SYSCLK_20MHZ);
#ifdef PLATFORM_HAS_UART
	 uartInit(SYSCLK_20MHZ);
#endif
	 task();
	 etimer_set(&et, CLOCK_SECOND*2);
	 PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

	 setVCoreValue(VCORE_16MHZ);
	 setSystemClock(SYSCLK_16MHZ);
#ifdef PLATFORM_HAS_UART
	 uartInit(SYSCLK_16MHZ);
#endif
	 task();
	 etimer_set(&et, CLOCK_SECOND*2);
	 PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

	 setVCoreValue(VCORE_12MHZ);
	 setSystemClock(SYSCLK_12MHZ);
#ifdef PLATFORM_HAS_UART
	 uartInit(SYSCLK_12MHZ);
#endif
	 task();
	 etimer_set(&et, CLOCK_SECOND*2);
	 PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

	 setVCoreValue(VCORE_8MHZ);
	 setSystemClock(SYSCLK_8MHZ);
#ifdef PLATFORM_HAS_UART
	 uartInit(SYSCLK_8MHZ);
#endif
	 task();
	 etimer_set(&et, CLOCK_SECOND*2);
	 PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

	setVCoreValue(VCORE_4MHZ);
	setSystemClock(SYSCLK_4MHZ);
#ifdef PLATFORM_HAS_UART
	 uartInit(SYSCLK_4MHZ);
#endif
	 task();
	 etimer_set(&et, CLOCK_SECOND*2);
	 PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

   setVCoreValue(VCORE_1MHZ);
   setSystemClock(SYSCLK_1MHZ);
#ifdef PLATFORM_HAS_UART
	 uartInit(SYSCLK_1MHZ);
#endif
	 task();
	 while(1){
		 etimer_set(&et, CLOCK_SECOND*2);
		 PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
	 }

  PROCESS_END();
}

unsigned long long task(){
	unsigned long long i = 0;
	for(i = 0; i < 30000;i++){
		sum*=i;

	}
	printf("sum %ld\n", sum);
	return sum;
}
/*---------------------------------------------------------------------------*/
