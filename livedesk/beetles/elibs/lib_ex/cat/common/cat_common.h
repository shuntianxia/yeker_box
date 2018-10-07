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

#ifndef __CAT_COMMON_H__
#define __CAT_COMMON_H__

#include ".\\..\\cat_p.h"
#include ".\\..\\engine\\cat_engine.h"

#define ORCHID_FILE_PATH                "b:\\module\\orchid"

/*
**********************************************************************************************************************
*                                               cat_create_root
*
* author:          terry.zeng 
*
* date:             2010-09-13
*
* Description:       创建根浏览信息 
*
* parameters:       param 创建输入参数 cat_list为输出的浏览信息
*
* return:           success: EPDK_OK;	failed: EPDK_FAIL;
* modify history: 
**********************************************************************************************************************
*/
__s32 cat_create_root(cat_create_param_t *param, cat_list_t *cat_list);


/*
**********************************************************************************************************************
*                                               cat_destroy_root
*
* author:          terry.zeng 
*
* date:             2010-09-13
*
* Description:       删除根目录信息
*
* parameters:       __cat_item_t *item
*
* return:           success: EPDK_OK;	failed: EPDK_FAIL;
* modify history: 
**********************************************************************************************************************
*/
__s32 cat_destroy_root(cat_list_t *cat_list);


/*
**********************************************************************************************************************
*                                               cat_create_partition_info
*
* author:          terry.zeng 
*
* date:             2010-09-13
*
* Description:       创建分区浏览信息 
*
* parameters:       param 创建输入参数,输出的浏览信息
*
* return:           success: EPDK_OK;	failed: EPDK_FAIL;
* modify history: 
**********************************************************************************************************************
*/
__s32 cat_create_partition_info(cat_create_param_t *param, cat_list_t *cat_list);


/*
**********************************************************************************************************************
*                                               cat_destroy_partition_info
*
* author:          terry.zeng 
*
* date:             2010-09-13
*
* Description:       销毁分区浏览信息 
*
* parameters:       cat_list 浏览信息
*
* return:           success: EPDK_OK;	failed: EPDK_FAIL;
* modify history: 
**********************************************************************************************************************
*/
__s32 cat_destroy_partition_info(cat_list_t *cat_list);


/*
**********************************************************************************************************************
*                                               cat_create_dir_info
*
* author:          terry.zeng 
*
* date:             2010-09-13
*
* Description:       创建目录浏览信息 
*
* parameters:       param 创建输入参数,输出的浏览信息
*
* return:           success: EPDK_OK;	failed: EPDK_FAIL;
* modify history: 
**********************************************************************************************************************
*/
__s32 cat_create_dir_info(cat_create_param_t *param, cat_list_t *cat_list, cat_engine_t *engine, __u32 start_pos);


/*
**********************************************************************************************************************
*                                               cat_destroy_playlist_info
*
* author:          terry.zeng 
*
* date:             2010-09-30
*
* Description:       销毁目录浏览信息
*
* parameters:       cat_list 浏览信息
*
* return:           success: EPDK_OK;   failed: EPDK_FAIL;
* modify history: 
**********************************************************************************************************************
*/
__s32 cat_destroy_dir_info(cat_list_t *cat_list);


/*
**********************************************************************************************************************
*                                               cat_create_playlist
*
* author:          terry.zeng 
*
* date:             2010-09-30
*
* Description:       创建playlist浏览信息 
*
* parameters:       param 创建输入参数,输出的浏览信息
*
* return:           success: EPDK_OK;	failed: EPDK_FAIL;
* modify history: 
**********************************************************************************************************************
*/
__s32 cat_create_playlist_info(cat_create_param_t *param, cat_list_t *cat_list, cat_engine_t *engine, __u32 start_pos);


/*
**********************************************************************************************************************
*                                               cat_destroy_playlist_info
*
* author:          terry.zeng 
*
* date:             2010-09-30
*
* Description:       销毁playlist浏览信息 
*
* parameters:       cat_list 浏览信息
*
* return:           success: EPDK_OK;	failed: EPDK_FAIL;
* modify history: 
**********************************************************************************************************************
*/
__s32 cat_destroy_playlist_info(cat_list_t *cat_list);


/*
**********************************************************************************************************************
*                                               cat_get_file_count_in_dir
*
* author:          terry.zeng 
*
* date:             2009-09-30
*
* Description:       获取目录底下的符合要求格式的文件个数 
*
* parameters:       path 目录全路径名  media_type：媒体类型
*
* return:           文件个数
* modify history: 
**********************************************************************************************************************
*/
__u32 cat_get_file_count_in_dir(char *path, __u32 check_type);


#endif
