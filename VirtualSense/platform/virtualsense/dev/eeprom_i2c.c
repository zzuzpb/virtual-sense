/*
 *  eeprom_i2c.h
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
 * The I2C  communication system with 24AA512 EEPROM.
 *
 * @author Emanuele Lattanzi
 *
 */


#include <msp430.h>

#include "contiki-conf.h"



///
/// Registers set for UCB0 I2C interface
/// 2 wires
///
/*

	P3.1 : UCB0SDA				<-->
	P3.2 : UCB0SCL				--->

	P2.5 : digital : write protect, active high

	address: A2 - A1 : 0 0;   A0 : 1
*/

#include "eeprom_i2c.h"

///
/// this function initialize I2C on B0 UART
void eeprom_i2C_init(){

	/// P2.5 is connected to WP and is active high.
	P2DIR |= BIT5;
	P2OUT |= BIT5;														// write protect
	/// P3.2 and P3.1 in alternative mode
	P3SEL |= 0x06;                            // Assign I2C pins to USCI_B0
	UCB0CTL1 |= UCSWRST;                      // Enable SW reset
	UCB0CTL0 = UCMST + UCMODE_3 + UCSYNC;     // I2C Master, synchronous mode
  UCB0CTL1 = UCSSEL_2 + UCSWRST;            // Use SMCLK, keep SW reset
  UCB0BR0 = 0xA0;                             // fSCL = SMCLK/10 = ~100kHz
  UCB0BR1 = 0;
  UCB0I2CSA = 0x51;                         // Slave Address is 051h:  1010 001
  UCB0CTL1 &= ~UCSWRST;                     // Clear SW reset, resume operation
  //UCB0IE |= UCRXIE;                         // Enable RX interrupt
}

///
/// this function read a byte at stored address
/*unsigned char readI2CByte(){

	unsigned char tmp, flag;

	/// send STOP
	//UCB0CTL1 |= UCTXSTP;
	flag = 1;
	do	{
		//while (UCB0CTL1 & UCTXSTP);             // Ensure stop condition got sent
		UCB0CTL1 &= ~UCTR;											// clear tx mode
	  UCB0CTL1 |= UCTXSTT;                    // I2C start condition
	  /// wait start transmission
	  while (UCB0CTL1 & UCTXSTT);
	  /// and send stop condition (page 629 of UG)
	  UCB0CTL1 |= UCTXSTP;                		// Generate I2C stop condition
		while (!(UCB0IFG & UCRXIFG)){
			/// a NACK is produced?
			if (UCB0IFG & UCNACKIFG){
				UCB0IFG &= ~UCNACKIFG;
				break;
			}
			else
				flag = 0;
		}						/// wait a char
	}while(flag);

	tmp = UCB0RXBUF;
	return tmp;
}*/

///
/// this function read a byte at specified address
uint8_t eeprom_read_byte_from_address(uint16_t address, uint8_t *status){

	uint8_t tmp = (address >> 8);
	*status = 0;
	/// write enable
	P2OUT |= BIT5;
	printf("ensure stop condition\n");
	while (UCB0CTL1 & UCTXSTP);             // Ensure stop condition got sent
	/// start with a write session
	UCB0CTL1 |= UCTR + UCTXSTT;												// transmit
	//UCB0CTL1 |= UCTXSTT;                    // I2C start condition
	/// wait start transmission
	/// As stated in UG, page 627, UCTXSTT is cleared when address is transmitted
	/// from master to slave AND slave ACKNOWLEDGE the address
  //while (UCB0CTL1 & UCTXSTT);

  /// wait tx buffer free (it already free)
	printf("wait tx buffer free\n");
  while(!(UCB0IFG & UCTXIFG));
  /// transmit addr_h byte
  UCB0TXBUF = tmp;
  printf("transmit address 1--0x%x\n", tmp);
  /// a NACK is arrived?  Write cycle isn't finished
  if (UCB0IFG & UCNACKIFG){
  	/// send a STOP
		printf("send stop condition\n");
  	UCB0CTL1 |= UCTXSTP;
  	UCB0IFG &= ~UCNACKIFG;
  	*status = NACK_ERR;
  	return 0;
  }
  tmp = (address & 0xff);
  /// wait tx buffer free
	printf("transmit address 2--0x%x\n", tmp);
  while(!(UCB0IFG & UCTXIFG));
  /// transmit addr_h byte
  UCB0TXBUF = tmp;

  /// as soon data is transferred from buffer to tx reg
  /// it send a RESTART SIGNAL
  printf("receive\n");
  while(!(UCB0IFG & UCTXIFG));
  UCB0CTL1 &= ~UCTR;											// receive
	UCB0CTL1 |= UCTXSTT;                    // I2C start condition
  UCB0IFG &= ~UCTXIFG;										// clear tx flag

  /// wait start transmission
  printf("wait start transmission\n");
  while (UCB0CTL1 & UCTXSTT);
  /// and send stop condition (page 629 of UG)
  UCB0CTL1 |= UCTXSTP;                		// Generate I2C stop condition
	while (!(UCB0IFG & UCRXIFG));						/// wait a response
	printf("wait response\n");
	tmp = UCB0RXBUF;
	*status = STATUS_OK;
	printf("read %x\n", tmp);
	return tmp;
}

