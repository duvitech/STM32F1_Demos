/*
 * drv_GPRS.h
 *
 *  Created on: 2012-9-19
 *      Author: YangZhiyong
 */

#ifndef DRV_GPRS_H_
#define DRV_GPRS_H_

#ifdef __cplusplus
extern "C" {
#endif

#define DEBUG_GPRS      (1)

void GPRS_Connect(uint8_t* addr, uint8_t* port);

bool GPRS_IsTMode(void);
void GPRS_SwitchTMode(bool onoff);


#ifdef __cplusplus
}
#endif

#endif /* DRV_GPRS_H_ */
