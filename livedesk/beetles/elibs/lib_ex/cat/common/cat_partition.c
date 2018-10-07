/*
*******************************************************************************************************************
*                                                         	CAT  Framework
*                                         			the core of apps support module
*
*                             				 Copyright(C), 2006-2008, Softwinner Microelectronic Co., Ltd.
*											               All Rights Reserved
*
*File Name： 	cat_partition.c
*
*Author： 		Terry Zeng
*
*Version :   	2.0
*
*Date:  		2010-09-13
*
*Description : the partition info part of cat 
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



#define MAX_PARTITION  11

                
/*
**********************************************************************************************************************
*                                               get_part_by_name
*
* author:          terry.zeng 
*
* date:             2010-09-13
*
* Description:       获取分区 
*
* parameters:       target：输入， 目标分区 res_part：输出，返回的分区名
*
* return:           0 	get part success
					-1	part not exist
					-2	part not formatted
* modify history: 
**********************************************************************************************************************
*/
static int get_part_by_name(const char *equipment, char *des)
{
    __s32   ret = -1;
    __s32   i = 0;
    __s32   status = 0;
    char    equipment_name[MAX_FS_NAME_LEN];
    char    part_name[MAX_PARTITION][4];
    char    *partition[MAX_PARTITION] = {"e:","f:", "g:", "h:", "i:", "j:", "k:", "l:", "m:", "n:", "o:"};	

    eLIBs_memset(part_name, 0, MAX_PARTITION * 4);
    for(i = 0; i < MAX_PARTITION; i++)
    {
        eLIBs_memset(equipment_name, 0, MAX_FS_NAME_LEN);
        eLIBs_GetPTName(partition[i], equipment_name);
        if(eLIBs_strncmp(equipment_name, equipment, eLIBs_strlen(equipment)) == 0)
        {
            status = eLIBs_IsPartFormated(partition[i]);
            if(status==ELIBS_PART_FMT_FORMATED)
            {
                eLIBs_strcpy(part_name[i], partition[i]);
                ret = 0;
            }
            else
            {
                ret = -2;
            }
        }
    }
    eLIBs_memcpy(des, part_name, MAX_PARTITION * 4);
    
    return ret;
}


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
* return:           success 0;	failed:-1
* modify history: 
**********************************************************************************************************************
*/

__s32 cat_create_partition_info(cat_create_param_t *param, cat_list_t *cat_list)
{	
    char                part_name[MAX_PARTITION][4];
    char                *equipment[4] = {"UDISK", "SDMMC-DISK", "SCSI_DISK", "USB_CDROM"};
    cat_root_t          equipment_type[4] = {CAT_LOCAL, CAT_TF, CAT_USB, CAT_CD};
    __s32               equipment_index = 0;
    cat_item_node_t     *item = NULL;
    cat_item_node_t     *cur_item = NULL;
    __s32               i = 0;
    __u32               is_exist = 0;
    __u32               fs_attr = 0;

    if(eLIBs_strcmp(param->path, "LocalDisk") == 0)
    {
        equipment_index = 0;
    }
    else if(eLIBs_strcmp(param->path, "ExternalDisk0") == 0)
    {
        equipment_index = 1;
    }
    else if(eLIBs_strcmp(param->path, "ExternalDisk1") == 0)
    {
        equipment_index = 2;
    }
    else if(eLIBs_strcmp(param->path, "ExternalDisk2") == 0)
    {
        equipment_index = 3;
    }
    else
    {
        return EPDK_FAIL;
    }
    
    eLIBs_strcpy(cat_list->path, param->path);
    eLIBs_memset(part_name, 0, MAX_PARTITION * 4);    
    
    if(get_part_by_name(equipment[equipment_index], (char *)part_name) == 0)
    {
        for( i=0; i< MAX_PARTITION; i++)
        {
            if(part_name[i][0])
            {
                cur_item = cat_list->item_list;
                while(cur_item != NULL)
                {
                    if(eLIBs_strcmp(cur_item->cat_item.base_info.name, part_name[i])==0)
                    {
                        is_exist = 1;
                        break;
                    }
                    cur_item = cur_item->next;
                }
                if(is_exist == 1)
                {
                    is_exist = 0;
                    continue;
                }
    			
                cat_list->nitem++;
                item = esMEMS_Malloc(0, sizeof(cat_item_node_t));
                if(item == NULL)
                {
                    return EPDK_FAIL;
                }
                else
                {
                    eLIBs_memset(item, 0, sizeof(cat_item_node_t));
                }
                eLIBs_strcpy(item->cat_item.base_info.name, part_name[i]);
                eLIBs_GetFSAttribute(item->cat_item.base_info.name, &fs_attr);
                if(fs_attr & FS_ATTR_READONLY)
                {
                    item->cat_item.base_info.item_pri_info.pati.pati_attr = CAT_READONLY;
                }
                else
                {
                    item->cat_item.base_info.item_pri_info.pati.pati_attr = CAT_READWRITE;
                }
                item->cat_item.base_info.type = CAT_SW_PARTI;
                eLIBs_strcpy(item->cat_item.base_info.fpath, cat_list->path);
                item->cat_item.base_info.item_pri_info.pati.root_type = equipment_type[equipment_index];
                cur_item = cat_list->item_list;
                if(cur_item == NULL)
                {
                    cat_list->item_list = item;
                    item->next = NULL;
                }
                else
                {
                    while(cur_item->next != NULL)
                    {
                        cur_item = cur_item->next;
                    }
                    cur_item->next = item;
                    item->next = NULL;
                }
            }
        }
    }
    
    return EPDK_OK;
}


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
* return:           success 0;	failed:-1
* modify history: 
**********************************************************************************************************************
*/
__s32 cat_destroy_partition_info(cat_list_t *cat_list)
{	
    if(cat_list == NULL)
    {
        return EPDK_FAIL;
    }
    else
    {
        cat_item_node_t *tmp_node = NULL;	
        
        while(cat_list->item_list != NULL)
        {
            tmp_node = cat_list->item_list;
            cat_list->item_list = cat_list->item_list->next;
            esMEMS_Mfree(0, tmp_node);    
        }
        
        cat_list->nitem = 0;
        cat_list->item_list = NULL;
        eLIBs_memset(cat_list,0,sizeof(cat_list_t));
        
        return EPDK_OK;
    }
}

