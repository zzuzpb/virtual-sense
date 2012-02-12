/**
 * \file                cc2520ll.c
 *
 * \brief               Architecture-specific functions to interface the TI's CC2520 radio
 *
 * \author              Sergio A. Floriano Sanchez      <safs@kth.se>
 * \author              Joaquin Juan Toledo             <jjt@kth.se>
 * \author              Luis Maqueda                            <luis@sen.se>
 */

#include "dev/cc2520ll.h"

/*----------------------------------------------------------------------------*/
/**
 * LOCAL VARIABLES
 */
static cc2520ll_cfg_t pConfig;
static u8_t rxMpdu[128];
static ringbuf_t rxBuffer;
static u8_t buffer[CC2520_BUF_LEN];

/*
 * Recommended register settings which differ from the data sheet
 */

static regVal_t regval[]= {

    /* Tuning settings */
#ifdef INCLUDE_PA
    CC2520_TXPOWER,     0xF9,       /* Max TX output power */
    CC2520_TXCTRL,      0xC1,
#else
    CC2520_TXPOWER,     0xF7,       /* Max TX output power */
#endif
    CC2520_CCACTRL0,    0xF8,       /* CCA threshold -80dBm */

    /* Recommended RX settings */
    CC2520_MDMCTRL0,    0x85,
    CC2520_MDMCTRL1,    0x14,
    CC2520_RXCTRL,      0x3F,
    CC2520_FSCTRL,      0x5A,
    CC2520_FSCAL1,      0x03,
    CC2520_FRMFILT0,    0,                      /* enables promiscuous mode */
#ifdef INCLUDE_PA
    CC2520_AGCCTRL1,    0x16,
#else
    CC2520_AGCCTRL1,    0x11,
#endif
    CC2520_ADCTEST0,    0x10,
    CC2520_ADCTEST1,    0x0E,
    CC2520_ADCTEST2,    0x03,

    /* Configuration for applications using cc2520ll_init() */
    CC2520_FRMCTRL0,    0x40,               /* auto crc */
    CC2520_EXTCLOCK,    0x00,
    CC2520_GPIOCTRL0,   1 + CC2520_EXC_RX_FRM_DONE,
    CC2520_GPIOCTRL1,   CC2520_GPIO_SAMPLED_CCA,
    CC2520_GPIOCTRL2,   CC2520_GPIO_RSSI_VALID,
#ifdef INCLUDE_PA
    CC2520_GPIOCTRL3,   CC2520_GPIO_HIGH,   /* CC2590 HGM */
    CC2520_GPIOCTRL4,   0x46,               /* EN set to lna_pd[1] inverted */
    CC2520_GPIOCTRL5,   0x47,               /* PAEN set to pa_pd inverted */
    CC2520_GPIOPOLARITY,0x0F,               /* Invert GPIO4 and GPIO5 */
#else
    CC2520_GPIOCTRL3,   CC2520_GPIO_SFD,
    CC2520_GPIOCTRL4,   CC2520_GPIO_SNIFFER_DATA,
    CC2520_GPIOCTRL5,   CC2520_GPIO_SNIFFER_CLK,
#endif

};

/**
 * @fn      cc2520ll_waitRadioReady
 *
 * @brief   Wait for the crystal oscillator to stabilize.
 *
 * @param   none
 *
 * @return  SUCCESS if oscillator starts, FAILED otherwise
 */
/*----------------------------------------------------------------------------*/
static u8_t
cc2520ll_waitRadioReady(void)
{
  u8_t i;

  /* Wait for XOSC stable to be announced on the MISO pin */
  i = 200;
  CC2520_SPI_BEGIN();
  while (i > 0 && !(P5IN & (1 << CC2520_MISO_PIN))) {
    __delay_cycles(10*MSP430_USECOND);
    --i;
  }
  CC2520_SPI_END();

  return i > 0 ? SUCCESS : FAILED;
}
/*----------------------------------------------------------------------------*/

/**
 * @fn      cc2520ll_waitTransceiverReady
 *
 * @brief   Wait until the transceiver is ready (SFD low).
 *
 * @param   none
 *
 * @return  none
 */
