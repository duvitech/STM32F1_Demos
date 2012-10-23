/*
 * debug.c
 *
 *  Created on: 2012-10-22
 *      Author: YangZhiyong
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdarg.h>
#include "debug.h"
#include "fifo.h"
#include "launcher.h"
#include "os.h"

FIFO_DEFINE( DebugFIFO, (DEBUG_BUFFER_LENGTH*4) )

APP_MUTEX_DEFINE(Debug_Mutex)

static uint8_t buffer_in[DEBUG_BUFFER_LENGTH];   //  ‰»Îª∫≥Â
static uint8_t buffer_out[DEBUG_BUFFER_LENGTH];     //  ‰≥ˆª∫≥Â

static struct TRANSPORT_IF* TP_Debug = 0;


static bool TP_Debug_Busy;


void sendFIFO(void)
{
    if( FIFO_Length(&DebugFIFO) > 0 )
    {
        uint32_t length;
        length = FIFO_Get(&DebugFIFO, buffer_out, DEBUG_BUFFER_LENGTH);
        TP_Debug_Busy = true;
        TP_Debug->send(buffer_out, length);
    }
    else
    {
        TP_Debug_Busy = false;
    }
}



void Debug_Init(struct TRANSPORT_IF* tp)
{
    TP_Debug = tp;
    if( TP_Debug != 0 )
    {
        TP_Debug_Busy = false;
        TP_Debug->setEventHandler(TRANSPORT_Event_SendDone, sendFIFO);
    }
}



int Debug_Print(const char *format, ...)
{
    if( TP_Debug == 0 )
    {
        return 0;
    }

    OS_ERR error;
    uint32_t length;
    uint32_t ret_length;

    va_list args;
    va_start(args, format);
    length = vsnprintf((char*)buffer_in, sizeof(buffer_in), (char*)format, args);
    va_end(args);

    // lock DebugFIFO
    OSMutexPend(&Debug_Mutex, 0, OS_OPT_PEND_BLOCKING, 0, &error);

    ret_length = FIFO_Put(&DebugFIFO, buffer_in, length);   /// @ TODO ºÏ≤È∑µªÿ÷µ

    if( TP_Debug_Busy == false )
    {
        sendFIFO();
    }

    // unlock DebugFIFO
    OSMutexPost(&Debug_Mutex, OS_OPT_POST_NONE, &error);

    return ret_length;
}




