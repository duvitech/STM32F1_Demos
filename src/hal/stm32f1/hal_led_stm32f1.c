
/*
 * hal_led_stm32f1.c
 *
 *  Created on: 2012-9-22
 *      Author: YangZhiyong
 */

#include "stm32f10x_gpio.h"

const uint16_t LED_PIN[5] = {1<<6, 1<<7, 1<<8, 1<<9, 1<<10};






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


