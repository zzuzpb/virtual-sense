/**
 * \file                ringbuf.h
 *
 * \brief               Ring-Buffer library definitions and declarations.
 *
 * \author              Luis Maqueda <luis@sen.se>
 */

#ifndef _RINGBUF_H
#define _RINGBUF_H

#include "contiki.h"
/*----------------------------------------------------------------------------*/

/**
 * Ring-buffer data-type definition
 */
/*----------------------------------------------------------------------------*/
typedef struct {
    volatile u8_t *pData;
    volatile u16_t nBytes;
    volatile u16_t iHead;
    volatile u16_t iTail;
    volatile u16_t len;
} my_ringbuf_t;
/*----------------------------------------------------------------------------*/

/**
 * EXTERNAL FUNCTIONS
 */
/*----------------------------------------------------------------------------*/
void  my_ringbuf_init(my_ringbuf_t *pBuf, u8_t *buffer, u16_t len);
u16_t my_ringbuf_put(my_ringbuf_t *pBuf, const u8_t *pData, u16_t nBytes);
u16_t my_ringbuf_get(my_ringbuf_t *pBuf, u8_t *pData, u16_t n);
u16_t my_ringbuf_peek(my_ringbuf_t *pBuf, u8_t *pData, u16_t nBytes);
u16_t my_ringbuf_length(my_ringbuf_t *pBuf);
void my_ringbuf_flush(my_ringbuf_t *pBuf);
/*----------------------------------------------------------------------------*/

#endif /*_RINGBUF_H */

/*----------------------------------------------------------------------------*/