/*----------------------------------------------------------------------------*/
void
cc2520ll_waitTransceiverReady(void)
{
#ifdef INCLUDE_PA
  /* GPIO3 is not conncted to combo board; use SFD at GPIO2 instead */
  dint();
  /* GPIO2 = SFD */
  CC2520_REGWR8(CC2520_GPIOCTRL0 + 2, CC2520_GPIO_SFD);
  /* CC2520_GPIO_DIR_OUT(2); */
  P2DIR &= ~BIT2;
  /* CC2520_CFG_GPIO_OUT(2,CC2520_GPIO_SFD); */
  while (CC2520_SFD_PIN);
  /* GPIO2 = default (RSSI_VALID) */
  CC2520_CFG_GPIO_OUT(2,CC2520_GPIO_RSSI_VALID);
  eint();
#else
  while (CC2520_SFD_PIN);
#endif
}
/*----------------------------------------------------------------------------*/

/**
 * @fn      cc2520ll_interfaceInit
 *
 * @brief   Initialises SPI interface to CC2520 and configures reset and vreg
 *          signals as MCU outputs.
 *
 * @param   none
 *
 * @return  none
 */
/*----------------------------------------------------------------------------*/
void
cc2520ll_interfaceInit(void)
{

	// Initialize the CC2520 interface
	CC2520_SPI_END();
	/* Initialize the CC2520 interface */
	P5OUT &= ~(1 << CC2520_RESET_PIN);
	P1OUT &= ~(1 << CC2520_VREG_EN_PIN);

	P5DIR |= (1 << CC2520_RESET_PIN);
	P1DIR |= (1 << CC2520_VREG_EN_PIN);

   /* Port 1.3 configuration GPIO 0 interrupt*/
	P1SEL &= ~(1 << CC2520_INT_PIN);
	P1OUT &= ~(1 << CC2520_INT_PIN);
	P1DIR &= ~(1 << CC2520_INT_PIN);


}
/*----------------------------------------------------------------------------*/

/**
 * @fn          cc2520ll_spiInit
 *
 * @brief       Initalise Radio SPI interface
 *
 * @param       none
 *
 * @return      none
 */
/*----------------------------------------------------------------------------*/
void
cc2520ll_spiInit(void)
{
	/* Put state machine in reset */
	UCB1CTL1 |= UCSWRST;

	/* 8MHz spi */
	UCB1BR0 = 0x0002;
	UCB1BR1 = 0;

	/* Make sure SIMO is configured as output */
	P3DIR |= (1 << CC2520_SIMO_PIN);
	/* Make sure MISO is configured as input */
	P5DIR &= ~(1 << CC2520_MISO_PIN);
	//P5DIR |= (1 << CC2520_CLK_PIN);

	P3SEL |= (1 << CC2520_SIMO_PIN);
	P5SEL |= (1 << CC2520_MISO_PIN);
	/* P5 peripheral select CLK  */
	P5SEL |= (1 << CC2520_CLK_PIN);
	/* Select SMCLK */
	UCB1CTL1 = UCSSEL_2;
	/* 3-pin, 8-bit SPI master, rising edge capture */
	UCB1CTL0 |= UCCKPH /*UCCKPL*/ | UCSYNC | UCMSB | UCMST;
	/* Initialize USCI state machine */
	UCB1CTL1 &= ~UCSWRST;
	printf("SPI initialized\n");


#if 0





/* Put state machine in reset */
  UCB1CTL1 |= UCSWRST;
  /* 3-pin, 8-bit SPI master, rising edge capture */
  UCB1CTL0 |= UCCKPH /*UCCKPL*/ | UCSYNC | UCMSB | UCMST;

  /* Select SMCLK */
   UCB1CTL1 = UCSSEL_2;

  /* 8MHz spi */
  UCB1BR0 = 0x0002;
  UCB1BR1 = 0;

  /* Set CS high */
  P3OUT |= (1 << CC2520_CS_PIN);
#endif

}
/*----------------------------------------------------------------------------*/

/**
 * @fn      cc2520ll_config
 *
 * @brief   Power up, sets default tuning settings, enables autoack and
 *                      configures chip IO
 *
 * @param   none
 *
 * @return  SUCCESS if the radio has started, FAILURE otherwise
 */
