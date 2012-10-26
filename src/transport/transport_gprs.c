/*
 * transport_gprs.c
 *
 *  Created on: 2012-10-22
 *      Author: YangZhiyong
 */


#include "transport_gprs.h"
#include "service/launcher.h"
#include "hal_sys.h"
#include "service/fifo.h"

#define TP_GPRS_FLAG_RECVDONE       0x01
#define TP_GPRS_FLAG_SENDDONE       0x02
#define TP_GPRS_FLAG_DISCONECT      0x04
#define TP_GPRS_FLAG_ERROR          0x80

APP_FLAG_DEFINE(TP_GPRS_Flags, 0)
FIFO_DEFINE(GPRS_RecvFIFO, GPRS_RECV_FIFO_LENGTH)

static struct GPRS_CONFIG       GPRS_Config;
static enum TRANSPORT_Status    GPRS_Status;


static void (*RecvDone_Handler)(void) = 0;

static void (*SendDone_Handler)(void) = 0;

static void (*Disconnect_Handler)(void) = 0;

static const uint8_t CLOSED[] = "\r\nCLOSED\r\n";

static uint8_t bridge_buffer[0x20];
static uint8_t bridge_length;


static void onRecvDone(void)
{
    OS_ERR error;

    if( GPRS_IsTMode() == true ) /// 透明模式处理接收数据
    {
        // 判断是否为连接断开提示符  "\r\nCLOSED\r\n"
        int result = 0xFF;

        bridge_length = SIM90x_Transport->recv( bridge_buffer, sizeof(bridge_buffer) );

        if( bridge_length == (sizeof(CLOSED)-1) )
        {
            result = memcmp(bridge_buffer, CLOSED, (sizeof(CLOSED)-1) );
        }

        if( result == 0 )
        {// 收到断开CLOSED
            GPRS_Status = TRANSPORT_Status_Disconnect;

            OSFlagPost(     &TP_GPRS_Flags,
                            TP_GPRS_FLAG_DISCONECT,
                            OS_OPT_POST_FLAG_SET,
                            &error);

            if( (Disconnect_Handler != 0) )
                Disconnect_Handler();
        }
        else
        {// 没有收到断开CLOSED，把数据转移到本地FIFO。

            FIFO_Put(&GPRS_RecvFIFO, bridge_buffer, bridge_length);

            while( bridge_length == sizeof(bridge_buffer) )
            {// 还有余下数据
                bridge_length = SIM90x_Transport->recv( bridge_buffer, sizeof(bridge_buffer) );
                FIFO_Put(&GPRS_RecvFIFO, bridge_buffer, bridge_length);
            }

            GPRS_Status = TRANSPORT_Status_RecvDone;

            OSFlagPost(     &TP_GPRS_Flags,
                            TP_GPRS_FLAG_RECVDONE,
                            OS_OPT_POST_FLAG_SET,
                            &error);

            if( (RecvDone_Handler != 0) )
                RecvDone_Handler();
        }

    }

    SIM90x_StartIdleTime();
}



static void onSendDone(void)
{
    OS_ERR error;

    if( GPRS_IsTMode() == true )
    {
        GPRS_Status = TRANSPORT_Status_SendDone;

        OSFlagPost(     &TP_GPRS_Flags,
                        TP_GPRS_FLAG_SENDDONE,
                        OS_OPT_POST_FLAG_SET,
                        &error);

        if( (SendDone_Handler != 0) )
            SendDone_Handler();
    }

    SIM90x_StartIdleTime();
}



static uint8_t init(void* config)
{
    OS_ERR error;
    struct GPRS_CONFIG* config_set = config;

    GPRS_Connect(config_set->addr, config_set->port);

    GPRS_Status = TRANSPORT_Status_Idle;

    SIM90x_Transport->setEventHandler(TRANSPORT_Event_RecvDone, onRecvDone);
    SIM90x_Transport->setEventHandler(TRANSPORT_Event_SendDone, onSendDone);

    OSFlagPost(     &TP_GPRS_Flags,
                    TP_GPRS_FLAG_DISCONECT,
                    OS_OPT_POST_FLAG_CLR,
                    &error);

    return 0;
}



static uint32_t send(uint8_t* buffer, uint32_t length)
{
    OS_ERR error;

    if( GPRS_IsTMode() == true )
    {
        GPRS_Status = TRANSPORT_Status_SendBusy;

        OSFlagPost(     &TP_GPRS_Flags,
                        TP_GPRS_FLAG_SENDDONE,
                        OS_OPT_POST_FLAG_CLR,
                        &error);

        return SIM90x_Transport->send(buffer, length);
    }
    else
    {
        return 0;
    }
}



static uint32_t recv(uint8_t* buffer, uint32_t length)
{
    OS_ERR error;
    uint32_t recv_length;

    if( GPRS_IsTMode() == true )
    {
        recv_length = FIFO_Get(&GPRS_RecvFIFO, buffer, length);

        OSFlagPost(     &TP_GPRS_Flags,
                        TP_GPRS_FLAG_RECVDONE,
                        OS_OPT_POST_FLAG_CLR,
                        &error);

        return recv_length;
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
    OS_ERR error;
    OS_FLAGS flags = 0;
    enum TRANSPORT_Event ret_event;
    uint32_t tick_timeout;

    tick_timeout = SYS_MsToTick(timeout);
    if( (timeout != 0) && (tick_timeout == 0) )
    {
        tick_timeout = 1;
    }

    switch( event )
    {
    case TRANSPORT_Event_RecvDone:
        flags = OSFlagPend( &TP_GPRS_Flags,
                        TP_GPRS_FLAG_RECVDONE,
                        tick_timeout,
                        OS_OPT_PEND_FLAG_SET_AND,
                        (void*)0,
                        &error  );
        break;

    case TRANSPORT_Event_SendDone:
        flags = OSFlagPend( &TP_GPRS_Flags,
                        TP_GPRS_FLAG_SENDDONE,
                        tick_timeout,
                        OS_OPT_PEND_FLAG_SET_AND,
                        (void*)0,
                        &error  );
        break;

    case TRANSPORT_Event_Disconnect:
        flags = OSFlagPend( &TP_GPRS_Flags,
                        TP_GPRS_FLAG_DISCONECT,
                        tick_timeout,
                        OS_OPT_PEND_FLAG_SET_AND,
                        (void*)0,
                        &error  );
        break;

    case TRANSPORT_Event_Error:
        flags = OSFlagPend( &TP_GPRS_Flags,
                        TP_GPRS_FLAG_ERROR,
                        tick_timeout,
                        OS_OPT_PEND_FLAG_SET_AND,
                        (void*)0,
                        &error  );
        break;

    default:
        break;
    }

    if( flags == 0 )
    {
        ret_event = TRANSPORT_Event_Timeout;
    }
    else
    {
        ret_event = event;
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



