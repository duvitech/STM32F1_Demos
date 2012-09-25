/*
 * hal_usart.h
 *
 *  Created on: 2012-9-13
 *      Author: YangZhiyong
 */

#ifndef HAL_USART_H_
#define HAL_USART_H_

#ifdef __cplusplus
extern "C" {
#endif



#include <stdint.h>


/**
 * ��������жϷ�������
 */
typedef void (*USART_SendDoneISR)(void);

///**
// * �����ֽ�����жϷ�������
// * @param byte ���յ����ֽ�����
// */
//typedef void (*USART_RecvByteDoneISR)(uint8_t byte);

/**
 * �������ݳ�ʱ( �������Ͽ�����һ��ʱ�� )�жϷ�������
 */
typedef void (*USART_RecvTimeoutISR)(void);





/**
 * ��ʼ������
 */
void USART2_Init(void);

/**
 * �ֽڷ���
 * @param byte  �����ֽ�
 */
void USART2_SendByte(uint8_t byte);

/**
 *
 * @param buffer
 * @param length
 * @return
 */
uint32_t USART2_Send(void* buffer, uint32_t length);


/**
 *
 * @param buffer
 * @param max_length
 * @return
 */
uint32_t USART2_Recv(uint8_t* buffer, uint32_t max_length);


uint32_t USART2_DataAvailable(void);


/**
 *
 * @return
 */
uint32_t USART2_Flush(void);



///**
// * ���ý����ֽ�����жϷ���
// * @param isr   �жϷ�����
// */
//void USART2_SetRecvByteDoneISR   (USART_RecvByteDoneISR isr);

/**
 *
 * @param isr
 */
void USART2_SetRecvTimeoutISR    (USART_RecvTimeoutISR isr);

/**
 *
 * @param isr
 */
void USART2_SetSendDoneISR       (USART_SendDoneISR isr);


#ifdef __cplusplus
}
#endif

#endif /* HAL_USART_H_ */



