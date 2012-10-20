/*
 * app_test_sim908.c
 *
 *  Created on: 2012-10-12
 *      Author: YangZhiyong
 */
#include "app_terminal.h"
#include "service/launcher.h"
#include "transport/transport_usart2.h"
#include "driver/sim908/drv_sim90x.h"
#include "driver/sim908/drv_gprs.h"

void TestSIM908(void *p_arg);

APP_TASK_DEFINE(TestSIM908,       /*name name##_TCB*/
                TestSIM908,       /*p_task*/
                0,              /*p_arg*/
                8,              /*prio*/
                512,            /*stk_size_32*/
                0,              /*q_size*/
                0,              /*time_quanta*/
                0,              /*p_ext*/
                (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR) /*opt*/
)



uint8_t test0[] = "ABCDEFGHIJK";

uint8_t test1[] = "0123456789ABCDEFGHIJK";


struct GPRS_CONFIG eeyyyee_xicp_net =
{
                .addr = "eeyyyee.xicp.net",
                .port = "20000",
};


uint8_t list[] = "aaaa,bbbb,ccccc,dddd,eeee,f,g,0123";


//void onDisconneted(void)
//{
//
//}



void TestSIM908(void *p_arg)
{

    OS_ERR error;

    (void)p_arg;


//    uint32_t temp;
//    temp = list_string("aaaaa", list, ',');
//    temp = list_string("bbbbb", list, ',');
//    temp = list_string("cccccd", list, ',');
//    temp = list_string("dddde", list, ',');
//    temp = list_string("fee", list, ',');
//    temp = list_string("gd", list, ',');
//    temp = list_string("0123as", list, ',');
//    temp = list_string("0000df", list, ',');
//
//    (void)temp;
//    while(1);

    TP_USART2.init((void*)0);
    SIM90x_Init(&TP_USART2);

//    TP_GPRS.setEventHandler()
    TP_GPRS.init(&eeyyyee_xicp_net);


    while(1)
    {
        OSTimeDly(5000, OS_OPT_TIME_DLY, &error);
        TP_GPRS.send(test0, strlen((const char*)test0));
        OSTimeDly(5000, OS_OPT_TIME_DLY, &error);
        TP_GPRS.send(test1, strlen((const char*)test1));

        if( TP_GPRS.status() == TRANSPORT_Status_Disconnect )
        {// 自动重连
            TP_GPRS.init(&eeyyyee_xicp_net);
        }

    }

}

