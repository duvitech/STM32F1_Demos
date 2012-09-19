
/*
 * hal_nvic_stm32f1.c
 *
 *  Created on: 2012-9-19
 *      Author: YangZhiyong
 */

#include "misc.h"

#pragma section=".intvec"


void Intrrupt_Init(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_SetVectorTable((uint32_t)__section_begin(".intvec"), 0);

    /* Enable the USARTy Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}



