/*
 * app_led.c
 *
 *  Created on: 2012-9-22
 *      Author: YangZhiyong
 */
#include "service/launcher.h"
#include <stdint.h>
#include "hal_led.h"

void LED_Task(void *p_arg);

APP_TASK_DEFINE(LED_Task,       /*name name##_TCB*/
                LED_Task,       /*p_task*/
                0,              /*p_arg*/
                8,              /*prio*/
                512,            /*stk_size_32*/
                0,              /*q_size*/
                0,              /*time_quanta*/
                0,              /*p_ext*/
                (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR) /*opt*/
)


void LED_Task(void *p_arg)
{
    OS_ERR error;
    uint8_t cnt = 0;

    while(1)
    {
//        OSTimeDlyHMSM   (0, 0, 0, 200, OS_OPT_TIME_DLY, &error);
        OSTimeDly(200, OS_OPT_TIME_DLY, &error);
        LED_On(cnt%5);
//        OSTimeDlyHMSM   (0, 0, 0, 200, OS_OPT_TIME_DLY, &error);
        OSTimeDly(200, OS_OPT_TIME_DLY, &error);
        LED_Off(cnt%5);

        cnt ++;
    }
}


