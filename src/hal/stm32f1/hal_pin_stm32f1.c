
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

    /* Configure USARTy Rx as input floating */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* Configure USARTy Tx as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}
