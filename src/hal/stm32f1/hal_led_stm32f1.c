
/*
 * hal_led_stm32f1.c
 *
 *  Created on: 2012-9-22
 *      Author: YangZhiyong
 */

#include "stm32f10x_gpio.h"

const uint16_t LED_PIN[5] = {1<<6, 1<<7, 1<<8, 1<<9, 1<<10};



static void initGPIO(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOF, &GPIO_InitStructure);

}



void LED_On(uint8_t led)
{
    GPIO_ResetBits(GPIOF, LED_PIN[led]);
}


void LED_Off(uint8_t led)
{
    GPIO_SetBits(GPIOF, LED_PIN[led]);
}


void LED_Set(uint8_t led, uint8_t value)
{
    if( value )
    {
        LED_On(led);
    }
    else
    {
        LED_Off(led);
    }
}



void LED_Init(void)
{
    uint8_t led;

    initGPIO();

    for(led = 0; led < 5; led++)
    {
        LED_Off(led);
    }

//    for(led = 0; led < 5; led++)
//    {
//        LED_On(led);
//        for(volatile int i = 0; i < 3000000; i++);
//    }
//
//    for(led = 0; led < 5; led++)
//    {
//        LED_Off(led);
//        for(volatile int i = 0; i < 3000000; i++);
//    }

}


