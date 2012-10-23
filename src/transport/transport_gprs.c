/*
 * transport_gprs.c
 *
 *  Created on: 2012-10-22
 *      Author: YangZhiyong
 */


#include "transport_gprs.h"


static struct GPRS_CONFIG       GPRS_Config;
static enum TRANSPORT_Status    GPRS_Status;


static void (*RecvDone_Handler)(void) = 0;

static void (*SendDone_Handler)(void) = 0;

static void (*Disconnect_Handler)(void) = 0;

static const uint8_t CLOSED[] = "\r\nCLOSED\r\n";

static uint8_t CheckClosed[] = "\r\nCLOSED\r\n";
static uint8_t CheckLength;



static void onRecvDone(void)
{
    if( GPRS_IsTMode() == true ) /// 透明模式处理接收数据
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
    if( (GPRS_IsTMode() == true) && (SendDone_Handler != 0) )
    {
        GPRS_Status = TRANSPORT_Status_SendDone;
        SendDone_Handler();
    }

    SIM90x_StartIdleTime();
}



static uint8_t init(void* config)
{
    struct GPRS_CONFIG* config_set = config;

    GPRS_Connect(config_set->addr, config_set->port);

    GPRS_Status = TRANSPORT_Status_Idle;

    SIM90x_Transport->setEventHandler(TRANSPORT_Event_RecvDone, onRecvDone);
    SIM90x_Transport->setEventHandler(TRANSPORT_Event_SendDone, onSendDone);

    return 0;
}



static uint32_t send(uint8_t* buffer, uint32_t length)
{
    if( GPRS_IsTMode() == true )
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

    if( GPRS_IsTMode() == true )
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



