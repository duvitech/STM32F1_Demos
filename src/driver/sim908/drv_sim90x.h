/*
 * drv_sim90x.h
 *
 *  Created on: 2012-9-19
 *      Author: YangZhiyong
 */

#ifndef DRV_SIM90X_H_
#define DRV_SIM90X_H_

#ifdef __cplusplus
extern "C" {
#endif


#define DEBUG_SIM908        (1)
#define RESPONSE_MAX_LENGTH 0x80


extern struct TRANSPORT_IF* SIM90x_Transport;
extern uint8_t SIM90x_ResponseBuffer[RESPONSE_MAX_LENGTH];


void SIM90x_Init(const struct TRANSPORT_IF *tp);
void SIM90x_PowerUp(void);
void SIM90x_PowerDown(void);

void SIM90x_Write(   uint8_t* const buffer,
                            uint32_t length,
                            uint32_t timeout   );

void SIM90x_WriteString(   uint8_t* const string,
                           uint32_t timeout   );

void SIM90x_WriteLine(uint8_t* const line,
                     uint32_t timeout );

int SIM90x_ReadLine( uint8_t* const line,
                     uint32_t length,
                     uint32_t timeout );

//uint32_t SIM90x_WaitResponse1(uint32_t timeout, char* const response);
//uint32_t SIM90x_WaitResponse2(uint32_t timeout, char* const response1, char* const response2 );
//uint32_t SIM90x_WaitResponseList0(uint32_t timeout, uint8_t* const response_list);
void SIM90x_StartIdleTime(void);
void SIM90x_WaitIdleTime(uint32_t ms);

uint32_t SIM90x_Flush(void);


////// TEST   /////
uint8_t list_string(uint8_t* string, uint8_t* list, uint8_t div);

uint32_t SIM90x_WaitResponseList(uint32_t timeout, ... );


#ifdef __cplusplus
}
#endif

#endif /* DRV_SIM90X_H_ */
