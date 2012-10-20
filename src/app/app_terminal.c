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
#include <string.h>


#define LINE_END_STR    "\r\n"


/////////////////////////////////////////////////////

void Term_Init(struct TRANSPORT_IF*  tp)
{
//    tp->init((void*)0);
}



int Term_ReadLine  (    struct TRANSPORT_IF* tp,
                        uint8_t* buffer,
                        uint32_t buffer_length,
                        uint32_t timeout   )
{
    enum TRANSPORT_Event event;
    int line_length = 0;
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
            event = tp->waitEventTrigger(TRANSPORT_Event_RecvDone, timeout);
            if( event == TRANSPORT_Event_Timeout )
            {
                line_length = -1;
                break;
            }
        }

    } while( (char_CR != 0x0D) | (char_LF != 0x0D) );

    return line_length;
}



uint32_t Term_Write (struct TRANSPORT_IF* tp,
                     uint8_t* const buffer,
                     uint32_t buffer_length,
                     uint32_t timeout   )
{
    uint32_t send_length = 0;

    do {
        send_length += tp->send(buffer, buffer_length);
        tp->waitEventTrigger(TRANSPORT_Event_SendDone, timeout);
    } while( send_length < buffer_length );

    return send_length;
}



uint32_t Term_WriteString(struct TRANSPORT_IF* tp,
                          uint8_t* const string,
                          uint32_t timeout  )
{
    uint32_t send_length;
    uint32_t string_length;

    string_length = strnlen((char*)string, 0x800);
    send_length = Term_Write(tp, string, string_length, timeout);

    return send_length;
}



uint32_t Term_WriteLine (struct TRANSPORT_IF* tp,
                         uint8_t* const line,
                         uint32_t timeout   )
{
    uint32_t send_length = 0;

    send_length = Term_WriteString(tp, line, timeout);
    Term_WriteString(tp, LINE_END_STR, timeout);

    return send_length;
}