///
/// this function read a byte at specified address
uint8_t eeprom_read_bytes_from_address(uint16_t address, uint8_t num_bytes, uint8_t buff[] ){

	uint8_t tmp = (address >> 8), i;
	/// write enable
	P2OUT |= BIT5;

	while (UCB0CTL1 & UCTXSTP);             	// Ensure stop condition got sent
	/// start with a write session
	UCB0CTL1 |= UCTR + UCTXSTT;								// transmit
	//UCB0CTL1 |= UCTXSTT;                    // I2C start condition
	/// wait start transmission
	/// As stated in UG, page 627, UCTXSTT is cleared when address is transmitted
	/// from master to slave AND slave ACKNOWLEDGE the address
  //while (UCB0CTL1 & UCTXSTT);

  /// wait tx buffer free (it already free)
  while(!(UCB0IFG & UCTXIFG));
  /// transmit addr_h byte
  UCB0TXBUF = tmp;
  /// a NACK is arrived?  Write cycle isn't finished
  if (UCB0IFG & UCNACKIFG){
  	/// send a STOP
  	UCB0CTL1 |= UCTXSTP;
  	UCB0IFG &= ~UCNACKIFG;
  	return NACK_ERR;
  }
  tmp = (address & 0xff);
  /// wait tx buffer free
  while(!(UCB0IFG & UCTXIFG));
  /// transmit addr_h byte
  UCB0TXBUF = tmp;

  /// as soon data is transferred from buffer to tx reg
  /// it send a RESTART SIGNAL
  while(!(UCB0IFG & UCTXIFG));
  UCB0CTL1 &= ~UCTR;											// receive
	UCB0CTL1 |= UCTXSTT;                    // I2C start condition
  UCB0IFG &= ~UCTXIFG;										// clear tx flag

  /// wait start transmission
  while (UCB0CTL1 & UCTXSTT);
  for ( i = 0; i < num_bytes; i++){
  	while (!(UCB0IFG & UCRXIFG));						/// wait a response
  	buff[i] = UCB0RXBUF;
  	if ( i == num_bytes - 2)
  		UCB0CTL1 |= UCTXSTP;                		// Generate I2C stop condition
  }
  /// and send stop condition (page 629 of UG)
  //UCB0CTL1 |= UCTXSTP;                		// Generate I2C stop condition

	return STATUS_OK;
}

/* 																																				*
 * 															WRITE FUNCTIONS														*
 * 																																				*/
