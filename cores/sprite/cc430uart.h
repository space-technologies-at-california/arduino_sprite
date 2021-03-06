/**
 * Copyright (c) 2011 panStamp <contact@panstamp.com>
 * 
 * This file is part of the panStamp project.
 * 
 * panStamp  is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * any later version.
 * 
 * panStamp is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with panStamp; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 
 * USA
 * 
 * Author: Daniel Berenguer
 * Creation date: 10/27/2013
 */

#ifndef _CC430UART_H
#define _CC430UART_H

#include "wiring.h"

#define SERIAL_BUFFER_SIZE 64

// Define config for Serial.begin(baud, config);
#define SERIAL_7N1 0x10
#define SERIAL_8N1 0x00
#define SERIAL_7N2 0x18
#define SERIAL_8N2 0x08
#define SERIAL_7E1 0xD0
#define SERIAL_8E1 0xC0
#define SERIAL_7E2 0xD8
#define SERIAL_8E2 0xC8
#define SERIAL_7O1 0x90
#define SERIAL_8O1 0x80
#define SERIAL_7O2 0x98
#define SERIAL_8O2 0x88

class CC430UART
{   
  public:
    /**
     * Rx buffer
     */
    static uint8_t rxBuffer[SERIAL_BUFFER_SIZE];
    
    /**
     * Number of bytes in Rx Buffer
     */
    static uint8_t rxLength;

    /**
     * Index in the rxBuffer
     */
    static uint8_t rxIndex;

    /**
     * TxBuffer
     */
    static uint8_t txBuffer[SERIAL_BUFFER_SIZE];
    static uint8_t tx_buffer_head;
    static volatile uint8_t tx_buffer_tail;

    /**
     * begin
     * 
     * Initialize UART port
     *
     * @param baud Serial baudrate
     * @param config additional UART configuration
     */
    void begin(uint32_t baud, uint8_t config=SERIAL_8N1);

    /**
     * end
     * 
     * Close UART port
     */
    inline void end(void)
    {
      // Wait for transmission of outgoing data
      while (tx_buffer_head != tx_buffer_tail);

      while (UCA0STAT & UCBUSY);  // Wait whilst the UART is busy
      UCA0IE &= ~UCRXIE;          // Disable Rx interrupt
      UCA0IE &= ~UCTXIE;          // Disable Tx interrupt
      UCA0CTL1 |= UCSWRST;        // Reset UART
    }

    /**
     * write
     * 
     * Send single byte
     *
     * @param c Character to be transmitted
     *
     * @return Amount of bytes transmitted
     */
    size_t write(uint8_t c);

    /**
     * write
     * 
     * Send data buffer
     *
     * @param buf Buffer to be sent
     * @param len Amount of bytes to be transferred
     *
     * @return Amount of bytes transmitted
     */
    size_t write(uint8_t *buf, uint16_t len);

    /**
     * read
     * 
     * Read string from the Rx FIFO
     *
     * @param buf Buffer to be sent
     *
     * @return amount of bytes read or -1 if the Rx FIFO is empty
     */
    int read(uint8_t *buf);

    /**
     * read
     * 
     * Read single byte from Rx FIFO
     *
     * @return byte read
     */
    int read(void);
    
    /**
     * available
     * 
     * Get he amount of bytes waiting to be read from the
     * UART Rx FIFO
     *
     * @return true if there is at least one byte in the Rx FIFO
     */
    bool available(void);
};

#endif

