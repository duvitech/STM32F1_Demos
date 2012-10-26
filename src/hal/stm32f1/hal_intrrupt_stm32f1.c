
/*
 * hal_nvic_stm32f1.c
 *
 *  Created on: 2012-9-19
 *      Author: YangZhiyong
 */

#include "misc.h"
#include "hal_intrrupt.h"

#pragma section=".intvec"


void Intrrupt_Init(void)
{

    NVIC_SetVectorTable((uint32_t)__section_begin(".intvec"), 0);


    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);



    EXTI_InitTypeDef EXTI_InitStructure;

    /**
     * SIM908 STATUS
     */
    EXTI_InitStructure.EXTI_Line = EXTI_Line14;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init( &EXTI_InitStructure );
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource14 );

}



static volatile ISR_Handler EXTI_Isr[16] = {0};

void Intrrupt_SetExtiHandler(uint8_t exti, ISR_Handler handler)
{
    EXTI_Isr[exti] = handler;
}


void EXTI15_10_IRQHandler(void)
{

    if(EXTI_GetITStatus(EXTI_Line14) != RESET)
    {
        if( EXTI_Isr[14] != (void*)0 )
            EXTI_Isr[14]();
        EXTI_ClearITPendingBit(EXTI_Line14);
    }
}









