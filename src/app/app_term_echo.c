/*
 * app_term_echo.c
 *
 *  Created on: 2012-9-21
 *      Author: YangZhiyong
 */

#include "app_terminal.h"
#include "service/launcher.h"
#include "transport/transport_usart2.h"

void TermEcho(void *p_arg);

//APP_TASK_DEFINE(TermEcho,       /*name name##_TCB*/
//                TermEcho,       /*p_task*/
//                0,              /*p_arg*/
//                8,              /*prio*/
//                512,            /*stk_size_32*/
//                0,              /*q_size*/
//                0,              /*time_quanta*/
//                0,              /*p_ext*/
//                (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR) /*opt*/
//)
//
//
//static uint8_t LineBuffer[0x80];
//
//
//void TermEcho(void *p_arg)
//{
//    uint32_t read_length;
//    (void)p_arg;
//
//    Term_Init(&TP_USART2);
//
//    Term_WriteLine(&TP_USART2, "Hello World", 0);
//
//    while(1)
//    {
//        read_length = Term_ReadLine(&TP_USART2, LineBuffer, sizeof(LineBuffer), 0);
//        Term_WriteLine(&TP_USART2, LineBuffer, read_length, 0);
//    }
//
//}









