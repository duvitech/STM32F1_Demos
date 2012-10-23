/*
 * debug.h
 *
 *  Created on: 2012-10-22
 *      Author: YangZhiyong
 */

#ifndef DEBUG_H_
#define DEBUG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "transport/transport_if.h"

#define DEBUG_BUFFER_LENGTH (0x80)


void Debug_Init(struct TRANSPORT_IF* tp);
int Debug_Print(const char *format, ...);


#ifdef __cplusplus
}
#endif

#endif /* DEBUG_H_ */
