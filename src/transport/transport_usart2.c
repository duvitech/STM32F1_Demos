/*
 * transport_usart2.c
 *
 *  Created on: 2012-9-21
 *      Author: YangZhiyong
 */


#include "service/launcher.h"
#include "transport/transport_if.h"
#include "hal_usart.h"


static enum TRANSPORT_Status status;


#define TP_USART2_FLAG_RECVDONE      0x01
#define TP_USART2_FLAG_SENDDONE      0x02
#define TP_USART2_FLAG_ERROR         0x80

APP_FLAG_DEFINE(TP_USART2_Flags, 0)


enum TRANSPORT_Status getStatus(void)
{
    return status;
}



static uint32_t recv(uint8_t* buffer, uint32_t max_length)
{
    return USART2_Recv(buffer, max_length);
}


static void (*RecvDone_Handler)(void) = 0;

static void onRecvDone(void)
{
    OS_ERR error;

    if( RecvDone_Handler != 0 )
        RecvDone_Handler();

    OSFlagPost(     &TP_USART2_Flags,
                    TP_USART2_FLAG_RECVDONE,
                    OS_OPT_POST_FLAG_SET,
                    &error);
}



static uint32_t send(void* buffer, uint32_t length)
{
    OS_ERR error;

    OSFlagPost(     &TP_USART2_Flags,
                    TP_USART2_FLAG_SENDDONE,
                    OS_OPT_POST_FLAG_CLR,
                    &error);

    return USART2_Send(buffer, length);
}


static void (*SendDone_Handler)(void) = 0;

static void onSendDone(void)
{
    OS_ERR error;

    if( SendDone_Handler != 0 )
        SendDone_Handler();

    OSFlagPost(     &TP_USART2_Flags,
                    TP_USART2_FLAG_SENDDONE,
                    OS_OPT_POST_FLAG_SET,
                    &error);
}



static uint8_t init(void* config)
{
    OS_ERR error;

    (void)config;

    USART2_SetRecvTimeoutISR(onRecvDone);
    USART2_SetSendDoneISR(onSendDone);

    return true;
}



void setEventHandler(enum TRANSPORT_Event event, void (*handler)(void) )
{
    switch( event )
    {
    case TRANSPORT_Event_RecvDone:
        RecvDone_Handler = handler;
        break;

    case TRANSPORT_Event_SendDone:
        SendDone_Handler = handler;
        break;

    case TRANSPORT_Event_Error:

        break;

    default:
        break;
    }
}



void waitEventTrigger(enum TRANSPORT_Event event)
{
    OS_ERR error;

    switch( event )
    {
    case TRANSPORT_Event_RecvDone:
        OSFlagPend(     &TP_USART2_Flags,
                        TP_USART2_FLAG_RECVDONE,
                        0,
                        OS_OPT_PEND_FLAG_SET_ANY,
                        (void*)0,
                        &error  );
        break;

    case TRANSPORT_Event_SendDone:
        OSFlagPend(     &TP_USART2_Flags,
                        TP_USART2_FLAG_SENDDONE,
                        0,
                        OS_OPT_PEND_FLAG_SET_ANY,
                        (void*)0,
                        &error  );
        break;

    case TRANSPORT_Event_Error:
        OSFlagPend(     &TP_USART2_Flags,
                        TP_USART2_FLAG_ERROR,
                        0,
                        OS_OPT_PEND_FLAG_SET_ANY,
                        (void*)0,
                        &error  );
        break;

    default:
        break;
    }
}



const struct TRANSPORT_IF TP_USART2 =
{
                .name       = "USART2",
                .init       = init,
                .send       = send,
                .recv       = recv,
                .status     = getStatus,
                .setEventHandler    = setEventHandler,
                .waitEventTrigger   = waitEventTrigger,
};




