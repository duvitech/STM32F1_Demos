
/*
 * hal_pin_stm32f1.c
 *
 *  Created on: 2012-9-19
 *      Author: YangZhiyong
 */

#include "stm32f10x_gpio.h"


void PIN_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;



    // LED
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOF, &GPIO_InitStructure);
}





