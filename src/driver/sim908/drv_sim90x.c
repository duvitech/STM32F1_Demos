/*
 * drv_sim90x.c
 *
 *  Created on: 2012-9-19
 *      Author: YangZhiyong
 */


#include <string.h>
#include <stdarg.h>
#include "hal_sys.h"
#include "stm32f10x_gpio.h"
#include "app/app_terminal.h"
#include <os.h>
#include "service/debug.h"
#include "drv_sim90x.h"
#include "service/launcher.h"
#include "hal_intrrupt.h"

#define SIM90X_POWER_FLAG   (0x01)

APP_FLAG_DEFINE(SIM90X_Flags, 0)

struct TRANSPORT_IF* SIM90x_Transport = 0;

uint8_t SIM90x_ResponseBuffer[RESPONSE_MAX_LENGTH];

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
    idle_ms = SYS_TickToMs(tick);

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



uint32_t SIM90x_WaitResponseList(uint32_t timeout, ... )
{
    int recv_length;
    int result;
    uint32_t count;
    char* arg;
    va_list args;


    do
    {
        recv_length = Term_ReadLine(    SIM90x_Transport,
                                        SIM90x_ResponseBuffer,
                                        sizeof(SIM90x_ResponseBuffer),
                                        timeout     );
        if( recv_length < 0 )
        {// 超时
            return 0;
        }
        else if( recv_length == 0 )
        {// 空行
            continue;
        }
        else
        {// 比较参数列表
            count = 1;
            va_start(args, timeout);
            do
            {
                arg = va_arg( args, char* );    // 取下一个参数
                if( arg != 0 )
                {
                    result = memcmp( arg, SIM90x_ResponseBuffer, strlen(arg) );
                    if( result == 0 )
                    {// 比较相等
                        return count;
                    }
                    count ++;
                }
                else
                {// 参数列表结束
                    break;
                }
            } while(1);
            va_end(args);
        }
    }
    while(1);

}



uint32_t SIM90x_Flush(void)
{
    uint32_t idx;
    uint8_t* ptr;

    ptr = SIM90x_ResponseBuffer;

    for( idx = 0; idx < sizeof(SIM90x_ResponseBuffer); idx ++ )
    {
        if( *ptr != 0x00 )
        {
            *ptr = 0x00;
            ptr ++;
        }
        else
        {
            break;
        }
    }

    return SIM90x_Transport->flush();
}



////////////////////////////////////////////////////////////////////////////


void onStatusChanged(void)
{
    uint8_t pin;
    OS_ERR error;

    pin = GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_14);

    if( pin == Bit_RESET )
    {
        OSFlagPost(     &SIM90X_Flags,
                        SIM90X_POWER_FLAG,
                        OS_OPT_POST_FLAG_CLR,
                        &error);
    }
    else
    {
        OSFlagPost(     &SIM90X_Flags,
                        SIM90X_POWER_FLAG,
                        OS_OPT_POST_FLAG_SET,
                        &error);
    }
}



static bool SIM90x_WaitPowerUp(uint32_t timeout)
{
    OS_ERR error;
    OS_FLAGS flags;

    timeout = SYS_MsToTick(timeout);

    flags = OSFlagPend( &SIM90X_Flags,
                    SIM90X_POWER_FLAG,
                    timeout,
                    OS_OPT_PEND_FLAG_SET_AND,
                    (void*)0,
                    &error  );

    return (flags == SIM90X_POWER_FLAG);
}



static bool SIM90x_WaitPowerDown(uint32_t timeout)
{
    OS_ERR error;
    OS_FLAGS flags;

    timeout = SYS_MsToTick(timeout);

    flags = OSFlagPend( &SIM90X_Flags,
                        SIM90X_POWER_FLAG,
                        timeout,
                        OS_OPT_PEND_FLAG_CLR_AND,
                        (void*)0,
                        &error  );

    return (flags == SIM90X_POWER_FLAG);
}



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

    Intrrupt_SetExtiHandler(14, onStatusChanged);
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
    uint32_t retry = 0;

    do
    {
        Debug(DEBUG_SIM908, "SIM908 PowerUp retry %d times !!! \n", retry++);
        releasePWRKEY();
        SYS_DelayMs(500);
        pressPWRKEY();
    } while(SIM90x_WaitPowerUp(2000) == false);

    releasePWRKEY();

    retry = 0;
    do{
        SYS_DelayMs(800);
        Debug(DEBUG_SIM908, "SIM908 Autobauding retry %d times !!! \n", retry++);

//        pressPWRKEY();
//        SYS_DelayMs(1200);
//        releasePWRKEY();
//        SYS_DelayMs(5000);

        SIM90x_WriteLine("AT", 200);
        resp = SIM90x_WaitResponseList(100, "OK", 0);
        SIM90x_WriteLine("AT", 200);
        resp = SIM90x_WaitResponseList(100, "OK", 0);
        SIM90x_WriteLine("AT", 200);
        resp = SIM90x_WaitResponseList(100, "OK", 0);
        SIM90x_WriteLine("AT", 200);
        resp = SIM90x_WaitResponseList(100, "OK", 0);
        SIM90x_WriteLine("AT", 200);
        resp = SIM90x_WaitResponseList(100, "OK", 0);
        SIM90x_WriteLine("AT", 200);
        resp = SIM90x_WaitResponseList(100, "OK", 0);
        SIM90x_WriteLine("AT", 200);
        resp = SIM90x_WaitResponseList(100, "OK", 0);

    }while(resp != 1);

}



void SIM90x_PowerDown(void)
{
    uint32_t resp;
    uint32_t retry = 0;

    do{
        Debug(DEBUG_SIM908, "SIM908 PowerDown retry %d times !!! \n", retry);
        retry ++;

        pressPWRKEY();
        resp = SIM90x_WaitResponseList(1000, "NORMAL POWER DOWN", 0);
        releasePWRKEY();

    }while(resp != 1);

}



void SIM90x_Init(const struct TRANSPORT_IF *tp)
{
    uint32_t resp;
    uint32_t retry = 0;
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

    while(1)
    {
        SYS_DelayMs(500);
        Debug(DEBUG_SIM908, "\n\nInit SIM908 retry %d times !!!! \n", retry);
        retry ++;

        // Echo mode off
        Debug(DEBUG_SIM908, "\nATE0; Echo mode off\n");
        SIM90x_Flush();
        SIM90x_WriteLine("ATE0", 1000);
        resp = SIM90x_WaitResponseList(1000, "OK", 0);
        Debug(DEBUG_SIM908, "Response: %s\n", SIM90x_ResponseBuffer);
        if( resp == 0 )
            continue;

        //  Disable network registration unsolicited result code
        Debug(DEBUG_SIM908, "\nAT+CREG=0;    Network Registration \n");
        SIM90x_Flush();
        SIM90x_WriteLine("AT+CREG=0", 1000);
        resp = SIM90x_WaitResponseList(1000, "OK", 0);
        Debug(DEBUG_SIM908, "Response: %s\n", SIM90x_ResponseBuffer);
        if( resp == 0 )
            continue;


        Debug(DEBUG_SIM908, "\nAT+CPIN?    Enter PIN \n");
        SIM90x_Flush();
        SIM90x_WriteLine("AT+CPIN?", 1000);
        resp = SIM90x_WaitResponseList(1000, "+CPIN: READY", 0);
        Debug(DEBUG_SIM908, "Response: %s\n", SIM90x_ResponseBuffer);
        if( resp == 0 )
            continue;

        break;

    }

    SIM90x_StartIdleTime();

}


