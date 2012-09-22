/*
 * transport_if.h
 *
 *  Created on: 2012-9-4
 *      Author: YangZhiyong
 */

#ifndef TRANSPORT_IF_H_
#define TRANSPORT_IF_H_

#ifdef __cplusplus
extern "C" {
#endif


#include <stdint.h>
#include <stdbool.h>
#include <string.h>


//typedef void (*TRANSPORT_EventHandler)(struct TRANSPORT_IF* tp);



/**
 * ͨ��״̬
 */
enum TRANSPORT_Status
{
    TRANSPORT_Status_Idle      = 0x00,//!< ����
    TRANSPORT_Status_RecvBusy  = 0x01,//!< ����æ
    TRANSPORT_Status_RecvDone  = 0x02,//!< �������
    TRANSPORT_Status_SendBusy  = 0x04,//!< ����æ
    TRANSPORT_Status_SendDone  = 0x08,//!< �������
};


/**
 * ͨ���¼�
 */
enum TRANSPORT_Event
{
    TRANSPORT_Event_RecvDone    = 0x02,//!< ��������¼�
    TRANSPORT_Event_SendDone    = 0x08,//!< ��������¼�
    TRANSPORT_Event_Error       = 0x80,//!< �����¼�
};



/**
 * ͨ�Žӿ�(IF = Interface)
 */
struct TRANSPORT_IF
{
    /**
     * �ӿ�
     */
    char* name;


    /**
     * ��ʼ���ӿ�
     * @param config �ӿ����ò������ɽӿ�ʵ���߾��嶨��
     * @return
     */
    uint8_t (*init)(void* config);


    /**
     * ���ͺ���
     * @param buffer    ���ͻ����ַ
     * @param length    ���ͳ���
     * @return
     */
    uint32_t (*send)(void* buffer, uint32_t length);


    /**
     * ���պ���
     * @param buffer    ���ջ����ַ
     * @param length    ���ճ���
     * @return
     */
    uint32_t (*recv)(uint8_t* buffer, uint32_t length);


    /**
     * �����¼�����ص�����
     * @param event     ��Ҫ������¼�
     * @param handler   �¼�����ص�����
     */
    void (*setEventHandler)(enum TRANSPORT_Event event, void (*handler)(void) );


    /**
     *
     * @param event
     */
    void (*waitEventTrigger)(enum TRANSPORT_Event event);


    /**
     * ��ȡ�ӿ�״̬
     * @return
     */
    enum TRANSPORT_Status   (*status)(void);

//
//    void* (*setUserData)(void* userData);
//    void* (*getUserData)(void);

};




#ifdef __cplusplus
}
#endif

#endif /* TRANSPORT_IF_H_ */



