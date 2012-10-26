
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

    /* Enable GPIO clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |
                           RCC_APB2Periph_GPIOB |
                           RCC_APB2Periph_GPIOC |
                           RCC_APB2Periph_GPIOD |
                           RCC_APB2Periph_GPIOE |
                           RCC_APB2Periph_GPIOF,
                           ENABLE   );


}


