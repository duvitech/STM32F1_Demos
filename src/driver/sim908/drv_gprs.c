/*
 * drv_gprs.c
 *
 *  Created on: 2012-9-19
 *      Author: YangZhiyong
 */

#include <stdint.h>
#include <stdbool.h>
#include "driver/sim908/drv_sim90x.h"
#include <os.h>
#include "hal_sys.h"
#include "transport/transport_usart2.h"
#include "driver/sim908/drv_gprs.h"


static bool TMode = false;

static struct GPRS_CONFIG       GPRS_Config;
static enum TRANSPORT_Status    GPRS_Status;

void GPRS_Connect(void)
{
    uint32_t resp;

    do
    {
        SIM90x_Flush();
        SIM90x_WriteLine("AT+CSQ", 1000);
        resp = SIM90x_WaitResponseList(1000,    "+CSQ:,"
                                                "ERROR");
        if( resp != 1 )
            continue;


        SIM90x_Flush();
        SIM90x_WriteLine("AT+CREG?", 1000);
        resp = SIM90x_WaitResponseList(1000,    "+CREG,"
                                                "ERROR");
        if( resp != 1 )
            continue;


        // Enable transparent mode
        SIM90x_Flush();
        SIM90x_WriteLine("AT+CIPMODE=1", 1000);
        resp = SIM90x_WaitResponseList(1000,    "OK,"
                                                "ERROR");
        if( resp != 1 )
            continue;


        SIM90x_Flush();
        SIM90x_WriteLine("AT+CGATT?", 1000);
        resp = SIM90x_WaitResponseList(1000,    "+CGATT: 1,"
                                                "+CGATT: 0,"
                                                "ERROR");
        if( resp != 1 )
            continue;


        // Start Task and Set APN, USER NAME, PASSWORD
        SIM90x_Flush();
        SIM90x_WriteLine("AT+CSTT?", 1000);
        resp = SIM90x_WaitResponseList(3000,    "+CSTT,"
                                                "ERROR");
        if( resp != 1 )
            continue;


        //  Bring  up  wireless  connection  (GPRS  or CSD)
//        SIM90x_Flush();
//        SIM90x_WriteLine("AT+CIICR", 1000);
//        resp = SIM90x_WaitResponseList(3000,    "OK,"
//                                                "ERROR");
//        if( resp != 1 )
//            continue;

        /// AT+CIPSTART=”TCP”,”<ADDR>”,”<PORT>”
        SIM90x_Flush();
        SIM90x_WriteString("AT+CIPSTART=", 1000);
        SIM90x_WriteString("\"TCP\",\"", 1000);
        SIM90x_WriteString(GPRS_Config.addr, 1000); // addr
        SIM90x_WriteString("\",\"", 1000);
        SIM90x_WriteString(GPRS_Config.port, 1000); // port
        SIM90x_WriteString("\"\r\n", 1000);

        resp = SIM90x_WaitResponseList(5000,    "ERROR,"
                                                "STATE:,"
                                                "CONNECT FAIL,"
                                                "CONNECT OK,"
                                                "ALREADY CONNECT,"
                                                "CONNECT");
        if( resp < 4 )
            continue;

        break;

    } while(1);

    /// 连接成功后自动进入透明模式
    TMode = true;
}




void GPRS_SwitchTM(bool onoff)
{
    uint32_t resp;

    if( TMode == onoff )
    {
        return;
    }

    do
    {
        SIM90x_WaitIdleTime(1200);

        SIM90x_WriteLine("+++", 1000);
        resp = SIM90x_WaitResponse1(1000, "OK");

        if( resp == 0 )
            continue;

    }while(0);

    TMode = onoff;

    SYS_DelayMs(600);
}



/////////////////////////////////////////////////////////////////////////////

static void (*RecvDone_Handler)(void) = 0;

static void (*SendDone_Handler)(void) = 0;

static void (*Disconnect_Handler)(void) = 0;

static const uint8_t CLOSED[] = "\r\nCLOSED\r\n";

