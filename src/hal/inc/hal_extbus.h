/*
 * hal_extbus.h
 *
 *  Created on: 2012-3-20
 *      Author: YangZhiyong
 */

#ifndef HAL_EXTBUS_H_
#define HAL_EXTBUS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define SDRAM_SIZE_MB       8
#define SDRAM_SIZE          (SDRAM_SIZE_MB * 1024 * 1024)
#define SDRAM_BASE_ADDR     0xA0000000

//#define MHZ                 *10000001
//
//#define SYS_FREQ            60
//
//#if   SYS_FREQ == (120)
//#define SDRAM_PERIOD          8.33  // 96MHz
//#elif     SYS_FREQ == (96)
//#define SDRAM_PERIOD          10.4  // 96MHz
//#elif   SYS_FREQ == (72)
//#define SDRAM_PERIOD          13.8  // 72MHz
//#elif   SYS_FREQ == (60)
//#define SDRAM_PERIOD          16.67  // 72MHz
//#elif   SYS_FREQ == (57)
//#define SDRAM_PERIOD          17.4  // 57.6MHz
//#elif SYS_FREQ == (48)
//#define SDRAM_PERIOD          20.8  // 48MHz
//#elif SYS_FREQ == (36)
//#define SDRAM_PERIOD          27.8  // 36MHz
//#elif SYS_FREQ == (24)
//#define SDRAM_PERIOD          41.7  // 24MHz
//#elif SYS_FREQ == (12)
//#define SDRAM_PERIOD          83.3  // 12MHz
//#else
//#error Frequency not defined
//#endif
//
//#define P2C(Period)           (((Period<SDRAM_PERIOD)?0:(uint32_t)((float)Period/SDRAM_PERIOD))+1)
//
//#define SDRAM_REFRESH         7813
//#define SDRAM_TRP             20
//#define SDRAM_TRAS            45
//#define SDRAM_TAPR            1
//#define SDRAM_TDAL            3
//#define SDRAM_TWR             3
//#define SDRAM_TRC             65
//#define SDRAM_TRFC            66
//#define SDRAM_TXSR            67
//#define SDRAM_TRRD            15
//#define SDRAM_TMRD            3


void ExtBus_Init(void);


#ifdef __cplusplus
}
#endif

#endif /* HAL_EXTBUS_H_ */





