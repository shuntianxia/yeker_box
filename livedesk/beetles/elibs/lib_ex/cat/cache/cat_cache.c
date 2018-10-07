/*
*******************************************************************************************************************
*                                                         	CAT  Framework
*                                         			the core api of apps support module
*
*                             				 Copyright(C), 2006-2008, Softwinner Microelectronic Co., Ltd.
*											               All Rights Reserved
*
*File Name： 	cat_cache.c
*
*Author： 		Terry Zeng
*
*Version :   	1.1.0
*
*Date:  		2010-10-13
*
*Description : the api of cat 
*
*Others : 		None at present.
*
* History :
*
* <Author>          <time>         <version>     <description>
*
* Terry.Zeng     2010-10-13         1.1.0          Create File
*
*******************************************************************************************************************
*/

#include "cat_cache.h"


typedef struct
{
    cat_cache_t         cache_info;
    cat_cache_para_t    para;
    char                *buf_pool;
    cat_db_t            *db;
    cat_list_handle_t   *list_handle;
}cache_prc_t;


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
void *cat_cache_inner_create(cat_db_t *db, cat_list_handle_t *list_handle, cat_cache_para_t *para)
{
    cache_prc_t             *cache_prc = NULL;
    cat_cache_node_t        *cache_node = NULL;
    cat_cache_node_t        *tmp_node = NULL;
    __s32                   i = 0;
    __s32                   palloc_size = 0;    //KB
    
    cache_prc = (cache_prc_t *)esMEMS_Malloc(0, sizeof(cache_prc_t));
    if(cache_prc == NULL)
    {
        return NULL;
    }
    else
    {
        eLIBs_memset(cache_prc, 0, sizeof(cache_prc_t));
    }

    cache_prc->para.cache_num = para->cache_num;
    cache_prc->para.buf_size = para->buf_size;
    palloc_size = para->cache_num * para->buf_size / 1024 + 1;
    __msg("----cache_num: %d, buf_size: %d, palloc_size: %d\n", cache_prc->para.cache_num, cache_prc->para.buf_size, palloc_size);
    cache_prc->buf_pool = (char *)esMEMS_Palloc(palloc_size, 0);
    if(cache_prc->buf_pool == NULL)
    {
        esMEMS_Mfree(0, cache_prc);
        return NULL;
    }
    else
    {
        eLIBs_memset(cache_prc->buf_pool, 0, palloc_size * 1024);
    }
    cache_prc->cache_info.used_list = NULL;
    cache_prc->cache_info.unused_list = NULL;

    for(i = 0; i < para->cache_num; i++)
    {
        cache_node = (cat_cache_node_t *)esMEMS_Malloc(0, sizeof(cat_cache_node_t));
        if(cache_node == NULL)
        {
            esMEMS_Pfree(cache_prc->buf_pool, palloc_size);
            cache_prc->buf_pool = NULL;
            esMEMS_Mfree(0, cache_prc);
            cache_prc = NULL;
            return NULL;
        }
        else
        {
            eLIBs_memset(cache_node, 0, sizeof(cat_cache_node_t));
            cache_node->next = NULL;
            cache_node->buf = cache_prc->buf_pool + i * para->buf_size;
        }

        if(cache_prc->cache_info.unused_list == NULL)
        {
            cache_prc->cache_info.unused_list = cache_node;
            tmp_node = cache_node;
        }
        else
        {
            tmp_node->next = cache_node;
            tmp_node = cache_node;
        }
    }
    cache_prc->db = db;
    cache_prc->list_handle = list_handle;

    __msg("----cache_prc create: %x\n", cache_prc);
    return (void *)cache_prc;
}
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
* return:           if success return EPDK_OK
*                   if fail return EPDK_FAIL
* modify history:
**********************************************************************************************************************
*/
__s32 cat_cache_inner_delete(void *handle)
{
    cat_cache_t         *cache_info = NULL;
    cat_cache_node_t    *cache_node = NULL;
    cat_cache_node_t    *tmp_node = NULL;
    cache_prc_t         *cache_prc = NULL;
    __s32               palloc_size = 0;    //KB
    if(handle == NULL)
    {
        return EPDK_FAIL;
    }
    else
    {
        __msg("----cache_prc delete : %x\n", handle);
        cache_prc = (cache_prc_t *)handle;
    }
    
    cache_info = (cat_cache_t *)&(cache_prc->cache_info);
    
    cache_node = cache_info->used_list;
    while(cache_node != NULL)
	{
        tmp_node = cache_node;
        cache_node = cache_node->next;
        cat_db_disable_extra_info(cache_prc->db, cache_prc->list_handle, tmp_node->db_id);
        esMEMS_Mfree(0, tmp_node);
    }
    cache_info->used_list = NULL;
    
    cache_node = cache_info->unused_list;
    while(cache_node != NULL)
    {
        tmp_node = cache_node;
        cache_node = cache_node->next;
        esMEMS_Mfree(0, tmp_node);
    }
    cache_info->unused_list = NULL;
    
    palloc_size = cache_prc->para.cache_num * cache_prc->para.buf_size / 1024 + 1;
    if(cache_prc->buf_pool != NULL)
    {
        esMEMS_Pfree(cache_prc->buf_pool, palloc_size);
        cache_prc->buf_pool = NULL;
    }
    
    esMEMS_Mfree(0, cache_prc);
    cache_prc = NULL;
    
    return EPDK_OK;		
}


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
* return:           if success return EPDK_OK
*                   if fail return EPDK_FAIL
* modify history:
**********************************************************************************************************************
*/
__s32 cat_free_one_cache_buf(void *handle)
{
    cache_prc_t             *cache_prc = NULL;
    cat_cache_t             *cache_info = NULL;
    cat_cache_node_t        *prev_node = NULL;    
    cat_cache_node_t        *cur_node = NULL;    
        
    if(handle == NULL)
    {
        return EPDK_FAIL;
    }
    else
    {
        cache_prc = (cache_prc_t *)handle;
        cache_info = (cat_cache_t *)&(cache_prc->cache_info);
    }        
    
    if(cache_info->used_list == NULL)
    {
        __msg("----no cache used, cache free failed\n");
        return EPDK_FAIL;
    }
    else
    {
        prev_node = cur_node = cache_info->used_list;
        if(cur_node->next == NULL)//only one node
        {            
            cur_node->next = cache_info->unused_list;
            cache_info->unused_list = cur_node;                  
            cache_info->used_list = NULL;  
        }
        else//more than one node
        {
            cur_node = cur_node->next;                	
            while(cur_node->next != NULL)
            {
                prev_node = cur_node;
                cur_node = cur_node->next;
            }        
            cur_node->next = cache_info->unused_list;
            cache_info->unused_list = cur_node;
            prev_node->next = NULL;
        }
    
        return EPDK_OK;
    }
}


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
* parameters:		handle:cat_cache_create返回的cache指针，id:对应的浏览id，同时表示在数据库表里面的id，item：浏览项
*
* return:           if success return EPDK_OK
*                   if fail return EPDK_FAIL
* modify history:
**********************************************************************************************************************
*/
__s32 cat_get_one_cache_buf(void *handle, __u32 id, __cat_item_t *item)
{
    cache_prc_t         *cache_prc = NULL;
    cat_cache_t         *cache_info = NULL;
    cat_cache_node_t    *cache_node = NULL;
    cat_cache_node_t    *tmp_node = NULL;
    
    if(handle == NULL)
    {
        return EPDK_FAIL;
    }
    else
    {
        cache_prc = (cache_prc_t *)handle;
        cache_info = (cat_cache_t *)&(cache_prc->cache_info);
    }
        
    if(cache_info->unused_list != NULL)
    {
        cache_node = cache_info->unused_list;
        cache_info->unused_list = cache_info->unused_list->next; 
    }
    else
    {
        cache_node = cache_info->used_list;
        cache_info->used_list = cache_info->used_list->next; 
        cat_db_disable_extra_info(cache_prc->db, cache_prc->list_handle, cache_node->db_id);
    }
    
    if(cache_info->used_list == NULL)
    {
        cache_info->used_list = cache_node;        
    }
    else
    {
        tmp_node = cache_info->used_list;            
        while(tmp_node->next != NULL)
        {            	
        	tmp_node = tmp_node->next;
        }
        tmp_node->next = cache_node;                        
    }
    //__msg("-----cache_node->buf: %x, buf_size: %d, db_id: %d\n", cache_node->buf, cache_prc->para.buf_size, id);
    
    eLIBs_memset(cache_node->buf, 0, cache_prc->para.buf_size);
    cache_node->next = NULL;
    cache_node->db_id = id;            
    item->extra_info.extra_info.file.miniature.buf = cache_node->buf;
    
    return EPDK_OK;
}

