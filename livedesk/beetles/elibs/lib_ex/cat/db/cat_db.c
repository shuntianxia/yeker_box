/*
*******************************************************************************************************************
*                                                         	CAT  Framework
*                                         			the core api of apps support module
*
*                             				 Copyright(C), 2006-2008, Softwinner Microelectronic Co., Ltd.
*											               All Rights Reserved
*
*File Name： 	cat_db.c
*
*Author： 		Terry Zeng
*
*Version :   	2.0
*
*Date:  		2010-09-16
*
*Description : the api of cat 
*
*Others : 		None at present.
*
* History :
*
* <Author>          <time>         <version>     <description>
*
* Terry.Zeng     2010-09-16         2.0          Create File
*
*******************************************************************************************************************
*/

#include "cat_db.h"

static __s32 _mtime_to_string(ES_TIMESTAMP *m_time, char *string)
{
    /*
    if((m_time == NULL) || (string == NULL))
    {
        __msg("----input param error\n");
        return EPDK_FAIL;
    }
    else
    {    
        eLIBs_sprintf(string, "%d%d%d%d%d%d", m_time->year, m_time->month, m_time->day, m_time->hour, m_time->minite, m_time->second);
        
        return EPDK_OK;
    }    
    */
    string[0]   = m_time->year / 1000 + '0';
    string[1]   = (m_time->year % 1000) / 100 + '0';
    string[2]   = (m_time->year % 100) / 10 + '0';
    string[3]   = m_time->year % 10 + '0';
    string[4]   = m_time->month / 10 + '0';
    string[5]   = m_time->month % 10 + '0';
    string[6]   = m_time->day / 10 + '0';
    string[7]   = m_time->day % 10 + '0';
    string[8]   = m_time->hour / 10 + '0';
    string[9]   = m_time->hour % 10 + '0';
    string[10]  = m_time->minute / 10 + '0';
    string[11]  = m_time->minute % 10 + '0';
    string[12]  = m_time->second / 10 + '0';
    string[13]  = m_time->second % 10 + '0';
    string[14]  = '\0';
    
    return EPDK_OK;
}


static __s32 _string_to_mtime(char *string, ES_TIMESTAMP *m_time)
{
    m_time->year    = (string[0] - '0') * 1000 + (string[1] - '0') * 100 + (string[2] - '0') * 10 + (string[3] - '0');
    m_time->month   = (string[4] - '0') * 10 + (string[5] - '0');
    m_time->day     = (string[6] - '0') * 10 + (string[7] - '0');
    m_time->hour    = (string[8] - '0') * 10 + (string[9] - '0');
    m_time->minute  = (string[10] - '0') * 10 + (string[11] - '0');
    m_time->second  = (string[12] - '0') * 10 + (string[13] - '0');
    
    return EPDK_OK;
}


/*
**********************************************************************************************************************
*                                               cat_db_open
*
* author:        terry.zeng
*
* date:             2010-09-16
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
cat_db_t *cat_db_open(void)
{
    __s32       ret = EPDK_OK;
    cat_db_t    *db_handle = NULL;
    
    db_handle = (cat_db_t *)esMEMS_Malloc(0, sizeof(cat_db_t));
    if(db_handle == NULL)
    {
        return NULL;
    }
    else
    {
        eLIBs_memset(db_handle, 0, sizeof(cat_db_t));
    }
    
    /* initialize db libary */
    ret = DBLibary_Init();
    if(ret != EPDK_OK)
    {
        __wrn("DBLibary_Init failed\n");
        esMEMS_Mfree(0, db_handle);
        
        return NULL;
    }
    
    /* try to create demo database */
    ret = DB_Create(DB_FILE_NAME);
    if(ret != EPDK_OK)
    {
        __wrn("DB_Create %s failed\n", DB_FILE_NAME);
        esMEMS_Mfree(0, db_handle);
        DBLibary_Exit();
        
        return NULL;
    }
    
    /* connect to database */
    db_handle->hdb = DB_Connect(DB_FILE_NAME);
    if(db_handle->hdb == NULL)
    {
        __wrn("DB_Connect %s failed\n", DB_FILE_NAME);
        DB_Delete(DB_FILE_NAME);
        esMEMS_Mfree(0, db_handle);
        DBLibary_Exit();
        
        return NULL;
    }
    
    return db_handle;
}


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
__s32 cat_db_close(cat_db_t *db_handle)
{
    __s32 ret = EPDK_OK;
    
    if(db_handle == NULL)
    {
        return EPDK_FAIL;
    }
    
    /* disconnect  database */
    if(db_handle->hdb)
    {
        ret = DB_Disconnect(db_handle->hdb);
        if(ret == EPDK_FAIL)
        {
            __wrn("DB_Disconnect %s failed\n", DB_FILE_NAME);
            return EPDK_FAIL;
        }
        db_handle->hdb = NULL;
    }
    
    ret = DB_Delete(DB_FILE_NAME);
    if(ret != EPDK_OK)
    {
        __wrn("DB_Del %s failed\n", DB_FILE_NAME);
        //return EPDK_FAIL;//内存数据库会提示删除失败，不能直接退出;  
    }
    
    DBLibary_Exit();
    esMEMS_Mfree(0, db_handle);
    
    return EPDK_OK;
}


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
__s32 cat_db_table_create(cat_db_t *db_handle, cat_list_handle_t *list_handle)
{
    char    sql[CAT_MAX_FULL_PATH_LEN];
    __s32   ret = EPDK_OK;
    
    if((db_handle == NULL) || (list_handle == NULL))
    {
        return EPDK_FAIL;
    }
    
    eLIBs_memset(sql, 0, CAT_MAX_FULL_PATH_LEN);
    
    if((list_handle->create_para.scan_type == CAT_SCAN_ALL_DEVICE)
        || (list_handle->create_para.scan_type == CAT_SCAN_ALL_PATI)
        || (list_handle->create_para.scan_type == CAT_SCAN_PARTI))
    {        
        eLIBs_sprintf(sql, "create table %s \
                            ( fpath TEXT, type INTEGER, name TEXT, encoding INTEGER, pati_attr INTEGER, \
                            root_type INTEGER, size INTEGER, space_left INTEGER, extra_flag INTEGER )",
                        list_handle->table_name);
    }
    else
    {
        eLIBs_sprintf(sql, "create table %s \
                            ( fpath TEXT, type INTEGER, name TEXT, encoding INTEGER, m_time TEXT, \
                            media_type INTEGER, format INTEGER, item_num INTEGER, size INTEGER, \
                            extra1 INTEGER, extra2 INTEGER, extra3 INTEGER, extra4 INTEGER, extra5 INTEGER, \
                            extra_flag INTEGER, thumbs INTEGER, thumbs_len INTEGER )",
                        list_handle->table_name);
    }
    //__msg("sql = %s\n", sql);
    
    /* execute ctreate table SQL state */
    ret = DBConn_Execute(db_handle->hdb, sql);
    if (ret != EPDK_OK) 
    {
        __msg("create table %s failed!\n", list_handle->table_name);
       return EPDK_FAIL;
    }
    else
    {
        __msg("create table %s success\n", list_handle->table_name);
        return EPDK_OK;
    }
}


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
__s32 cat_db_table_del(cat_db_t *db_handle, cat_list_handle_t *list_handle)
{
    char    sql[CAT_MAX_FULL_PATH_LEN];
    __s32   ret = EPDK_OK;
    
    if((db_handle == NULL) || (list_handle == NULL))
    {
        return EPDK_FAIL;
    }
    
    eLIBs_memset(sql, 0, CAT_MAX_FULL_PATH_LEN);
    
    if(list_handle->cursor != NULL)
    {
        DBCursor_Free(list_handle->cursor);
        list_handle->cursor = NULL;
    }
    
    eLIBs_sprintf(sql, "drop table %s", list_handle->table_name);
    /* execute ctreate table SQL state */
    ret = DBConn_Execute(db_handle->hdb, sql);
    if(ret != EPDK_OK)
    {
        __wrn("drop table %s failed!", list_handle->table_name);
        return EPDK_FAIL;
    }
    else
    { 
        return EPDK_OK;
    }
}


