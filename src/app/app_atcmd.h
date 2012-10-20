/*
 * app_atcmd.h
 *
 *  Created on: 2012-10-11
 *      Author: YangZhiyong
 */

#ifndef APP_ATCMD_H_
#define APP_ATCMD_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "transport/transport_if.h"



void ATCMD_Write(struct TRANSPORT_IF* tp,
                 uint8_t* const buffer,
                 uint32_t length,
                 uint32_t timeout   );

void ATCMD_WriteLine(struct TRANSPORT_IF* tp,
                     uint8_t* const line,
                     uint32_t timeout );

int ATCMD_ReadLine(struct TRANSPORT_IF* tp,
                   uint8_t* const line,
                   uint32_t length,
                   uint32_t timeout );

uint32_t ATCMD_WaitResponse1(struct TRANSPORT_IF* tp,
                             uint8_t* response_buffer,
                             uint32_t buffer_length,
                             uint32_t timeout,
                             char* const response);

uint32_t ATCMD_WaitResponse2(struct TRANSPORT_IF* tp,
                             uint8_t* response_buffer,
                             uint32_t buffer_length,
                             uint32_t timeout,
                             char* response1,
                             char* response2 );

uint32_t ATCMD_WaitResponse3(struct TRANSPORT_IF* tp,
                             uint8_t* response_buffer,
                             uint32_t buffer_length,
                             uint32_t timeout,
                             char* response1,
                             char* response2,
                             char* response3 );


#ifdef __cplusplus
}
#endif

#endif /* APP_ATCMD_H_ */
