/*
 *	javax_virtualsense_powermanagement_PowerManager.c
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
 * Controls power manager of the board
 * based on modified contiki operating system.
 *
 * @author Emanuele Lattanzi
 *
 */

#include "dev/board.h"

// generated at infusion time
#include "base_definitions.h"
#include "virtualsense_definitions.h"

#include "common/execution/execution.h"
#include "common/heap/heap.h"
#include "common/djtimer.h"
#include "common/debug.h"
#include "common/system_hibernation.h"

#include "sys/clock.h"
#include "dev/watchdog.h"
#include "dev/leds.h"
#ifdef PLATFORM_HAS_RTC_PCF2123
#include "dev/pcf2123_spi.h"
#endif

//void javax.virtualsense.powermanagement.PowerManager.slowDownClockByFactor(short)
void javax_virtualsense_powermanagement_PowerManager_void_slowDownClockByFactor_int()
{
	uint32_t factor = dj_exec_stackPopInt();
	// TODO: to implement dj_timer_slowDownClockByFactor((uint16_t)factor);
}

//void javax.virtualsense.powermanagement.PowerManager.setSystemClockMillis(int)
void javax_virtualsense_powermanagement_PowerManager_void_setSystemClockMillis_int()
{
	uint32_t millis = dj_exec_stackPopInt();
	dj_timer_setSystemClockMillis(millis);
}

//void javax.virtualsense.powermanagement.PowerManager.setMCUFreqency(short)
void javax_virtualsense_powermanagement_PowerManager_void_setMCUFrequency_short()
{
	unsigned char cpu_speed = (unsigned char)dj_exec_stackPopShort();
#if 0 //TODO: implement
#ifdef PLATFORM_HAS_UART
	uartShutDown();
#endif
	switch (cpu_speed)
	 {
	 case 1:
		 setVCoreValue(VCORE_1MHZ);
		 setSystemClock(SYSCLK_1MHZ);
#ifdef PLATFORM_HAS_UART
		 uartInit(SYSCLK_1MHZ);
#endif
	    break;
	 case 4:
		 setVCoreValue(VCORE_4MHZ);
		 setSystemClock(SYSCLK_4MHZ);
#ifdef PLATFORM_HAS_UART
		 uartInit(SYSCLK_4MHZ);
#endif
	   break;
	 case 8:
		 setVCoreValue(VCORE_8MHZ);
		 setSystemClock(SYSCLK_8MHZ);
#ifdef PLATFORM_HAS_UART
		 uartInit(SYSCLK_8MHZ);
#endif
	   break;
	 case 12:
		 setVCoreValue(VCORE_12MHZ);
		 setSystemClock(SYSCLK_12MHZ);
#ifdef PLATFORM_HAS_UART
		 uartInit(SYSCLK_12MHZ);
#endif
	   break;
	 case 16:
		 setVCoreValue(VCORE_16MHZ);
		 setSystemClock(SYSCLK_16MHZ);
#ifdef PLATFORM_HAS_UART
		 uartInit(SYSCLK_16MHZ);
#endif
	   break;
	 case 20:
		 setVCoreValue(VCORE_20MHZ);
		 setSystemClock(SYSCLK_20MHZ);
#ifdef PLATFORM_HAS_UART
		 uartInit(SYSCLK_20MHZ);
#endif
	   break;
	 case 25:
		 setVCoreValue(VCORE_25MHZ);
		 setSystemClock(SYSCLK_25MHZ);
#ifdef PLATFORM_HAS_UART
		 uartInit(SYSCLK_25MHZ);
#endif
		 break;
	 //default:
	 		//printf("Default %d\n", cpu_speed);
	 }
#endif

}

