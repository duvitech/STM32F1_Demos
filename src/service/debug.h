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

#define DEBUG_BUFFER_IN     (0x200)
#define DEBUG_BUFFER_FIFO   (0x200)
#define DEBUG_BUFFER_DMA    (0x80)


#define Debug(enable, format, arg...)   do{ if(enable)  Debug_Print(format, ##arg); }while(0)


void Debug_Init(struct TRANSPORT_IF* tp);
int Debug_Print(const char *format, ...);


#ifdef __cplusplus
}
#endif

#endif /* DEBUG_H_ */
