/*
 * drv_sim90x.c
 *
 *  Created on: 2012-9-19
 *      Author: YangZhiyong
 */


#include <string.h>
#include "hal_sys.h"
#include "stm32f10x_gpio.h"
#include "app/app_terminal.h"
#include <os.h>


#define RESPONSE_LIST_DIV_CHAR ((uint8_t)',')

#define RESPONSE_MAX_LENGTH 0x80

struct TRANSPORT_IF* SIM90x_Transport = 0;

static uint8_t SIM90x_ResponseBuffer[RESPONSE_MAX_LENGTH];

static OS_TMR IdleTimer;
static OS_TICK IdleTick;


void SIM90x_StartIdleTime(void)
{
    OS_ERR error;
    IdleTick = OSTimeGet(&error);
}



uint32_t SIM90x_GetIdleTime(void)
{
    OS_ERR error;
    OS_TICK tick;
    uint32_t idle_ms;

    tick = OSTimeGet(&error) - IdleTick;
    idle_ms = tick * (OSCfg_TickRate_Hz/1000);

    return idle_ms;
}



void SIM90x_WaitIdleTime(uint32_t ms)
{
    OS_ERR error;
    OS_TICK tick;

    tick = ms/(OSCfg_TickRate_Hz/1000) + IdleTick;

    while( tick < OSTimeGet(&error) )
    {
        SYS_DelayMs(10);
    }

}



void SIM90x_Write(   uint8_t* const buffer,
                     uint32_t length,
                     uint32_t timeout   )
{
    Term_Write(SIM90x_Transport, buffer, length, timeout);
}



void SIM90x_WriteString(   uint8_t* const string,
                           uint32_t timeout   )
{
    Term_WriteString(SIM90x_Transport, string, timeout);
}



void SIM90x_WriteLine(uint8_t* const line,
                      uint32_t timeout )
{
    Term_WriteLine(SIM90x_Transport, line, timeout);
}



int SIM90x_ReadLine( uint8_t* const line,
                     uint32_t length,
                     uint32_t timeout )
{
    return Term_ReadLine(SIM90x_Transport, line, length, timeout);
}



