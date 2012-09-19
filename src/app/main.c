
/*
 * main.c
 *
 *  Created on: 2012-9-18
 *      Author: YangZhiyong
 */

#include "hal.h"
#include "stm32f10x.h"
#include "stm32f10x_usart.h"




void main(void)
{
    HAL_Init();

    while (1)
    {
        USART_SendData(USART2, 0xA5);
        for( int i = 0; i < 10000000; i++);
    }
}








