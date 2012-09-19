
/*
 * hal_clock_stm32f1.c
 *
 *  Created on: 2012-9-19
 *      Author: YangZhiyong
 */
#include "stm32f10x_rcc.h"
#include "stm32f10x.h"

void Clock_Init(void)
{
    /* Setup the microcontroller system. Initialize the Embedded Flash Interface,
       initialize the PLL and update the SystemFrequency variable. */
    SystemInit();

    /* DMA clock enable */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1|RCC_AHBPeriph_DMA2, ENABLE);

    /* Enable GPIO clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |
                           RCC_APB2Periph_GPIOB |
                           RCC_APB2Periph_GPIOC |
                           RCC_APB2Periph_GPIOD |
                           RCC_APB2Periph_GPIOE,
                           ENABLE);


    /* Enable USARTy Clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

    /* Enable USARTz Clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
}









