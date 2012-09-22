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
 * 通信状态
 */
enum TRANSPORT_Status
{
    TRANSPORT_Status_Idle      = 0x00,//!< 空闲
    TRANSPORT_Status_RecvBusy  = 0x01,//!< 接收忙
    TRANSPORT_Status_RecvDone  = 0x02,//!< 接收完成
    TRANSPORT_Status_SendBusy  = 0x04,//!< 发送忙
    TRANSPORT_Status_SendDone  = 0x08,//!< 发送完成
};


/**
 * 通信事件
 */
enum TRANSPORT_Event
{
    TRANSPORT_Event_RecvDone    = 0x02,//!< 接收完成事件
    TRANSPORT_Event_SendDone    = 0x08,//!< 发送完成事件
    TRANSPORT_Event_Error       = 0x80,//!< 出错事件
};



/**
 * 通信接口(IF = Interface)
 */
struct TRANSPORT_IF
{
    /**
     * 接口
     */
    char* name;


    /**
     * 初始化接口
     * @param config 接口配置参数，由接口实现者具体定义
     * @return
     */
    uint8_t (*init)(void* config);


    /**
     * 发送函数
     * @param buffer    发送缓存地址
     * @param length    发送长度
     * @return
     */
    uint32_t (*send)(void* buffer, uint32_t length);


    /**
     * 接收函数
     * @param buffer    接收缓存地址
     * @param length    接收长度
     * @return
     */
    uint32_t (*recv)(uint8_t* buffer, uint32_t length);


    /**
     * 设置事件处理回调函数
     * @param event     需要处理的事件
     * @param handler   事件处理回调函数
     */
    void (*setEventHandler)(enum TRANSPORT_Event event, void (*handler)(void) );


    /**
     *
     * @param event
     */
    void (*waitEventTrigger)(enum TRANSPORT_Event event);


    /**
     * 获取接口状态
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