static uint8_t CheckClosed[] = "\r\nCLOSED\r\n";
static uint8_t CheckLength;



static void onRecvDone(void)
{
    if( TMode ) /// 透明模式处理接收数据
    {
        // 判断是否为连接断开提示符  "\r\nCLOSED\r\n"
        int result = 0xFF;

        CheckLength = SIM90x_Transport->recv( CheckClosed, sizeof(CLOSED)-1 );
        if( CheckLength == (sizeof(CLOSED)-1) )
        {
            result = memcmp(CheckClosed, CLOSED, (sizeof(CLOSED)-1) );
        }


        if( result == 0 )
        {// 收到断开CLOSED
            GPRS_Status = TRANSPORT_Status_Disconnect;
            if( (Disconnect_Handler != 0) )
                Disconnect_Handler();
        }
        else
        {// 没有收到断开CLOSED
            GPRS_Status = TRANSPORT_Status_RecvDone;
            if( (RecvDone_Handler != 0) )
                RecvDone_Handler();
        }

    }

    SIM90x_StartIdleTime();
}



static void onSendDone(void)
{
    if( TMode && (SendDone_Handler != 0) )
    {
        GPRS_Status = TRANSPORT_Status_SendDone;
        SendDone_Handler();
    }

    SIM90x_StartIdleTime();
}



static uint8_t init(void* config)
{
    struct GPRS_CONFIG* config_set = config;

    memset((void*)&GPRS_Config, 0x00, sizeof(GPRS_Config));

    strncpy((char*)GPRS_Config.addr,
            (char*)config_set->addr,
            sizeof(GPRS_Config.addr)    );

    strncpy((char*)GPRS_Config.port,
            (char*)config_set->port,
            sizeof(GPRS_Config.port)    );

    GPRS_Connect();

    GPRS_Status = TRANSPORT_Status_Idle;

    SIM90x_Transport->setEventHandler(TRANSPORT_Event_RecvDone, onRecvDone);
    SIM90x_Transport->setEventHandler(TRANSPORT_Event_SendDone, onSendDone);

    return 0;
}



static uint32_t send(uint8_t* buffer, uint32_t length)
{
    if( TMode == true )
    {
        GPRS_Status = TRANSPORT_Status_SendBusy;
        return SIM90x_Transport->send(buffer, length);
    }
    else
    {
        return 0;
    }
}



static uint32_t recv(uint8_t* buffer, uint32_t length)
{
    uint32_t recv_length;

    if( TMode == true )
    {
        memcpy(buffer, CheckClosed, CheckLength);
        recv_length = SIM90x_Transport->recv(buffer, (length - CheckLength) );

        return (recv_length + CheckLength);
    }
    else
    {
        return 0;
    }
}



static void setEventHandler(enum TRANSPORT_Event event, void (*handler)(void) )
{
    switch( event )
    {
    case TRANSPORT_Event_RecvDone:
        RecvDone_Handler = handler;
        break;

    case TRANSPORT_Event_SendDone:
        SendDone_Handler = handler;
        break;

    case TRANSPORT_Event_Disconnect:
        Disconnect_Handler = handler;
        break;

    case TRANSPORT_Event_Error:

        break;

    default:
        break;
    }
}



static enum TRANSPORT_Status getStatus(void)
{
    return GPRS_Status;
}



static enum TRANSPORT_Event waitEventTrigger(enum TRANSPORT_Event event, uint32_t timeout)
{
    enum TRANSPORT_Event ret_event;

    if( event == TRANSPORT_Event_Disconnect )
    {// @ TODO 暂时不支持等待断开连接
        ret_event = TRANSPORT_Event_NONE;
    }
    else
    {
        ret_event = SIM90x_Transport->waitEventTrigger(event, timeout);
    }

    return ret_event;
}



const struct TRANSPORT_IF TP_GPRS =
{
                .name       = "GPRS",
                .init       = init,
                .send       = send,
                .recv       = recv,
                .status     = getStatus,
                .setEventHandler    = setEventHandler,
                .waitEventTrigger   = waitEventTrigger,
};




