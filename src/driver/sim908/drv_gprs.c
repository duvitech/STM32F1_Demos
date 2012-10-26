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
#include "service/debug.h"


static bool TMode = false;

void GPRS_Connect(uint8_t* addr, uint8_t* port)
{
    uint32_t resp;
    uint32_t retry = 0;

    do
    {
        SYS_DelayMs(500);
        Debug(DEBUG_GPRS, "\n\nInit GPRS retry %d times !!!! \n", retry);
        retry ++;

        Debug(DEBUG_GPRS, "AT+CSQ; Signal Quality Report.\n");
        SIM90x_Flush();
        SIM90x_WriteLine("AT+CSQ", 1000);
        resp = SIM90x_WaitResponseList(1000,    "+CSQ:",
                                                "ERROR", 0);
        Debug(DEBUG_GPRS, "Response: %s\n", SIM90x_ResponseBuffer);
        if( resp != 1 )
            continue;

        SYS_DelayMs(200);

        Debug(DEBUG_GPRS, "\nAT+CREG?; Network Registration status\n");
        SIM90x_Flush();
        SIM90x_WriteLine("AT+CREG?", 1000);
        resp = SIM90x_WaitResponseList(1000,    "+CREG: 0,1",
                                                "ERROR", 0);
        Debug(DEBUG_GPRS, "Response: %s\n", SIM90x_ResponseBuffer);
        if( resp != 1 )
            continue;


        // Enable transparent mode
        Debug(DEBUG_GPRS, "\nAT+CIPMODE=1; Enable transparent mode.\n");
        SIM90x_Flush();
        SIM90x_WriteLine("AT+CIPMODE=1", 1000);
        resp = SIM90x_WaitResponseList(1000,    "OK",
                                                "ERROR", 0);
        Debug(DEBUG_GPRS, "Response: %s\n", SIM90x_ResponseBuffer);
        if( resp != 1 )
            continue;


        // Attach from GPRS Service.
        Debug(DEBUG_GPRS, "\nAT+CGATT?; Attach from GPRS Service.\n");
        SIM90x_Flush();
        SIM90x_WriteLine("AT+CGATT?", 1000);
        resp = SIM90x_WaitResponseList(1000,    "+CGATT: 1",
                                                "+CGATT: 0",
                                                "ERROR", 0);
        Debug(DEBUG_GPRS, "Response: %s\n", SIM90x_ResponseBuffer);
        if( resp != 1 )
            continue;


        // Start Task and Set APN.
//        Debug(DEBUG_GPRS, "\nAT+CSTT?; Start Task and Set APN.\n");
//        SIM90x_Flush();
//        SIM90x_WriteLine("AT+CSTT?", 1000);
//        resp = SIM90x_WaitResponseList(3000,    "+CSTT",
//                                                "ERROR", 0);
//        Debug(DEBUG_GPRS, "Response: %s\n", SIM90x_ResponseBuffer);
//        if( resp != 1 )
//            continue;


        //  Bring  up  wireless  connection  (GPRS  or CSD)
//        SIM90x_Flush();
//        SIM90x_WriteLine("AT+CIICR", 1000);
//        resp = SIM90x_WaitResponseList(3000,    "OK,"
//                                                "ERROR");
//        if( resp != 1 )
//            continue;

        SYS_DelayMs(500);
        /// AT+CIPSTART="TCP","180.136.156.186","20000"
        /// AT+CIPSTART="TCP","<ADDR>","<PORT>" Start Up TCP or UDP Connection
        Debug(DEBUG_GPRS, "\nAT+CIPSTART; Start Up TCP Connection <%s:%s>.\n", addr, port);
        SIM90x_Flush();
        SIM90x_WriteString("AT+CIPSTART=", 1000);
        SIM90x_WriteString("\"TCP\",\"", 1000);
        SIM90x_WriteString(addr, 1000); // addr
        SIM90x_WriteString("\",\"", 1000);
        SIM90x_WriteString(port, 1000); // port
        SIM90x_WriteString("\"\r\n", 1000);

        resp = SIM90x_WaitResponseList(500,     "OK", 0);
        Debug(DEBUG_GPRS, "Response: %s\n", SIM90x_ResponseBuffer);
        if( resp != 1 )
            continue;


        resp = SIM90x_WaitResponseList(8000,    "ERROR",
                                                "STATE:",
                                                "CONNECT FAIL",
                                                "CONNECT OK",
                                                "ALREADY CONNECT",
                                                "CONNECT", 0);
        Debug(DEBUG_GPRS, "Response: %s\n", SIM90x_ResponseBuffer);
        if( resp < 4 )
            continue;

        break;

    } while(1);

    /// 连接成功后自动进入透明模式
    TMode = true;
}



bool GPRS_IsTMode(void)
{
    return TMode;
}



void GPRS_SwitchTMode(bool onoff)
{
    uint32_t resp;
    uint32_t retry = 0;

    if( TMode == onoff )
    {
        return;
    }

    do
    {
//        SIM90x_WaitIdleTime(1200);
        SYS_DelayMs(1200);

        Debug(DEBUG_GPRS, "SwitchTMode Retry: %d\n", retry++);
        SIM90x_WriteLine("+++", 1000);
        resp = SIM90x_WaitResponseList(800, "OK", 0);

        if( resp == 0 )
            continue;

    }while(0);

    TMode = onoff;

    SYS_DelayMs(600);
}



/////////////////////////////////////////////////////////////////////////////


