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
 * @(#)$Id: i2c.c,v 1.2 2007/01/12 13:41:57 bg- Exp $
 */

/*
 * Small and portable implementation of a bit-banging I2C bus master.
 *
 * The code should port really easily to platforms other than the
 * msp430 but has some hardcoded constants in it.
 *
 * More info at:
 *   http://i2c-bus.org/
 *   http://www.esacademy.com/faq/i2c/
 */



#include <contiki.h>
#include <dev/spi.h>
#include <dev/leds.h>

#include "dev/i2c.h"


#include "hw_ints.h"
#include "hw_memmap.h"
#include "_gpio.h"
#include "_interrupt.h"
#include "_ioc.h"
#include "hw_ioc.h"
#include "_sys_ctrl.h"
#include "hw_i2cm.h"
#include "hw_i2cs.h"
#include "_i2c.h"

/*
 *
 */

void     i2c_enable(void);
void     i2c_disable(void);
int      i2c_start(void);
unsigned i2c_read(int send_ack);
int      i2c_write(unsigned);
void     i2c_stop(void);

#define I2C_PxDIR   (0) //TODO to implement P3DIR
#define I2C_PxIN    (0) //TODO to implementP3IN
#define I2C_PxOUT   (0) //TODO to implementP3OUT
#define I2C_PxSEL   (0) //TODO to implementP3SEL
#define I2C_PxREN   (0) //TODO to implementP3REN




#define EXAMPLE_PIN_I2C_SCL             GPIO_PIN_2
#define EXAMPLE_PIN_I2C_SDA             GPIO_PIN_3
#define EXAMPLE_GPIO_I2C_BASE           GPIO_D_BASE

#define EEPROM_ADR012_PIN				GPIO_PIN_4

#define SLAVE_ADDRESS 0x57//0xAE//0x3C

/*
 * SDA == P3.1
 * SCL == P3.2
 */
#define SDA       1
#define SCL       2

#define SDA_0()   (0) //TODO to implement(I2C_PxDIR |=  BV(SDA))		/* SDA Output */
#define SDA_1()   (0) //TODO to implement(I2C_PxDIR &= ~BV(SDA))		/* SDA Input */
#define SDA_IS_1  (0) //TODO to implement(I2C_PxIN & BV(SDA))

#define SCL_0()   (0) //TODO to implement(I2C_PxDIR |=  BV(SCL))		/* SCL Output */
#define SCL_1()   (0) //TODO to implement(I2C_PxDIR &= ~BV(SCL))		/* SCL Input */
#define SCL_IS_1  (0) //TODO to implement(I2C_PxIN & BV(SCL))

/*
 * Should avoid infinite looping while waiting for SCL_IS_1. xxx/bg
 */
#define SCL_WAIT_FOR_1() do{}while (0 /* TODO to implement !SCL_IS_1 */)

//#define delay_4_7us() do{ _NOP(); _NOP(); _NOP(); _NOP(); \
                          _NOP(); _NOP(); _NOP(); _NOP(); \
                          _NOP(); _NOP(); _NOP(); _NOP(); }while(0)

//#define delay_4us()   do{ _NOP(); _NOP(); _NOP(); _NOP(); \
                          _NOP(); _NOP(); _NOP(); _NOP(); \
                          _NOP(); _NOP(); }while(0)

#define delay_4_7us() do{ /* TODO to implement __delay_cycles(5000); */}while(0)

#define delay_4us()   do{ /* TODO to implement __delay_cycles(4000); */}while(0)



static unsigned char old_pxsel, old_pxout, old_pxdir;
unsigned char spi_busy = 0;

/*
 * set EEPROM pin to height impedence in order to
 * reduce current drain. Then i2c_enable will remember these
 * configuration an will restore them after use
 */
