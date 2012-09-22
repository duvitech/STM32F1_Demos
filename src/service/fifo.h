/*
 * fifo.h
 *
 *  Created on: 2012-9-14
 *      Author: YangZhiyong
 */

#ifndef FIFO_H_
#define FIFO_H_

#ifdef __cplusplus
extern "C" {
#endif


#include <stdint.h>


struct FIFO
{
    uint32_t read_idx;
    uint32_t write_idx;
    uint8_t* buffer;
    uint32_t size;
};


#define FIFO_DEFINE(name, size) \
                static uint8_t name##_buffer[size];\
                static struct FIFO name = {\
                                0, \
                                0, \
                                name##_buffer, \
                                size, \
                };\


uint32_t FIFO_PutByte(struct FIFO* fifo, uint8_t byte);


uint32_t FIFO_Put(struct FIFO* fifo,
                  uint8_t* buffer,
                  uint32_t length   );



uint32_t FIFO_Get(struct FIFO* fifo,
                  uint8_t* buffer,
                  uint32_t length   );

uint32_t FIFO_Length(struct FIFO* fifo);


#ifdef __cplusplus
}
#endif

#endif /* FIFO_H_ */
