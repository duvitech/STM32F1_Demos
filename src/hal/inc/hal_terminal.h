/*
 * hal_terminal.h
 *
 *  Created on: 2012-9-13
 *      Author: YangZhiyong
 */

#ifndef HAL_TERMINAL_H_
#define HAL_TERMINAL_H_

#ifdef __cplusplus
extern "C" {
#endif



void Term_WriteLine(char* line);
uint32_t Term_ReadLine(char* line);


#ifdef __cplusplus
}
#endif

#endif /* HAL_TERMINAL_H_ */
