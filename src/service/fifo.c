/*
 * fifo.c
 *
 *  Created on: 2012-9-14
 *      Author: YangZhiyong
 */


#include "fifo.h"

#include <string.h>


//inline uint32_t empty_space(struct FIFO* fifo)
//{
//    return fifo->size - (fifo->write_idx - fifo->read_idx);
//}
//
//
//inline uint32_t fill_space(struct FIFO* fifo)
//{
//    return fifo->write_idx - fifo->read_idx;
//}



uint32_t FIFO_Length(struct FIFO* fifo)
{
    return fifo->write_idx - fifo->read_idx;
}



uint32_t FIFO_PutByte(struct FIFO* fifo, uint8_t byte)
{
    uint32_t empty;
    uint32_t write_offset;


    empty = fifo->size - (fifo->write_idx - fifo->read_idx);

    if( empty == 0 )
    {
        return 0;
    }

    write_offset = fifo->write_idx & (fifo->size - 1);
    fifo->buffer[write_offset] = byte;

    fifo->write_idx ++;

    return 1;
}



uint32_t FIFO_Put(struct FIFO* fifo,
                  uint8_t* buffer,
                  uint32_t length   )
{
//    uint32_t mask;
    uint32_t empty;
    uint32_t write_length;
    uint32_t write_offset;
    uint32_t write_to_end;

//    mask = fifo->size - 1;

    empty = fifo->size - (fifo->write_idx - fifo->read_idx);

    if( empty > length )
    {
        write_length = length;
    }
    else
    {
        write_length = empty;
    }

    write_offset = fifo->write_idx & (fifo->size - 1);
    write_to_end = fifo->size - write_offset;

    if( write_length > write_to_end )
    {
        memcpy(&(fifo->buffer[write_offset]), buffer, write_to_end );
        memcpy(&(fifo->buffer[0]), &(buffer[write_to_end]), write_length-write_to_end );
    }
    else
    {
        memcpy(&(fifo->buffer[write_offset]), buffer, write_length );
    }

    fifo->write_idx += write_length;

    return write_length;
}




uint32_t FIFO_Get(struct FIFO* fifo,
                  uint8_t* buffer,
                  uint32_t length   )
{
//    uint32_t mask;
    uint32_t fill;
    uint32_t read_length;
    uint32_t read_offset;
    uint32_t read_to_end;


//    mask = fifo->size - 1;

    fill = fifo->write_idx - fifo->read_idx;

    if ( length > fill )
    {
        read_length = fill;
    }
    else
    {
        read_length = length;
    }

    read_offset = fifo->read_idx & (fifo->size - 1);
    read_to_end = fifo->size - read_offset;

    if( read_length > read_to_end )
    {
        memcpy(buffer, &(fifo->buffer[read_offset]), read_to_end );
        memcpy(&(buffer[read_to_end]), &(fifo->buffer[0]), read_length-read_to_end );
    }
    else
    {
        memcpy(buffer, &(fifo->buffer[read_offset]), read_length );
    }

    fifo->read_idx += read_length;

    return read_length;
}


