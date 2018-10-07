/*
*******************************************************************************************************************
*                                                         	CAT  Framework
*                                         			the core of apps support module
*
*                             				 Copyright(C), 2006-2008, Softwinner Microelectronic Co., Ltd.
*											               All Rights Reserved
*
*File Name： 	cat_playlist.c
*
*Author： 		Terry Zeng
*
*Version :   	1.1.0
*
*Date:  		2009-09-30
*
*Description : the playlist part of cat 
*
*Others : 		None at present.
*
* History :
*
* <Author>          <time>         <version>     <description>
*
* Terry.Zeng     2009-09-30         1.1.0          Create File
*
*******************************************************************************************************************
*/

#include "cat_common.h"


/*
**********************************************************************************************************************
*                                               extract_str_tail
*
* author:          terry.zeng 
*
* date:             2009-10-09
*
* Description:       获取文件名 
*
* parameters:       src_str：输入，文件全路径  dst_str：输出，文件名  flag：文件名分隔符，如“\\”
*
* return:           void
* modify history: 
**********************************************************************************************************************
*/
static void extract_str_tail(const char *src_str, char *dst_str, char flag)
{
    __s32 i = 0;
    
    for(i = eLIBs_strlen(src_str) - 1;  i >= 0;  i--)
    {
        if(src_str[i] == flag)
        {
            eLIBs_strcpy(dst_str, src_str + i + 1);
            return;
        }
    }
    dst_str[0] = '\0';
}


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
__s32 cat_create_playlist_info(cat_create_param_t *param, cat_list_t *cat_list, cat_engine_t *engine, __u32 start_pos)
{
    __s32               ret = EPDK_OK;
    ES_FILE             *orchid_model = NULL;
    HDIR                hdir = NULL;
    HENT                hent = NULL;
    HDIR                h_sub_dir = NULL;
    __u32               nr = 0;    
    char                medialib_path[CAT_MAX_FULL_PATH_LEN];
    char                item_name[CAT_MAX_FULL_PATH_LEN];
    char                sub_dir_path[CAT_MAX_FULL_PATH_LEN];    
    __u32               uid = 0;
    media_info_t        *media_info = NULL;   
    cat_item_node_t     *item = NULL;
    cat_item_node_t     *cur_item = NULL;    
    __s32               cur_item_attr = (__s32)ORC_ENTRY_ATTR_UNKNOWN;      
    __s32               len1 = 0, len2 = 0;
      
    
    if((param == NULL) || (cat_list == NULL) || (engine == NULL))
    {
        return EPDK_FAIL;
    }                
            
    eLIBs_memset(medialib_path, 0, CAT_MAX_FULL_PATH_LEN);        
    eLIBs_memset(item_name, 0, CAT_MAX_FULL_PATH_LEN);
    eLIBs_memset(sub_dir_path, 0, CAT_MAX_FULL_PATH_LEN);

    eLIBs_strcpy(medialib_path, param->path);
    __msg("-------------param->path: %s\n", param->path);
    /*add media dir if only root dir*/
    if(eLIBs_strstr(param->path, ORC_MEDIA_LIB_ROOT) == NULL)
    {
        __wrn("----path is error, not medialib mode\n");
        return EPDK_FAIL;
    }
    else if(eLIBs_strcmp(param->path, ORC_MEDIA_LIB_ROOT) == 0)
    {        
        eLIBs_strcat(medialib_path,"\\");
        if(param->media_type == CAT_MEDIA_TYPE_AUDIO)
        {
            eLIBs_strcat(medialib_path, AUDIO_TYPE_STR);
        }
        else if(param->media_type == CAT_MEDIA_TYPE_VIDEO)
        {
            eLIBs_strcat(medialib_path, VIDEO_TYPE_STR);
        }
        else if(param->media_type == CAT_MEDIA_TYPE_PIC)
        {
            eLIBs_strcat(medialib_path, PIC_TYPE_STR);
        }
        else if(param->media_type == CAT_MEDIA_TYPE_EBOOK)
        {
            eLIBs_strcat(medialib_path, EBOOK_TYPE_STR);
        }
        else if(param->media_type == CAT_MEDIA_TYPE_GAME)
        {
            eLIBs_strcat(medialib_path, GAME_TYPE_STR);
        }
        else
        {            
            return EPDK_FAIL;
        }                
    }           
    eLIBs_strcpy(cat_list->path, medialib_path);
    
    orchid_model = eLIBs_fopen(ORCHID_FILE_PATH, "r+");
    if(orchid_model == 0)
    {
        __wrn("open orchid failed!\n");
        return EPDK_FAIL;
    }
        
    if(eLIBs_fioctrl(orchid_model, ORC_CMD_ORCHID_IS_VALID, 0, 0) == 0)
    {
        __wrn("----orchid is invalid\n");
        eLIBs_fclose(orchid_model);
        return EPDK_FAIL;
    }

    /*open media lib*/    
    hdir = (HDIR)eLIBs_fioctrl(orchid_model, ORC_CMD_OPEN_DIR, 0, medialib_path);
    if(hdir == NULL)
    {
        __wrn("Error in opening orchid dir %s\n", medialib_path);
        eLIBs_fclose(orchid_model);
        return EPDK_FAIL;
    }
    
    /*get media number*/
    nr = eLIBs_fioctrl(orchid_model, ORC_CMD_GET_ENTRY_COUNT, 0, (void *)hdir);
    if((nr == 0xFFFFFFFF) || (nr == 0))
    {
        __inf("Fail in getting the number of dir %s\n",medialib_path);
        eLIBs_fioctrl(orchid_model, ORC_CMD_CLOSE_DIR, 0, (void *)hdir);
        eLIBs_fclose(orchid_model);
        if(nr == 0)
        {
            return EPDK_OK;
        }
        else
        {
            return EPDK_FAIL;
        }
    }
    
    /*get current path attribute*/	
    while(((hent = (HENT)eLIBs_fioctrl(orchid_model, ORC_CMD_GET_NEXT_ENTRY, 0, (void *)hdir )) != NULL) && (engine->status != ENGINE_STOPPED))
    {
        /*get the attribute*/
        cur_item_attr = eLIBs_fioctrl(orchid_model, ORC_CMD_ENTRY_TO_ATTR, 0, (void *)hent);       
        __msg("----cur_item_attr: %d\n", cur_item_attr);
        
        if(cur_item_attr == (__s32)ORC_ENTRY_ATTR_UNKNOWN)
        {
            __msg("----ORC_ENTRY_ATTR_UNKNOWN\n");
            ret = EPDK_FAIL;
            break;
        }
        else if(cur_item_attr == (__s32)ORC_ENTRY_ATTR_ITEM)
        {
            if(eLIBs_fioctrl(orchid_model, ORC_CMD_ENTRY_TO_NAME, hent, (void *)&uid) != ORC_OK)
            {
                __msg("----ORC_CMD_ENTRY_TO_NAME failed\n");
                ret = EPDK_FAIL;
                break;
            }
            
            media_info = esMEMS_Malloc(0, sizeof(media_info_t));
            if(media_info == NULL)
            {
                __msg("----malloc media_info failed\n");
                ret = EPDK_FAIL;
                break;
            }
            
            if(eLIBs_fioctrl(orchid_model, ORC_CMD_GET_MEDIA_INFO, uid, (void *)media_info) != ORC_OK)
            {
                __msg("----ORC_CMD_GET_MEDIA_INFO failed\n");
                ret = EPDK_FAIL;
                break;
            }
            
            extract_str_tail(media_info->audio_info.media_file, item_name, '\\');
                        
            item = esMEMS_Malloc(0, sizeof(cat_item_node_t));
            if(item == NULL)
            {
                __msg("----malloc item failed\n");
                ret = EPDK_FAIL;
                break;
            }
            else
            {
                eLIBs_memset(item, 0, sizeof(cat_item_node_t));
            }                        
            
            eLIBs_strcpy(item->cat_item.base_info.name, item_name);
            item->cat_item.base_info.type = CAT_SW_FILE;
            len1 = eLIBs_strlen(media_info->audio_info.media_file);
            len2 = eLIBs_strlen(eLIBs_strchrlast(media_info->audio_info.media_file, '\\'));
            eLIBs_strncpy(item->cat_item.base_info.fpath, media_info->audio_info.media_file, len1 - len2);
            item->cat_item.base_info.item_pri_info.file.media_type = param->media_type;
            item->cat_item.base_info.item_pri_info.file.format = media_info->audio_info.file_format;
            item->cat_item.base_info.item_pri_info.file.size = eLIBs_GetFileSize(media_info->audio_info.media_file);
            eLIBs_GetFileMTime(media_info->audio_info.media_file, &(item->cat_item.base_info.item_pri_info.file.m_time)); 
            eLIBs_GetFSCharset(item->cat_item.base_info.fpath, (__s32 *)&item->cat_item.base_info.char_encoding);  
            __msg("----cat_list->path: %s, name: %s, path: %s\n", cat_list->path, item->cat_item.base_info.name, item->cat_item.base_info.fpath);
            __msg("----char_encoding: %d, name: %s\n", item->cat_item.base_info.char_encoding, media_info->audio_info.media_file);
                              
            if((cat_list->item_list == NULL) || (cur_item == NULL))
            {
                cat_list->item_list = item;
            }
            else
            {
                cur_item->next = item;
            }
            item->next = NULL;
            cur_item = item;
            cat_list->nitem++;
            
            esMEMS_Mfree(0, media_info);
        }
        else
        {
            if(eLIBs_fioctrl(orchid_model, ORC_CMD_ENTRY_TO_NAME, hent, (void *)item_name) != ORC_OK)
            {
                __msg("----ORC_CMD_ENTRY_TO_NAME failed\n");
                ret = EPDK_FAIL;
                break;
            }
            
            item = esMEMS_Malloc(0, sizeof(cat_item_node_t));
            if(item == NULL)
            {
                __msg("----malloc item failed\n");
                ret = EPDK_FAIL;
                break;
            }
            else
            {
                eLIBs_memset(item, 0, sizeof(cat_item_node_t));
            }
            
            eLIBs_strcpy(item->cat_item.base_info.name, item_name);            
            item->cat_item.base_info.char_encoding = (__epdk_charset_enm_e)eLIBs_fioctrl(orchid_model, ORC_CMD_ENTRY_TO_ENCODING, 0, (void *)hent);      
            __msg("----name: %s, encoding: %d\n", item_name, item->cat_item.base_info.char_encoding);
            
            eLIBs_strcpy(sub_dir_path, cat_list->path);
            eLIBs_strcat(sub_dir_path, "\\");
            eLIBs_strcat(sub_dir_path, item_name);   
            h_sub_dir = (HDIR) eLIBs_fioctrl(orchid_model, ORC_CMD_OPEN_DIR, 0, sub_dir_path);
            __msg("----sub_dir_path: %s\n", sub_dir_path);
            if(h_sub_dir == NULL)
            {
                __wrn("Error in opening orchid dir %s\n", sub_dir_path);   
                ret = EPDK_FAIL;
                break;
            }
            
            nr = eLIBs_fioctrl(orchid_model, ORC_CMD_GET_ENTRY_COUNT, 0, (void *)h_sub_dir);
            if(nr == 0xffffffff)
            {
            	item->cat_item.extra_info.extra_info.dir.item_cnt = 0;
            }
            else
            {
            	item->cat_item.extra_info.extra_info.dir.item_cnt = nr;
            }
            eLIBs_fioctrl(orchid_model, ORC_CMD_CLOSE_DIR, 0, (void *)h_sub_dir);
            
            eLIBs_strcpy(item->cat_item.base_info.fpath, cat_list->path);
            item->cat_item.base_info.type = CAT_SW_DIR; 
            if(cat_list->item_list == NULL)
            {
                cat_list->item_list= item;
            }
            else
            {
                cur_item->next = item;
            }
            item->next = NULL;
            cur_item = item;
            cat_list->nitem++;
        }
        if(cat_list->nitem % 100 == 0)
        {
            /*添加到数据库*/
            cat_db_add_base_info(engine->db, engine->list_handle);
            cat_destroy_playlist_info(cat_list);
            cur_item = NULL;
            esKRNL_TimeDly(5);
        }
    }
    if(ret == EPDK_OK)
    {
        cat_db_add_base_info(engine->db, engine->list_handle);
    }
    cat_destroy_playlist_info(cat_list);
    eLIBs_fioctrl(orchid_model, ORC_CMD_CLOSE_DIR, 0, (void *)hdir);    
    eLIBs_fclose(orchid_model);
        
    return ret;
}


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
__s32 cat_destroy_playlist_info(cat_list_t *cat_list)
{
    if(cat_list == NULL)
    {
        return EPDK_FAIL;
    }
    else
    {
        cat_item_node_t *tmp_item = NULL;
        
        while(cat_list->item_list != NULL)
        {
            tmp_item = cat_list->item_list;
            cat_list->item_list = cat_list->item_list->next;
            esMEMS_Mfree(0, tmp_item);
        }
        
        cat_list->nitem = 0;
        cat_list->item_list = NULL;
        eLIBs_memset(cat_list, 0, sizeof(cat_list_t));
        
        return EPDK_OK;
    }
}

