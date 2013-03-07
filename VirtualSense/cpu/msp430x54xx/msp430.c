/*
 * Copyright (c) 2005, Swedish Institute of Computer Science
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
 * This file is part of the Contiki operating system.
 *
 * @(#)$Id: msp430.c,v 1.13 2010/03/21 10:40:15 nifi Exp $
 */

//#include <msp430.h>
#include <msp430.h>
#include "msp430.h"
#include "msp430def.h"
#include "watchdog.h"
#include "net/uip.h"

/*---------------------------------------------------------------------------*/
#if defined(__MSP430__) && defined(__GNUC__) && MSP430_MEMCPY_WORKAROUND
void *
w_memcpy(void *out, const void *in, size_t n)
{
  uint8_t *src, *dest;
  src = (uint8_t *) in;
  dest = (uint8_t *) out;
  while(n-- > 0) {
    *dest++ = *src++;
  }
  return out;
}
#endif /* __GNUC__ &&  __MSP430__ && MSP430_MEMCPY_WORKAROUND */
/*---------------------------------------------------------------------------*/
#if defined(__MSP430__) && defined(__GNUC__) && MSP430_MEMCPY_WORKAROUND
void *
w_memset(void *out, int value, size_t n)
{
  uint8_t *dest;
  dest = (uint8_t *) out;
  while(n-- > 0) {
    *dest++ = value & 0xff;
  }
  return out;
}
#endif /* __GNUC__ &&  __MSP430__ && MSP430_MEMCPY_WORKAROUND */

void
init_ports(void)
{

  /* remove PM5 lock to enable I/O pin
   * this is needed when wake-up from LPM4.5
   */
  PMMCTL0_H = PMMPW_H;                      // open PMM
  PM5CTL0 &= ~LOCKIO;                       // Clear LOCKIO and enable ports
  PMMCTL0_H = 0x00;                         // close PMM



#if 0

  //Tie all ports
  P1SEL  = 0;
  P1DIR  = 0x00;
  P1REN  = 0xFF;
  P1OUT  = 0x00;



  P2SEL  = 0;
  P2DIR  = 0x00;
    P2REN  = 0xFF;
    P2OUT  = 0x00;


  P3SEL  = 0;
  P3DIR  = 0x00;
    P3REN  = 0xFF;
    P3OUT  = 0x00;


  P4SEL  = 0;
  P4DIR  = 0x00;
  P4DIR |= BIT2|BIT4|BIT5|BIT6; //PM pin to transistor to move in power manager ?
  P4REN  = 0xFF;
  P4OUT  = 0x00;


  P5SEL  = 0;
  P5DIR  = 0x00;
    P5REN  = 0xFF;
    P5OUT  = 0x00;

  P6SEL  = 0;
  P6DIR  = 0x00;
    P6REN  = 0xFF;
    P6OUT  = 0x00;



  P7SEL  = 0;
  P7DIR  = 0x00;
    P7REN  = 0xFF;
    P7OUT  = 0x00;

  P8SEL  = 0;
  P8DIR  = 0x00;
    P8REN  = 0xFF;
    P8OUT  = 0x00;


  P11SEL  = 0;
  PJDIR  = 0x00;
    PJREN  = 0xFF;
    PJOUT  = 0x00;
#endif





#if 1 //FOR MEASURES
  //Tie all ports

  P1DIR  = 0x00;
  P1REN  = 0xFF;
  P1OUT  = 0;
  P1OUT  = 0x00;
  P1SEL  = 0;


  P2DIR  = 0x00;
  //P2REN  = 0xFF;
  P2OUT  = 0;
  P2DIR  |= (BIT1);
  P2SEL  = 0;


  P3DIR  = 0xFF;
  P3REN  = 0xFF;
  P3OUT  = 0;
  P3DIR  &= ~(BIT0|BIT3);
  P3SEL  = 0;


  P4DIR  = 0x00;
  P4DIR |= BIT2|BIT4|BIT5|BIT6; //PM pin to transistor to move in power manager ?
  P4REN  = 0xFF;
  P4OUT  = 0;
  P4SEL  = 0;


  P5DIR  = 0xFF;
  P5REN  = 0xFF;
  P5OUT  = 0;
  P5DIR  &= ~(BIT0|BIT1|BIT2|BIT3|BIT6);
  P5SEL  = 0;


  P6DIR = 0x00;
  P6REN = 0xFF;
  P6OUT = 0;
  P6SEL = 0;


  P7DIR = 0x00;
  P7REN = 0xFF;
  P7OUT = 0;
  P7SEL = 0;


  P8DIR = 0x00;
  P8REN = 0xFF;
  P8OUT = 0;
  P8SEL = 0;


  PJDIR  = 0x00;
  PJREN  = 0xFF;
  PJOUT  = 0;
  P11SEL = 0;
#endif
}

