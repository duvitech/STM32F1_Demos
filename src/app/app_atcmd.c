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

#define AT_TRANSPORT    (TP_USART2)
#define AT_TIMEOUT      (1000)

#define CMD_LENGTH  0x80

uint8_t CMD_Buffer[CMD_LENGTH];


void ATCMD_Write(uint8_t* const buffer, uint32_t length)
{
    Term_Write(&AT_TRANSPORT, buffer, length, AT_TIMEOUT);
}



void ATCMD_WriteLine(uint8_t* const line, uint32_t length)
{
    Term_WriteLine(&AT_TRANSPORT, line, length, AT_TIMEOUT);
}



int ATCMD_ReadLine(uint8_t* const line, uint32_t length, uint32_t timeout)
{
    int recv_length;

    recv_length = Term_ReadLine(&AT_TRANSPORT, CMD_Buffer, sizeof(CMD_Buffer), timeout);

    return recv_length;
}



/**
 *
 * @param response
 * @param timeout
 * @return
 */
uint32_t ATCMD_WaitResponse1(char* const response, uint32_t timeout)
{
    int recv_length;
    int result;

    do
    {
        recv_length = Term_ReadLine(&AT_TRANSPORT, CMD_Buffer, sizeof(CMD_Buffer), timeout);

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
//            str = strstr((char const*)CMD_Buffer, (char const*)response);
            result = memcmp( (char const*)CMD_Buffer, (char const*)response, recv_length);
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



uint32_t ATCMD_WaitResponse2(char* response1,
                             char* response2,
                             uint32_t timeout)

{
    int recv_length;
    int result;

    do
    {
        recv_length = Term_ReadLine(&AT_TRANSPORT, CMD_Buffer, sizeof(CMD_Buffer), timeout);

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
//            str = strstr((char const*)CMD_Buffer, (char const*)response);
            result = memcmp( (char const*)CMD_Buffer, (char const*)response1, recv_length);
            if ( result == 0 )
            {// 期待的回应
                return 1;
            }

            result = memcmp( (char const*)CMD_Buffer, (char const*)response2, recv_length);
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



uint32_t ATCMD_WaitResponse3(char* response1,
                             char* response2,
                             char* response3,
                             uint32_t timeout)
{
    int recv_length;
    int result;

    do
    {
        recv_length = Term_ReadLine(&AT_TRANSPORT, CMD_Buffer, sizeof(CMD_Buffer), timeout);

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
//            str = strstr((char const*)CMD_Buffer, (char const*)response);
            result = memcmp( (char const*)CMD_Buffer, (char const*)response1, recv_length);
            if ( result == 0 )
            {// 期待的回应
                return 1;
            }

            result = memcmp( (char const*)CMD_Buffer, (char const*)response2, recv_length);
            if ( result == 0 )
            {// 期待的回应
                return 2;
            }

            result = memcmp( (char const*)CMD_Buffer, (char const*)response3, recv_length);
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




