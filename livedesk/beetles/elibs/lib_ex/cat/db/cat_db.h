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
#ifndef __CAT_DB_H__
#define __CAT_DB_H__

#include ".\\..\\cat_p.h"

#define DB_FILE_NAME   ":memory:"   //"d:\\apps\\db_cat.db"  


typedef struct cat_db_s
{
    HDBConn     hdb;
}cat_db_t;


/*
**********************************************************************************************************************
*                                               cat_db_open
*
* author:        terry.zeng
*
* date:             2010-09-06
*
* Description:      cat 数据库打开函数，执行相关数据库的初始化操作
*
* parameters:
*
* return:           if success return cat db handle
*                   if fail return NULL
* modify history:
**********************************************************************************************************************
*/
cat_db_t *cat_db_open(void);


/*
**********************************************************************************************************************
*                                               cat_db_close
*
* author:        terry.zeng
*
* date:             2010-09-06
*
* Description:      关闭 数据库
*
* parameters:
*
* return:           if success return EPDK_OK
*                   if fail return EPDK_FAIL
* modify history:
**********************************************************************************************************************
*/
__s32 cat_db_close(cat_db_t *db_handle);


/*
**********************************************************************************************************************
*                                               cat_db_table_create
*
* author:        terry.zeng
*
* date:             2010-09-06
*
* Description:      创建浏览信息数据库文件
*
* parameters:
*
* return:           if success return EPDK_OK
*                   if fail return EPDK_FAIL
* modify history:
**********************************************************************************************************************
*/
__s32 cat_db_table_create(cat_db_t *db_handle, cat_list_handle_t *list_handle);


/*
**********************************************************************************************************************
*                                               cat_db_table_del
*
* author:        terry.zeng
*
* date:             2010-09-06
*
* Description:      删除浏览信息数据库文件
*
* parameters:
*
* return:           if success return EPDK_OK
*                   if fail return EPDK_FAIL
* modify history:
**********************************************************************************************************************
*/
__s32 cat_db_table_del(cat_db_t *db_handle, cat_list_handle_t *list_handle);


/*
**********************************************************************************************************************
*                                               cat_db_get_num
*
* author:        terry.zeng
*
* date:             2010-09-06
*
* Description:      获取浏览信息的总数
*
* parameters:
*
* return:           return item number
*
* modify history:
**********************************************************************************************************************
*/
__s32 cat_db_get_num(cat_db_t *db_handle, cat_list_handle_t *list_handle);


/*
**********************************************************************************************************************
*                                               cat_db_add_base_info
*
* author:        terry.zeng
*
* date:             2010-09-06
*
* Description:      往数据库里面添加浏览项基本信息
*
* parameters:
*
* return:           if success return EPDK_OK
*                   if fail return EPDK_FAIL
* modify history:
**********************************************************************************************************************
*/
__s32 cat_db_add_base_info(cat_db_t *db_handle, cat_list_handle_t *list_handle);


/*
**********************************************************************************************************************
*                                               cat_db_get_base_info
*
* author:        terry.zeng
*
* date:             2010-09-06
*
* Description:      从数据库里面获取浏览项基本信息
*
* parameters:
*
* return:           if success return EPDK_OK
*                   if fail return EPDK_FAIL
* modify history:
**********************************************************************************************************************
*/
__s32 cat_db_get_base_info(cat_db_t *db_handle, cat_list_handle_t *list_handle, __u32 index, __cat_item_t *item);


/*
**********************************************************************************************************************
*                                               cat_db_add_extra_info
*
* author:        terry.zeng
*
* date:             2010-09-06
*
* Description:      往数据库里面添加浏览项额外信息
*
* parameters:
*
* return:           if success return EPDK_OK
*                   if fail return EPDK_FAIL
* modify history:
**********************************************************************************************************************
*/
__s32 cat_db_add_extra_info(cat_db_t *db_handle, cat_list_handle_t *list_handle, __u32 index, __cat_item_t *item);