void i2c_init(void){
	/*
#if 0 //TODO to implement
	unsigned char sda_scl = BV(SDA)|BV(SCL);

	I2C_PxOUT &= ~ sda_scl;
	I2C_PxDIR &= ~ sda_scl;
	I2C_PxREN &= ~ sda_scl;
#endif*/

	uint32_t pui32DataTx = 0x0F;

	uint32_t adrHi = 0x00;
	uint32_t adrLo = 0x01;

	uint32_t write_byte = 0x55;

	GPIOPinTypeGPIOOutput(EXAMPLE_GPIO_I2C_BASE, EEPROM_ADR012_PIN);
	GPIOPinWrite(EXAMPLE_GPIO_I2C_BASE, EEPROM_ADR012_PIN, EEPROM_ADR012_PIN);

	//SysCtrlClockSet(false, false, SYS_CTRL_SYSDIV_8MHZ);
	//SysCtrlIOClockSet(SYS_CTRL_SYSDIV_8MHZ);

	//
	//  The I2C peripheral must be enabled before use.
	//
	SysCtrlPeripheralEnable(SYS_CTRL_PERIPH_I2C);

	//
	// Do reset of I2C module
	//
	SysCtrlPeripheralReset(SYS_CTRL_PERIPH_I2C);

	//
	// Configure I2C pins
	//
	GPIOPinTypeI2C(EXAMPLE_GPIO_I2C_BASE, EXAMPLE_PIN_I2C_SCL);
	GPIOPinTypeI2C(EXAMPLE_GPIO_I2C_BASE, EXAMPLE_PIN_I2C_SDA);


    //
    // Configure pins as peripheral input and output
    //
    IOCPinConfigPeriphInput(EXAMPLE_GPIO_I2C_BASE, EXAMPLE_PIN_I2C_SCL,
                            IOC_I2CMSSCL);
    IOCPinConfigPeriphInput(EXAMPLE_GPIO_I2C_BASE, EXAMPLE_PIN_I2C_SDA,
                            IOC_I2CMSSDA);
    IOCPinConfigPeriphOutput(EXAMPLE_GPIO_I2C_BASE, EXAMPLE_PIN_I2C_SCL,
                             IOC_MUX_OUT_SEL_I2C_CMSSCL);
    IOCPinConfigPeriphOutput(EXAMPLE_GPIO_I2C_BASE, EXAMPLE_PIN_I2C_SDA,
                             IOC_MUX_OUT_SEL_I2C_CMSSDA);

    I2CMasterInitExpClk(SysCtrlClockGet(), false);

    // WRITE
    printf("write page on eeprom\n");
    I2CMasterSlaveAddrSet(SLAVE_ADDRESS, false);

    I2CMasterDataPut(adrHi);
    I2CMasterControl(I2C_MASTER_CMD_BURST_SEND_START);
    while(I2CMasterBusy())
    {
    }

    I2CMasterDataPut(adrLo);
	I2CMasterControl(I2C_MASTER_CMD_BURST_SEND_CONT);
	while(I2CMasterBusy())
	{
	}

	uint32_t j = 0x00;
	uint32_t err1 = 0x00;
	for(j = 0x00; j < 126; j++){
		I2CMasterDataPut(j);
		I2CMasterControl(I2C_MASTER_CMD_BURST_SEND_CONT);
		while(I2CMasterBusy())
		{
		}
		err1 = I2CMasterErr();
		printf("write byte %x: %x\n", j, err1);
	}

	I2CMasterDataPut(write_byte);
	I2CMasterControl(I2C_MASTER_CMD_BURST_SEND_FINISH);
	while(I2CMasterBusy())
	{
	}
	err1 = I2CMasterErr();
	printf("write byte %x: %x\n", write_byte, err1);


    // READ
	uint32_t read = 0x00;
	printf("read byte on eeprom\n");
	printf("delay\n");
	printf("delay\n");
	printf("delay\n");
	printf("delay\n");
	printf("delay\n");
	printf("delay\n");
	printf("delay\n");
    I2CMasterSlaveAddrSet(SLAVE_ADDRESS, false);

	I2CMasterDataPut(adrHi);
	I2CMasterControl(I2C_MASTER_CMD_BURST_SEND_START);//I2C_MASTER_CMD_SINGLE_SEND);
    while(I2CMasterBusy())
    {
    }

    I2CMasterDataPut(adrLo);
    I2CMasterControl(I2C_MASTER_CMD_BURST_SEND_CONT);
    while(I2CMasterBusy())
    {
    }

    I2CMasterSlaveAddrSet(SLAVE_ADDRESS, true);
    printf("delay\n");

    j = 0x01;
    I2CMasterControl(I2C_MASTER_CMD_BURST_RECEIVE_START);
	while(I2CMasterBusy())
	{
	}
	err1 = I2CMasterErr();
	read = I2CMasterDataGet();
	printf("read byte%x: %x (ret: %x)\n", j, read, err1);

    for(j = 0x02; j < 128; j++){
    	I2CMasterControl(I2C_MASTER_CMD_BURST_RECEIVE_CONT);
    	while(I2CMasterBusy())
    	{
    	}
    	err1 = I2CMasterErr();
    	read = I2CMasterDataGet();
    	printf("read byte%x: %x (ret: %x)\n", j, read, err1);
    }

    j++;
    I2CMasterControl(I2C_MASTER_CMD_BURST_RECEIVE_FINISH);
	while(I2CMasterBusy())
	{
	}
	err1 = I2CMasterErr();
	read = I2CMasterDataGet();
	printf("read byte%x: %x (ret: %x)\n", j, read, err1);

}
/*
 * Grab SDA and SCL pins for exclusive use but remember old
 * configuration so that it may be restored when we are done.
 */
