/*
 * hal_sys.h
 *
 *  Created on: 2012-9-13
 *      Author: YangZhiyong
 */

#ifndef HAL_SYS_H_
#define HAL_SYS_H_

#ifdef __cplusplus
extern "C" {
#endif


#include <stdint.h>


void SYS_EnterInt(void);
void SYS_ExitInt(void);
void SYS_DelayMs(uint32_t ms);
uint32_t SYS_MsToTick(uint32_t ms);


#ifdef __cplusplus
}
#endif

#endif /* HAL_SYS_H_ */