/*
**********************************************************************************************************************
*                                               cat_db_get_extra_info
*
* author:        terry.zeng
*
* date:             2010-09-06
*
* Description:      从数据库里面获取浏览项额外信息
*
* parameters:
*
* return:           if success return EPDK_OK
*                   if fail return EPDK_FAIL
* modify history:
**********************************************************************************************************************
*/
__s32 cat_db_get_extra_info(cat_db_t *db_handle, cat_list_handle_t *list_handle, __u32 index, __cat_item_t *item);


/*
**********************************************************************************************************************
*                                               cat_db_disable_extra_info
*
* author:        terry.zeng
*
* date:             2010-09-06
*
* Description:      使某一项的额外信息无效
*
* parameters:
*
* return:           if success return EPDK_OK
*                   if fail return EPDK_FAIL
* modify history:
**********************************************************************************************************************
*/
__s32 cat_db_disable_extra_info(cat_db_t *db_handle, cat_list_handle_t *list_handle, __u32 index);


/*
**********************************************************************************************************************
*                                               cat_db_sort
*
* author:        terry.zeng
*
* date:             2010-09-06
*
* Description:      对数据库数据进行排序
*
* parameters:		type：排序类型，按大小、名字、时间、类型等， mode：排序模式，升序或降序
*
* return:           if success return EPDK_OK
*                   if fail return EPDK_FAIL
* modify history:
**********************************************************************************************************************
*/
__s32 cat_db_sort(cat_db_t *db_handle, cat_list_handle_t *list_handle, cat_sort_type_t type, cat_sort_mode_t mode);


/*
**********************************************************************************************************************
*                                               cat_db_del_item
*
* author:        terry.zeng
*
* date:             2010-10-18
*
* Description:      删除数据库某一项
*
* parameters:		
*
* return:           if success return EPDK_OK
*                   if fail return EPDK_FAIL
* modify history:
**********************************************************************************************************************
*/
__s32 cat_db_del_item(cat_db_t *db_handle, cat_list_handle_t *list_handle, __u32 index, __cat_item_t *cat_item);


/*
**********************************************************************************************************************
*                                               cat_db_add_item
*
* author:        terry.zeng
*
* date:             2010-10-18
*
* Description:      往数据库里添加某一项
*
* parameters:		
*
* return:           if success return EPDK_OK
*                   if fail return EPDK_FAIL
* modify history:
**********************************************************************************************************************
*/
__s32 cat_db_add_item(cat_db_t *db_handle, cat_list_handle_t *list_handle, __cat_item_t *cat_item);


/*
**********************************************************************************************************************
*                                               cat_db_check_item_exist
*
* author:        terry.zeng
*
* date:             2010-10-18
*
* Description:      check某一项是否存在于数据库里
*
* parameters:		
*
* return:           if exist return EPDK_TRUE
*                   else return EPDK_FALSE
* modify history:
**********************************************************************************************************************
*/
__bool cat_db_check_item_exist(cat_db_t *db_handle, cat_list_handle_t *list_handle, __cat_item_t *item);


/*
**********************************************************************************************************************
*                                               cat_db_get_rowid
*
* author:        terry.zeng
*
* date:             2010-10-18
*
* Description:      通过名字获取ID
*
* parameters:		
*
* return:           if success return ID
*                   if fail return EPDK_FAIL
* modify history:
**********************************************************************************************************************
*/
__s32 cat_db_get_rowid(cat_db_t *db_handle, cat_list_handle_t *list_handle, char *filename);


/*
**********************************************************************************************************************
*                                               cat_db_search
*
* author:        terry.zeng
*
* date:             2010-09-06
*
* Description:      从数据库里面搜索文件
*
* parameters:		keyword:关键字
*
* return:           if success return EPDK_OK
*                   if fail return EPDK_FAIL
* modify history:
**********************************************************************************************************************
*/
__s32 cat_db_search(cat_db_t *db_handle, cat_list_handle_t *list_handle, char *keyword, cat_list_handle_t *search_handle);

#endif

