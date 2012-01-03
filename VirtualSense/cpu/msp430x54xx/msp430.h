/*
 * Copyright (c) 2007, Swedish Institute of Computer Science.
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
 * $Id: msp430.h,v 1.2 2008/02/03 20:58:11 adamdunkels Exp $
 */

/**
 * \file
 *         MSP430 definitions
 * \author
 *         Adam Dunkels <adam@sics.se>
 */

#ifndef __MSP430_H__
#define __MSP430_H__

#include "contiki-conf.h"

#ifdef F_CPU
#define MSP430_CPU_SPEED F_CPU
#else
#define MSP430_CPU_SPEED 2457600UL
#endif

void init_ports(void);
void setVCoreUp(unsigned char level);
void setVCoreDown(unsigned char level);
void getSystemClockSettings(unsigned char systemClockSpeed, 
                                           unsigned char *setDcoRange,
                                           unsigned char *setVCore,
                                           unsigned int  *setMultiplier);
void startXT1(void);
void setSystemClock(unsigned char systemClockSpeed);
void setVCoreValue(unsigned char level);

#endif /* __MSP430_H__ */
