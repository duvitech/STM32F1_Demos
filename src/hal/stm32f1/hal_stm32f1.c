
/*
 * hal_stm32f1.c
 *
 *  Created on: 2012-9-19
 *      Author: YangZhiyong
 */


#include "hal_clock.h"
#include "hal_usart.h"
#include "hal_pin.h"
#include "hal_intrrupt.h"
#include "hal_led.h"

void HAL_Init(void)
{
    Clock_Init();
    PIN_Init();
    LED_Init();
    USART2_Init();
    Intrrupt_Init();
}




