/*
 *	i2c.c
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
 * I2C access definitions.
 *
 * @author Matteo Dromedari
 *
 */


#include <contiki.h>
//#include <dev/spi.h>
//#include <dev/leds.h>

#include "board.h"
#include "i2c.h"



/*
 * set EEPROM pin to height impedence in order to
 * reduce current drain. Then i2c_enable will remember these
 * configuration an will restore them after use
 */
void i2c_init(){
    //
    // Set IO clock to the same as system clock
    //
    SysCtrlIOClockSet(SysCtrlClockGet());

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
	GPIOPinTypeI2C(I2C_SCL_PORT, I2C_SCL_PIN);
	GPIOPinTypeI2C(I2C_SDA_PORT, I2C_SDA_PIN);

    //
    // Configure pins as peripheral input and output
    //
    IOCPinConfigPeriphInput(I2C_SCL_PORT, I2C_SCL_PIN,
                            IOC_I2CMSSCL);
    IOCPinConfigPeriphInput(I2C_SDA_PORT, I2C_SDA_PIN,
                            IOC_I2CMSSDA);
    IOCPinConfigPeriphOutput(I2C_SCL_PORT, I2C_SCL_PIN,
                             IOC_MUX_OUT_SEL_I2C_CMSSCL);
    IOCPinConfigPeriphOutput(I2C_SDA_PORT, I2C_SDA_PIN,
                             IOC_MUX_OUT_SEL_I2C_CMSSDA);

    //
    // Enable and initialize the I2C master module.  Use the system clock for
    // the I2C module.  The last parameter sets the I2C data transfer rate.
    // If false the data rate is set to 100kbps and if true the data rate will
    // be set to 400kbps.  For this example we will use a data rate of 100kbps.
    //
    I2CMasterInitExpClk(SysCtrlClockGet(), false);
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
#if 0
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
#endif
  return 0;
}

void
i2c_stop(void)
{
#if 0
  SDA_0();
  delay_4us();
  SCL_1();
  //SCL_WAIT_FOR_1();
  delay_4us();
  SDA_1();
#endif
}

/*
 * Return true if we received an ACK.
 */
int
i2c_write(unsigned _c)
{
#if 0
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
#endif
  return 0;
}

unsigned
i2c_read(int send_ack)
{
#if 0
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
#endif
  return 0;
}
