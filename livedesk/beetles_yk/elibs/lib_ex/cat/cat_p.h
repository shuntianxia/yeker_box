/*
*********************************************************************************************************
*											        ePDK
*						            the Easy Portable/Player Develop Kits
*									           cat  sample
*
*						        (c) Copyright 2006-2007, terry, China
*											All	Rights Reserved
*
* File    : cat_p.h
* By      : terry.zeng
* Version : V2.00
* Date	  : 2010-09-09
*********************************************************************************************************
*/
#ifndef __CAT_P_H__
#define __CAT_P_H__

#include "epdk.h"
#include "lib_ex.h"

#define TABLE_PRE "table"


typedef enum cat_finish_status_s
{
    CAT_UNFINISHED = 0x01,
    CAT_FINISHED
}cat_finish_status_t;


typedef struct cat_item_node_s
{
    __cat_item_t            cat_item;
    struct cat_item_node_s  *next;
}cat_item_node_t;


typedef struct cat_list_s
{
    __u32               nitem;
    char                path[CAT_MAX_FULL_PATH_LEN];
    cat_item_node_t     *item_list;
}cat_list_t;


typedef enum cat_list_handle_attr_s
{
    CREATE_DB = 0,
    SEARCH_DB
}cat_list_handle_attr_t;


typedef struct cat_list_handle_s
{
    cat_list_t                  *bg_list;           /*后台用于存储的链表*/
    cat_finish_status_t         finished_status;    /*是否已经搜索完毕*/
    cat_create_param_t          create_para;        /*创建的参数*/
    char                        table_name[32];     /*对应数据库的表名*/
    HDBCursor                   cursor;
    char                        search_key[CAT_MAX_FULL_PATH_LEN];
    void                        *cache_handle;    
    cat_list_handle_attr_t      attr;
    struct cat_list_handle_s    *next;
}cat_list_handle_t;


typedef enum cat_file_op_cmd_s
{
    CAT_DEL = 0x01,
    CAT_PASTE_FILE,
    CAT_PASTE_DIR
}cat_file_op_cmd_t;

typedef enum cat_file_op_status_s
{
    CAT_DELETING = 0x01,
    CAT_DELOVER,
    CAT_FILE_PASTING,
    CAT_FILE_PASTEOVER,
    CAT_DIR_PASTING,
    CAT_DIR_PASTEOVER
}cat_file_op_status_t;



#endif
