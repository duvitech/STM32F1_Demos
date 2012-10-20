
/*
 * main.c
 *
 *  Created on: 2012-9-18
 *      Author: YangZhiyong
 */

#include "hal.h"
#include "stm32f10x.h"
#include "hal_usart.h"
//#include <cpu.h>
#include <os.h>
#include "service/launcher.h"


//#pragma location=0x800
//__root const uint8_t BUILD_TIME[] = __TIME__;


uint8_t DemoString[] =
 "@copy                                                                        \r\n"
 "THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS  \r\n"
 "WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE   \r\n"
 "TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY       \r\n"
 "DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING \r\n"
 "FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE    \r\n"
 "CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.       \r\n";



void RecvDone(void)
{
    uint32_t recv_length;
    recv_length = USART2_Recv(DemoString, sizeof(DemoString));
    USART2_Send(DemoString, recv_length);
}



void main(void)
{
    OS_ERR error;


    HAL_Init();

    CPU_IntDis();

    OSInit(&error);

    Launcher_Exec();

    /////////////////////////////////////////////////////////////////
    RCC_ClocksTypeDef RCC_Clocks;
    RCC_GetClocksFreq(&RCC_Clocks);

    OS_CPU_SysTickInit( (RCC_Clocks.HCLK_Frequency/OSCfg_TickRate_Hz) - 1 );
    /////////////////////////////////////////////////////////////////

    OSStart(&error);

}








