/*
 * hal_usart_stm32f1.c
 *
 *  Created on: 2012-9-18
 *      Author: YangZhiyong
 */

#include "hal_usart.h"
#include "stm32f10x.h"
#include "hal_sys.h"


static uint8_t USART2_RxBuffer[0x400];



/**
 * 初始化配置
 */
void USART2_Init(void)
{
    USART_InitTypeDef USART_InitStructure;
    DMA_InitTypeDef DMA_InitStructure;


    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    /* Configure USARTy */
    USART_Init(USART2, &USART_InitStructure);

    DMA_DeInit(DMA1_Channel6);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(USART2->DR);
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&USART2_RxBuffer[0];
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize = sizeof(USART2_RxBuffer);
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel6, &DMA_InitStructure);

    USART_DMACmd(USART2, USART_DMAReq_Rx, ENABLE);

    DMA_Cmd(DMA1_Channel6, ENABLE);

    USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);
    USART_Cmd(USART2, ENABLE);
}




/**
 * 缓冲发送
 * @param buffer    缓冲指针
 * @param length    缓冲长度
 */
void USART2_Send(void* buffer, uint32_t length)
{

}


static USART_RecvByteDoneISR RecvByteDoneISR2 = 0;

/**
 * 设置接收字节完成中断服务
 * @param isr   中断服务函数
 */
void USART2_SetRecvByteDoneISR   (USART_RecvByteDoneISR isr)
{
    RecvByteDoneISR2 = isr;
}


static USART_RecvTimeoutISR RecvTimeoutISR2 = 0;

/**
 *
 * @param isr
 */
void USART2_SetRecvTimeoutISR    (USART_RecvTimeoutISR isr)
{
    RecvTimeoutISR2 = isr;
}



void USART2_IRQHandler(void)
{
    uint16_t recv;
    SYS_EnterInt();

    if(USART_GetITStatus(USART2, USART_IT_IDLE) != RESET)
    {
        recv = USART_ReceiveData(USART2);

        if( RecvTimeoutISR2 != 0 )
        {
            RecvTimeoutISR2();
        }

        USART_ClearITPendingBit(USART2, USART_IT_IDLE);
    }


    SYS_ExitInt();
}






