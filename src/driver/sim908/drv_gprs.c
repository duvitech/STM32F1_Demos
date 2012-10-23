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

void GPRS_Connect(uint8_t* addr, uint8_t* port)
{
    uint32_t resp;

    do
    {
        Debug_Print("AT+CSQ\n");
        SIM90x_Flush();
        SIM90x_WriteLine("AT+CSQ", 1000);
        resp = SIM90x_WaitResponseList(1000,    "+CSQ:,"
                                                "ERROR");
        if( resp != 1 )
            continue;


        Debug_Print("AT+CREG?\n");
        SIM90x_Flush();
        SIM90x_WriteLine("AT+CREG?", 1000);
        resp = SIM90x_WaitResponseList(1000,    "+CREG,"
                                                "ERROR");
        if( resp != 1 )
            continue;


        // Enable transparent mode
        Debug_Print("Enable transparent mode\n");
        SIM90x_Flush();
        SIM90x_WriteLine("AT+CIPMODE=1", 1000);
        resp = SIM90x_WaitResponseList(1000,    "OK,"
                                                "ERROR");
        if( resp != 1 )
            continue;


        Debug_Print("AT+CGATT?\n");
        SIM90x_Flush();
        SIM90x_WriteLine("AT+CGATT?", 1000);
        resp = SIM90x_WaitResponseList(1000,    "+CGATT: 1,"
                                                "+CGATT: 0,"
                                                "ERROR");
        if( resp != 1 )
            continue;


        // Start Task and Set APN, USER NAME, PASSWORD
        Debug_Print("AT+CSTT?\n");
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
        Debug_Print("AT+CIPSTART\n");
        SIM90x_Flush();
        SIM90x_WriteString("AT+CIPSTART=", 1000);
        SIM90x_WriteString("\"TCP\",\"", 1000);
        SIM90x_WriteString(addr, 1000); // addr
        SIM90x_WriteString("\",\"", 1000);
        SIM90x_WriteString(port, 1000); // port
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



bool GPRS_IsTMode(void)
{
    return TMode;
}



void GPRS_SwitchTMode(bool onoff)
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


