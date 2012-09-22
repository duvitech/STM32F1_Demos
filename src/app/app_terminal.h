/*
 * app_terminal.h
 *
 *  Created on: 2012-9-21
 *      Author: YangZhiyong
 */

#ifndef APP_TERMINAL_H_
#define APP_TERMINAL_H_


#ifdef __cplusplus
extern "C" {
#endif


#include <stdint.h>
#include "transport/transport_if.h"

void        Term_Init      (struct TRANSPORT_IF const* tp);
uint32_t    Term_ReadLine  (struct TRANSPORT_IF const* tp, uint8_t* buffer, uint32_t buffer_length);
uint32_t    Term_WriteLine (struct TRANSPORT_IF const* tp, uint8_t* buffer, uint32_t buffer_length);


#ifdef __cplusplus
}
#endif

#endif /* APP_TERMINAL_H_ */
