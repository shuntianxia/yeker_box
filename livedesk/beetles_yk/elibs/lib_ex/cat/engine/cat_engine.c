/*
*******************************************************************************************************************
*                                                         	CAT  Framework
*                                         			the core api of apps support module
*
*                             				 Copyright(C), 2006-2008, Softwinner Microelectronic Co., Ltd.
*											               All Rights Reserved
*
*File Name： 	cat.c
*
*Author： 		Terry Zeng
*
*Version :   	2.0
*
*Date:  		2010-09-09
*
*Description : the api of cat 
*
*Others : 		None at present.
*
* History :
*
* <Author>          <time>         <version>     <description>
*
* Terry.Zeng     2010-09-09         2.0          Create File
*
*******************************************************************************************************************
*/

#include "cat_engine.h"
#include ".\\..\\common\\cat_common.h"


void engine_thread(void *arg)
{
    cat_engine_t        *engine = NULL;
    cat_create_param_t  *create_para = NULL;
    __s32               result = 0;
    __u32               start_pos = 0;
    
    if(arg == NULL)
    {
        return;
    }
    else
    {
        engine = (cat_engine_t *)arg;
    }
    
    while(1)
    {
        if(esKRNL_TDelReq(EXEC_prioself) == OS_TASK_DEL_REQ)
        {
            esKRNL_TDel(EXEC_prioself);
            break;
        }
        
        if(engine->cmd_pend_flag == 0x01)
        {
            switch(engine->cmd)
            {
                case READ_START:
                {
                    create_para = &(engine->list_handle->create_para);
                    /*已存在于数据库*/
                    if(engine->list_handle->finished_status == CAT_FINISHED)
                    {
                        break;
                    }
                    
                    result = cat_db_table_create(engine->db, engine->list_handle);
                    if(result != EPDK_OK)
                    {
                        break;
                    }
                    
                    if( (create_para->scan_type == CAT_SCAN_ALL_DEVICE)
                        || (create_para->scan_type == CAT_SCAN_ALL_PATI) )
                    {
                        engine->list_handle->finished_status = CAT_FINISHED;
                        result = cat_create_root(create_para, engine->list_handle->bg_list);
                        if(result != EPDK_OK)
                        {
                            __wrn("create root explorer is fail ! \n");
                            break;
                        }
                        /*添加到数据库*/
                        cat_db_add_base_info(engine->db, engine->list_handle);
                        cat_destroy_root(engine->list_handle->bg_list);
                    }
                    else if(create_para->scan_type == CAT_SCAN_PARTI)
                    {
                        engine->list_handle->finished_status = CAT_FINISHED;
                        result = cat_create_partition_info(create_para, engine->list_handle->bg_list);
                        if(result != EPDK_OK)
                        {
                            __wrn("create partition explorer is fail ! \n");
                            break;
                        }
                        /*添加到数据库*/
                        cat_db_add_base_info(engine->db, engine->list_handle);
                        cat_destroy_partition_info(engine->list_handle->bg_list);
                    }
                    else
                    {
                        engine->list_handle->finished_status = CAT_UNFINISHED;
                        engine->status = ENGINE_BUSY;
                        /*从数据库获取已存个数*/
                        start_pos = cat_db_get_num(engine->db, engine->list_handle);
                    }
                    
                    break;
                }
                case READ_PAUSE:
                {
                    //engine->status = ENGINE_PAUSED;
                    break;
                }
                case READ_STOP:
                {
                    /*
                    if(engine->status == ENGINE_BUSY)
                    {
                        engine->list_handle->finished_status = CAT_UNFINISHED;
                    }
                    engine->status = ENGINE_STOPPED;
                    */
                    /*关闭句柄*/
                    break;
                }
            }
            engine->result = result;
            engine->cmd_pend_flag = 0x00;
            esKRNL_SemPost(engine->sem);
        }
        if(engine->status == ENGINE_BUSY)
        {
            eLIBs_strcpy(engine->list_handle->bg_list->path, create_para->path);
            if(eLIBs_strncmp(create_para->path, CAT_MEDIALIB, 14) == 0)
            {                
                result = cat_create_playlist_info(create_para, engine->list_handle->bg_list, engine, start_pos);
                if(result == EPDK_OK)
                {
                    engine->list_handle->finished_status = CAT_FINISHED;
                }
            }
            else
            {
                result = cat_create_dir_info(create_para,engine->list_handle->bg_list,engine,start_pos);
                if(result == EPDK_OK)
                {
                    cat_db_add_base_info(engine->db, engine->list_handle);
                    cat_destroy_dir_info(engine->list_handle->bg_list);
                    engine->list_handle->finished_status = CAT_FINISHED;
                    __msg("----total_num: %d\n", cat_db_get_num(engine->db, engine->list_handle));
                }
            }
            engine->status = ENGINE_STOPPED;
            //engine->list_handle->finished_status = CAT_FINISHED;
        }
        else
        {
            esKRNL_TimeDly(3);
        }
    }
    
    return;
}
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
void *cat_engine_open(cat_db_t *db)
{
    cat_engine_t    *engine = NULL;
    
    if(db == NULL)
    {
        return NULL;
    }
    
    engine = (cat_engine_t *)esMEMS_Malloc(0, sizeof(cat_engine_t));
    if(engine == NULL)
    {
        return NULL;
    }
    else
    {
        eLIBs_memset(engine, 0, sizeof(cat_engine_t));
    }
    
    engine->db = db;
    engine->thread_id = esKRNL_TCreate(engine_thread, (void *)engine, 0x8000, KRNL_priolevel4);
    engine->sem = esKRNL_SemCreate(0);
    
    return engine;
}


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
__s32 cat_engine_control(void *hengine, __u32 cmd, __s32 param1, __s32 param2)
{
    cat_engine_t    *engine = NULL;   
    __u8            err = 0;
    
    if(hengine == NULL)
    {
        return EPDK_FAIL;
    }
    else
    {
        engine = (cat_engine_t *)hengine;
    }
    
    switch(cmd)
    {
        case READ_START:
        {
            /*当前引擎正在读取其他目录信息时*/
            if(engine->status != ENGINE_STOPPED)
            {
                while(engine->cmd_pend_flag == 0x01)
                {
                    esKRNL_TimeDly(2);
                }
                if(engine->status == ENGINE_BUSY)
                {
                    engine->list_handle->finished_status = CAT_UNFINISHED;
                }
                engine->status = ENGINE_STOPPED;
                /*停止当前读取*/
                engine->cmd_pend_flag = 0x01;
                engine->cmd = READ_STOP;
                esKRNL_SemPend(engine->sem, 0, &err);
            }
            /*开始新的目录读取*/
            engine->list_handle = (cat_list_handle_t *)param1;
            engine->cmd = READ_START;
            engine->cmd_pend_flag = 0x01;
            esKRNL_SemPend(engine->sem, 0, &err);
            break;
        }
        case READ_PAUSE:
        {
            if(engine->status == ENGINE_BUSY)
            {
                while(engine->cmd_pend_flag == 0x01)
                {
                    esKRNL_TimeDly(2);
                }
                engine->status = ENGINE_PAUSED;
                /*停止当前读取*/
                engine->cmd_pend_flag = 0x01;
                engine->cmd = READ_PAUSE;
                esKRNL_SemPend(engine->sem, 0, &err);
            }
            break;
        }
        case READ_STOP:
        {
            if(engine->status != ENGINE_STOPPED)
            {
                while(engine->cmd_pend_flag == 0x01)
                {
                    esKRNL_TimeDly(2);
                }
                if(engine->status == ENGINE_BUSY)
                {
                    engine->list_handle->finished_status = CAT_UNFINISHED;
                }
                engine->status = ENGINE_STOPPED;
            }
            /*停止当前读取*/
            engine->cmd_pend_flag = 0x01;
            engine->cmd = READ_STOP;
            esKRNL_SemPend(engine->sem, 0, &err);
            break;
        }
        case READ_RESUME:
        {
            break;
        }
        case GET_STATUS:
        {
            cat_engine_status_t *status = (cat_engine_status_t *)param2;
            *status = engine->status;
            break;
        }
        case UPDEATE_ROOT:
        {
            break;
        }
        default:
        {
            break;
        }
    }
    
    return engine->result;
}


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
__s32 cat_engine_close(void *hengine)
{
    cat_engine_t    *engine = NULL;
    __u8            err = 0;
    
    if(hengine == NULL)
    {
        return EPDK_FAIL;
    }
    else
    {
        engine = (cat_engine_t *)hengine;
    }
    
    cat_engine_control(engine, READ_STOP, 0, 0);
    while(1)
    {
        if(esKRNL_TDelReq(engine->thread_id) == OS_TASK_NOT_EXIST)
        {
            break;
        }
        esKRNL_TimeDly(3);
    }
    
    esKRNL_SemDel(engine->sem, 0, &err);
    esMEMS_Mfree(0, engine);
    engine = NULL;
    
    return EPDK_OK;
}

