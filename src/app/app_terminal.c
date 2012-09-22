/*
 * app_terminal.c
 *
 *  Created on: 2012-9-21
 *      Author: YangZhiyong
 */


#include "service/launcher.h"
#include <stdint.h>
#include "service/fifo.h"
#include "transport/transport_usart2.h"


/////////////////////////////////////////////////////

void Term_Init(struct TRANSPORT_IF const*  tp)
{
    tp->init((void*)0);
}



uint32_t Term_ReadLine  (struct TRANSPORT_IF const* tp, uint8_t* buffer, uint32_t buffer_length)
{
    uint32_t line_length = 0;
    uint32_t recv_length;
    uint8_t read_char;
    uint8_t char_CR = 0;
    uint8_t char_LF = 0;

    do {
        recv_length = tp->recv( &read_char, 1 );

        if( recv_length != 0 )
        {
            if( read_char == 0x0D )
            {
                char_CR = 0x0D;
            }
            else if( read_char == 0x0A )
            {
                char_LF = 0x0D;
            }
            else
            {
                buffer[line_length] = read_char;
                line_length ++;
                if( line_length > buffer_length )
                {
                    break;
                }
            }
        }
        else
        {
            tp->waitEventTrigger(TRANSPORT_Event_RecvDone);
        }

    } while( (char_CR != 0x0D) | (char_LF != 0x0D) );

    return line_length;
}



uint32_t Term_WriteLine (struct TRANSPORT_IF const* tp, uint8_t* buffer, uint32_t buffer_length)
{
    uint32_t send_length = 0;
    uint8_t end_line[2] = {0x0D, 0x0A};

    do {
        send_length += tp->send(buffer, buffer_length);
        tp->waitEventTrigger(TRANSPORT_Event_SendDone);
    }while( send_length < buffer_length );

    tp->send(end_line, 2);
    tp->waitEventTrigger(TRANSPORT_Event_SendDone);

    return send_length;
}




