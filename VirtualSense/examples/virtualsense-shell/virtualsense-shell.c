/*
 * Copyright (c) 2008, Swedish Institute of Computer Science.
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
 * $Id: sky-shell.c,v 1.27 2010/11/12 13:17:45 nifi Exp $
 */

/**
 * \file
 *         Tmote Sky-specific Contiki shell
 * \author
 *         Adam Dunkels <adam@sics.se>
 */

#include "contiki.h"
#include "shell.h"
#include "serial-shell.h"
#include "virtualsense-gateway-shell.h"
//#include "net/rime.h"

/*---------------------------------------------------------------------------*/
PROCESS(virtualsense_shell_process, "VirtualSense gateway shell");
AUTOSTART_PROCESSES(&virtualsense_shell_process);
/*---------------------------------------------------------------------------*/
#define WITH_PERIODIC_DEBUG 0
#if WITH_PERIODIC_DEBUG
static struct ctimer debug_timer;
static void
periodic_debug(void *ptr)
{
  ctimer_set(&debug_timer, 20 * CLOCK_SECOND, periodic_debug, NULL);
  collect_print_stats();
}
#endif /* WITH_PERIODIC_DEBUG */
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(virtualsense_shell_process, ev, data)
{
  PROCESS_BEGIN();

#if WITH_PERIODIC_DEBUG
  ctimer_set(&debug_timer, 20 * CLOCK_SECOND, periodic_debug, NULL);
#endif /* WITH_PERIODIC_DEBUG */

  serial_shell_init();
  shell_reboot_init();
  shell_gateway_init();

#if DEBUG_SNIFFERS
  rime_sniffer_add(&s);
#endif /* DEBUG_SNIFFERS */
  
  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
