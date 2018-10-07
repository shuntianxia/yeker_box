/*
*******************************************************************************************************************
*                                                         	CAT  Framework
*                                         			the core of apps support module
*
*                             				 Copyright(C), 2006-2008, Softwinner Microelectronic Co., Ltd.
*											               All Rights Reserved
*
*File Name： 	cat_root.c
*
*Author： 		Terry Zeng
*
*Version :   	2.0
*
*Date:  		2010-09-13
*
*Description : the root info create of cat 
*
*Others : 		None at present.
*
* History :
*
* <Author>          <time>         <version>     <description>
*
* Terry.Zeng     2010-09-13         2.0          Create File
*
*******************************************************************************************************************
*/

#include "cat_common.h"


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
__s32 cat_create_root(cat_create_param_t *param, cat_list_t *cat_list)
{
    cat_item_node_t     *item = NULL;        
    cat_create_param_t  create_param;     
    
    eLIBs_memset(&create_param, 0, sizeof(cat_create_param_t));    
    eLIBs_strcpy(create_param.path, param->path);
    create_param.scan_type = param->scan_type;
    create_param.media_type = param->media_type;
    
    if(create_param.scan_type == CAT_SCAN_ALL_DEVICE)
    {
        __s32 i = 0;
        cat_item_node_t *cur_item = NULL;
        char item_name[][CAT_MAX_FULL_PATH_LEN]  = { CAT_LOCAL_DISK, CAT_USB_DISK, CAT_TF_CARD, CAT_CDROM, CAT_MEDIALIB };
        
        eLIBs_strcpy(cat_list->path,"");
        if(create_param.media_type == CAT_MEDIA_TYPE_AUDIO)
        {
            cat_list->nitem = 5;
        }
        else
        {
            cat_list->nitem = 4;
        }
        
        for(i = 0; i < cat_list->nitem; i++)
        {
            item = esMEMS_Malloc(0, sizeof(cat_item_node_t));
            if(item == NULL)
            {
                return EPDK_FAIL;
            }
            else
            {
                eLIBs_memset(item, 0, sizeof(cat_item_node_t));
                item->cat_item.base_info.type = CAT_SW_DEVICE;
                eLIBs_strcpy(item->cat_item.base_info.fpath, cat_list->path);
                eLIBs_strcpy(item->cat_item.base_info.name, item_name[i]);
                
                if((i == 0) || (cat_list->item_list == NULL) || (cur_item == NULL))
                {
                    cat_list->item_list = item;
                }
                else
                {
                    cur_item->next = item;
                }
                cur_item = item;
            }
        }
    }
    else if(create_param.scan_type == CAT_SCAN_ALL_PATI)
    {
        cat_list->nitem = 0;
        if(create_param.media_type == CAT_MEDIA_TYPE_AUDIO)
        {
            item = esMEMS_Malloc(0, sizeof(cat_item_node_t));
            if(item == NULL)
            {
                return EPDK_FAIL;
            }
            else
            {
                eLIBs_memset(item, 0, sizeof(cat_item_node_t));
                item->cat_item.base_info.type = CAT_SW_DEVICE;
                eLIBs_strcpy(item->cat_item.base_info.fpath, cat_list->path);
                eLIBs_strcpy(item->cat_item.base_info.name, CAT_MEDIALIB);
                cat_list->item_list = item;
                cat_list->nitem++;
            }
        }
        
        create_param.scan_type = CAT_SCAN_PARTI;
        eLIBs_strcpy(create_param.path, "LocalDisk");
        cat_create_partition_info(&create_param, cat_list);
        
        create_param.scan_type = CAT_SCAN_PARTI;
        eLIBs_strcpy(create_param.path, "ExternalDisk0");
        cat_create_partition_info(&create_param, cat_list);
        
        create_param.scan_type = CAT_SCAN_PARTI;
        eLIBs_strcpy(create_param.path, "ExternalDisk1");
        cat_create_partition_info(&create_param, cat_list);
        
        create_param.scan_type = CAT_SCAN_PARTI;
        eLIBs_strcpy(create_param.path, "ExternalDisk2");
        cat_create_partition_info(&create_param, cat_list);
        
        eLIBs_strcpy(cat_list->path, "");
        if(cat_list->nitem == 0)
        {
            return EPDK_FAIL;
        }
        else if((cat_list->nitem == 1) && (create_param.media_type == CAT_MEDIA_TYPE_AUDIO))
        {
            return EPDK_FAIL;
        }
    }
    
    return EPDK_OK;
}


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
* return:           success 0;	failed:-1
* modify history: 
**********************************************************************************************************************
*/
__s32 cat_destroy_root(cat_list_t *cat_list)
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