/*---------------------------------------------------------------------------*/
/* msp430-ld may align _end incorrectly. Workaround in cpu_init. */
extern int _end;		/* Not in sys/unistd.h */
static char *cur_break = (char *)&_end;


/*---------------------------------------------------------------------------*/
#define asmv(arg) __asm__ __volatile__(arg)

#define STACK_EXTRA 32

/*
 * Allocate memory from the heap. Check that we don't collide with the
 * stack right now (some other routine might later). A watchdog might
 * be used to check if cur_break and the stack pointer meet during
 * runtime.
 */
void *
sbrk(int incr)
{
  char *stack_pointer;

  asmv("mov r1, %0" : "=r" (stack_pointer));
  stack_pointer -= STACK_EXTRA;
  if(incr > (stack_pointer - cur_break))
    return (void *)-1;		/* ENOMEM */

  void *old_break = cur_break;
  cur_break += incr;
  /*
   * If the stack was never here then [old_break .. cur_break] should
   * be filled with zeros.
  */
  return old_break;
}
/*---------------------------------------------------------------------------*/
/*
 * Mask all interrupts that can be masked.
 */
int
splhigh_(void)
{
  /* Clear the GIE (General Interrupt Enable) flag. */
  int sr;
  asmv("mov r2, %0" : "=r" (sr));
  asmv("bic %0, r2" : : "i" (GIE));
  return sr & GIE;		/* Ignore other sr bits. */
}
/*---------------------------------------------------------------------------*/
/*
 * Restore previous interrupt mask.
 */
void
splx_(int sr)
{
  /* If GIE was set, restore it. */
  asmv("bis %0, r2" : : "r" (sr));
}
/*---------------------------------------------------------------------------*/

/**********************************************************************//**
 * @brief  Get function for the DCORSEL, VCORE, and DCO multiplier settings 
 *         that map to a given clock speed. 
 * 
 * @param  systemClockSpeed Target DCO frequency - SYSCLK_xxMHZ.
 * 
 * @param  setDcoRange      Pointer to the DCO range select bits.
 * 
 * @param  setVCore         Pointer to the VCore level bits. 
 * 
 * @param  setMultiplier    Pointer to the DCO multiplier bits. 
 *
 * @return none
 ************************************************************************/
void getSystemClockSettings(unsigned char systemClockSpeed, 
                                    unsigned char *setDcoRange,
                                    unsigned char *setVCore,
                                    unsigned int  *setMultiplier)
{
  switch (systemClockSpeed)
  {
  case SYSCLK_1MHZ: 
    *setDcoRange = DCORSEL_1MHZ;
    *setVCore = VCORE_1MHZ;
    *setMultiplier = DCO_MULT_1MHZ;
    break;
  case SYSCLK_4MHZ: 
    *setDcoRange = DCORSEL_4MHZ;
    *setVCore = VCORE_4MHZ;
    *setMultiplier = DCO_MULT_4MHZ;
    break;
  case SYSCLK_8MHZ: 
    *setDcoRange = DCORSEL_8MHZ;
    *setVCore = VCORE_8MHZ;
    *setMultiplier = DCO_MULT_8MHZ;
    break;
  case SYSCLK_12MHZ: 
    *setDcoRange = DCORSEL_12MHZ;
    *setVCore = VCORE_12MHZ;
    *setMultiplier = DCO_MULT_12MHZ;
    break;
  case SYSCLK_16MHZ: 
    *setDcoRange = DCORSEL_16MHZ;
    *setVCore = VCORE_16MHZ;
    *setMultiplier = DCO_MULT_16MHZ;
    break;
  case SYSCLK_20MHZ:
    *setDcoRange = DCORSEL_20MHZ;
    *setVCore = VCORE_20MHZ;
    *setMultiplier = DCO_MULT_20MHZ;
    break;
  case SYSCLK_25MHZ:
      *setDcoRange = DCORSEL_25MHZ;
      *setVCore = VCORE_25MHZ;
      *setMultiplier = DCO_MULT_25MHZ;
      break;
  }	
}

