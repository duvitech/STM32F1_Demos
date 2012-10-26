/*
 * hal_usart1_stm32f1.c
 *
 *  Created on: 2012-9-18
 *      Author: YangZhiyong
 */

#include "hal_usart.h"
#include "stm32f10x.h"
#include "hal_sys.h"
#include <string.h>


static uint8_t USART1_RxBuffer[0x400];
static uint8_t USART1_TxBuffer[0x400];

static uint32_t USART1_ReadIndex = 0;



/**
 * 初始化配置
 */
void USART1_Init(void)
{
    /**
     * 1.初始化时钟
     */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1|RCC_AHBPeriph_DMA2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);


    /**
     * 2.初始化IO
     */
    GPIO_InitTypeDef GPIO_InitStructure;

    /* Configure USART1 Rx as input floating */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* Configure USART1 Tx as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);


    /**
     * 3.初始化控制器
     */
    USART_InitTypeDef USART_InitStructure;

    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    /* Configure USARTy */
    USART_Init(USART1, &USART_InitStructure);


    /**
     * 4.初始化DMA
     */
    DMA_InitTypeDef DMA_InitStructure;

    DMA_DeInit(DMA1_Channel5);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(USART1->DR);
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&USART1_RxBuffer[0];
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize = sizeof(USART1_RxBuffer);
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel5, &DMA_InitStructure);
    USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);
    DMA_Cmd(DMA1_Channel5, ENABLE);


    DMA_DeInit(DMA1_Channel4);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(USART1->DR);
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&USART1_TxBuffer[0];
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_BufferSize = sizeof(USART1_TxBuffer);
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel4, &DMA_InitStructure);
    USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
    DMA_ITConfig(DMA1_Channel4, DMA_IT_TC, ENABLE);

    USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);
    USART_Cmd(USART1, ENABLE);


    /**
     * 5.初始化中断系统
     */
    NVIC_InitTypeDef NVIC_InitStructure;

    /* Enable the USART2 Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* Enable the DMA1_Channel7 Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}




/**
 *
 * @param buffer
 * @param length
 * @return
 */
uint32_t USART1_Send(void* buffer, uint32_t length)
{
    uint32_t send_length;

    if( length > sizeof(USART1_TxBuffer) )
    {
        send_length = sizeof(USART1_TxBuffer);
    }
    else
    {
        send_length = length;
    }

    memcpy(USART1_TxBuffer, buffer, send_length);
    DMA_Cmd(DMA1_Channel4, DISABLE);
    DMA_SetCurrDataCounter(DMA1_Channel4, send_length);
    DMA_Cmd(DMA1_Channel4, ENABLE);

    return send_length;
}



static USART_SendDoneISR SendDoneISR = 0;

/**
 *
 * @param isr
 */
void USART1_SetSendDoneISR       (USART_SendDoneISR isr)
{
    SendDoneISR = isr;
}



/**
 *
 * @param buffer
 * @param max_length
 * @return
 */
uint32_t USART1_Recv(uint8_t* buffer, uint32_t max_length)
{
    uint32_t recv_length;
    uint32_t write_index;

    write_index = sizeof(USART1_RxBuffer) - DMA_GetCurrDataCounter(DMA1_Channel5);
    recv_length = 0;

    while( (write_index - USART1_ReadIndex) != 0 )
    {
        buffer[recv_length] = USART1_RxBuffer[USART1_ReadIndex];
        recv_length ++;
        USART1_ReadIndex ++;

        if( USART1_ReadIndex == sizeof(USART1_RxBuffer) )
        {
            USART1_ReadIndex = 0;
        }

        if( recv_length == max_length )
        {
            break;
        }
    }

    return recv_length;
}



uint32_t USART1_DataAvailable(void)
{
    uint32_t filled_length;
    uint32_t write_index;

    write_index = sizeof(USART1_RxBuffer) - DMA_GetCurrDataCounter(DMA1_Channel5);

    if( USART1_ReadIndex > write_index )
    {
        filled_length = (sizeof(USART1_RxBuffer) - USART1_ReadIndex) + write_index;
    }
    else
    {
        filled_length = write_index - USART1_ReadIndex;
    }


    return filled_length;
}



uint32_t USART1_Flush(void)
{
    uint32_t flush_length;
    uint32_t write_index;

    write_index = sizeof(USART1_RxBuffer) - DMA_GetCurrDataCounter(DMA1_Channel5);

    if( USART1_ReadIndex > write_index )
    {
        flush_length = (sizeof(USART1_RxBuffer) - USART1_ReadIndex) + write_index;
    }
    else
    {
        flush_length = write_index - USART1_ReadIndex;
    }

    USART1_ReadIndex = write_index;

    return flush_length;
}




static USART_RecvTimeoutISR RecvTimeoutISR = 0;

/**
 *
 * @param isr
 */
void USART1_SetRecvTimeoutISR    (USART_RecvTimeoutISR isr)
{
    RecvTimeoutISR = isr;
}



void USART1_IRQHandler(void)
{
//    uint16_t recv;
    SYS_EnterInt();

    if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)
    {
//        recv =
        USART_ReceiveData(USART1);

        if( RecvTimeoutISR != 0 )
        {
            RecvTimeoutISR();
        }

        USART_ClearITPendingBit(USART1, USART_IT_IDLE);
    }

    SYS_ExitInt();
}



void DMA1_Channel4_IRQHandler(void)
{
    SYS_EnterInt();

    if ( DMA_GetITStatus(DMA1_IT_TC4) )
    {
        if( SendDoneISR != 0 )
        {
            SendDoneISR();
        }
        DMA_ClearITPendingBit(DMA1_IT_TC4);
    }

    SYS_ExitInt();
}






