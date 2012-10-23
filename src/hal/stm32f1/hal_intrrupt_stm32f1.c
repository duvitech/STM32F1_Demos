
/*
 * hal_nvic_stm32f1.c
 *
 *  Created on: 2012-9-19
 *      Author: YangZhiyong
 */

#include "misc.h"

#pragma section=".intvec"


void Intrrupt_Init(void)
{


    NVIC_SetVectorTable((uint32_t)__section_begin(".intvec"), 0);


}