/*-------------------------------------------------------------------------*/
/**********************************************************************//**
 * @brief  Set function for the PMM core voltage (PMMCOREV) setting
 * 
 * @param  level Target VCore setting 
 * 
 * @return none
 *************************************************************************/
void setVCoreValue(unsigned char level)
{
  unsigned int currentVCore;

  currentVCore = PMMCTL0 & PMMCOREV_3;      // Get actual VCore  
  // Change VCore step by step 
  while (level != currentVCore) 
  {   
    if (level > currentVCore) 
      setVCoreUp(++currentVCore);
    else
      setVCoreDown(--currentVCore);
  }
}

/**********************************************************************//**
 * @brief  Initialization routine for XT1. 
 * 
 * Sets the necessary internal capacitor values and loops until all 
 * ocillator fault flags remain cleared. 
 * 
 * @param  none
 * 
 * @return none
 *************************************************************************/
void startXT1(void)
{

	/*do
	{
		UCSCTL7 &= ~(XT2OFFG + XT1LFOFFG + XT1HFOFFG + DCOFFG);
		                                            // Clear XT2,XT1,DCO fault flags
		SFRIFG1 &= ~OFIFG;                      // Clear fault flags

	}while (SFRIFG1&OFIFG);   */                // Test oscillator fault flag

  /*// Set up XT1 Pins to analog function, and to lowest drive
  P7SEL |= 0x03;                            
  UCSCTL6 |= XCAP_3 ;                       // Set internal cap values
  
  while(SFRIFG1 & OFIFG) {                  // Check OFIFG fault flag
    while ( (SFRIFG1 & OFIFG))              // Check OFIFG fault flag
    {    
      // Clear OSC fault flags 
      UCSCTL7 &= ~(DCOFFG + XT1LFOFFG + XT1HFOFFG + XT2OFFG);
      SFRIFG1 &= ~OFIFG;                    // Clear OFIFG fault flag
    }
    UCSCTL6 &= ~(XT1DRIVE1_L+XT1DRIVE0);    // Reduce the drive strength 
  }*/
}

/**********************************************************************//**
 * @brief  Set function for MCLK frequency.
 * 
 * @param  systemClockSpeed Intended frequency of operation - SYSCLK_xxMHZ.
 * 
 * @return none
 *************************************************************************/
void setSystemClock(unsigned char systemClockSpeed)
{
  unsigned char setDcoRange, setVCore;
  unsigned int  setMultiplier;
  uint16_t d, dco_div_bits;
  uint16_t mode = 0;
  uint16_t srRegisterState;
  uint16_t fsystem = 0;

  getSystemClockSettings( systemClockSpeed, &setDcoRange,  \
                                  &setVCore, &setMultiplier);
  	fsystem = systemClockSpeed * 1000;
  	  if (setVCore > (PMMCTL0 & PMMCOREV_3))	// Only change VCore if necessary
  		  setVCoreValue( setVCore );

     // Save actual state of FLL loop control, then disable it. This is needed to
     // prevent the FLL from acting as we are making fundamental modifications to
     // the clock setup.
     srRegisterState = __read_status_register() & SCG0;

     d = setMultiplier;
     dco_div_bits = FLLD__2;                                     // Have at least a divider of 2

     if (fsystem > SYSCLK_16MHZ){
         d >>= 1;
         mode = 1;
     }
     else {
         fsystem <<= 1;                                          // fsystem = fsystem * 2
     }

     while (d > 512){
         dco_div_bits = dco_div_bits + FLLD0;                    // Set next higher div level
         d >>= 1;
     }

     __bis_SR_register(SCG0);                                    // Disable FLL

     UCSCTL0 = 0x0000;                                           // Set DCO to lowest Tap

     UCSCTL2 &= ~(0x03FF);                                       // Reset FN bits
     UCSCTL2 = dco_div_bits | (d - 1);

     UCSCTL1 = setDcoRange;                    // Select suitable range
     __bic_SR_register(SCG0);                                    // Re-enable FLL

     while (UCSCTL7 & DCOFFG) {                                  // Check DCO fault flag
    	 UCSCTL7 &= ~DCOFFG;                                     // Clear DCO fault flag

		// Clear the global fault flag. In case the DCO caused the global fault flag to get
		// set this will clear the global error condition. If any error condition persists,
		// global flag will get again.
		SFRIFG1 &= ~OFIFG;
     }

	__bis_SR_register(srRegisterState);                         // Restore previous SCG0

	if (mode == 1) {                                            // fsystem > 16000
		//SELECT_MCLK_SMCLK(SELM__DCOCLK + SELS__DCOCLK);         // Select DCOCLK
		UCSCTL4 = (UCSCTL4 & ~(SELM_7 + SELS_7)) | (SELM__DCOCLK + SELS__DCOCLK);
	}
	else {
		//SELECT_MCLK_SMCLK(SELM__DCOCLKDIV + SELS__DCOCLKDIV);   // Select DCODIVCLK
		UCSCTL4 = (UCSCTL4 & ~(SELM_7 + SELS_7)) | (SELM__DCOCLKDIV + SELS__DCOCLKDIV);
	}


  /*WDTCTL = WDTPW+WDTHOLD;                   // Stop WDT
  UCSCTL0 = 0x0000;                           // Set lowest possible DCOx, MODx
  UCSCTL1 = setDcoRange;                    // Select suitable range
  
  UCSCTL2 = setMultiplier + FLLD_1;         // Set DCO Multiplier
  UCSCTL4 = SELA__XT1CLK | SELS__DCOCLKDIV  |  SELM__DCOCLKDIV ;
  
  // Worst-case settling time for the DCO when the DCO range bits have been 
  // changed is n x 32 x 32 x f_FLL_reference. See UCS chapter in 5xx UG 
  // for optimization.
  // 32 x 32 x / f_FLL_reference (32,768 Hz) = .03125 = t_DCO_settle
  // t_DCO_settle / (1 / 18 MHz) = 562500 = counts_DCO_settle

  long int cnt;
  for (cnt=1;cnt<=562500;++cnt); */
	__delay_cycles(562500);

	/*UCSCTL4 |= SELA__DCOCLK ;
	UCSCTL5 |= DIVA__32;*/

}

