/*
 * app_atcmd.c
 *
 *  Created on: 2012-9-24
 *      Author: YangZhiyong
 */

#include <stdint.h>
#include "app_terminal.h"
#include "transport/transport_usart2.h"

#define AT_TRANSPORT    (TP_USART2)
#define AT_TIMEOUT      (1000)


void ATCMD_Write(uint8_t* const buffer, uint32_t length)
{
    Term_Write(&AT_TRANSPORT, buffer, length, AT_TIMEOUT);
}



void ATCMD_WriteLine(uint8_t* const line, uint32_t length)
{
    Term_WriteLine(&AT_TRANSPORT, line, length, AT_TIMEOUT);
}



void ATCMD_ReadLine(char* cmd)
{

}



uint32_t ATCMD_WaitResponse1(char* const response, uint32_t timeout)
{
    Term_ReadLine(&AT_TRANSPORT, );
}



uint32_t ATCMD_WaitResponse2(char* response1,
                             char* response2,
                             uint32_t timeout);

uint32_t ATCMD_WaitResponse3(char* response1,
                             char* response2,
                             char* response3,
                             uint32_t timeout);

