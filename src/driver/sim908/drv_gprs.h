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



struct GPRS_CONFIG
{
    uint8_t addr[64];
    uint8_t port[8];
};


extern const struct TRANSPORT_IF TP_GPRS;

void GPRS_Connect(void);



#ifdef __cplusplus
}
#endif

#endif /* DRV_GPRS_H_ */