/*----------------------------------------------------------------------------*/
u8_t
cc2520ll_config(void)
{
  u8_t val;
  u16_t i = 0;

  /* Avoid GPIO0 interrupts during reset */
  P1IE &= ~(1 << CC2520_INT_PIN);

  /* Make sure to pull the CC2520 RESETn and VREG_EN pins low */
  P5OUT &= ~(1 << CC2520_RESET_PIN);
  /* Raise CS */
  P3OUT |= (1 << CC2520_CS_PIN);
  P1OUT &= ~(1 << CC2520_VREG_EN_PIN);

  printf("1 delay\n");
  __delay_cycles(MSP430_USECOND*2000);


  /* Make sure MISO is configured as output */
  P5DIR &= ~(1 << CC2520_MISO_PIN);

  /* Enable the voltage regulator and wait for it (CC2520 power-up) */
  P1OUT |= (1 << CC2520_VREG_EN_PIN);
  __delay_cycles(MSP430_USECOND*CC2520_VREG_MAX_STARTUP_TIME);
  printf("2 delay\n");

  /* Release reset */
  P5OUT |= (1 << CC2520_RESET_PIN);

  printf("Wait for XOSC\n");
  /* Wait for XOSC stable to be announced on the MISO pin */
  if (cc2520ll_waitRadioReady()==FAILED) {
	printf("XOSC failed\n");
    return FAILED;
  }
  printf("XOSC OK writing in mem\n");
  /* Write non-default register values */
  for (i = 0; i < sizeof(regval)/sizeof(regVal_t); i++) {
    CC2520_MEMWR8(regval[i].reg, regval[i].val);
  }

  /* Verify a register */
  val= CC2520_MEMRD8(CC2520_MDMCTRL0);

  printf("Device configured with val 0x%x\n", val);
  return val==0x85? SUCCESS : FAILED;
}
/*----------------------------------------------------------------------------*/

/**
 * @fn      cc2520ll_setChannel
 *
 * @brief   Set RF channel in the 2.4GHz band. The Channel must be in the range 11-26,
 *          11= 2005 MHz, channel spacing 5 MHz.
 *
 * @param   channel - logical channel number
 *
 * @return  none
 */
/*----------------------------------------------------------------------------*/
void
cc2520ll_setChannel(u8_t channel)
{
  CC2520_REGWR8(CC2520_FREQCTRL, MIN_CHANNEL + ((channel - MIN_CHANNEL) * CHANNEL_SPACING));
}
/*----------------------------------------------------------------------------*/

/**
 * @fn      cc2520ll_setShortAddr
 *
 * @brief   Write short address to chip
 *
 * @param   none
 *
 * @return  none
 */
/*----------------------------------------------------------------------------*/
void
cc2520ll_setShortAddr(u16_t shortAddr)
{
  CC2520_MEMWR16(CC2520_RAM_SHORTADDR, shortAddr);
}
/*----------------------------------------------------------------------------*/

/**
 * @fn      cc2520ll_setShortAddr
 *
 * @brief   Write long address to chip
 *
 * @param   none
 *
 * @return  none
 */
/*----------------------------------------------------------------------------*/
void
cc2520ll_setLongAddr(rimeaddr_t *longAddr)
{
  u16_t i = 0;

  for (i = 0; i < 8; i++) {
        CC2520_MEMWR8(CC2520_RAM_EXTADDR + i, longAddr->u8[7 - i]);
  }
}
/*----------------------------------------------------------------------------*/

/**
 * @fn      cc2520ll_setPanId
 *
 * @brief   Write PAN Id to chip
 *
 * @param   none
 *
 * @return  none
 */
/*----------------------------------------------------------------------------*/
void
cc2520ll_setPanId(u16_t panId)
{
  CC2520_MEMWR16(CC2520_RAM_PANID, panId);
}
/*----------------------------------------------------------------------------*/

/**
 * @fn          cc2520ll_init
 *
 * @brief       Initialise cc2520 datastructures. Sets channel, short address and
 *              PAN id in the chip and configures interrupt on packet reception
 *
 *              txState - file scope variable that keeps tx state info
 *
 * @return      none
 */