/*
**********************************************************************************************************************
*                                               cat_db_del
*
* author:        terry.zeng
*
* date:             2010-09-06
*
* Description:      删除浏览信息数据库文件
*
* parameters:
*
* return:           if success return number
*                   if fail return 0
* modify history:
**********************************************************************************************************************
*/
__s32 cat_db_get_num(cat_db_t *db_handle, cat_list_handle_t *list_handle)
{
    HDBCursor   cursor = NULL;
    __s32       num = 0;
    
    if((db_handle == NULL) || (list_handle == NULL))
    {
        __wrn("----input param is error\n");
        return 0;
    }
    
    if(list_handle->attr == SEARCH_DB)
    {
        num = DBCursor_GetRowsNumber(list_handle->cursor);
    }
    else
    {
        cursor = DBConn_Query(db_handle->hdb, 0, list_handle->table_name, "type", NULL);
        if(cursor == NULL)
        {
            __wrn("Execute query SQL statement error: %s\n", DBConn_ErrorMsg(db_handle->hdb));
            return 0;
        }
        
        num = DBCursor_GetRowsNumber(cursor);
        DBCursor_Free(cursor);
        cursor = NULL;
    }
    
    return num;
}


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
__s32 cat_db_add_base_info(cat_db_t *db_handle, cat_list_handle_t *list_handle)
{
    char                sql[CAT_MAX_FULL_PATH_LEN];
    __s32               row = 0;    
    cat_item_node_t     *item = NULL;
    __s32               total_num = 0;
    __s32               ret = EPDK_OK;
    char                time_string[128];
    
    
    if((db_handle == NULL) || (list_handle == NULL) || (list_handle->bg_list == NULL) || (list_handle->bg_list->item_list == NULL))
    {
        return EPDK_FAIL;
    }
    else if(list_handle->attr == SEARCH_DB)
    {
        return EPDK_FAIL;
    }
    else
    {        
        item = list_handle->bg_list->item_list;
        total_num = list_handle->bg_list->nitem;
    }
    
    __msg("----DBConn_BeginTransaction--total_num: %d\n", total_num);
    DBConn_BeginTransaction(db_handle->hdb);
    
    if( (list_handle->create_para.scan_type == CAT_SCAN_ALL_DEVICE) 
        || (list_handle->create_para.scan_type == CAT_SCAN_ALL_PATI)
        || (list_handle->create_para.scan_type == CAT_SCAN_PARTI) )
    {
        while((row < total_num) && (item != NULL))
        {            
            eLIBs_memset(sql, 0, CAT_MAX_FULL_PATH_LEN);    
                    
            eLIBs_sprintf(sql, "insert into %s values(\"%s\", %d, \"%s\", %d, %d, %d, 0, 0, 0);",
                            list_handle->table_name,
                            item->cat_item.base_info.fpath,
                            item->cat_item.base_info.type,
                            item->cat_item.base_info.name,
                            item->cat_item.base_info.char_encoding,
                            item->cat_item.base_info.item_pri_info.pati.pati_attr,
                            item->cat_item.base_info.item_pri_info.pati.root_type);
                            
            //__msg("sql = %s\n", sql);
            ret = DBConn_Execute(db_handle->hdb, sql);
            if(ret != EPDK_OK)
            {
                __wrn("----insert error!\n");
                break;
            }
            row++;
            item = item->next;
        }
    }
    else
    {
        while((row < total_num) && (item != NULL))
        {
            eLIBs_memset(time_string, 0, 128);
            eLIBs_memset(sql, 0, CAT_MAX_FULL_PATH_LEN);
            
            if(item->cat_item.base_info.type == CAT_SW_DIR)
            {            
                _mtime_to_string(&(item->cat_item.base_info.item_pri_info.dir.m_time), time_string);
                
                eLIBs_sprintf(sql, "insert into %s values(\"%s\", %d, \"%s\", %d, \"%s\", 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);",
                                list_handle->table_name,
                                item->cat_item.base_info.fpath,
                                item->cat_item.base_info.type,
                                item->cat_item.base_info.name,
                                item->cat_item.base_info.char_encoding,
                                time_string);
            }
            else
            {
                _mtime_to_string(&(item->cat_item.base_info.item_pri_info.file.m_time), time_string);
                
                eLIBs_sprintf(sql, "insert into %s values(\"%s\", %d, \"%s\", %d, \"%s\", %d, %d, 0, %lld, 0, 0, 0, 0, 0, 0, 0, 0);",
                                list_handle->table_name,
                                item->cat_item.base_info.fpath,
                                item->cat_item.base_info.type,
                                item->cat_item.base_info.name,
                                item->cat_item.base_info.char_encoding,
                                time_string,
                                item->cat_item.base_info.item_pri_info.file.media_type,
                                item->cat_item.base_info.item_pri_info.file.format,
                                item->cat_item.base_info.item_pri_info.file.size);
            }
            
            //__msg("sql = %s\n", sql);
            ret = DBConn_Execute(db_handle->hdb, sql);
            
            if(ret != EPDK_OK)
            {
                __wrn("----insert error!\n");
                break;
            }
            row++;
            item = item->next;
        }
    }
    
    ret = DBConn_CommitTransaction(db_handle->hdb);
    __msg("----DBConn_CommitTransaction\n");
    if(ret != EPDK_OK)
    {
        return EPDK_FAIL;
    }
    else
    {
        return EPDK_OK;
    }
}


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
__s32 cat_db_get_base_info(cat_db_t *db_handle, cat_list_handle_t *list_handle, __u32 index, __cat_item_t *item)
{
    HDBCursor   cursor = NULL;
    char        time_string[128];
    __s64       size = 0;
    
    if((db_handle == NULL) || (list_handle == NULL) || (item == NULL))
    {
        return EPDK_FAIL;
    }        
    
    eLIBs_memset(time_string, 0, 128);    
    
    if(list_handle->attr == SEARCH_DB)
    {
        cursor = list_handle->cursor;
        if(cursor == NULL)
        {
            return EPDK_FAIL;
        }
        
        DBCursor_MoveToPosition(cursor, index);
        
        eLIBs_strcpy(item->base_info.fpath, DBCursor_GetStringByIdx(cursor, 0));
        item->base_info.type = (cat_item_type_t)DBCursor_GetIntByIdx(cursor, 1);
        eLIBs_strcpy(item->base_info.name, DBCursor_GetStringByIdx(cursor, 2));
        item->base_info.char_encoding = (__epdk_charset_enm_e)DBCursor_GetIntByIdx(cursor, 3);
        if((item->base_info.type == CAT_SW_FILE) || (item->base_info.type == CAT_SW_EMPTY))
        {
            eLIBs_strcpy(time_string, DBCursor_GetStringByIdx(cursor, 4));
            _string_to_mtime(time_string, &(item->base_info.item_pri_info.file.m_time));
            item->base_info.item_pri_info.file.media_type = (cat_media_type_t)DBCursor_GetIntByIdx(cursor, 5);
            item->base_info.item_pri_info.file.format = DBCursor_GetIntByIdx(cursor, 6);
            size = item->base_info.item_pri_info.file.size = (__s64)DBCursor_GetInt64ByIdx(cursor, 8);
            //__msg("size_high: %x, size_low: %x\n", (__s32)((size & 0x7fffffff00000000) >> 32), (__s32)(size & 0x00000000ffffffff));
        }
        else
        {
            eLIBs_strcpy(time_string, DBCursor_GetStringByIdx(cursor, 4));
            _string_to_mtime(time_string, &(item->base_info.item_pri_info.dir.m_time));
        }
        
        //__msg("----name: %s\n", item->base_info.name);
    }
    else if( (list_handle->create_para.scan_type == CAT_SCAN_ALL_DEVICE)
            || (list_handle->create_para.scan_type == CAT_SCAN_ALL_PATI)
            || (list_handle->create_para.scan_type == CAT_SCAN_PARTI) )
    {
        if((list_handle->cursor != NULL) && (index >= DBCursor_GetRowsNumber(list_handle->cursor)))
        {
            __msg("index: %d, num in cursor: %d\n", index, DBCursor_GetRowsNumber(list_handle->cursor));
            DBCursor_Free(list_handle->cursor);
            list_handle->cursor = NULL;            
        } 
        
        if(list_handle->cursor == NULL)
        {            
            list_handle->cursor = DBConn_Query(db_handle->hdb, 0, list_handle->table_name, 
                                "fpath, type, name, encoding, pati_attr, \
                                root_type, size, space_left", 
                                NULL);
                                
            if(list_handle->cursor == NULL)
            {
                __wrn("Execute query SQL statement error: %s\n", DBConn_ErrorMsg(db_handle->hdb));
                return EPDK_FAIL;
            }
            else
            {   
                __msg("----create cursor: %x\n", list_handle->cursor);
            }
        }
        
        cursor = list_handle->cursor;
        
        DBCursor_MoveToPosition(cursor, index);
        
        eLIBs_strcpy(item->base_info.fpath, DBCursor_GetStringByIdx(cursor, 0));
        item->base_info.type = (cat_item_type_t)DBCursor_GetIntByIdx(cursor, 1);
        eLIBs_strcpy(item->base_info.name, DBCursor_GetStringByIdx(cursor, 2));
        item->base_info.char_encoding = (__epdk_charset_enm_e)DBCursor_GetIntByIdx(cursor, 3);
        if(item->base_info.type == CAT_SW_PARTI)
        {
            item->base_info.item_pri_info.pati.pati_attr = (cat_pati_attr_t)DBCursor_GetIntByIdx(cursor, 4);
            item->base_info.item_pri_info.pati.root_type = (cat_root_t)DBCursor_GetIntByIdx(cursor, 5);
        }
    }
    else
    {
        if((list_handle->cursor != NULL) && (index >= DBCursor_GetRowsNumber(list_handle->cursor)))
        {
            __msg("index: %d, num in cursor: %d\n", index, DBCursor_GetRowsNumber(list_handle->cursor));
            DBCursor_Free(list_handle->cursor);
            list_handle->cursor = NULL;            
        }                
            
        if(list_handle->cursor == NULL)
        {
            list_handle->cursor = DBConn_Query(db_handle->hdb, 0, list_handle->table_name, 
                                            "fpath, type, name, encoding, m_time, \
                                            media_type, format, item_num, size", 
                                            NULL);
                                
            if(list_handle->cursor == NULL)
            {
                __wrn("Execute query SQL statement error: %s\n", DBConn_ErrorMsg(db_handle->hdb));
                return EPDK_FAIL;
            }
            else
            {   
                __msg("----create cursor: %x\n", list_handle->cursor);
            }
        }
        
        cursor = list_handle->cursor;
        
        DBCursor_MoveToPosition(cursor,index);
        
        eLIBs_strcpy(item->base_info.fpath, DBCursor_GetStringByIdx(cursor, 0));
        item->base_info.type = (cat_item_type_t)DBCursor_GetIntByIdx(cursor, 1);
        eLIBs_strcpy(item->base_info.name,DBCursor_GetStringByIdx(cursor, 2));
        item->base_info.char_encoding = (__epdk_charset_enm_e)DBCursor_GetIntByIdx(cursor, 3);
        if((item->base_info.type == CAT_SW_FILE) || (item->base_info.type == CAT_SW_EMPTY))
        {
            eLIBs_strcpy(time_string,DBCursor_GetStringByIdx(cursor,4));
            _string_to_mtime(time_string, &(item->base_info.item_pri_info.file.m_time));
            item->base_info.item_pri_info.file.media_type = (cat_media_type_t)DBCursor_GetIntByIdx(cursor, 5);
            item->base_info.item_pri_info.file.format = DBCursor_GetIntByIdx(cursor, 6);
            size = item->base_info.item_pri_info.file.size = (__s64)DBCursor_GetInt64ByIdx(cursor, 8);
            //__msg("size_high: %x, size_low: %x\n", (__s32)((size & 0x7fffffff00000000) >> 32), (__s32)(size & 0x00000000ffffffff));
        }
        else
        {
            eLIBs_strcpy(time_string,DBCursor_GetStringByIdx(cursor, 4));
            _string_to_mtime(time_string, &(item->base_info.item_pri_info.dir.m_time)); 
        }
        //__msg("----time_string: %s\n", time_string);
    }
    
    return EPDK_OK;
}


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
__s32 cat_db_add_extra_info(cat_db_t *db_handle, cat_list_handle_t *list_handle, __u32 index, __cat_item_t *item)
{
    HDBCursor       cursor = NULL;
    char            sql[CAT_MAX_FULL_PATH_LEN];    
    __cat_item_t    tmp_item;
    __s32           ret = EPDK_OK;
    
    if((db_handle == NULL) || (list_handle == NULL) || (item == NULL))
    {
        return EPDK_FAIL;
    }        
    
    eLIBs_memset(&tmp_item, 0, sizeof(__cat_item_t));    
    eLIBs_memset(sql, 0, CAT_MAX_FULL_PATH_LEN);
            
    ret = cat_db_get_base_info(db_handle, list_handle, index, &tmp_item);
    if(ret == EPDK_FAIL)
    {
        __msg("----%s---\n", __func__);
        return EPDK_FAIL;
    }
    else
    {
        //__msg("----fpath: %s, name: %s\n", tmp_item.base_info.fpath, tmp_item.base_info.name);
        eLIBs_sprintf(sql, "where fpath = \"%s\" and name = \"%s\" ", tmp_item.base_info.fpath, tmp_item.base_info.name);
        //eLIBs_sprintf(sql, "limit 1 offset %d", index);
    }
    
    if( (list_handle->create_para.scan_type == CAT_SCAN_ALL_DEVICE)
        || (list_handle->create_para.scan_type == CAT_SCAN_ALL_PATI)
        || (list_handle->create_para.scan_type == CAT_SCAN_PARTI) )
    {        
        cursor = DBConn_Query(db_handle->hdb, 0, list_handle->table_name, 
                            "fpath, type, name, encoding, pati_attr, \
                            root_type, size, space_left, extra_flag", 
                            sql);
                            
        if(cursor == NULL)
        {
            __wrn("Execute query SQL statement error: %s\n", DBConn_ErrorMsg(db_handle->hdb));
            return EPDK_FAIL;
        }
        
        DBCursor_MoveToPosition(cursor, 0);
        DBCursor_UpdateIntByIdx(cursor, 8, 1);
        DBCursor_UpdateIntByIdx(cursor, 6, item->extra_info.extra_info.pati.size);
        DBCursor_UpdateIntByIdx(cursor, 7, item->extra_info.extra_info.pati.space_left);
    }
    else
    {
        cursor = DBConn_Query(db_handle->hdb, 0, list_handle->table_name, 
                            "fpath, type, name, encoding, m_time, \
                            media_type, format, item_num, size, \
                            extra1, extra2, extra3, extra4, extra5, \
                            extra_flag, thumbs, thumbs_len", 
                            sql);
                            
        if (cursor == NULL)
        {
            __wrn("Execute query SQL statement error: %s\n", DBConn_ErrorMsg(db_handle->hdb));
            return EPDK_FAIL;
        }
        
        DBCursor_MoveToPosition(cursor, 0);
        DBCursor_UpdateIntByIdx(cursor, 14, 1);
        //__msg("name: %s, index: %d, type: %d\n", item->base_info.name, index, item->base_info.type);
        if(item->base_info.type == CAT_SW_FILE)
        {
            if(item->base_info.item_pri_info.file.media_type == CAT_MEDIA_TYPE_PIC)
            {
                DBCursor_UpdateIntByIdx(cursor, 9, item->extra_info.extra_info.file.file_info.pic.width);
                DBCursor_UpdateIntByIdx(cursor, 10, item->extra_info.extra_info.file.file_info.pic.height);
                DBCursor_UpdateIntByIdx(cursor, 15, (__u32)item->extra_info.extra_info.file.miniature.buf);
                DBCursor_UpdateIntByIdx(cursor, 16, (__u32)item->extra_info.extra_info.file.miniature.len);
            }
            else if(item->base_info.item_pri_info.file.media_type == CAT_MEDIA_TYPE_AUDIO)
            {
                DBCursor_UpdateIntByIdx(cursor, 9, item->extra_info.extra_info.file.file_info.audio.bitrate);
                DBCursor_UpdateIntByIdx(cursor, 10, item->extra_info.extra_info.file.file_info.audio.sample_rate);
                DBCursor_UpdateIntByIdx(cursor, 11, item->extra_info.extra_info.file.file_info.audio.channel);
                DBCursor_UpdateIntByIdx(cursor, 12, item->extra_info.extra_info.file.file_info.audio.duration);
                DBCursor_UpdateIntByIdx(cursor, 13, item->extra_info.extra_info.file.file_info.audio.bits);
                DBCursor_UpdateIntByIdx(cursor, 15, (__u32)item->extra_info.extra_info.file.miniature.buf);
                DBCursor_UpdateIntByIdx(cursor, 16, (__u32)item->extra_info.extra_info.file.miniature.len);
            }
            else if(item->base_info.item_pri_info.file.media_type == CAT_MEDIA_TYPE_VIDEO)
            {
                DBCursor_UpdateIntByIdx(cursor, 9, item->extra_info.extra_info.file.file_info.video.width);
                DBCursor_UpdateIntByIdx(cursor, 10, item->extra_info.extra_info.file.file_info.video.height);
                DBCursor_UpdateIntByIdx(cursor, 11, item->extra_info.extra_info.file.file_info.video.frame_rate);
                DBCursor_UpdateIntByIdx(cursor, 12, item->extra_info.extra_info.file.file_info.video.duration);
                //DBCursor_UpdateIntByIdx(cursor, 12, item->extra_info.extra_info.file.file_info.video.video_fmt);
                //DBCursor_UpdateIntByIdx(cursor, 13, item->extra_info.extra_info.file.file_info.video.audio_fmt);
                DBCursor_UpdateIntByIdx(cursor, 15, (__u32)item->extra_info.extra_info.file.miniature.buf);
                DBCursor_UpdateIntByIdx(cursor, 16, (__u32)item->extra_info.extra_info.file.miniature.len);
            }
        }
        else
        {
            DBCursor_UpdateIntByIdx(cursor, 7, item->extra_info.extra_info.dir.item_cnt);
            DBCursor_UpdateIntByIdx(cursor, 8, item->extra_info.extra_info.dir.size);
        }
    }
    DBCursor_CommitUpdate(cursor);    
    
    if(cursor != NULL)
    {
        DBCursor_Free(cursor);
        cursor = NULL;
    }
    
    return EPDK_OK;
}


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
__s32 cat_db_get_extra_info(cat_db_t *db_handle, cat_list_handle_t *list_handle, __u32 index, __cat_item_t *item)
{
    HDBCursor       cursor = NULL;
    char            time_string[128];
    __u32           extra_flag = 0;
    char            sql[CAT_MAX_FULL_PATH_LEN];
    __cat_item_t    tmp_item;
    __s32           ret = EPDK_OK;
    
    if((db_handle == NULL) || (list_handle == NULL) || (item == NULL))
    {
        return EPDK_FAIL;
    }        
    
    eLIBs_memset(&tmp_item, 0, sizeof(__cat_item_t));
    eLIBs_memset(time_string, 0, 128);
    eLIBs_memset(sql, 0, CAT_MAX_FULL_PATH_LEN);
            
    ret = cat_db_get_base_info(db_handle, list_handle, index, &tmp_item);
    if(ret == EPDK_FAIL)
    {
        __msg("----%s---\n", __func__);
        return EPDK_FAIL;
    }
    else
    {
        //__msg("----fpath: %s, name: %s\n", tmp_item.base_info.fpath, tmp_item.base_info.name);
        eLIBs_sprintf(sql, "where fpath = \"%s\" and name = \"%s\" ", tmp_item.base_info.fpath, tmp_item.base_info.name);
        //eLIBs_sprintf(sql, "limit 1 offset %d", index);
    }
    
    if( (list_handle->create_para.scan_type == CAT_SCAN_ALL_DEVICE)
        || (list_handle->create_para.scan_type == CAT_SCAN_ALL_PATI)
        || (list_handle->create_para.scan_type == CAT_SCAN_PARTI) )
    {
        cursor = DBConn_Query(db_handle->hdb, 0, list_handle->table_name, 
                            "fpath, type, name, encoding, pati_attr, \
                            root_type, size, space_left, extra_flag", 
                            sql);
                            
        if(cursor == NULL)
        {
            __wrn("Execute query SQL statement error: %s\n", DBConn_ErrorMsg(db_handle->hdb));
            return EPDK_FAIL;
        }     
        
        DBCursor_MoveToPosition(cursor, 0);
        extra_flag = DBCursor_GetIntByIdx(cursor, 8);
        if(extra_flag == 0)
        {
            if(cursor != NULL)
            {
                DBCursor_Free(cursor);
                cursor = NULL;
            }
            return EPDK_FAIL;
        }
        
        item->base_info.type = (cat_item_type_t)DBCursor_GetIntByIdx(cursor, 1);
        if(item->base_info.type == CAT_SW_PARTI)
        {
            item->extra_info.extra_info.pati.size = (cat_pati_attr_t)DBCursor_GetIntByIdx(cursor, 6);
            item->extra_info.extra_info.pati.space_left = (cat_root_t)DBCursor_GetIntByIdx(cursor, 7);
        }
    }
    else
    {
        cursor = DBConn_Query(db_handle->hdb, 0, list_handle->table_name, 
                            "fpath, type, name, encoding, m_time, \
                            media_type, format, item_num, size, \
                            extra1, extra2, extra3, extra4, extra5, \
                            extra_flag, thumbs, thumbs_len", 
                            sql);
                            
        if(cursor == NULL)
        {
            __wrn("Execute query SQL statement error: %s\n", DBConn_ErrorMsg(db_handle->hdb)); 
            return EPDK_FAIL;
        }
                
        DBCursor_MoveToPosition(cursor, 0);
        extra_flag = DBCursor_GetIntByIdx(cursor, 14);
        //__msg("----extra_flag: %d, name: %s\n", extra_flag, DBCursor_GetStringByIdx(cursor, 2));
        if(extra_flag == 0)
        {
            if(cursor != NULL)
            {
                DBCursor_Free(cursor);
                cursor = NULL;
            }
            return EPDK_FAIL;
        }
        
        eLIBs_strcpy(item->base_info.fpath,DBCursor_GetStringByIdx(cursor, 0));
        item->base_info.type = (cat_item_type_t)DBCursor_GetIntByIdx(cursor, 1);
        eLIBs_strcpy(item->base_info.name,DBCursor_GetStringByIdx(cursor, 2));
        item->base_info.char_encoding = (__epdk_charset_enm_e)DBCursor_GetIntByIdx(cursor, 3);
        if( item->base_info.type == CAT_SW_FILE)
        {
            __s64 size = 0;
            eLIBs_strcpy(time_string, DBCursor_GetStringByIdx(cursor, 4));
            _string_to_mtime(time_string, &(item->base_info.item_pri_info.file.m_time));
            item->base_info.item_pri_info.file.media_type = (cat_media_type_t)DBCursor_GetIntByIdx(cursor, 5);
            item->base_info.item_pri_info.file.format = DBCursor_GetIntByIdx(cursor, 6);
            size = item->base_info.item_pri_info.file.size = (__s64)DBCursor_GetInt64ByIdx(cursor, 8);
            //__msg("size_high: %x, size_low: %x\n", (__s32)((size & 0x7fffffff00000000) >> 32), (__s32)(size & 0x00000000ffffffff));
            if(item->base_info.item_pri_info.file.media_type == CAT_MEDIA_TYPE_PIC)	
            {
                item->extra_info.extra_info.file.file_info.pic.width = DBCursor_GetIntByIdx(cursor, 9);
                item->extra_info.extra_info.file.file_info.pic.height = DBCursor_GetIntByIdx(cursor, 10);
                item->extra_info.extra_info.file.miniature.buf = (char *)DBCursor_GetIntByIdx(cursor, 15);
                item->extra_info.extra_info.file.miniature.len = (__u32)DBCursor_GetIntByIdx(cursor, 16);
            }
            else if(item->base_info.item_pri_info.file.media_type == CAT_MEDIA_TYPE_AUDIO)   
            {
                item->extra_info.extra_info.file.file_info.audio.bitrate = DBCursor_GetIntByIdx(cursor, 9);
                item->extra_info.extra_info.file.file_info.audio.sample_rate = DBCursor_GetIntByIdx(cursor, 10);
                item->extra_info.extra_info.file.file_info.audio.channel = DBCursor_GetIntByIdx(cursor, 11);
                item->extra_info.extra_info.file.file_info.audio.duration = DBCursor_GetIntByIdx(cursor, 12);
                item->extra_info.extra_info.file.file_info.audio.bits = DBCursor_GetIntByIdx(cursor, 13);
                item->extra_info.extra_info.file.miniature.buf = (char *)DBCursor_GetIntByIdx(cursor, 15);
                item->extra_info.extra_info.file.miniature.len = (__u32)DBCursor_GetIntByIdx(cursor, 16);
            }
            else if(item->base_info.item_pri_info.file.media_type == CAT_MEDIA_TYPE_VIDEO) 	
            {
                item->extra_info.extra_info.file.file_info.video.width = DBCursor_GetIntByIdx(cursor, 9);
                item->extra_info.extra_info.file.file_info.video.height = DBCursor_GetIntByIdx(cursor, 10);
                item->extra_info.extra_info.file.file_info.video.frame_rate = DBCursor_GetIntByIdx(cursor, 11);
                item->extra_info.extra_info.file.file_info.video.duration = DBCursor_GetIntByIdx(cursor, 12);
                //item->extra_info.extra_info.file.file_info.video.video_fmt = DBCursor_GetIntByIdx(cursor, 12);
                //item->extra_info.extra_info.file.file_info.video.audio_fmt = DBCursor_GetIntByIdx(cursor, 13);
                item->extra_info.extra_info.file.miniature.buf = (char *)DBCursor_GetIntByIdx(cursor, 15);
                item->extra_info.extra_info.file.miniature.len = (__u32)DBCursor_GetIntByIdx(cursor, 16);
            }
        }
        else
        {
            item->extra_info.extra_info.dir.size = DBCursor_GetIntByIdx(cursor, 8);
            item->extra_info.extra_info.dir.item_cnt = DBCursor_GetIntByIdx(cursor,7);
        }
    }
    if(cursor != NULL)
    {
        DBCursor_Free(cursor);
        cursor = NULL;
    }
    
    return EPDK_OK;
}


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
__s32 cat_db_disable_extra_info(cat_db_t *db_handle, cat_list_handle_t *list_handle, __u32 index)
{
    HDBCursor       cursor = NULL;
    char            sql[CAT_MAX_FULL_PATH_LEN];
    __cat_item_t    tmp_item;
    __s32           ret = EPDK_OK;
    
    if((db_handle == NULL) || (list_handle == NULL))
    {
        return EPDK_FAIL;
    }
    
    //__msg("----index: %d\n", index);
        
    eLIBs_memset(&tmp_item, 0, sizeof(__cat_item_t));    
    eLIBs_memset(sql, 0, CAT_MAX_FULL_PATH_LEN);
            
    ret = cat_db_get_base_info(db_handle, list_handle, index, &tmp_item);
    if(ret == EPDK_FAIL)
    {
        __msg("----%s---\n", __func__);
        return EPDK_FAIL;
    }
    else
    {
        //__msg("----fpath: %s, name: %s\n", tmp_item.base_info.fpath, tmp_item.base_info.name);
        eLIBs_sprintf(sql, "where fpath = \"%s\" and name = \"%s\" ", tmp_item.base_info.fpath, tmp_item.base_info.name);
        //eLIBs_sprintf(sql, "limit 1 offset %d", index);
    }
    
    if( (list_handle->create_para.scan_type == CAT_SCAN_ALL_DEVICE) 
        || (list_handle->create_para.scan_type == CAT_SCAN_ALL_PATI)
        || (list_handle->create_para.scan_type == CAT_SCAN_PARTI) )
    {
        cursor = DBConn_Query(db_handle->hdb, 0, list_handle->table_name, 
                            "fpath, type, name, encoding, pati_attr, \
                            root_type, size, space_left, extra_flag", 
                            sql);
                            
        if(cursor == NULL)
        {
            __wrn("Execute query SQL statement error: %s\n", DBConn_ErrorMsg(db_handle->hdb));
            return EPDK_FAIL;
        }
        
        DBCursor_MoveToPosition(cursor, 0);
        DBCursor_UpdateIntByIdx(cursor, 8, 0);
    }
    else
    {
        cursor = DBConn_Query(db_handle->hdb, 0, list_handle->table_name, 
                            "fpath, type, name, encoding, m_time, \
                            media_type, format, item_num, size, \
                            extra1, extra2, extra3, extra4, extra5, \
                            extra_flag, thumbs, thumbs_len", 
                            sql);
                            
        if(cursor == NULL)
        {
            __wrn("Execute query SQL statement error: %s\n", DBConn_ErrorMsg(db_handle->hdb));
            return EPDK_FAIL;
        }
        
        DBCursor_MoveToPosition(cursor, 0);
        DBCursor_UpdateIntByIdx(cursor, 14, 0);
    }
    DBCursor_CommitUpdate(cursor);
    
    if(cursor != NULL )
    {
        DBCursor_Free(cursor);
        cursor = NULL;
    }
    
    return EPDK_OK;
    
}


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
__s32 cat_db_sort(cat_db_t *db_handle, cat_list_handle_t *list_handle, cat_sort_type_t type, cat_sort_mode_t mode)
{
    char    sql[CAT_MAX_FULL_PATH_LEN];
    char    sort_expl[128];
    char    sort_mode[32];
    char    sort_type[32];
    
    if((db_handle == NULL) || (list_handle == NULL))
    {
        return EPDK_FAIL;
    }
    
    eLIBs_memset(sql, 0, CAT_MAX_FULL_PATH_LEN);
    eLIBs_memset(sort_expl, 0, 128);
    eLIBs_memset(sort_mode, 0, 32);
    eLIBs_memset(sort_type, 0, 32);
    
    if(mode == CAT_ASCEND)
    {
        eLIBs_sprintf(sort_mode, "asc");
    }
    else
    {
        eLIBs_sprintf(sort_mode, "desc");
    }
    
    if(type == CAT_SORT_NAME)
    {
        eLIBs_sprintf(sort_type, "name");
    }
    else if(type == CAT_SORT_SIZE)
    {
        eLIBs_sprintf(sort_type, "size");
    }
    else if(type == CAT_SORT_TIME)
    {
        eLIBs_sprintf(sort_type, "m_time");
    }
    else if(type == CAT_SORT_FORMAT)
    {
        eLIBs_sprintf(sort_type, "format");
    }
    else
    {
        eLIBs_sprintf(sort_type, "name");
    }
    
    if(list_handle->cursor != NULL)
    {
        DBCursor_Free(list_handle->cursor);
        list_handle->cursor = NULL;
    }
    
    if(list_handle->attr == SEARCH_DB)
    {        
        if(type == CAT_SORT_NAME)
        {
            eLIBs_sprintf(sql, "where name like \"%%%s%%\" order by %s %s", list_handle->search_key, sort_type, sort_mode);
        }
        else
        {
            eLIBs_sprintf(sql, "where name like \"%%%s%%\" order by %s %s, name %s", list_handle->search_key, sort_type, sort_mode, sort_mode);
        }
        __msg("----sql: %s\n", sql);
        
        list_handle->cursor = DBConn_Query(db_handle->hdb, 0, list_handle->table_name, 
                                        "fpath, type, name, encoding, m_time, \
                                        media_type, format, item_num, size",
                                        sql);
                                        
        if(list_handle->cursor == NULL)
        {
            __wrn("Execute query SQL statement error: %s\n", DBConn_ErrorMsg(db_handle->hdb));
            return EPDK_FAIL;
        }
        else
        {   
            __msg("----create cursor: %x\n", list_handle->cursor);
        }
    }
    else if( (list_handle->create_para.scan_type == CAT_SCAN_ALL_DEVICE)
        || (list_handle->create_para.scan_type == CAT_SCAN_ALL_PATI)
        || (list_handle->create_para.scan_type == CAT_SCAN_PARTI) )
    {
        eLIBs_sprintf(sql, "order by name %s", sort_mode);        
        __msg("----sql: %s\n", sql);
        
        list_handle->cursor = DBConn_Query(db_handle->hdb, 0, list_handle->table_name, 
                                        "fpath, type, name, encoding, pati_attr, \
                                        root_type, size, space_left", 
                                        sql);
                                        
        if(list_handle->cursor == NULL)
        {
            __wrn("Execute query SQL statement error: %s\n", DBConn_ErrorMsg(db_handle->hdb));
            return EPDK_FAIL;
        }
        else
        {   
            __msg("----create cursor: %x\n", list_handle->cursor);
        }
    }
    else
    {        
        if(type == CAT_SORT_NAME)
        {
            eLIBs_sprintf(sql, "order by name %s", sort_mode);
        }
        else
        {
            eLIBs_sprintf(sql, "order by %s %s, name %s", sort_type, sort_mode, sort_mode);
        }
        __msg("----sql: %s\n", sql);
        
        list_handle->cursor = DBConn_Query(db_handle->hdb, 0, list_handle->table_name,  
                                        "fpath, type, name, encoding, m_time, \
                                        media_type, format, item_num, size",
                                        sql);
                                                
        if (list_handle->cursor == NULL)
        {
            __wrn("Execute query SQL statement error: %s\n", DBConn_ErrorMsg(db_handle->hdb));
            return EPDK_FAIL;
        }
        else
        {   
            __msg("----create cursor: %x\n", list_handle->cursor);
        }
    }
    
    return EPDK_OK;
}


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
__s32 cat_db_del_item(cat_db_t *db_handle, cat_list_handle_t *list_handle, __u32 index, __cat_item_t *cat_item)
{    
    char        sql[CAT_MAX_FULL_PATH_LEN];
    __s32       result = 0;
    
    if((db_handle == NULL) || (list_handle == NULL) || (cat_item == NULL))
    {
        return EPDK_FAIL;
    }
    
    eLIBs_memset(sql, 0, CAT_MAX_FULL_PATH_LEN);
    
    if(list_handle->cursor != NULL)
    {        
        if(list_handle->attr == SEARCH_DB)
        {
            DBCursor_DeleteRowByIdx(list_handle->cursor, index);
        }
        else
        {
            DBCursor_Free(list_handle->cursor);
            list_handle->cursor = NULL;
        }
    }
    
    if(eLIBs_strcmp(cat_item->base_info.fpath, "") == 0)
    {
        eLIBs_sprintf(sql, "delete from %s where name = \"%s\";", list_handle->table_name, cat_item->base_info.name);
    }
    else
    {
        eLIBs_sprintf(sql, "delete from %s where fpath = \"%s\" and name = \"%s\";", list_handle->table_name, cat_item->base_info.fpath, cat_item->base_info.name);
    }
    
    __msg("----sql: %s\n", sql);
    DBConn_BeginTransaction(db_handle->hdb);
    result = DBConn_Execute(db_handle->hdb, sql);
    DBConn_CommitTransaction(db_handle->hdb);
    
    if(result != EPDK_OK) 
    {
        __wrn("Execute SQL statement [%s] error: %s\n", sql, DBConn_ErrorMsg(db_handle->hdb));
        return EPDK_FAIL;
    }
    else
    {
        __s32 total = cat_db_get_num(db_handle, list_handle);
        __msg("----delete success----total: %d\n", total);
    }    
    
    return EPDK_OK;
}