void
i2c_enable(void)
{
#if 0 //TODO to implement
  unsigned char sda_scl = BV(SDA)|BV(SCL);


  old_pxsel = I2C_PxSEL & sda_scl;
  old_pxout = I2C_PxOUT & sda_scl;
  old_pxdir = I2C_PxDIR & sda_scl;

  spi_busy = 1;

  I2C_PxSEL &= ~sda_scl;

  I2C_PxOUT &= ~sda_scl;

  I2C_PxDIR |=  BV(SCL);		/* SCL Output */
  I2C_PxDIR &= ~BV(SDA);		/* SDA Input */
#endif
}

/*
 * Restore bus to what it was before i2c_enable.
 *
 */
void
i2c_disable(void)
{
#if 0 //TODO to implement
  unsigned char not_sda_scl = ~(BV(SDA)|BV(SCL));

  I2C_PxDIR = (I2C_PxDIR & not_sda_scl) | old_pxdir;
  I2C_PxOUT = (I2C_PxOUT & not_sda_scl) | old_pxout;
  I2C_PxSEL = (I2C_PxSEL & not_sda_scl) | old_pxsel;

  spi_busy = 0;
#endif
}

int
i2c_start(void)
{
  SDA_1();
  SCL_1();
#if 1
  SCL_WAIT_FOR_1();
#else
  {
    unsigned long n;
    for (n = 0; n < 100000 && !SCL_IS_1; n++)
      ;
    if (!SCL_IS_1)
      return -1;
  }
#endif
  delay_4_7us();
  SDA_0();
  delay_4us();
  SCL_0();
  delay_4us();
  return 0;
}

void
i2c_stop(void)
{
  SDA_0();
  delay_4us();
  SCL_1();
  //SCL_WAIT_FOR_1();
  delay_4us();
  SDA_1();
}

/*
 * Return true if we received an ACK.
 */
int
i2c_write(unsigned _c)
{
  unsigned char c = _c;
  unsigned long n;
  int i;
  int ret;

  for (i = 0; i < 8; i++, c <<= 1) {
    if (c & 0x80)
      SDA_1();
    else
      SDA_0();
    SCL_1();
    //SCL_WAIT_FOR_1();
    delay_4us();
    SCL_0();
    delay_4us();
  }

  SDA_1();
  SCL_1();
  delay_4us();
  ret = 0;		   /* Loop waiting for an ACK to arrive. */
  for (n = 0; n < 250000; n++) {
    if (!SDA_IS_1) {
      ret = 1;
      break;
    }
  }

  //SCL_WAIT_FOR_1();		/* clock stretching? */

  SCL_0();
  delay_4us();
  return ret;
}

unsigned
i2c_read(int send_ack)
{
  int i;
  unsigned char c = 0x00;
  SDA_1();
  for (i = 0; i < 8; i++) {
    c <<= 1;
    SCL_1();
    //SCL_WAIT_FOR_1();
    delay_4us();
    if (SDA_IS_1)
      c |= 0x1;
    SCL_0();
    delay_4us();
  }

  if (send_ack)
    SDA_0();
  SCL_1();
  //SCL_WAIT_FOR_1();
  delay_4us();
  SCL_0();

  return c;
}
