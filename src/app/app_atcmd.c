/*
 * app_atcmd.c
 *
 *  Created on: 2012-9-24
 *      Author: YangZhiyong
 */

#include <stdint.h>
#include <string.h>
#include "app_terminal.h"
#include "transport/transport_usart2.h"

//#define AT_TRANSPORT    (TP_USART2)
#define AT_TIMEOUT      (1000)

#define CMD_LENGTH  0x80

//uint8_t response_buffer[CMD_LENGTH];



void ATCMD_Write(struct TRANSPORT_IF * tp,
                 uint8_t* const buffer,
                 uint32_t length,
                 uint32_t timeout   )
{
    Term_Write(tp, buffer, length, timeout);
}



void ATCMD_WriteLine(struct TRANSPORT_IF* tp,
                     uint8_t* const line,
                     uint32_t timeout )
{
    Term_WriteLine(tp, line, timeout);
}



int ATCMD_ReadLine(struct TRANSPORT_IF * tp,
                   uint8_t* const line,
                   uint32_t length,
                   uint32_t timeout )
{
    int recv_length;

    recv_length = Term_ReadLine(tp, line, length, timeout);

    return recv_length;
}



/**
 *
 * @param response
 * @param timeout
 * @return
 */
uint32_t ATCMD_WaitResponse1(struct TRANSPORT_IF * tp,
                             uint8_t* response_buffer,
                             uint32_t buffer_length,
                             uint32_t timeout,
                             char* const response)
{
    int recv_length;
    int result;

    do
    {
        recv_length = Term_ReadLine(tp, response_buffer, buffer_length, timeout);

        if( recv_length < 0 )
        {// 超时
            return 0;
        }
        else if( recv_length == 0 )
        {// 空行
            continue;
        }
        else
        {
            result = memcmp( (char const*)response_buffer, (char const*)response, recv_length);
            if ( result == 0 )
            {// 期待的回应
                return 1;
            }

            {// 意外回应 TODO AT自动回应。
            }
        }

    }
    while(1);

}



/**
 *
 * @param response1
 * @param response2
 * @param timeout
 * @return
 */
uint32_t ATCMD_WaitResponse2(struct TRANSPORT_IF * tp,
                             uint8_t* response_buffer,
                             uint32_t buffer_length,
                             uint32_t timeout,
                             char* response1,
                             char* response2 )
{
    int recv_length;
    int result;

    do
    {
        recv_length = Term_ReadLine(tp, response_buffer, buffer_length, timeout);

        if( recv_length < 0 )
        {// 超时
            return 0;
        }
        else if( recv_length == 0 )
        {// 空行
            continue;
        }
        else
        {
            result = memcmp( (char const*)response_buffer, (char const*)response1, recv_length);
            if ( result == 0 )
            {// 期待的回应
                return 1;
            }

            result = memcmp( (char const*)response_buffer, (char const*)response2, recv_length);
            if ( result == 0 )
            {// 期待的回应
                return 2;
            }

            {// 意外回应 TODO AT自动回应。
            }
        }

    }
    while(1);

}



uint32_t ATCMD_WaitResponse3(struct TRANSPORT_IF * tp,
                             uint8_t* response_buffer,
                             uint32_t buffer_length,
                             uint32_t timeout,
                             char* response1,
                             char* response2,
                             char* response3 )
{
    int recv_length;
    int result;

    do
    {
        recv_length = Term_ReadLine(tp, response_buffer, buffer_length, timeout);

        if( recv_length < 0 )
        {// 超时
            return 0;
        }
        else if( recv_length == 0 )
        {// 空行
            continue;
        }
        else
        {
//            str = strstr((char const*)response_buffer, (char const*)response);
            result = memcmp( (char const*)response_buffer, (char const*)response1, recv_length);
            if ( result == 0 )
            {// 期待的回应
                return 1;
            }

            result = memcmp( (char const*)response_buffer, (char const*)response2, recv_length);
            if ( result == 0 )
            {// 期待的回应
                return 2;
            }

            result = memcmp( (char const*)response_buffer, (char const*)response3, recv_length);
            if ( result == 0 )
            {// 期待的回应
                return 2;
            }

            {// 意外回应 TODO AT自动回应。
            }
        }

    }
    while(1);

}




