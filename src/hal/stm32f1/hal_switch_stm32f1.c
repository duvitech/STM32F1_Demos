/*
 * hal_relay_stm32f1.c
 *
 *  Created on: 2012-10-25
 *      Author: YangZhiyong
 */


#include <stdint.h>
#include "stm32f10x_gpio.h"


struct PORT_PIN
{
    GPIO_TypeDef*   port;
    uint16_t        pin;
};


const struct PORT_PIN Switch_ID[] =
{
                {GPIOC, GPIO_Pin_3 },   ///< Key_Insert_EN
                {GPIOA, GPIO_Pin_15},   ///< Start_Switch_EN
                {GPIOC, GPIO_Pin_5 },   ///< Key_Online_EN
                {GPIOC, GPIO_Pin_0 },   ///< LF_Cut_Switch
                {GPIOD, GPIO_Pin_3 },   ///< RF_Switch_EN
                {GPIOC, GPIO_Pin_2 },   ///< Unused_Switch1_Ctrl/SWITCH1
                {GPIOC, GPIO_Pin_1 },   ///< Unused_Switch2_Ctrl/SWITCH2
};


#define SWITCH_COUNT     (sizeof(Switch_ID)/sizeof(struct PORT_PIN))


static void initGPIO(void)
{
    uint8_t id;
    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
//
//    GPIO_Init(GPIOF, &GPIO_InitStructure);

    for( id = 0; id < SWITCH_COUNT; id++)
    {
        GPIO_InitStructure.GPIO_Pin = Switch_ID[id].pin;
        GPIO_Init(Switch_ID[id].port, &GPIO_InitStructure);
    }

}




void Switch_TurnOn(uint8_t id)
{
    GPIO_SetBits(Switch_ID[id].port, Switch_ID[id].pin);
}


void Switch_TurnOff(uint8_t id)
{
    GPIO_ResetBits(Switch_ID[id].port, Switch_ID[id].pin);
}



void Switch_Init(void)
{
    uint8_t id;

    initGPIO();

    for( id = 0; id < SWITCH_COUNT; id++)
    {
        Switch_TurnOff(id);
    }
}