/*
**********************************************************************************************************************
*                                               cat_db_add_item
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
__s32 cat_db_add_item(cat_db_t *db_handle, cat_list_handle_t *list_handle, __cat_item_t *cat_item)
{    
    __s32       rowIdx = 0;
    __s32       ret = 0;
    char        time_string[128];
    char        sql[CAT_MAX_FULL_PATH_LEN];
    
    if((db_handle == NULL) || (list_handle == NULL))
    {
        return EPDK_FAIL;
    }        
    
    if( (list_handle->create_para.scan_type == CAT_SCAN_ALL_DEVICE) 
        || (list_handle->create_para.scan_type == CAT_SCAN_ALL_PATI)
        || (list_handle->create_para.scan_type == CAT_SCAN_PARTI) )
    {
        if(list_handle->cursor != NULL)
        {
            DBCursor_Free(list_handle->cursor);
            list_handle->cursor = NULL;
        }
        
        eLIBs_memset(sql, 0, CAT_MAX_FULL_PATH_LEN);    
        eLIBs_sprintf(sql, "insert into %s values(\"%s\", %d, \"%s\", %d, %d, %d, 0, 0, 0);",
                            list_handle->table_name,
                            cat_item->base_info.fpath,
                            cat_item->base_info.type,
                            cat_item->base_info.name,
                            cat_item->base_info.char_encoding,
                            cat_item->base_info.item_pri_info.pati.pati_attr,
                            cat_item->base_info.item_pri_info.pati.root_type);
                            
        __msg("sql = %s\n", sql);
        ret = DBConn_Execute(db_handle->hdb, sql);
        if(ret != EPDK_OK)
        {
            __wrn("----insert error!\n");
            return EPDK_FAIL;
        }
        else
        {
            __msg("----insert success\n");
            return EPDK_OK;
        }
    }
    else
    {
        if(list_handle->cursor == NULL)
        {
            list_handle->cursor = DBConn_Query(db_handle->hdb, 0, list_handle->table_name, 
                                            "fpath, type, name, encoding, m_time, \
                                            media_type, format, item_num, size", 
                                            NULL);
                                
            if(list_handle->cursor == NULL)
            {
                __wrn("Execute query SQL statement error: %s\n", DBConn_ErrorMsg(db_handle->hdb));
                return EPDK_FAIL;
            }            
            else
            {   
                __msg("----create cursor: %x\n", list_handle->cursor);
            }
        }
        
        eLIBs_memset(time_string, 0, 128);
        eLIBs_memset(sql, 0, CAT_MAX_FULL_PATH_LEN);
        
        if(cat_item->base_info.type == CAT_SW_DIR)
        {
            _mtime_to_string(&(cat_item->base_info.item_pri_info.dir.m_time), time_string);
            
            rowIdx = DBCursor_InsertRow(list_handle->cursor, "(\"%s\", %d, \"%s\", %d, \"%s\", %d, %d, %d, %d);",
                                            cat_item->base_info.fpath, 
                                            cat_item->base_info.type,
                                            cat_item->base_info.name, 
                                            cat_item->base_info.char_encoding,
                                            time_string, 
                                            0, 0, 0, 0);
                                            
            eLIBs_sprintf(sql, "insert into %s values(\"%s\", %d, \"%s\", %d, \"%s\", 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);",
                            list_handle->table_name,
                            cat_item->base_info.fpath,
                            cat_item->base_info.type,
                            cat_item->base_info.name,
                            cat_item->base_info.char_encoding,
                            time_string);
        }
        else
        {
            _mtime_to_string(&(cat_item->base_info.item_pri_info.file.m_time), time_string);
        
            rowIdx = DBCursor_InsertRow(list_handle->cursor, "(\"%s\", %d, \"%s\", %d, \"%s\", %d, %d, %d, %lld);",
                                            cat_item->base_info.fpath, 
                                            cat_item->base_info.type,
                                            cat_item->base_info.name, 
                                            cat_item->base_info.char_encoding,
                                            time_string, 
                                            cat_item->base_info.item_pri_info.file.media_type,
                                            cat_item->base_info.item_pri_info.file.format,
                                            0, 
                                            cat_item->base_info.item_pri_info.file.size);
                                                        
            eLIBs_sprintf(sql, "insert into %s values(\"%s\", %d, \"%s\", %d, \"%s\", %d, %d, 0, %lld, 0, 0, 0, 0, 0, 0, 0, 0);",
                            list_handle->table_name,
                            cat_item->base_info.fpath,
                            cat_item->base_info.type,
                            cat_item->base_info.name,
                            cat_item->base_info.char_encoding,
                            time_string,
                            cat_item->base_info.item_pri_info.file.media_type,
                            cat_item->base_info.item_pri_info.file.format,
                            cat_item->base_info.item_pri_info.file.size);
        }
                                    
        __msg("----------rowIdx: %d\n", rowIdx);        
        __msg("----sql: %s\n", sql);
        
        if(rowIdx < 0)
        {
            __wrn("insert row error\n");
            if(list_handle->cursor != NULL)
            {
                DBCursor_Free(list_handle->cursor);
                list_handle->cursor = NULL;
            }
        }
        
        DBConn_BeginTransaction(db_handle->hdb);
        ret = DBConn_Execute(db_handle->hdb, sql);
        DBConn_CommitTransaction(db_handle->hdb);
        
        if(ret != EPDK_OK) 
        {
            __wrn("Execute SQL statement [%s] error: %s\n", sql, DBConn_ErrorMsg(db_handle->hdb));
            return EPDK_FAIL;
        }
        else
        {
            __s32 total = cat_db_get_num(db_handle, list_handle);
            __msg("----insert success----total: %d\n", total);
        }        
    }
    
    return EPDK_OK;
}


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
__bool cat_db_check_item_exist(cat_db_t *db_handle, cat_list_handle_t *list_handle, __cat_item_t *item)
{
    if((db_handle == NULL) || (list_handle == NULL) || (item == NULL))
    {
        __wrn("----input param is error\n");
        return EPDK_FALSE;
    }
    else
    {
        HDBCursor   cursor = NULL;
        __s32       num = 0;
        char  sql[CAT_MAX_FULL_PATH_LEN];
        
        eLIBs_memset(sql, 0, CAT_MAX_FULL_PATH_LEN);
        
        if(eLIBs_strcmp(item->base_info.fpath, "") == 0)
        {
            eLIBs_sprintf(sql, "where name = \"%s\"", item->base_info.name);
        }
        else
        {
            eLIBs_sprintf(sql, "where fpath = \"%s\" and name = \"%s\"", item->base_info.fpath, item->base_info.name);
        }
        
        __msg("----sql: %s\n", sql);        
    
        cursor = DBConn_Query(db_handle->hdb, 0, list_handle->table_name, "type", sql);
        if(cursor == NULL)
        {
            __wrn("Execute query SQL statement error: %s\n", DBConn_ErrorMsg(db_handle->hdb));
            return 0;
        }
        
        num = DBCursor_GetRowsNumber(cursor);
        DBCursor_Free(cursor);
        cursor = NULL;
        __msg("----num: %d\n", num);
        if(num == 0)
        {
            return EPDK_FALSE;
        }
        else
        {
            return EPDK_TRUE;
        }
    }
}

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
* return:           if success return row id
*                   if fail return -1
* modify history:
**********************************************************************************************************************
*/
__s32 cat_db_get_rowid(cat_db_t *db_handle, cat_list_handle_t *list_handle, char *filename)
{
    __s32       rowid = -1;    
    __s32       num = 0;
    __u32       i = 0;    
    char        fullpath[CAT_MAX_FULL_PATH_LEN];
    
    if((db_handle == NULL) || (list_handle == NULL) || (filename == NULL))
    {
        return -1;
    }
    
    if( (list_handle->create_para.scan_type == CAT_SCAN_ALL_DEVICE)
        || (list_handle->create_para.scan_type == CAT_SCAN_ALL_PATI)
        || (list_handle->create_para.scan_type == CAT_SCAN_PARTI) )
    {
        return -1;
    }
    else
    {
        if(list_handle->cursor == NULL )
        {
            list_handle->cursor = DBConn_Query(db_handle->hdb, 0, list_handle->table_name, 
                                            "fpath, type, name, encoding, m_time, \
                                            media_type, format, item_num, size", 
                                            NULL);
                                
            if(list_handle->cursor == NULL)
            {
                __wrn("Execute query SQL statement error: %s\n", DBConn_ErrorMsg(db_handle->hdb));
                return -1;
            }
            else
            {   
                __msg("----create cursor: %x\n", list_handle->cursor);
            }
        }
        num = DBCursor_GetRowsNumber(list_handle->cursor);
        for(i=0; i<num; i++)
        {
            DBCursor_MoveToPosition(list_handle->cursor, i);
            eLIBs_memset(fullpath, 0, CAT_MAX_FULL_PATH_LEN);
            eLIBs_sprintf(fullpath, "%s\\%s", DBCursor_GetStringByIdx(list_handle->cursor, 0), DBCursor_GetStringByIdx(list_handle->cursor, 2));
            __msg("filename: %s, fullpath: %s\n", filename, fullpath);
            if(eLIBs_strcmp(filename, fullpath) == 0)
            {
                rowid = i;
                break;
            }
        }
    }
    
    return rowid;
}


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
__s32 cat_db_search(cat_db_t *db_handle, cat_list_handle_t *list_handle, char *keyword, cat_list_handle_t *search_handle)
{
    char  sql[CAT_MAX_FULL_PATH_LEN];
    
    if((db_handle == NULL) || (list_handle == NULL) || (keyword == NULL) || (search_handle == NULL))
    {
        return EPDK_FAIL;
    }
    
    if(search_handle->cursor != NULL)
    {
        DBCursor_Free(search_handle->cursor);
        search_handle->cursor = NULL;
    }
    
    eLIBs_strcpy(search_handle->search_key, keyword);
    
    eLIBs_memset(sql, 0, CAT_MAX_FULL_PATH_LEN);
    eLIBs_sprintf(sql, "where name like \"%%%s%%\"", keyword);
    search_handle->cursor = DBConn_Query(db_handle->hdb, 0, list_handle->table_name, 
                                        "fpath, type, name, encoding, m_time, \
                                        media_type, format, item_num, size",
                                        sql);


    __msg("----table_name: %s, sql: %s, cursor: %x\n", list_handle->table_name, sql, search_handle->cursor);      
                                              
    if(search_handle->cursor == NULL)
    {
        __wrn("Execute query SQL statement error: %s\n", DBConn_ErrorMsg(db_handle->hdb));
        return EPDK_FAIL;
    }
    else
    {
        return EPDK_OK;
    }
}