/**********************************************************************//**
 * @brief  Increments the VCore setting.
 * 
 * @param  level The target VCore setting
 * 
 * @return none
 *************************************************************************/
void setVCoreUp (unsigned char level)
{
  // Open PMM module registers for write access 
  PMMCTL0_H = 0xA5;                         
  
  // Set SVS/M high side to new level
  SVSMHCTL = (SVSMHCTL & ~(SVSHRVL0*3 + SVSMHRRL0)) | \
             (SVSHE + SVSHRVL0 * level + SVMHE + SVSMHRRL0 * level); 
  
  // Set SVM new Level    
  SVSMLCTL = SVSLE + SVMLE + SVSMLRRL0 * level;       
  // Set SVS/M low side to new level
  SVSMLCTL = (SVSMLCTL & ~(SVSMLRRL_3)) | (SVMLE + SVSMLRRL0 * level);
   
  while ((PMMIFG & SVSMLDLYIFG) == 0);      // Wait till SVM is settled (Delay)
  PMMCTL0_L = PMMCOREV0 * level;            // Set VCore to x
  PMMIFG &= ~(SVMLVLRIFG + SVMLIFG);        // Clear already set flags
  
  if ((PMMIFG & SVMLIFG))
    while ((PMMIFG & SVMLVLRIFG) == 0);     // Wait till level is reached
  
  // Set SVS/M Low side to new level
  SVSMLCTL = (SVSMLCTL & ~(SVSLRVL0*3 + SVSMLRRL_3)) | \
             (SVSLE + SVSLRVL0 * level + SVMLE + SVSMLRRL0 * level);
  
  // Lock PMM module registers from write access
  PMMCTL0_H = 0x00;                         
}

/**********************************************************************//**
 * @brief  Decrements the VCore setting.
 * 
 * @param  level The target VCore.  
 * 
 * @return none
 *************************************************************************/
void setVCoreDown(unsigned char level)
{
  // Open PMM module registers for write access
  PMMCTL0_H = 0xA5;                         
  
  // Set SVS/M low side to new level
  SVSMLCTL = (SVSMLCTL & ~(SVSLRVL0*3 + SVSMLRRL_3)) | \
             (SVSLRVL0 * level + SVMLE + SVSMLRRL0 * level); 
  
  while ((PMMIFG & SVSMLDLYIFG) == 0);      // Wait till SVM is settled (Delay)
  PMMCTL0_L = (level * PMMCOREV0);          // Set VCore to new level 
  // Lock PMM module registers for write access
  
  PMMCTL0_H = 0x00;                         
}



