/*
 *  spi_UCB1.h
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
 * 	UCB1 spi management
 *
 * @author Emanuele Lattanzi
 *
 */
// SPI access macros

// SPI register definitions
//#define UCB1_SPI_TX_REG               (0) //TODO to implement(UCB1TXBUF)
//#define UCB1_SPI_RX_REG               (0) //TODO to implement(UCB1RXBUF)
//#define UCB1_SPI_RX_IS_READY()        (0) //TODO to implement(UCB1IFG & UCRXIFG)
//#define UCB1_SPI_RX_NOT_READY()       (0) //TODO to implement(UCB1IFG &= ~UCRXIFG)

//#define UCB1_SPI_TX(x)                /*st*/( UCB1_SPI_RX_NOT_READY(); UCB1_SPI_TX_REG = x; )
//#define UCB1_SPI_RX()                 (UCB1_SPI_RX_REG)
//#define UCB1_SPI_WAIT_RXRDY()         st( while (!UCB1_SPI_RX_IS_READY()); )

//uint8_t UCB1_SPI_TXRX(uint8_t x);

//void spi_UCB1_init(uint8_t clock_divider);
//void spi_UCB1_shutdown(void);
//uint8_t spi_UCB1_is_up(void);

