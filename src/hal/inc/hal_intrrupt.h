/*
 * hal_nvic.h
 *
 *  Created on: 2012-9-19
 *      Author: YangZhiyong
 */

#ifndef HAL_NVIC_H_
#define HAL_NVIC_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*ISR_Handler)(void);

void Intrrupt_Init(void);
void Intrrupt_SetExtiHandler(uint8_t exti, ISR_Handler handler);

#ifdef __cplusplus
}
#endif

#endif /* HAL_NVIC_H_ */
