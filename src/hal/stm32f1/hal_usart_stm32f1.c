/*
 * hal_usart_stm32f1.c
 *
 *  Created on: 2012-9-18
 *      Author: YangZhiyong
 */

#include "hal_usart.h"
#include "stm32f10x.h"
#include "hal_sys.h"
#include <string.h>


static uint8_t USART2_RxBuffer[0x400];
static uint8_t USART2_TxBuffer[0x400];

static uint32_t USART2_ReadIndex = 0;


/**
 * ³õÊ¼»¯ÅäÖÃ
 */
void USART2_Init(void)
{
    USART_InitTypeDef USART_InitStructure;
    DMA_InitTypeDef DMA_InitStructure;


    USART_InitStructure.USART_BaudRate = 115200;
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


    DMA_DeInit(DMA1_Channel7);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(USART2->DR);
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&USART2_TxBuffer[0];
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_BufferSize = sizeof(USART2_TxBuffer);
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel7, &DMA_InitStructure);
    USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE);
    DMA_ITConfig(DMA1_Channel7, DMA_IT_TC, ENABLE);

    USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);
    USART_Cmd(USART2, ENABLE);
}




/**
 *
 * @param buffer
 * @param length
 * @return
 */
uint32_t USART2_Send(void* buffer, uint32_t length)
{
    uint32_t send_length;

    if( length > sizeof(USART2_TxBuffer) )
    {
        send_length = sizeof(USART2_TxBuffer);
    }
    else
    {
        send_length = length;
    }

    memcpy(USART2_TxBuffer, buffer, send_length);
    DMA_Cmd(DMA1_Channel7, DISABLE);
    DMA_SetCurrDataCounter(DMA1_Channel7, send_length);
    DMA_Cmd(DMA1_Channel7, ENABLE);

    return send_length;
}



static USART_SendDoneISR SendDoneISR2 = 0;

/**
 *
 * @param isr
 */
void USART2_SetSendDoneISR       (USART_SendDoneISR isr)
{
    SendDoneISR2 = isr;
}



/**
 *
 * @param buffer
 * @param max_length
 * @return
 */
uint32_t USART2_Recv(uint8_t* buffer, uint32_t max_length)
{
    uint32_t recv_length;
    uint32_t write_index;

    write_index = sizeof(USART2_RxBuffer) - DMA_GetCurrDataCounter(DMA1_Channel6);
    recv_length = 0;

    while( (write_index - USART2_ReadIndex) != 0 )
    {
        buffer[recv_length] = USART2_RxBuffer[USART2_ReadIndex];
        recv_length ++;
        USART2_ReadIndex ++;

        if( USART2_ReadIndex == sizeof(USART2_RxBuffer) )
        {
            USART2_ReadIndex = 0;
        }

        if( recv_length == max_length )
        {
            break;
        }
    }

    return recv_length;
}



uint32_t USART2_DataAvailable(void)
{
    uint32_t filled_length;
    uint32_t write_index;

    write_index = sizeof(USART2_RxBuffer) - DMA_GetCurrDataCounter(DMA1_Channel6);

    if( USART2_ReadIndex > write_index )
    {
        filled_length = (sizeof(USART2_RxBuffer) - USART2_ReadIndex) + write_index;
    }
    else
    {
        filled_length = write_index - USART2_ReadIndex;
    }


    return filled_length;
}



uint32_t USART2_Flush(void)
{
    uint32_t flush_length;
    uint32_t write_index;

    write_index = sizeof(USART2_RxBuffer) - DMA_GetCurrDataCounter(DMA1_Channel6);

    if( USART2_ReadIndex > write_index )
    {
        flush_length = (sizeof(USART2_RxBuffer) - USART2_ReadIndex) + write_index;
    }
    else
    {
        flush_length = write_index - USART2_ReadIndex;
    }

    USART2_ReadIndex = write_index;

    return flush_length;
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
//    uint16_t recv;
    SYS_EnterInt();

    if(USART_GetITStatus(USART2, USART_IT_IDLE) != RESET)
    {
//        recv =
        USART_ReceiveData(USART2);

        if( RecvTimeoutISR2 != 0 )
        {
            RecvTimeoutISR2();
        }

        USART_ClearITPendingBit(USART2, USART_IT_IDLE);
    }


    SYS_ExitInt();
}



void DMA1_Channel7_IRQHandler(void)
{
    SYS_EnterInt();

    if ( DMA_GetITStatus(DMA1_IT_TC7) )
    {
        if( SendDoneISR2 != 0 )
        {
            SendDoneISR2();
        }
        DMA_ClearITPendingBit(DMA1_IT_TC7);
    }

    SYS_ExitInt();
}