u16_t
cc2520ll_init()
{
  pConfig.panId = PAN_ID;
  pConfig.channel = RF_CHANNEL;
  pConfig.ackRequest = FALSE;

  /* initialize spi */
   cc2520ll_spiInit();
  /* initialize the rest of the interface */
  cc2520ll_interfaceInit();


  if (cc2520ll_config() == FAILED) {
        return FAILED;
  }

  /* initialize the ring buffer */
  ringbuf_init(&rxBuffer, buffer, sizeof(buffer));

  dint();

  /* Set channel */
  cc2520ll_setChannel(pConfig.channel);

  /* Write the short address and the PAN ID to the CC2520 RAM */
  cc2520ll_setPanId(pConfig.panId);

  /* Set up receive interrupt (received data or acknowledgment) */
  /* Set rising edge */
  P1IES &= ~(1 << CC2520_INT_PIN);
  P1IFG &= ~(1 << CC2520_INT_PIN);
  P1IE |= (1 << CC2520_INT_PIN);

  /* Clear the exception */
  CLEAR_EXC_RX_FRM_DONE();

  /* Register the interrupt handler for P1.3 */
  //register_port2IntHandler(CC2520_INT_PIN, cc2520ll_packetReceivedISR);
  /* Enable general interrupts */
  eint();

  /* And enable reception on cc2520 */
  cc2520ll_receiveOn();

  return SUCCESS;
}
/*----------------------------------------------------------------------------*/

/**
 * @fn      cc2520ll_readRxBuf
 *
 * @brief   Read RX buffer
 *
 * @param   u8_t* pData - data buffer. This must be allocated by caller.
 *          u8_t length - number of bytes
 *
 * @return  none
 */
/*----------------------------------------------------------------------------*/
void
cc2520ll_readRxBuf(u8_t* pData, u8_t length)
{
  CC2520_RXBUF(length, pData);
}
/*----------------------------------------------------------------------------*/

/**
 * @fn      cc2520ll_writeTxBuf
 *
 * @brief   Write to TX buffer
 *
 * @param   u8_t* data - buffer to write
 *          u8_t length - number of bytes
 *
 * @return  none
 */
/*----------------------------------------------------------------------------*/
void
cc2520ll_writeTxBuf(u8_t* data, u8_t length)
{
  /* Copy packet to TX FIFO */
  CC2520_TXBUF(length, data);
}
/*----------------------------------------------------------------------------*/

/**
 * @fn      cc2520ll_transmit
 *
 * @brief   Transmits frame with Clear Channel Assessment.
 *
 * @param   none
 *
 * @return  int - SUCCESS or FAILED
 */
u16_t
cc2520ll_transmit()
{
  u16_t timeout = 2500; // 2500 x 20us = 50ms
  u8_t status=0;

  /* Wait for RSSI to become valid */
  while(!CC2520_RSSI_VALID_PIN);

  /* Reuse GPIO2 for TX_FRM_DONE exception */
  _disable_interrupts();
  CC2520_CFG_GPIO_OUT(2, 1 + CC2520_EXC_TX_FRM_DONE);
  _enable_interrupts();

  /* Wait for the transmission to begin before exiting (makes sure that this
   * function cannot be called a second time, and thereby canceling the first
   * transmission. */
  while (--timeout > 0) {
    _disable_interrupts();
    CC2520_INS_STROBE(CC2520_INS_STXONCCA);
    _enable_interrupts();
    if (CC2520_SAMPLED_CCA_PIN) {
        break;
    }
    __delay_cycles(20*MSP430_USECOND);
  }
  if (timeout == 0) {
    status = FAILED;
    CC2520_INS_STROBE(CC2520_INS_SFLUSHTX);
  } else {
    status = SUCCESS;
    /* Wait for TX_FRM_DONE exception */
    while(!CC2520_TX_FRM_DONE_PIN);
    _disable_interrupts();
    CC2520_CLEAR_EXC(CC2520_EXC_TX_FRM_DONE);
    _enable_interrupts();
  }

  /* Reconfigure GPIO2 */
  _disable_interrupts();
  CC2520_CFG_GPIO_OUT(2, CC2520_GPIO_RSSI_VALID);
  _enable_interrupts();

  return status;
}
/*----------------------------------------------------------------------------*/

