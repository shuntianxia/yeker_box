/*
*********************************************************************************************************
*											        ePDK
*						            the Easy Portable/Player Develop Kits
*									           cat  sample
*
*						        (c) Copyright 2006-2007, terry, China
*											All	Rights Reserved
*
* File    : cat_db_interface.h
* By      : terry.zeng
* Version : V2.00
* date    :2010-09-06
*********************************************************************************************************
*/

#ifndef __CAT_ENGINE_H__
#define __CAT_ENGINE_H__

#include ".\\..\\cat_p.h"
#include ".\\..\\db\\cat_db.h"


typedef enum cat_engine_cmd_s
{
    READ_START = 0x01,      //开始对某个浏览信息进行搜索
    READ_PAUSE,             //暂停当前的搜索
    READ_STOP,              //停止当前搜索
    READ_RESUME,            //恢复当前搜索
    UPDEATE_ROOT,
    GET_STATUS,             //获取当前引擎状态
}cat_engine_cmd_t;


typedef enum cat_engine_status_s
{
    ENGINE_STOPPED,     //当前引擎处于停止状态
    ENGINE_BUSY,        //当前引擎处于忙碌状态
    ENGINE_PAUSED,      //当前引擎处于暂停状态	
}cat_engine_status_t;


typedef struct cat_engine_s
{
    cat_db_t                *db;
    __u8                    thread_id;          /*线程ID*/	
    __krnl_event_t          *sem;               /*信号量*/
    __s32                   cmd_pend_flag;      /*线程处理标志位*/
    __u32                   cmd;                /*线程处理命令*/
    __s32                   param1;             /*命令参数1*/
    __s32                   param2;             /*命令参数2*/
    __s32                   result;             /*命令调用的结果*/
    cat_engine_status_t     status;
    cat_list_handle_t       *list_handle;
}cat_engine_t;


/*
**********************************************************************************************************************
*                                               cat_engine_open
*
* author:        terry.zeng
*
* date:             2010-09-06
*
* Description:      cat 后台搜索引擎
*
* parameters:
*
* return:           if success return cat engine handle
*                   if fail return NULL
* modify history:
**********************************************************************************************************************
*/
void* cat_engine_open(cat_db_t *db);


/*
**********************************************************************************************************************
*                                               cat_engine_control
*
* author:        terry.zeng
*
* date:             2010-09-06
*
* Description:      cat 后台搜索引擎控制函数
*
* parameters:
*
* return:           if success return EPDK_OK
*                   if fail return EPDK_FAIL
* modify history:
**********************************************************************************************************************
*/
__s32 cat_engine_control(void *hengine, __u32 cmd, __s32 param1, __s32 param2);


/*
**********************************************************************************************************************
*                                               cat_engine_close
*
* author:        terry.zeng
*
* date:             2010-09-06
*
* Description:      cat 后台搜索引擎关闭
*
* parameters:
*
* return:           if success return EPDK_OK
*                   if fail return EPDK_FAIL
* modify history:
**********************************************************************************************************************
*/
__s32 cat_engine_close(void *hengine); 


#endif

