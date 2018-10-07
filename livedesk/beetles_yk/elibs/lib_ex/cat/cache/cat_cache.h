/*
*********************************************************************************************************
*											        ePDK
*						            the Easy Portable/Player Develop Kits
*									           cat  sample
*
*						        (c) Copyright 2006-2007, terry, China
*											All	Rights Reserved
*
* File    : cat_cache.h
* By      : terry.zeng
* Version : V2.00
* date    :2010-09-06
*********************************************************************************************************
*/
#ifndef __CAT_CACHE_H__
#define __CAT_CACHE_H__

#include ".\\..\\cat_p.h"
#include ".\\..\\db\cat_db.h"


typedef struct cat_cache_node_s
{
    __u32                   db_id;      //cache节点项对应的浏览项
    char                    *buf;       //节点项的buf指针
    struct cat_cache_node_s *next;      //下一节点指针
}cat_cache_node_t;


typedef struct
{
    cat_cache_node_t    *used_list;     //已使用的节点链表
    cat_cache_node_t    *unused_list;   //未使用的节点链表
}cat_cache_t;


/*
**********************************************************************************************************************
*                                               cat_cache_create
*
* author:        terry.zeng
*
* date:             2010-09-06
*
* Description:      创建一个cache
*
* parameters:
*
* return:           if success return cache point
*                   if fail return NULL
* modify history:
**********************************************************************************************************************
*/
void* cat_cache_inner_create(cat_db_t *db, cat_list_handle_t *list_handle, cat_cache_para_t *para);


/*
**********************************************************************************************************************
*                                               cat_cache_delete
*
* author:        terry.zeng
*
* date:             2010-09-06
*
* Description:      删除一个cache
*
* parameters:		handle:cat_cache_create返回的cache指针
*
* return:           if success return EPDK_OKt
*                   if fail return EPDK_FAIL
* modify history:
**********************************************************************************************************************
*/
__s32 cat_cache_inner_delete(void *handle);


/*
**********************************************************************************************************************
*                                               cat_get_one_cache_buf
*
* author:        terry.zeng
*
* date:             2010-09-06
*
* Description:      从cache里获取一个buffer
*
* parameters:		handle:cat_cache_create返回的cache指针，item：浏览项
*
* return:           if success return EPDK_OKt
*                   if fail return EPDK_FAIL
* modify history:
**********************************************************************************************************************
*/
__s32 cat_get_one_cache_buf(void *handle, __u32 id, __cat_item_t *item);


/*
**********************************************************************************************************************
*                                               cat_free_one_cache_buf
*
* author:        terry.zeng
*
* date:             2010-09-06
*
* Description:      释放一个cache buffer
*
* parameters:		handle:cat_cache_create返回的cache指针
*
* return:           if success return EPDK_OKt
*                   if fail return EPDK_FAIL
* modify history:
**********************************************************************************************************************
*/
__s32 cat_free_one_cache_buf(void *handle);


#endif

