/*
 * app_relay_test.c
 *
 *  Created on: 2012-10-25
 *      Author: YangZhiyong
 */

#include <stdint.h>
#include "service/launcher.h"
#include "hal_switch.h"
#include "hal_sys.h"

void Switch_Task(void *p_arg);


APP_TASK_DEFINE(Switch_Task,       /*name name##_TCB*/
                Switch_Task,       /*p_task*/
                0,              /*p_arg*/
                12,              /*prio*/
                0x40,            /*stk_size_32*/
                0,              /*q_size*/
                0,              /*time_quanta*/
                0,              /*p_ext*/
                (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR) /*opt*/
)


void Switch_Task(void *p_arg)
{
    (void)p_arg;

    Switch_Init();

    while(1)
    {
        SYS_DelayMs(500);
        Switch_TurnOn(SWITCH_KEY_PRESS);
        SYS_DelayMs(500);
        Switch_TurnOff(SWITCH_KEY_PRESS);

        SYS_DelayMs(500);
        Switch_TurnOn(SWITCH_START_PRESS);
        SYS_DelayMs(500);
        Switch_TurnOff(SWITCH_START_PRESS);

        SYS_DelayMs(500);
        Switch_TurnOn(SWITCH_KEY_LOCK);
        SYS_DelayMs(500);
        Switch_TurnOff(SWITCH_KEY_LOCK);

        SYS_DelayMs(500);
        Switch_TurnOn(SWITCH_LF);
        SYS_DelayMs(500);
        Switch_TurnOff(SWITCH_LF);

        SYS_DelayMs(500);
        Switch_TurnOn(SWITCH_RKE);
        SYS_DelayMs(500);
        Switch_TurnOff(SWITCH_RKE);

        SYS_DelayMs(500);
        Switch_TurnOn(SWITCH_UNUSER1);
        SYS_DelayMs(500);
        Switch_TurnOff(SWITCH_UNUSER1);

        SYS_DelayMs(500);
        Switch_TurnOn(SWITCH_UNUSER2);
        SYS_DelayMs(500);
        Switch_TurnOff(SWITCH_UNUSER2);

    }

}





