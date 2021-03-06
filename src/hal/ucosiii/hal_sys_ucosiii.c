/*
 * hal_sys_ucosiii.c
 *
 *  Created on: 2012-9-19
 *      Author: YangZhiyong
 */

#include <cpu.h>
#include <os.h>
#include <stdint.h>


uint32_t SYS_MsToTick(uint32_t ms)
{
    uint32_t ticks;
    ticks = (ms*OSCfg_TickRate_Hz)/1000;
    return ticks;
}



uint32_t SYS_TickToMs(uint32_t ticks)
{
    uint32_t ms;
    ms = ticks*(1000/OSCfg_TickRate_Hz);
    return ms;
}



void SYS_EnterInt(void)
{
    CPU_SR_ALLOC();
    CPU_CRITICAL_ENTER();
    OSIntEnter();
    CPU_CRITICAL_EXIT();
}



void SYS_ExitInt(void)
{
    OSIntExit();
}



void SYS_DelayMs(uint32_t ms)
{
    OS_ERR error;
    uint32_t ticks;

    ticks = (ms*OSCfg_TickRate_Hz)/1000;

    OSTimeDly(ticks, OS_OPT_TIME_DLY, &error);
}