//void javax.virtualsense.powermanagement.PowerManager.setSystemHibernation()
void javax_virtualsense_powermanagement_PowerManager_void_systemHibernation()
{
	dj_vm *vm;
	dj_thread *thread;
	dj_thread *currentThread;
	uint8_t saved = 0;
	int i = 0;

#if 0 //TODO: implement

	dj_mem_gc();
	vm = dj_exec_getVM();
	currentThread = vm->currentThread;

	/*DEBUG_LOG("VM pointer %p\n", vm);
	DEBUG_LOG("heap left pointer %p\n", (dj_mem_get_base_pointer()+dj_mem_get_left_pointer()));
	DEBUG_LOG("heap right pointer %p\n", (dj_mem_get_base_pointer()+dj_mem_get_right_pointer()));
	DEBUG_LOG("current thread pointer %p\n", currentThread); */

	if(currentThread!=NULL){
		/*DEBUG_LOG("Preparing hibernation triggered by thread %d\n", currentThread->id);
		DEBUG_LOG("PC before hibernation %d\n", currentThread->frameStack->pc); */
		dj_exec_deactivateThread(currentThread);
	}

	/* hibernate all threads */
	thread = vm->threads;
	while (thread!=NULL){
		thread->hibernated = 1;
		thread=thread->next;
	}
	//heap_dump(dj_mem_get_base_pointer());
	printf("Saving machine state on non-volatile memory\n");
	saved = save_heap(dj_mem_get_base_pointer(),
					  dj_mem_get_left_pointer(),
					  dj_mem_get_right_pointer(),
					  dj_mem_get_panic_exception_object_pointer(),
					  dj_mem_get_ref_stack());
	if(saved){
		printf("Hibernation done....\n");
		watchdog_stop();
		/* reset UART */
#ifdef PLATFORM_HAS_UART
		uartShutDown();
#endif

		/* enable interrupt on port P2.0 (button) and 2.2 (RTC) */
		enable_wakeup_from_interrupt();

		/* close I/O port to prevent current drain
		 *
		 */
		prepare_for_LPM4_5();
		/* shut-down timer A and B and
		 * enter LPM4.5
		 */
		enter_LPM4_5();
		/* LPM4.5 shut-down mode. When the cpu
		* will be woken up by an interrupt over P1 or P2
		* it will receive a brownout reset (BOR) event and
		* the MCU will reboot. So the execution will
		* not restart from this point
		*/

		leds_on(LEDS_ALL); // this statement should never be executed
		while(1){ // trap for debugging in order to block execution here in case of
				  // failure on entering LPM4.5
		}
	}else {
		dj_exec_createAndThrow(VIRTUALSENSE_CDEF_javax_virtualsense_powermanagement_HibernationException);
	}
#endif
}

//void javax.virtualsense.powermanagement.PowerManager.standby()
void javax_virtualsense_powermanagement_PowerManager_void_standby()
{
#if 0 //TODO to Implement
	 watchdog_stop();
	 standby(); /* invoking the clock function in order to exit from LPM3
	 	 	 	 * at next timer interrupt */
	 PMMCTL0_H = PMMPW_H; // PMM Password
			    SVSMHCTL &= ~(SVMHE+SVSHE); // Disable High side SVS
			    SVSMLCTL &= ~(SVMLE+SVSLE); // Disable Low side SVS
			    PMMCTL0_H = 0x00;                         // close PMM

	_BIS_SR(GIE | SCG0 | SCG1 | CPUOFF);		  /*LPM3 sleep. This
													statement will block
												 	until the CPU is
												 	woken up by an
												 	interrupt that sets
												 	the wake up flag.
												 	LPM3 has ram retention
												 	the execution will
												 	restart from this point.
												 	*/
	 watchdog_start();
#endif
}


//void javax.virtualsense.powermanagement.PowerManager.deepSleep()
void javax_virtualsense_powermanagement_PowerManager_void_deepSleep()
{
#if 0 //TODO to implement
	watchdog_stop();
	/* enable interrupt on port P2.0 (button) and 2.2 (RTC) */
	enable_wakeup_from_interrupt();
	PMMCTL0_H = PMMPW_H; // PMM Password
	SVSMHCTL &= ~(SVMHE+SVSHE); // Disable High side SVS
	SVSMLCTL &= ~(SVMLE+SVSLE); // Disable Low side SVS
	PMMCTL0_H = 0x00;                         // close PMM
#ifdef PLATFORM_HAS_UART
		uartShutDown();
#endif

	//LELE: clear request clock to allow LPM4 entering FOR DEBUG HERE.
    UCSCTL8 &= ~(ACLKREQEN | MCLKREQEN | SMCLKREQEN | MODOSCREQEN);
    //UCSCTL6 |= (XT1OFF | XT2OFF);

	_BIS_SR(GIE | SCG0 | SCG1 | CPUOFF | OSCOFF); /*LPM4 sleep. This
													statement will block
												 	until the CPU is
												 	woken up by an
												 	interrupt that sets
												 	the wake up flag.
												 	LPM4 has ram retention
												 	the execution will
												 	restart from this point.
												 	*/
	watchdog_start();
#ifdef PLATFORM_HAS_UART
    uartInit(SYSCLK_16MHZ);
#endif

#ifdef PLATFORM_HAS_RTC_PCF2123
		  printf(" TIME %u:%u:%u\n", RTC_get_hours(),RTC_get_minutes(),RTC_get_seconds()) ;
		  RTC_clear_interrupt();
		  RTC_disable_all_interrupts();
		  P2IFG &= ~(BIT2);
#endif

		  P2IFG &= ~(BIT0);
	  	                         // P2.0 and P2.2 IFG cleared
#endif
}

void javax_virtualsense_powermanagement_PowerManager_void_scheduleRTCInterruptAfter_int(){
	int32_t minutes = dj_exec_stackPopInt();

#ifdef PLATFORM_HAS_RTC_PCF2123
	uint8_t actual_minutes = RTC_get_minutes();
	RTC_schedule_interrupt_at_minutes(actual_minutes+minutes);
#else
	dj_exec_createAndThrow(BASE_CDEF_java_lang_VirtualMachineError);
#endif
}
