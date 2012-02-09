#include <stdlib.h>
#include <msp430.h>
#include <legacymsp430.h>
#include "dev/eeprom.h"
#include "dev/leds.h"
#include "eeprom_24AA512.h"




unsigned char eeprom_address;

void eeprom_write(eeprom_addr_t addr, unsigned char *buf, int size){
	unsigned int num_bytes = size;
	unsigned int current_address = addr;

	while(num_bytes > MAXPAGEWRITE){
		while(!write_page(current_address, MAXPAGEWRITE, buf));
		EEPROM_AckPolling();
		num_bytes -= MAXPAGEWRITE;
		current_address+=MAXPAGEWRITE;
		buf+=MAXPAGEWRITE;
	}
	if(num_bytes > 0){
		while(!write_page(current_address, num_bytes, buf));
		EEPROM_AckPolling();
	}
}


/**
 * Read data from the EEPROM.
 *
 * This function reads a number of bytes from the specified address in
 * EEPROM and into a buffer in memory.
 *
 * \param addr The address in EEPROM from which the data should be read.
 *
 * \param buf A pointer to the buffer to which the data should be stored.
 *
 * \param size The number of bytes to read.
 *
 *
 */
void eeprom_read(eeprom_addr_t addr, unsigned char *buf, int size){
	while(!read_page( (unsigned int)addr, (unsigned int)size, buf ));
}



/**
 * Initialize the EEPROM module
 *
 * This function initializes the EEPROM module and is called from the
 * bootup code.
 *
 */
void eeprom_init(void){
	EEPROM_init_I2C(EEPROM_ADDRESS);
}



/*----------------------------------------------------------------------------*/
// Description:
//   Initialization of the I2C Module
/*----------------------------------------------------------------------------*/
void EEPROM_init_I2C(unsigned char eeprom_i2c_address)
{
	eeprom_address = eeprom_i2c_address;
	/// P2.5 is connected to WP and is active high.
  P2DIR |= BIT5;
  P2OUT |= BIT5;
  I2C_PORT_SEL |= SDA_PIN + SCL_PIN;        // Assign I2C pins to USCI_B0

  // Recommended initialisation steps of I2C module as shown in User Guide:
  UCB0CTL1 |= UCSWRST;                      // Enable SW reset
  UCB0CTL0 = UCMST + UCMODE_3 + UCSYNC;     // I2C Master, synchronous mode
  UCB0CTL1 = UCSSEL_2 + /*UCTR +*/ UCSWRST;     // Use SMCLK, TX mode, keep SW reset
  UCB0BR0 = SCL_CLOCK_DIV;                  // fSCL = SMCLK/12 = ~100kHz
  UCB0BR1 = 0;
  UCB0I2CSA  = eeprom_i2c_address;          // define Slave Address
                                            // In this case the Slave Address
                                            // defines the control byte that is
                                            // sent to the EEPROM.
  UCB0I2COA = 0x01A5;                       // own address.
  UCB0CTL1 &= ~UCSWRST;                     // Clear SW reset, resume operation

}

/*----------------------------------------------------------------------------*/
// Description:
//   Acknowledge Polling. The EEPROM will not acknowledge if a write cycle is
//   in progress. It can be used to determine when a write cycle is completed.
/*----------------------------------------------------------------------------*/
void EEPROM_AckPolling(void)
{
  while (UCB0STAT & UCBUSY);                // wait until I2C module has
                                            // finished all operations
  long int cnt;
     for (cnt=1;cnt<=400000;++cnt);
}



/// this function write N byte at specified address
 char write_page(unsigned int address, unsigned char numElm, unsigned char buff[] ){

	/// as stated in 24AA512 datasheet, the sequence is:
	/// start byte, addr_h, addr_l, data
	unsigned char tmp = (address >> 8), i;

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
  		return 0;
  	}
  }
  /// transmit addr_h byte
  UCB0TXBUF = tmp;

  /// transmit data
  for (i = 0; i < numElm; i++){
	  while(!(UCB0IFG & UCTXIFG)){
		  /// if NACK is produced, UCTXIFG = 0 and thus we are here
	  	if (UCB0IFG & UCNACKIFG){
	  		/// there is a NACK. Fig. 28.12 assert that stop signal must be done
	  		UCB0IFG &= ~UCNACKIFG;
	  		// A stop condition is built and we check it next time we call this function
	  		UCB0CTL1 |= UCTXSTP;
	  		///Exit from this while cycle and try again
	  		return 0;
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
  return 1;
}

 char read_page( unsigned int address, unsigned char numElm, unsigned char buff[] ){

 	unsigned char tmp = (address >> 8), i;
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
   	return 0;
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
   for ( i = 0; i < numElm; i++){
   	while (!(UCB0IFG & UCRXIFG));						/// wait a response
   	buff[i] = UCB0RXBUF;
   	if ( i == numElm - 2)
   		UCB0CTL1 |= UCTXSTP;                		// Generate I2C stop condition
   }
   /// and send stop condition (page 629 of UG)
   //UCB0CTL1 |= UCTXSTP;                		// Generate I2C stop condition

 	return 1;
 }
