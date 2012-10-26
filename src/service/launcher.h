/*
 * launcher.h
 *
 *  Created on: 2012-3-23
 *      Author: YangZhiyong
 */

#ifndef LAUNCHER_H_
#define LAUNCHER_H_

#ifdef __cplusplus
extern "C" {
#endif



#include "os.h"


#define DEBUG_LAUNCHER      (1)

struct OSTaskCreate_Arg
{
    OS_TCB        *p_tcb;
    CPU_CHAR      *p_name;
    OS_TASK_PTR    p_task;
    void          *p_arg;
    OS_PRIO        prio;
    CPU_STK       *p_stk_base;
    CPU_STK_SIZE   stk_limit;
    CPU_STK_SIZE   stk_size;
    OS_MSG_QTY     q_size;
    OS_TICK        time_quanta;
    void          *p_ext;
    OS_OPT         opt;
};



#define APP_TASK_DEFINE(name, p_task, p_arg, prio, stk_size, q_size, time_quanta, p_ext, opt)  \
                static OS_TCB   name##_TCB;\
                static CPU_STK  name##_Stk[stk_size];\
                __root const struct OSTaskCreate_Arg name##_Arg @ "APP_TASK_DEFINE" =\
                {\
                    &name##_TCB,\
                    #name,\
                    p_task,\
                    p_arg,\
                    prio,\
                    &name##_Stk[0],\
                    stk_size/10,\
                    stk_size,\
                    q_size,\
                    time_quanta,\
                    p_ext,\
                    opt\
                };\


/**
 * Demo
 */
//APP_TASK_DEFINE(task,          /*name name##_TCB */
//                task_func,   /*p_task*/
//                0,              /*p_arg*/
//                4,              /*prio*/
//                2048,            /*stk_size_32*/
//                0,              /*q_size*/
//                0,              /*time_quanta*/
//                0,              /*p_ext*/
//                (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR) /*opt*/
//)





struct OSSemCreate_Arg
{
    OS_SEM      *p_sem;
    CPU_CHAR    *p_name;
    OS_SEM_CTR   cnt;
};



#define APP_SEM_DEFINE(name, count) \
                static OS_SEM name;\
                __root const struct OSSemCreate_Arg name##_Arg @ "APP_SEM_DEFINE" =\
                {\
                    &name,\
                    #name,\
                    count,\
                };\


/**
 * Demo
 */
//APP_SEM_DEFINE(Demo_Sem, 0);


struct OSFlagCreate_Arg
{
    OS_FLAG_GRP *p_grp;
    CPU_CHAR    *p_name;
    OS_FLAGS    flags;
};


#define APP_FLAG_DEFINE(name, flags) \
                OS_FLAG_GRP name;\
                __root const struct OSFlagCreate_Arg name##_Arg @ "APP_FLAG_DEFINE" =\
                {\
                    &name,\
                    #name,\
                    flags,\
                };\




#define APP_Q_DEFINE()


////////////////////////////////////////////////////////////////////////////////

#pragma section="APP_MUTEX_DEFINE"

struct OSMutexCreate_Arg
{
    OS_MUTEX *mutex;
    CPU_CHAR *p_name;
};

#define APP_MUTEX_DEFINE(name) \
                OS_MUTEX name;\
                __root const struct OSMutexCreate_Arg name##_Arg @ "APP_MUTEX_DEFINE" =\
                {\
                    &name,\
                    #name,\
                };\



extern OS_MEM  AppMem4KB;
extern OS_MEM  AppMem2MB;

void Launcher_Exec(void);



#ifdef __cplusplus
}
#endif

#endif /* LAUNCHER_H_ */
