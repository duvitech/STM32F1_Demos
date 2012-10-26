/*
 * hal_relay.h
 *
 *  Created on: 2012-10-25
 *      Author: YangZhiyong
 */

#ifndef HAL_RELAY_H_
#define HAL_RELAY_H_

#ifdef __cplusplus
extern "C" {
#endif


#define SWITCH_KEY_PRESS        0
#define SWITCH_START_PRESS      1
#define SWITCH_KEY_LOCK         2
#define SWITCH_LF               3
#define SWITCH_RKE              4
#define SWITCH_UNUSER1          5
#define SWITCH_UNUSER2          6


void Switch_Init(void);
void Switch_TurnOn(uint8_t id);
void Switch_TurnOff(uint8_t id);


#ifdef __cplusplus
}
#endif

#endif /* HAL_RELAY_H_ */

