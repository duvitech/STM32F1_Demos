/*
 * transport_gprs.h
 *
 *  Created on: 2012-10-22
 *      Author: YangZhiyong
 */

#ifndef TRANSPORT_GPRS_H_
#define TRANSPORT_GPRS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "transport_if.h"
#include "driver/sim908/drv_gprs.h"
#include "driver/sim908/drv_sim90x.h"


struct GPRS_CONFIG
{
    uint8_t addr[64];
    uint8_t port[8];
};

extern const struct TRANSPORT_IF TP_GPRS;


#ifdef __cplusplus
}
#endif

#endif /* TRANSPORT_GPRS_H_ */