///
/// this function write a byte at specified address
 uint8_t eeprom_write_byte_at_address(uint8_t data, uint16_t address){

	/// as stated in 24AA512 datasheet, the sequence is:
	/// start byte, addr_h, addr_l, data
	uint8_t tmp;

	tmp = (address >> 8);
	P2OUT &= ~BIT5;													// write enable

	printf("ensure stop condition\n");
	while (UCB0CTL1 & UCTXSTP);             // Ensure stop condition got sent
	UCB0CTL1 |= UCTXSTT + UCTR;             // I2C start condition
	/// wait start transmission
	/// As stated in UG, page 627, UCTXSTT is cleared when address is transmitted
	/// from master to slave AND slave ACKNOWLEDGE the address
  //while (UCB0CTL1 & UCTXSTT);

  /// wait tx buffer free (it already free because as shown in fig. 28.12,
  /// UCTXIFG is set after a start command
	printf("write address %x\n", tmp);

  while(!(UCB0IFG & UCTXIFG));
  /// transmit addr_h byte
  UCB0TXBUF = tmp;
  tmp = (address & 0xff);
  /// however a NACK could happen, if a previous write cycle is not finished,
  /// so wait tx buffer free will be an infinite cycle
  printf("NACK\n");
  while (!(UCB0IFG & UCTXIFG)){
	 	/// if NACK is produced, UCTXIFG = 0 and thus we are here
  	if (UCB0IFG & UCNACKIFG){
  		/// there is a NACK. Fig. 28.12 assert that stop signal must be done
  		UCB0IFG &= ~UCNACKIFG;
  		// A stop condition is built and we check it next time we call this function
  		UCB0CTL1 |= UCTXSTP;
  		///Exit from this while cycle and try again
  		return NACK_ERR;
  	}
  }
	//while(!(UCB0IFG & UCTXIFG));
	/// ok, an ACK is arrived and we can transmit
  printf("trasmit addr_hn\n");
	/// transmit addr_h byte
	UCB0TXBUF = tmp;

  /// transmit data
	printf("transmit data\n");
  while(!(UCB0IFG & UCTXIFG)){
	 	/// if NACK is produced, UCTXIFG = 0 and thus we are here
  	if (UCB0IFG & UCNACKIFG){
  		/// there is a NACK. Fig. 28.12 assert that stop signal must be done
  		UCB0IFG &= ~UCNACKIFG;
  		// A stop condition is built and we check it next time we call this function
  		UCB0CTL1 |= UCTXSTP;
  		///Exit from this while cycle and try again
  		return NACK_ERR;
  	}
  }
  /// transmit addr_h byte
  UCB0TXBUF = data;

  /// check if data is shifted from buffer to tx register
  /// as soon data is shifted in tx register, it send a STOP signal
  printf("send stop condition\n");
  while(!(UCB0IFG & UCTXIFG));
  /// and now it must send stop
  UCB0CTL1 |= UCTXSTP;                // Generate I2C stop condition
  printf("return\n");
  return STATUS_OK;

}

///
/// this function write N byte at specified address
 uint8_t eeprom_write_bytes_at_address(uint16_t address, uint8_t num_bytes, uint8_t buff[] ){

	/// as stated in 24AA512 datasheet, the sequence is:
	/// start byte, addr_h, addr_l, data
	uint8_t tmp = (address >> 8), i;

	P2OUT &= ~BIT5;													// write enable

	while (UCB0CTL1 & UCTXSTP);             // Ensure stop condition got sent
	UCB0CTL1 |= UCTXSTT + UCTR;             // I2C start condition
	/// wait start transmission
	/// As stated in UG, page 627, UCTXSTT is cleared when address is transmitted
	/// from master to slave AND slave ACKNOWLEDGE the address
  //while (UCB0CTL1 & UCTXSTT);

  /// wait tx buffer free (it already free)
  while(!(UCB0IFG & UCTXIFG));
  /// transmit addr_h byte
  UCB0TXBUF = tmp;
  tmp = (address & 0xff);
  /// wait tx buffer free
  while(!(UCB0IFG & UCTXIFG)){
  /// if NACK is produced, UCTXIFG = 0 and thus we are here
  	if (UCB0IFG & UCNACKIFG){
  		/// there is a NACK. Fig. 28.12 assert that stop signal must be done
  		UCB0IFG &= ~UCNACKIFG;
  		// A stop condition is built and we check it next time we call this function
  		UCB0CTL1 |= UCTXSTP;
  		///Exit from this while cycle and try again
  		return NACK_ERR;
  	}
  }
  /// transmit addr_h byte
  UCB0TXBUF = tmp;

  /// transmit data
  for (i = 0; i < num_bytes; i++){
	  while(!(UCB0IFG & UCTXIFG)){
		  /// if NACK is produced, UCTXIFG = 0 and thus we are here
	  	if (UCB0IFG & UCNACKIFG){
	  		/// there is a NACK. Fig. 28.12 assert that stop signal must be done
	  		UCB0IFG &= ~UCNACKIFG;
	  		// A stop condition is built and we check it next time we call this function
	  		UCB0CTL1 |= UCTXSTP;
	  		///Exit from this while cycle and try again
	  		return NACK_ERR;
	  	}
	  }
	  /// transmit addr_h byte
	  UCB0TXBUF = buff[i];
  }

  /// check if data is shifted from buffer to tx register
  /// as soon data is shifted in tx register, it send a STOP signal
  while(!(UCB0IFG & UCTXIFG));
  /// and now it must send stop
  UCB0CTL1 |= UCTXSTP;                // Generate I2C stop condition
  return STATUS_OK;
}