uint32_t SIM90x_WaitResponse1(uint32_t timeout, char* const response)
{
    int recv_length;
    int resp_length;
    int result;

    resp_length = strnlen( response, sizeof(SIM90x_ResponseBuffer) );

    do
    {
        recv_length = Term_ReadLine(SIM90x_Transport, SIM90x_ResponseBuffer, sizeof(SIM90x_ResponseBuffer), timeout);

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
            result = memcmp( (char const*)SIM90x_ResponseBuffer, (char const*)response, resp_length);
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



uint32_t SIM90x_WaitResponse2(uint32_t timeout, char* const response1, char* const response2 )
{
    int recv_length;
    int resp1_length;
    int resp2_length;
    int result;

    resp1_length = strnlen( response1, sizeof(SIM90x_ResponseBuffer) );
    resp2_length = strnlen( response2, sizeof(SIM90x_ResponseBuffer) );

    do
    {
        recv_length = Term_ReadLine(SIM90x_Transport, SIM90x_ResponseBuffer, sizeof(SIM90x_ResponseBuffer), timeout);

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
            result = memcmp( (char const*)SIM90x_ResponseBuffer, (char const*)response1, resp1_length);
            if ( result == 0 )
            {// 期待的回应
                return 1;
            }

            result = memcmp( (char const*)SIM90x_ResponseBuffer, (char const*)response2, resp2_length);
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



uint8_t list_string(uint8_t* string, uint8_t* list, uint8_t div)
{
    uint32_t str_len;
    int result;
    uint8_t* seek_head;
    uint8_t* seek_end;
    uint8_t idx = 1;

    if( (string==0) | (list==0) )
        return 0;

    seek_head = list;

    do
    {
        seek_end = (uint8_t*)strchr((const char*)seek_head, div);
        if( seek_end != 0 )
        {// get div
            str_len = seek_end - seek_head;
        }
        else
        {
            str_len = strlen((const char*)seek_head);
        }

        // compare
        result = memcmp( string, seek_head, str_len);
        if ( result == 0 )
        {
            return idx;
        }

        // move to next string
        if( seek_end != 0 )
        {
            idx ++;
            seek_end ++;
            seek_head = seek_end; /// skip div char
        }
        else
        {
            return 0;
        }

    } while(1);

}




uint32_t SIM90x_WaitResponseList(uint32_t timeout, uint8_t* const response_list)
{
    int recv_length;
    uint8_t result;

    do
    {
        recv_length = Term_ReadLine(SIM90x_Transport, SIM90x_ResponseBuffer, sizeof(SIM90x_ResponseBuffer), timeout);

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
            result = list_string(   SIM90x_ResponseBuffer,
                                    response_list,
                                    RESPONSE_LIST_DIV_CHAR);

            if( result != 0 )
            {
                return result;
            }
        }

    }
    while(1);
}



uint32_t SIM90x_Flush(void)
{
    return SIM90x_Transport->flush();
}



////////////////////////////////////////////////////////////////////////////







void initPort(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    /*********************************************************
     * GPIOs For SIM908 GSM
     *********************************************************/
    /* SIM908 POWER */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    GPIO_SetBits(GPIOC, GPIO_Pin_13);

    /* SIM908 PWRKEY */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOE, &GPIO_InitStructure);


    /* SIM908 STATUS */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOE, &GPIO_InitStructure);
}



void pressPWRKEY(void)
{
    GPIO_SetBits(GPIOE, GPIO_Pin_15);

}



void releasePWRKEY(void)
{
    GPIO_ResetBits(GPIOE, GPIO_Pin_15);
}



/**
 * TODO 失败处理
 */
void SIM90x_PowerUp(void)
{
    uint32_t resp;

    do{
        pressPWRKEY();
        SYS_DelayMs(1200);
        releasePWRKEY();

        SYS_DelayMs(2500);

        SIM90x_WriteLine("AT", 1000);
        resp = SIM90x_WaitResponseList(1000, "OK");

    }while(resp != 1);

}



void SIM90x_PowerDown(void)
{
    uint32_t resp;

    do{
        pressPWRKEY();
//        resp = SIM90x_WaitResponse1(1000, "NORMAL POWER DOWN");
        resp = SIM90x_WaitResponseList(1000, "NORMAL POWER DOWN");
        releasePWRKEY();
    }while(resp != 1);

}



void SIM90x_Init(const struct TRANSPORT_IF *tp)
{
    uint32_t resp;
    OS_ERR error;

    SIM90x_Transport = (struct TRANSPORT_IF*)tp;

    ///////////////
    OSTmrCreate(&IdleTimer,
                "IdleTimer",
                SYS_MsToTick(1200),
                SYS_MsToTick(1200),
                OS_OPT_TMR_ONE_SHOT,
                (OS_TMR_CALLBACK_PTR)0,
                (void*)0,
                &error  );

    ///////////////
    initPort();
    Term_Init(SIM90x_Transport);

    ////////////////
    SIM90x_PowerUp();

    do
    {
        // Echo mode off
        SIM90x_Flush();
        SIM90x_WriteLine("ATE0", 1000);
        resp = SIM90x_WaitResponseList(1000, "OK");
        if( resp == 0 )
            continue;

        //  Disable network registration unsolicited result code
        SIM90x_Flush();
        SIM90x_WriteLine("AT+CREG=0", 1000);
        resp = SIM90x_WaitResponseList(1000, "OK");
        if( resp == 0 )
            continue;


        SIM90x_Flush();
        SIM90x_WriteLine("AT+CPIN? ", 1000);
        resp = SIM90x_WaitResponseList(1000, "+CPIN: READY");
        if( resp == 0 )
            continue;

        break;
    } while(1);

    SIM90x_StartIdleTime();

}