/**
 * @fn      cc2520ll_packetSend
 *
 * @brief   Sends a packet.
 *
 * @param   const void* packet - the packet to be sent.
 * @param   unsigned short len - teh length of the packet to be sent.
 *
 * @return  u8_t - SUCCESS or FAILED
 */
/*----------------------------------------------------------------------------*/
u16_t
cc2520ll_packetSend(const void* packet, unsigned short len)
{
  if (cc2520ll_prepare(packet, len)) {
        return cc2520ll_transmit();
  } else {
        return FAILED;
  }
}
/*----------------------------------------------------------------------------*/

/**
 * @fn      cc2520ll_prepare
 *
 * @brief   Prepares a packet to be sent.
 *
 * @param   const void* packet - the packet to be sent.
 * @param   unsigned short len - teh length of the packet to be sent.
 *
 * @return  u8_t - SUCCESS or FAILED
 */
/*----------------------------------------------------------------------------*/
u16_t
cc2520ll_prepare(const void *packet, unsigned short len)
{
  /* Check packet length */
  if (len + 3 > MAX_802154_PACKET_SIZE + 1) {
        return FAILED;
  } else {
        /* Wait until the transceiver is idle */
        cc2520ll_waitTransceiverReady();
        /* Turn off RX frame done interrupt to avoid interference on the SPI
         * interface */
        cc2520ll_disableRxInterrupt();
        /* Auto crc enabled. The packet will be 2 bytes longer */
        len += 2;
        cc2520ll_writeTxBuf(&len, 1);
        cc2520ll_writeTxBuf(packet, len-2);

        /* Turn on RX frame done interrupt for ACK reception */
        cc2520ll_enableRxInterrupt();
        return SUCCESS;
  }
}
/*----------------------------------------------------------------------------*/

/**
 * @fn          cc2520ll_channel_clear
 *
 * @brief       Performs a clear channel assesment (CCA)
 * @return      int - 1 if channel clear, 0 otherwise.
 */
u16_t
cc2520ll_channel_clear()
{
  u16_t result;

  while(!CC2520_RSSI_VALID_PIN);
  _disable_interrupts();
  CC2520_CFG_GPIO_OUT(2, CC2520_GPIO_CCA);
  _enable_interrupts();
  if (P2IN & BIT2) {
        result = 1;
  } else {
        result = 0;
  }
  /* restore GPIO 2 behavior */
  _disable_interrupts();
  CC2520_CFG_GPIO_OUT(2, CC2520_GPIO_RSSI_VALID);
  _enable_interrupts();
  return result;
}
/*----------------------------------------------------------------------------*/

/**
 * @fn          cc2520ll_receiving_packet()
 *
 * @brief       Returns true if a packet is being received (or transmited) in this
 *                         precise moment.
 * @return      int - a number != 0 if a packet is being received or transmited,
 *              0 otherwise.
 */
/*----------------------------------------------------------------------------*/
u16_t
cc2520ll_rxtx_packet()
{
  return CC2520_SFD_PIN;
}
/*----------------------------------------------------------------------------*/

/**
 * @fn          cc2520ll_pending_packet
 *
 * @brief       Returns true if there is data to be read in the receive buffer.
 * @return      u8_t - a number != 0 if there isnew data to be read, 0 otherwise.
 */
/*----------------------------------------------------------------------------*/
u16_t
cc2520ll_pending_packet(void)
{
  return ringbuf_length(&rxBuffer);
}
/*----------------------------------------------------------------------------*/

/**
 * @fn          cc2520ll_packetReceive
 *
 * @brief       Copies the payload of the last incoming packet into a buffer
 *
 * @param       packet - pointer to data buffer to fill. This buffer must be
 *                        allocated by higher layer.
 *              maxlen - Maximum number of bytes to read from buffer
 *
 * @return      u8_t - number of bytes actually copied into buffer
 */
