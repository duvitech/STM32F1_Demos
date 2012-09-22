/*
 * hal_led.h
 *
 *  Created on: 2012-9-3
 *      Author: YangZhiyong
 */

#ifndef HAL_LED_H_
#define HAL_LED_H_

#ifdef __cplusplus
extern "C" {
#endif


#include <stdint.h>

void LED_Init(void);
void LED_On(uint8_t led);
void LED_Off(uint8_t led);
void LED_Set(uint8_t led, uint8_t value);

#ifdef __cplusplus
}
#endif

#endif /* HAL_LED_H_ */