/*----------------------------------------------------------------------------*/
u16_t
cc2520ll_packetReceive(u8_t* packet, u8_t maxlen)
{
  u8_t len = 0;

  _disable_interrupts();
  if(ringbuf_length(&rxBuffer)) {
    ringbuf_get(&rxBuffer, &len, 1);
    /* The first byte in the packet is the packet's length */
    /* but it does not count the length field itself */
    if (len > maxlen) {
      ringbuf_flush(&rxBuffer);
      len = 0;
    } else {
      len = ringbuf_get(&rxBuffer, packet, len);
    }
  }
  _enable_interrupts();
  return len;
}
/*----------------------------------------------------------------------------*/

/**
 * @fn      cc2520ll_receiveOn
 *
 * @brief   Turn receiver on
 *
 * @param   none
 *
 * @return  none
 */
void
cc2520ll_receiveOn(void)
{
  CC2520_INS_STROBE(CC2520_INS_SRXON);
  cc2520ll_enableRxInterrupt();
}
/*----------------------------------------------------------------------------*/

/**
 * @fn      cc2520ll_receiveOff
 *
 * @brief   Turn receiver off
 *
 * @param   none
 *
 * @return  none
 */
void
cc2520ll_receiveOff(void)
{
  /* wait until we finish receiving/transmitting */
  while(cc2520ll_rxtx_packet());
  cc2520ll_disableRxInterrupt();
  CC2520_INS_STROBE(CC2520_INS_SRFOFF);
}
/*----------------------------------------------------------------------------*/

/**
 * @fn      cc2520ll_disableRxInterrupt
 *
 * @brief   Clear and disable RX interrupt.
 *
 * @param   none
 *
 * @return  none
 */
/*----------------------------------------------------------------------------*/
void
cc2520ll_disableRxInterrupt()
{
  /* Clear the exception and the IRQ */
  CLEAR_EXC_RX_FRM_DONE();
  P1IFG &= ~BIT0;
  P1IE &= ~BIT0;
}
/*----------------------------------------------------------------------------*/

/**
 * @fn      cc2520ll_enableRxInterrupt
 *
 * @brief   Enable RX interrupt.
 *
 * @param   none
 *
 * @return  none
 */
/*----------------------------------------------------------------------------*/
void
cc2520ll_enableRxInterrupt()
{
  P1IE |= (1 << CC2520_INT_PIN);
}
/*----------------------------------------------------------------------------*/

/**
 * @fn          cc2520ll_packetReceivedISR
 *
 * @brief       Interrupt service routine for received frame from radio
 *              (either data or acknowlegdement)
 *
 * @return      none
 */
/*----------------------------------------------------------------------------*/
interrupt(PORT1_VECTOR)
cc2520ll_packetReceivedISR(void)
{
  cc2520ll_packetHdr_t *pHdr;
  u8_t *pStatusWord;

  /* Map header to packet buffer */
  pHdr = (cc2520ll_packetHdr_t*)rxMpdu;
  /* Clear interrupt and disable new RX frame done interrupt */
  cc2520ll_disableRxInterrupt();
  /* Read payload length. */
  cc2520ll_readRxBuf(rxMpdu, 1);
  /* Ignore MSB */
  rxMpdu[0] &= CC2520_PLD_LEN_MASK;
  pHdr->packetLength = rxMpdu[0];

  /* Is this an acknowledgment packet? */
  /* Only ack packets may be 5 bytes in total. */
  if (pHdr->packetLength != CC2520_ACK_PACKET_SIZE) {
    /* It is assumed that the radio rejects packets with invalid length.
     * Subtract the number of bytes in the frame overhead to get actual
     * payload */
    cc2520ll_readRxBuf(&rxMpdu[1], pHdr->packetLength);
    /* Read the FCS to get the RSSI and CRC */
    pStatusWord = rxMpdu + pHdr->packetLength + 1 - 2;
    /* Notify the application about the received data packet if the CRC is OK */
    if(pStatusWord[1] & CC2520_CRC_OK_BM) {
      /* All ok; copy received frame to ring buffer */
          ringbuf_put(&rxBuffer, rxMpdu, rxMpdu[0] + 1);
    }
    /* Flush the cc2520 rx buffer to prevent residual data */
      CC2520_SFLUSHRX();
  }
  /* Enable RX frame done interrupt again */
  cc2520ll_enableRxInterrupt();
  /* Clear interrupt flag */
  P2IFG &= ~(1 << CC2520_INT_PIN);
}
/*----------------------------------------------------------------------------*/
