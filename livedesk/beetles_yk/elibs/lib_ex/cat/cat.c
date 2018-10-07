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

#include "cat_p.h"
#include "cat_i.h"

#define CAT_READ_BUFFER_SIZE (16 * 1024)


typedef struct
{
    cat_list_handle_t           *handle_list;
    void                        *engine;
    cat_db_t                    *db;
    willow_t                    *willow;    
    vdec_t                      *video_dec;    
    __u8                        file_op_thread_id;
    cat_file_op_info_t          file_op_info;
    __s32                       cmd_pend_flag;          /*线程处理标志位*/
    __u32                       cmd;                    /*线程处理命令*/
    __s32                       param1;                 /*命令参数1*/
    __s32                       param2;                 /*命令参数2*/    
    cat_file_op_status_t        status;
    char                        src[CAT_MAX_FULL_PATH_LEN];
    char                        dst[CAT_MAX_FULL_PATH_LEN];
    __cat_item_t                file_op_item;
}cat_core_t;


__u32 dir_step = 0;



static __s32 _get_es_timestamp(ES_TIMESTAMP *time)
{
    __s32       ret = EPDK_OK;
    __date_t    tmp_date;
    __time_t    tmp_time;
    
    if(time == NULL)
    {
        return EPDK_FAIL;
    }
    
    ret = esTIME_GetDate(&tmp_date);
    if(ret == EPDK_OK)
    {
        time->year = tmp_date.year;
        time->month = tmp_date.month;
        time->day = tmp_date.day;
    }
    else
    {
        __wrn("----esTIME_GetDate failed\n");
        time->year = 2011;
        time->month = 1;
        time->day = 1;
    }
    
    ret = esTIME_GetTime(&tmp_time);  
    if(ret == EPDK_OK)
    {
        time->hour = tmp_time.hour;
        time->minute = tmp_time.minute;
        time->second = tmp_time.second;
        time->milliseconds = 0;
    }
    else
    {
        __wrn("----esTIME_GetTime failed\n");
        time->hour = 0;
        time->minute = 0;
        time->second = 0;
        time->milliseconds = 0;
    }
    
    __msg("----year: %d, month: %d, day: %d\n", time->year, time->month, time->day);
    
    return EPDK_OK;    
}

static __s32 _is_file_exist(const char *file)
{
    ES_FILE         *fp = EPDK_OK;
    
    fp = eLIBs_fopen(file, "r");    
    __msg("----file: %s, fp: %x\n", file, fp);
    if(fp != NULL)
    {        
        eLIBs_fclose(fp);
        if(eLIBs_GetFileAttributes(file) == 0)
        {
            return EPDK_OK;
        }
        else
        {
            return EPDK_FAIL;
        }
    }
    else
    {
        return EPDK_FAIL;
    }
}


static __s32 _is_dir_exist(const char *dir)
{
    ES_FILE         *fp = EPDK_OK;    
    
    fp = eLIBs_fopen(dir, "r");
    __msg("----dir: %s, fp: %x\n", dir, fp);
    if(fp != NULL)
    {
        eLIBs_fclose(fp);        
        if(eLIBs_GetFileAttributes(dir) == 0)
        {
            return EPDK_FAIL;
        }
        else
        {
            return EPDK_OK;
        }
    }
    else
    {
        return EPDK_FAIL;
    }
}

    
static __s32 _get_item_num_in_dir(const char * dirpath, __u32 *total)
{
    ES_DIR          *dirp;
    ES_DIRENT       *direntp;
    char            sub_dir[CAT_MAX_FULL_PATH_LEN];        
    static __s32    dir_level = 0;
    
    if(dirpath == NULL)
    {
        return EPDK_FAIL;
    }
    else if(dir_level > MAX_DIR_SCAN_LEVEL)
    {
        return EPDK_FAIL;
    }
    else
    {
        dir_level++;
    }     

    dirp = (ES_DIR *)eLIBs_opendir(dirpath);
    if (dirp == NULL)
    {
        return EPDK_FAIL;
    }

    while((direntp = (ES_DIRENT *)eLIBs_readdir(dirp)) != NULL)
    {
        if (direntp->fatdirattr & FSYS_ATTR_DIRECTORY)
        {
            /* skip "." and ".." sub directory */
            if (direntp->d_name[0] == '.')
            {
                continue;
            }

            eLIBs_strcpy(sub_dir, dirpath);
            eLIBs_strcat(sub_dir, "\\");
            eLIBs_strcat(sub_dir, (const char *)direntp->d_name);
            if(_get_item_num_in_dir(sub_dir, total) == EPDK_FAIL)
            {
                return EPDK_FAIL;
            }  
            __msg("---total: %d, dirpath: %s, sub_dir: %s\n", *total, dirpath, sub_dir);
        }
        else
        {
            (*total)++;
            __msg("---total: %d, dirpath: %s, file: %s\n", *total, dirpath, (const char *)direntp->d_name);
        }
    }
        
    eLIBs_closedir(dirp);    
    (*total)++;
    dir_level--;
    
    __msg("----total: %d\n", *total);
    
    return EPDK_OK;
}


/*
**********************************************************************************************************************
*                                               cat_delete_dir
*
* author:          terry.zeng 
*
* date:             2009-10-12
*
* Description:       删除目录 
*
* parameters:       cat_list 浏览信息 dir：目录全路径
*
* return:           success 0;	failed:-1
* modify history: 
**********************************************************************************************************************
*/
static __s32 _inner_delete_dir(cat_core_t *core, char *dir_name, cat_file_op_info_t *info)
{
    ES_DIR          *dir = NULL;
    ES_DIRENT       *dirent = NULL;
    char            fullpath[CAT_MAX_FULL_PATH_LEN];
    __s32           ret = EPDK_OK;    
    static __s32    dir_level = 0;
    
    if((core == NULL) || (dir_name == NULL) || (info == NULL))
    {
        __msg("----input param error\n");
        return EPDK_FAIL;
    }    
    else if(dir_level > MAX_DIR_SCAN_LEVEL)
    {
        __msg("----dir_level is over than MAX_DIR_SCAN_LEVEL, failed\n");
        info->percent = 101;
        return EPDK_FAIL;
    }
    else
    {
        dir_level++;
    }
    
    if(_is_dir_exist(dir_name) == EPDK_FAIL)
    {
        info->percent = 101;
        return EPDK_FAIL;
    }
    
    eLIBs_memset(fullpath, 0, CAT_MAX_FULL_PATH_LEN);
    
    /*open dir*/
    dir = eLIBs_opendir(dir_name);
    if(dir == NULL)
    {
        info->percent = 101;
        return EPDK_FAIL;
    }  
        
    while(core->status == CAT_DELETING)
    {
        /*read dir*/
        dirent = eLIBs_readdir(dir);
        if(dirent == NULL)
        {
            __msg("----read dir %s is empty\n", dir_name);
            break;
        }        
        
        eLIBs_memset(fullpath, 0, CAT_MAX_FULL_PATH_LEN);
        eLIBs_strcpy(fullpath, dir_name);
        eLIBs_strcat(fullpath, "\\");
        eLIBs_strcat(fullpath, (const char *)dirent->d_name);
        
        __msg("----read dir %s ---fullpath: %s\n", (const char *)dirent->d_name, fullpath);
        
        if(dirent->fatdirattr & FSYS_ATTR_DIRECTORY)
        {
            if (dirent->d_name[0] == '.')
            {
                continue;
            }            
                 
            ret = _inner_delete_dir(core, fullpath, info);
            if(ret == EPDK_FAIL)            
            {
                __msg("--------_inner_delete_dir failed---fullpath: %s\n", fullpath);
                ret = EPDK_FAIL;
                break;
            }
            else
            {                
                __msg("-----_inner_delete_dir success----fullpath: %s, finish_num: %d\n", fullpath, info->finish_num);
            }
        }
        else
        {
            if(eLIBs_remove(fullpath) == -1)
            {
                __msg("----remove file %s failed\n", fullpath);
                ret = EPDK_FAIL;
                break;
            }
            else
            {
                ES_FILE *orchid = eLIBs_fopen(ORCHID_FILE_PATH, "r+");
                
                info->finish_num++;
                info->percent = (info->finish_num * 100) / info->total_num;
                
                if(orchid != NULL)
                {
                    __msg("----ORC_CMD_DELETE_FILE_FROM_ORCHID\n");
                    eLIBs_fioctrl(orchid, ORC_CMD_ORCHID_IS_VALID, 0, 0 );
                    eLIBs_fioctrl(orchid, ORC_CMD_DELETE_FILE_FROM_ORCHID, 0, fullpath);
                    eLIBs_fclose(orchid);
                }
                __msg("-----delete file success----fullpath: %s, finish_num: %d, total_num%d, percent: %d\n", fullpath, info->finish_num, info->total_num, info->percent);
            }
            
            esKRNL_TimeDly(4);
        }
    }
    
    dir_level--;
    eLIBs_closedir(dir);
    ret = eLIBs_rmdir(dir_name);
    __msg("--------eLIBs_rmdir %s--- ret: %d\n", dir_name, ret);
    info->finish_num++;
    info->percent = (info->finish_num * 100) / info->total_num;
    
    __msg("----dir_level: %d, finish_num: %d, total_num: %d, percent: %d\n", dir_level, info->finish_num, info->total_num, info->percent);
    if(ret == EPDK_FAIL)
    {
        info->percent = 101;
    }
    
    return ret;
}


/*
**********************************************************************************************************************
*                                               cat_copy_file
*
* author:          terry.zeng 
*
* date:             2009-10-12
*
* Description:       拷贝文件
*
* parameters:       src_path 原文件 dst_path：目标文件的目录
*
* return:           
* modify history: 
**********************************************************************************************************************
*/
static __s32 _inner_copy_file(cat_core_t *core, char *src_file, char *dst_path, cat_file_op_info_t *info, __cat_item_t *cat_item)
{
    __s32       ret = EPDK_OK;
    __u32       read_len = 0;
    __u32       write_len = 0;
    ES_FILE     *src_fp = NULL;
    ES_FILE     *dst_fp = NULL;
    char        fullpath[CAT_MAX_FULL_PATH_LEN];
    char        new_name[CAT_MAX_FULL_PATH_LEN];
    char        *data = NULL;
    __s32       audio_format = 0;        
    __u64       file_size = 0;
    __u64       writed_size = 0;
    
    __msg("----core: %x, src_file: %s, dst_path: %s, info: %x, cat_item: %x\n", core, src_file, dst_path, info, cat_item);
    
    if((core == NULL) || (src_file == NULL) || (dst_path == NULL) || (info == NULL) || (cat_item == NULL))
    {        
        __msg("----input param error\n");
        return EPDK_FAIL;
    }    
    
    if(_is_file_exist(src_file) == EPDK_FAIL)
    {
        info->percent = 101;
        return EPDK_FAIL;
    }
    else if(_is_dir_exist(dst_path) == EPDK_FAIL)
    {
        info->percent = 101;
        return EPDK_FAIL;
    }
    
    eLIBs_memset(new_name, 0, CAT_MAX_FULL_PATH_LEN);
    eLIBs_memset(fullpath, 0, CAT_MAX_FULL_PATH_LEN);
    
    eLIBs_strcpy(new_name, eLIBs_strchrlast(src_file, '\\') + 1);
    eLIBs_strcpy(fullpath, dst_path);
    eLIBs_strcat(fullpath, "\\");
    eLIBs_strcat(fullpath, new_name);
    __msg("----fullpath: %s, new_name: %s, des_path: %s\n", fullpath, new_name, dst_path);
    while(_is_file_exist(fullpath) == EPDK_OK)
    {
        eLIBs_memset(new_name, 0, CAT_MAX_FULL_PATH_LEN);
        eLIBs_strcpy(new_name, "new_");
        eLIBs_strcat(new_name, eLIBs_strchrlast(fullpath, '\\') + 1);
        if(eLIBs_strlen(fullpath) + 4 > CAT_MAX_FULL_PATH_LEN)
        {            
            info->percent = 101;
            return EPDK_FAIL;
        }
        else
        {
            eLIBs_strcpy(eLIBs_strchrlast(fullpath, '\\') + 1, new_name);
        }         
    }
    __msg("----fullpath: %s, new_name: %s, des_path: %s\n", fullpath, new_name, dst_path);
    
            
    src_fp = eLIBs_fopen(src_file, "r");
    if(src_fp == NULL)
    {                
        __wrn("open source file %s failed\n", src_file);
        info->percent = 101;
        return EPDK_FAIL;
    }
    else
    {
        file_size = eLIBs_GetFileSize(src_file);
    }
    
    dst_fp = eLIBs_fopen(fullpath, "w+");
    if(dst_fp == NULL)
    {
        __wrn("create file %s failed\n", fullpath);
        eLIBs_fclose(src_fp);
        src_fp = NULL;
        info->percent = 101;
        return EPDK_FAIL;
    }

    data = esMEMS_Malloc(0, CAT_READ_BUFFER_SIZE);
    if(data == NULL)
    {
        __wrn("----malloc data failed\n");
        eLIBs_fclose(src_fp);
        src_fp = NULL;
        eLIBs_fclose(dst_fp);
        dst_fp = NULL;
        info->percent = 101;
        return EPDK_FAIL;
    }
    else
    {
        eLIBs_memset(data, 0, CAT_READ_BUFFER_SIZE);
    }
    
    eLIBs_strcpy(cat_item->base_info.fpath, dst_path);
    eLIBs_strcpy(cat_item->base_info.name, eLIBs_strchrlast(fullpath, '\\') + 1);
    _get_es_timestamp(&(cat_item->base_info.item_pri_info.file.m_time));
    
    
    while(core->status == CAT_FILE_PASTING)
    {
        read_len = eLIBs_fread(data, 1, CAT_READ_BUFFER_SIZE, src_fp);
        if(read_len == 0)
        {
            if(info->total_size == 0)
            {
                info->percent = 100;
            }
            ret = EPDK_OK;
            break;
        }
        else if(read_len > CAT_READ_BUFFER_SIZE)
        {
            __wrn("----read data failed\n");
            ret = EPDK_FAIL;
            break;
        }
        
        write_len = eLIBs_fwrite(data, 1, read_len, dst_fp);
        if(write_len != read_len)
        {
            __wrn("----write data failed\n");
            ret = EPDK_FAIL;
            break;
        }
        else
        {
            info->finish_size += write_len;
            writed_size += write_len;
            if(info->total_size == 0)
            {
                info->percent = 100;
            }
            else
            {
                info->percent = info->finish_size * 100 / info->total_size;
            }
        }
        
        //__msg("----read_len: %d, write_len: %d, percent: %d, finish_size: %d, total_size: %d\n", read_len, write_len, info->percent, (__u32)info->finish_size, (__u32)info->total_size);
        esKRNL_TimeDly(2);
    }
    
    __msg("----file_size: %d, writed_size: %d\n", (__u32)file_size, (__u32)writed_size);
        
    if((ret == EPDK_OK) && (eLIBs_ferror(dst_fp) == 0) && (file_size == writed_size))
    {        
        info->finish_num++;
        eLIBs_fclose(dst_fp);
        dst_fp = NULL;
    }
    else
    {        
        __wrn("write file failed\n");
        eLIBs_fclose(dst_fp);
        dst_fp = NULL;        
        eLIBs_remove(fullpath);
        ret = EPDK_FAIL;
        info->percent = 101;
    }

    eLIBs_fclose(src_fp);
    src_fp = NULL;
 
    esMEMS_Mfree(0, data);
    data = NULL;     
    
    if(ret == EPDK_OK)
    {
        if((eLIBs_GetAudioFormat(fullpath, &audio_format) != EPDK_FALSE) && (audio_format != 0))
        {
            ES_FILE *orchid = eLIBs_fopen(ORCHID_FILE_PATH, "r+");
            if(orchid != NULL)
            {
                __msg("----ORC_CMD_ADD_FILE_TO_ORCHID\n");
                eLIBs_fioctrl(orchid, ORC_CMD_ORCHID_IS_VALID, 0, 0);
                eLIBs_fioctrl(orchid, ORC_CMD_ADD_FILE_TO_ORCHID, (__s32)fullpath, 0);
                eLIBs_fclose(orchid);
            }
        }
    }   
        
    return ret;
}


/*
**********************************************************************************************************************
*                                               cat_copy_dir
*
* author:          terry.zeng 
*
* date:             2009-10-12
*
* Description:       拷贝目录 
*
* parameters:       src_path 原文件 dst_path：目标文件的目录
*
* return:           success 0;	failed:-1
* modify history: 
**********************************************************************************************************************
*/
static __s32 _inner_copy_dir(cat_core_t *core, char *src_path, char *dst_path, cat_file_op_info_t *info, __cat_item_t *cat_item)
{
    __s32           ret = EPDK_OK;
    ES_DIR          *dir = NULL;
    ES_DIRENT       *dirent = NULL;    
    __cat_item_t    copy_item;
    char            fullpath[CAT_MAX_FULL_PATH_LEN];
    char            tmp[CAT_MAX_FULL_PATH_LEN];
    static __s32    dir_level = 0;

    if((core == NULL) || (src_path == NULL) || (dst_path == NULL) || (info == NULL) || (cat_item == NULL))
    {
        __msg("----input param error\n");
        return EPDK_FAIL;
    }
    else if(dir_level > MAX_DIR_SCAN_LEVEL)
    {
        __msg("----dir_level is over than MAX_DIR_SCAN_LEVEL, failed\n");
        info->percent = 101;
        return EPDK_FAIL;
    }
    else
    {
        dir_level++;
    }
    
    __msg("\n\n\n\n\n");
    __msg("++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    __msg("--------src_path: %s, dst_path: %s\n", src_path, dst_path);
    __msg("++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    
    if(_is_dir_exist(src_path) == EPDK_FAIL)
    {
        info->percent = 101;
        return EPDK_FAIL;
    }
    else if(_is_dir_exist(dst_path) == EPDK_FAIL)
    {
        info->percent = 101;
        return EPDK_FAIL;
    }
    else if(eLIBs_strncmp(src_path, dst_path, eLIBs_strlen(src_path)) == 0)
    {
        info->percent = 101;
        return EPDK_FAIL;
    }   
    
    eLIBs_memset(tmp, 0, CAT_MAX_FULL_PATH_LEN);
    eLIBs_memset(fullpath, 0, CAT_MAX_FULL_PATH_LEN);
    eLIBs_memset(&copy_item, 0, sizeof(__cat_item_t));
    
    eLIBs_strcpy(tmp, eLIBs_strchrlast(src_path, '\\') + 1);
    eLIBs_strcpy(fullpath, dst_path);
    eLIBs_strcat(fullpath, "\\");
    eLIBs_strcat(fullpath, tmp);
    __msg("----fullpath: %s, tmp: %s, des_path: %s\n", fullpath, tmp, dst_path);
    while(_is_dir_exist(fullpath) == EPDK_OK)
    {
        eLIBs_memset(tmp, 0, CAT_MAX_FULL_PATH_LEN);
        eLIBs_strcpy(tmp, "new_");
        eLIBs_strcat(tmp, eLIBs_strchrlast(fullpath, '\\') + 1);
        if(eLIBs_strlen(fullpath) + 4 > CAT_MAX_FULL_PATH_LEN)
        {            
            info->percent = 101;
            return EPDK_FAIL;
        }
        else
        {
            eLIBs_strcpy(eLIBs_strchrlast(fullpath, '\\') + 1, tmp);
        }         
    }    
    __msg("----fullpath: %s, tmp: %s, des_path: %s\n", fullpath, tmp, dst_path);
        
    if(eLIBs_mkdir(fullpath) != 0)
    {        
        __msg("----mkdir %s failed\n", fullpath);
        info->percent = 101;
        return EPDK_FAIL;
    }
    else
    {
        __msg("----mkdir %s success\n", fullpath);
        info->finish_num++;
    }
    
    /*open dir*/
    dir = eLIBs_opendir(src_path);
    if(dir == NULL)
    {        
        __msg("----open dir %s failed\n", src_path);        
        info->percent = 101;
        return EPDK_FAIL;
    }

    while(core->status == CAT_DIR_PASTING)
    {
        /*read dir*/
        dirent = eLIBs_readdir(dir);
        if(dirent == NULL)
        {
            __msg("----read dir %s is empty\n", src_path);
            break;
        }        
        
        eLIBs_memset(tmp, 0, CAT_MAX_FULL_PATH_LEN);
        eLIBs_strcpy(tmp, src_path);
        eLIBs_strcat(tmp, "\\");
        eLIBs_strcat(tmp, (const char *)dirent->d_name);
        
        __msg("----read dir %s ---tmp: %s\n", (const char *)dirent->d_name, tmp);
            
        if(dirent->fatdirattr & FSYS_ATTR_DIRECTORY)
        {
            if (dirent->d_name[0] == '.')
            {
                continue;
            }
        
            ret = _inner_copy_dir(core, tmp, fullpath, info, &copy_item);
            if(ret != EPDK_OK)
            {                
                __msg("--------_inner_copy_dir failed---tmp: %s\n", tmp);
                ret = EPDK_FAIL;
                break;
            }
            else
            {                
                __msg("-----_inner_copy_dir success----tmp: %s, finish_num: %d\n", tmp, info->finish_num);
            }
        }
        else
        {        
            __msg("----------copy file-------------\n");
            core->status = CAT_FILE_PASTING;
            ret = _inner_copy_file(core, tmp, fullpath, info, &copy_item);
            core->status = CAT_DIR_PASTING;
            if(ret != EPDK_OK)
            {
                __msg("-----_inner_copy_file failed---tmp: %s\n", tmp);
                ret = EPDK_FAIL;
                break;
            }
            else
            {                
                __msg("-----_inner_copy_file success----tmp: %s, finish_num: %d\n", tmp, info->finish_num);
            }
            esKRNL_TimeDly(2);
        }    
    }
    
    eLIBs_closedir(dir);    
    dir_level--;
    
    __msg("----dir_level: %d, finish_num: %d, total_num: %d\n", dir_level, info->finish_num, info->total_num);
    
    eLIBs_strcpy(cat_item->base_info.fpath, dst_path);
    eLIBs_strcpy(cat_item->base_info.name, eLIBs_strchrlast(fullpath, '\\') + 1);    
    _get_es_timestamp(&(cat_item->base_info.item_pri_info.dir.m_time));
    
    if(ret == EPDK_FAIL)
    {
        info->percent = 101;
    }
    else if(info->finish_num == info->total_num)
    {
        info->percent = 100;
    }
    
    return ret;
}


static void _file_operation_thread(void *arg)
{
    __s32               ret = EPDK_OK;
    cat_core_t          *core = NULL;
    char                file[CAT_MAX_FULL_PATH_LEN];
    cat_list_handle_t   *list_handle = NULL;
    __s32               index = 0;
    
    if(arg == NULL)
    {
        return;
    }
    else
    {
        core = (cat_core_t *)arg;
    }
    
    eLIBs_memset(file, 0, CAT_MAX_FULL_PATH_LEN);
    
    while(1)
    {
        if(esKRNL_TDelReq(EXEC_prioself) == OS_TASK_DEL_REQ)
        {
            esKRNL_TDel(EXEC_prioself);
            break;
        }
        if(core->cmd_pend_flag == 0x01)
        {
            switch(core->cmd)
            {
                case CAT_DEL:
                {
                    eLIBs_strcpy(file, core->file_op_item.base_info.fpath);
                    eLIBs_strcat(file, "\\");
                    eLIBs_strcat(file, core->file_op_item.base_info.name);
                    list_handle = (cat_list_handle_t *)core->param1;
                    index = (__s32)core->param2;
                    core->status = CAT_DELETING;
                    __msg("----file: %s, list_handle: %x, index: %d\n", file, list_handle, index);
                    break;
                }
                case CAT_PASTE_FILE:
                {
                    core->status = CAT_FILE_PASTING;
                    list_handle = (cat_list_handle_t *)core->param1;
                    break;
                }
                case CAT_PASTE_DIR:
                {
                    core->status = CAT_DIR_PASTING;
                    list_handle = (cat_list_handle_t *)core->param1;
                    break;
                }
                default:
                {
                    break;
                }
            }
            core->cmd_pend_flag = 0x00;
        }
        if(core->status == CAT_DELETING)
        {
            _get_item_num_in_dir(file, &(core->file_op_info.total_num));
            __msg("----in dir: %s, total_num: %d\n", file, core->file_op_info.total_num);
            ret = _inner_delete_dir(core, file, &core->file_op_info);
            core->status = CAT_DELOVER;
            if(ret == EPDK_FAIL)
            {
                __msg("----delete dir failed\n");
            }
            else
            {
                cat_db_del_item(core->db, list_handle, index, &core->file_op_item);
            }
        }
        else if(core->status == CAT_FILE_PASTING)
        {
            __s32 tmp = eLIBs_GetFileSize(core->src);             
            __msg("----total_size: %d\n", tmp);
            eLIBs_memset(&(core->file_op_info), 0, sizeof(cat_file_op_info_t));
            core->file_op_info.total_size = tmp < 0 ? 0 : tmp;
            ret = _inner_copy_file(core, core->src, core->dst, &(core->file_op_info), &(core->file_op_item));
            __msg("----ret: %d\n", ret);            
            core->status = CAT_FILE_PASTEOVER;
            if(ret == EPDK_FAIL)
            {
                __msg("----copy file failed\n");
            }
            else if(eLIBs_strcmp(list_handle->create_para.path, core->dst) == 0)
            {
                /*add item to db*/
                cat_db_add_item(core->db, list_handle, &core->file_op_item);
            }
            else
            {
                /*搜索相应的explr*/
                list_handle = core->handle_list;
                while(list_handle != NULL)
                {
                    if(eLIBs_strcmp(list_handle->create_para.path, core->dst) == 0)
                    {
                        /*add item to db*/
                        cat_db_add_item(core->db, list_handle, &core->file_op_item);
                        break;
                    }
                    list_handle = list_handle->next;
                }
            }
        }
        else if(core->status == CAT_DIR_PASTING)
        {
            __s32 tmp = eLIBs_GetDirSize(core->src);
            __msg("----total_size: %d\n", tmp);
            eLIBs_memset(&(core->file_op_info), 0, sizeof(cat_file_op_info_t));
            core->file_op_info.total_size = tmp < 0 ? 0 : tmp;
            _get_item_num_in_dir(core->src, &(core->file_op_info.total_num));
            __msg("----total_num: %d\n", core->file_op_info.total_num);
            ret = _inner_copy_dir(core, core->src, core->dst, &core->file_op_info, &core->file_op_item);
            __msg("create_path = %s dst_path = %s, ret: %d\n", list_handle->create_para.path, core->dst, ret);           
            core->status = CAT_DIR_PASTEOVER;
            if(ret == EPDK_FAIL)
            {
                __msg("----copy dir failed\n");
            }
            else if(eLIBs_strcmp(list_handle->create_para.path, core->dst) == 0)
            {
                /*add item to db*/
                cat_db_add_item(core->db, list_handle, &core->file_op_item);
            }
            else
            {
                /*搜索相应的explr*/
                list_handle = core->handle_list;
                while(list_handle != NULL)
                {
                    if(eLIBs_strcmp(list_handle->create_para.path,core->dst) == 0)
                    {
                        /*add item to db*/
                        cat_db_add_item(core->db, list_handle, &core->file_op_item);
                        break;
                    }
                    list_handle = list_handle->next;
                }
            }
        }
        esKRNL_TimeDly(3);
    }
    
    return;
}


/*
**********************************************************************************************************************
*                                               cat_core_open
*
* author:        terry.zeng
*
* date:             2010-09-08
*
* Description:      cat core 打开函数
*
* parameters:
*
* return:           if success return cat core handle
*                   if fail return NULL
* modify history:
**********************************************************************************************************************
*/
HCAT cat_core_open(void)
{
    cat_core_t  *core = NULL;
        
    core = (cat_core_t *)esMEMS_Malloc(0, sizeof(cat_core_t));
    if(core == NULL)
    {
        __inf("%s,%d esMEMS_Malloc memory of cat_core_t failed!\n", __FILE__, __LINE__);
        return NULL;
    }
    else
    {
        eLIBs_memset(core, 0, sizeof(cat_core_t));
    }
    
    core->db = cat_db_open();
    if(core->db == NULL)
    {
        esMEMS_Mfree(0, core);
        return NULL;
    }
    
    core->engine = cat_engine_open(core->db);
    if(core->engine == NULL)
    {
        if(core->db)
        {
            cat_db_close(core->db);
            core->db = NULL;
        }
        esMEMS_Mfree(0, core);
        return NULL;
    }        
    
    core->file_op_thread_id = esKRNL_TCreate(_file_operation_thread, (void *)core, 0x8000, KRNL_priolevel4);
    
    return (HCAT)core;
}


/*
**********************************************************************************************************************
*                                               cat_create_explr
*
* author:           terry.zeng
*
* date:             2010-09-08
*
* Description:      创建浏览信息，只获取基本信息，extra信息需要另外的接口调用方可得到
*
* parameters:		core:cat句柄；create_para创建列表的参数
*
* return:           if success return explorer info handle
*                   if fail return NULL
* modify history:
**********************************************************************************************************************
*/
HEXPLR cat_create_explr(HCAT cat_handle, cat_create_param_t * create_para)
{
    cat_core_t          *core = NULL;
    cat_list_handle_t   *list_handle = NULL;
    __s32               ret = EPDK_OK;
    __u32               i = 0;
    char                str[12];
    cat_list_handle_t   *temp_list_handle = NULL;
    
    if((cat_handle == NULL) || (create_para == NULL))
    {
        return NULL;
    }
    else
    {
        core = (cat_core_t *)cat_handle;
    }
    
    list_handle = core->handle_list;
    while(list_handle != NULL)
    {
        /*该目录的浏览已经存在*/
        if( (eLIBs_strcmp(list_handle->create_para.path, create_para->path) == 0) 
            && (list_handle->create_para.scan_type == create_para->scan_type) )
        {
            /*已经存在但未搜索完成*/
            if(list_handle->finished_status == CAT_UNFINISHED)
            {
                ret = cat_engine_control(core->engine, READ_START, (__s32)list_handle, 0);
                if(ret == -1)
                {
                    return NULL;
                }
            }
            return (HEXPLR)list_handle;
        }
        list_handle = list_handle->next;        
        i++;
    }
    
    list_handle = (cat_list_handle_t *)esMEMS_Malloc(0, sizeof(cat_list_handle_t));
    if(list_handle == NULL)
    {
        return NULL;
    }
    else
    {
        eLIBs_memset(list_handle, 0, sizeof(cat_list_handle_t));
    }
    
    eLIBs_strcpy(list_handle->table_name, TABLE_PRE);
    eLIBs_memset(str, 0, 12);
    eLIBs_sprintf(str, "%d", i);
    eLIBs_strcat(list_handle->table_name, str);
    
    eLIBs_strcpy(list_handle->create_para.path, create_para->path);
    list_handle->create_para.scan_type = create_para->scan_type;
    list_handle->create_para.media_type = create_para->media_type;
    __msg("----create path: %s, scan_type: %d, media_type: %d\n", list_handle->create_para.path, list_handle->create_para.scan_type, list_handle->create_para.media_type);
    
    list_handle->bg_list = (cat_list_t *)esMEMS_Malloc(0, sizeof(cat_list_t));
    if(list_handle->bg_list == NULL)
    {
        esMEMS_Mfree(0, list_handle);
        return NULL;
    }
    else
    {
        eLIBs_memset(list_handle->bg_list, 0, sizeof(cat_list_t));
    }
    
    list_handle->next = NULL;
    if(core->handle_list == NULL)
    {
        core->handle_list = list_handle;
    }
    else
    {
        temp_list_handle = core->handle_list;
        while(temp_list_handle->next != NULL)
        {
            temp_list_handle = temp_list_handle->next;
        }
        temp_list_handle->next = list_handle;
    }
    
    /*
    __inf("\n\n\n\n----%s----%d\n", __FILE__, __LINE__);
    temp_list_handle= core->handle_list;    
    __inf("===================================================\n");
    while(temp_list_handle != NULL)
    {
        __inf("====core: %x, handle: %x, table_name: %s, path: %s, cache_handle: %x, cursor: %x\n", core, temp_list_handle, temp_list_handle->table_name, temp_list_handle->create_para.path, temp_list_handle->cache_handle, temp_list_handle->cursor);
        temp_list_handle = temp_list_handle->next;
    }
    __inf("===================================================\n\n\n\n");        
    */
    
    ret = cat_engine_control(core->engine, READ_START, (__s32)list_handle, 0);
    if(ret == -1)
    {
        return NULL;
    }
    else
    {
        return (HEXPLR)list_handle;
    }
}


/*
**********************************************************************************************************************
*                                               cat_create_explr
*
* author:           terry.zeng
*
* date:             2010-09-08
*
* Description:      创建浏览信息，只获取基本信息，extra信息需要另外的接口调用方可得到
*
* parameters:		core:cat句柄；create_para创建列表的参数
*
* return:           if success return explorer info handle
*                   if fail return NULL
* modify history:
**********************************************************************************************************************
*/
cat_create_status_t cat_get_create_status(HCAT cat_handle, HEXPLR hexplr)
{
    cat_list_handle_t   *list_handle = NULL;
    
    if((cat_handle == NULL) || (hexplr == NULL))
    {
        return CAT_CREATE_UNKONWN;
    }
    else
    {
        list_handle = (cat_list_handle_t *)hexplr;
    }
    
    if(list_handle->finished_status == CAT_FINISHED)
    {
        return CAT_CREATE_OVER;
    }
    else if(list_handle->finished_status == CAT_UNFINISHED)
    {
        return CAT_CREATING;
    }
    else
    {
        return CAT_CREATE_UNKONWN;
    }
}

/*
**********************************************************************************************************************
*                                               cat_update_root
*
* author:           terry.zeng
*
* date:             2010-09-08
*
* Description:      更新信息，传递需要更新的浏览信息句柄;目前只对分区浏览有效
*
* parameters:		core:cat句柄;hexplr:浏览信息句柄；
*
* return:           if success return EPDK_OKt
*                   if fail return EPDK_FAIL
* modify history:
**********************************************************************************************************************
*/
__s32 cat_update_root(HCAT cat_handle, HEXPLR hexplr, __s32 plug_in, char disk)
{
    cat_core_t          *core = NULL;
    __s32               ret = EPDK_OK;
    char                disk_name[4];    
    cat_list_handle_t   *cur = NULL;
    cat_list_handle_t   *tmp = NULL;
    char                sql[CAT_MAX_FULL_PATH_LEN];
    __cat_item_t        item;
    
    
    __msg("----cat_handle: %x, hexplr: %x\n", cat_handle, hexplr);
    
    if((cat_handle == NULL) || (hexplr == NULL))
    {
        return EPDK_FAIL;
    }
    else
    {
        core = (cat_core_t *)cat_handle;        
        
        eLIBs_memset(disk_name, 0, 4);
        if((disk >= 'A') && (disk <= 'Z'))
        {
            disk_name[0] = disk + 'a' - 'A';
        }
        else
        {
            disk_name[0] = disk;
        }
        disk_name[1] = ':';
        __msg("----disk_name: %s\n", disk_name);
    }
    
    eLIBs_memset(sql, 0, CAT_MAX_FULL_PATH_LEN);
    eLIBs_memset(&item, 0, sizeof(__cat_item_t));
    
    
    if(plug_in == 1)
    {       
        cat_item_node_t *node = NULL;
        cat_create_param_t create_param;
        cat_list_t cat_list;
        
        eLIBs_memset(&create_param, 0, sizeof(cat_create_param_t));
        eLIBs_memset(&cat_list, 0, sizeof(cat_list_t));
        
        create_param.scan_type = CAT_SCAN_ALL_PATI;
        create_param.media_type = CAT_MEDIA_TYPE_ALL;
        eLIBs_strcpy(create_param.path, "");        
        __msg("----scan_type: %d, media_type: %d, path: %s\n", create_param.scan_type, create_param.media_type, create_param.path);
        ret = cat_create_root(&create_param, &cat_list);
        if(ret != EPDK_OK)
        {
            __wrn("create root explorer is fail ! \n");
            return EPDK_FAIL;
        }        
                
        node = cat_list.item_list;
        while(node != NULL)
        {
            __msg("----fpath: %s, name: %s, type: %d\n", node->cat_item.base_info.fpath, node->cat_item.base_info.name, node->cat_item.base_info.type);                        
            if(eLIBs_strnicmp(node->cat_item.base_info.name, disk_name, 1) == 0)
            {
                if(cat_db_check_item_exist(core->db, core->handle_list, &node->cat_item) == EPDK_FALSE)
                {
                    __msg("----cat_db_add_item\n");
                    cat_db_add_item(core->db, core->handle_list, &node->cat_item);
                }
                else
                {
                    __msg("----%s is exist\n", disk_name);
                }
            }
            node = node->next;
        }
        cat_destroy_root(&cat_list);
        
        return EPDK_OK;
    }
    else    
    {
        cur = core->handle_list;
        while(cur != NULL)
        {
            tmp = cur;
            cur = cur->next;
            /*该目录的浏览已经存在*/
            __msg("----tmp->create_para.path: %s\n", tmp->create_para.path);
            if(eLIBs_strnicmp(tmp->create_para.path, disk_name, 1) == 0)
            {
                cat_destroy_explr(cat_handle, (HEXPLR)tmp);
            }
        }
        
        tmp = core->handle_list;
        while(tmp != NULL)
        {
            if(eLIBs_strcmp(tmp->create_para.path, "") == 0)
            {
                __msg("----handle: %x, table_name: %s, path: %s\n", tmp, tmp->table_name, tmp->create_para.path);
                eLIBs_strcpy(item.base_info.fpath, "");
                eLIBs_strcpy(item.base_info.name, disk_name);
                cat_db_del_item(core->db, tmp, -1, &item);
                break;
            }
        }
        
        return EPDK_OK;
    }    
}


/*
**********************************************************************************************************************
*                                               cat_destroy_explr
*
* author:           terry.zeng
*
* date:             2010-09-08
*
* Description:      销毁浏览信息
*
* parameters:		core:cat句柄;hexplr:浏览信息句柄；
*
* return:           if success return EPDK_OK
*                   if fail return EPDK_FAIL
* modify history:
**********************************************************************************************************************
*/
__s32 cat_destroy_explr(HCAT cat_handle, HEXPLR hexplr)
{
    cat_core_t          *core = NULL;
    cat_list_handle_t   *list_handle = NULL;    
    cat_list_handle_t   *tmp = NULL;   
    
    if((cat_handle == NULL) || (hexplr == NULL))
    {
        return EPDK_FAIL;
    }
    else
    {
        core = (cat_core_t *)cat_handle;
        list_handle = (cat_list_handle_t *)hexplr;
    }
    
    /*
    __inf("\n\n\n\n----%s----%d\n", __FILE__, __LINE__);
    tmp = core->handle_list;    
    __inf("===================================================\n");
    while(tmp != NULL)
    {
        __inf("====core: %x, handle: %x, table_name: %s, path: %s, cache_handle: %x, cursor: %x\n", core, tmp, tmp->table_name, tmp->create_para.path, tmp->cache_handle, tmp->cursor);
        tmp = tmp->next;
    }
    __inf("===================================================\n\n\n\n");        
    */
    
    tmp = core->handle_list;    
    while(tmp!= NULL)
    {
        if(tmp == list_handle)
        {
            break;
        }
        else
        {
            tmp = tmp->next;
        }
    }
    if(tmp == NULL)
    {
        __msg("----hexplr has already destroyed\n");
        return EPDK_FAIL;
    }
    
    if(list_handle->cursor != NULL)
    {
        DBCursor_Free(list_handle->cursor);
        list_handle->cursor = NULL;
    }    
    
    if(list_handle->cache_handle != NULL)
    {
        cat_cache_inner_delete(list_handle->cache_handle);
        list_handle->cache_handle = NULL;
    }
    
    
    if(list_handle->finished_status == CAT_FINISHED)
    {
        //删除对应的数据库table
        cat_db_table_del(core->db, list_handle);
    }
    
    
    /*销毁hexplr*/
    if(list_handle->bg_list != NULL)
    {
        esMEMS_Mfree(0, list_handle->bg_list);
        list_handle->bg_list = NULL;
    }        
            
    /*从core列表中去除*/
    tmp = core->handle_list;
    if(tmp != NULL)
    {
        if(tmp == list_handle)
        {
            core->handle_list = core->handle_list->next;
        }
        else
        {
            while(tmp->next != NULL)
            {
                if(tmp->next == list_handle)
                {
                    tmp->next =  tmp->next->next;
                    break;
                }
                tmp = tmp->next;
            }
        }
        
        esMEMS_Mfree(0, list_handle);
        list_handle = NULL;        
    }    
    
    /*
    __inf("\n\n\n\n----%s----%d\n", __FILE__, __LINE__);
    tmp = core->handle_list;    
    __inf("===================================================\n");
    while(tmp != NULL)
    {
        __inf("====core: %x, handle: %x, table_name: %s, path: %s, cache_handle: %x, cursor: %x\n", core, tmp, tmp->table_name, tmp->create_para.path, tmp->cache_handle, tmp->cursor);
        tmp = tmp->next;
    }
    __inf("===================================================\n\n\n\n");    
    */
    
    return EPDK_OK;
}

/*
**********************************************************************************************************************
*                                               cat_get_explr_num
*
* author:           terry.zeng
*
* date:             2010-09-08
*
* Description:      获取浏览信息的总数
*
* parameters:		core:cat句柄;hexplr:浏览信息句柄；
*
* return:           if success return explorer info num
*                   if fail return EPDK_FAIL
* modify history:
**********************************************************************************************************************
*/
__s32 cat_get_explr_num(HCAT cat_handle, HEXPLR hexplr)
{
    __s32               num = 0;
    cat_core_t          *core = NULL;
    cat_list_handle_t   *list_handle = NULL;
    
    if((cat_handle == NULL) || (hexplr == NULL))
    {
        return 0;
    }
    else
    {
        core = (cat_core_t *)cat_handle;
        list_handle = (cat_list_handle_t *)hexplr;
        num = cat_db_get_num(core->db, list_handle);
    
        return num;
    }
}

/*
**********************************************************************************************************************
*                                               cat_get_explr_item
*
* author:           terry.zeng
*
* date:             2010-09-08
*
* Description:      获取某一项的浏览信息
*
* parameters:		core:cat句柄;hexplr:浏览信息句柄；id:需要获取的项ID
*
* return:           if success return item info
*                   if fail return NULL
* modify history:
**********************************************************************************************************************
*/
__s32 cat_get_explr_item(HCAT cat_handle, HEXPLR hexplr, __u32 id, __cat_item_t *cat_item)
{
    cat_core_t          *core = NULL;
    cat_list_handle_t   *list_handle = NULL;
    __s32               ret = EPDK_OK;
    
    if((cat_handle == NULL) || (hexplr == NULL) || (cat_item == NULL))
    {
        return EPDK_FAIL;
    }
    else
    {
        core = (cat_core_t *)cat_handle;
        list_handle = (cat_list_handle_t *)hexplr;        
        ret = cat_db_get_base_info(core->db, list_handle, id, cat_item);
        
        return ret;
    }
}


/*
**********************************************************************************************************************
*                                               cat_sort_explr
*
* author:           terry.zeng
*
* date:             2010-09-08
*
* Description:      对浏览信息进行排序
*
* parameters:		core:cat句柄;hexplr:浏览信息句柄；id:需要获取的项ID
*
* return:           if success return EPDK_OK
*                   if fail return EPDK_FAIL
* modify history:
**********************************************************************************************************************
*/
__s32 cat_sort_explr(HCAT cat_handle, HEXPLR hexplr, cat_sort_para_t sort_para)
{
    cat_core_t          *core = NULL;
    cat_list_handle_t   *list_handle = NULL;
    __s32               ret = EPDK_OK;
    
    if((cat_handle == NULL) || (hexplr == NULL))
    {
        return EPDK_FAIL;
    }
    else
    {
        core = (cat_core_t *)cat_handle;
        list_handle = (cat_list_handle_t *)hexplr;
        ret = cat_db_sort(core->db, list_handle, sort_para.type, sort_para.mode);
        
        return ret;
    }
}


/*
**********************************************************************************************************************
*                                               cat_get_explr_para
*
* author:           terry.zeng
*
* date:             2010-09-08
*
* Description:      获取浏览信息创建参数
*
* parameters:		core:cat句柄;hexplr:浏览信息句柄；
*
* return:           if success return create para
*                   if fail return NULL
* modify history:
**********************************************************************************************************************
*/
__s32 cat_get_explr_para(HCAT cat_handle, HEXPLR hexplr, cat_create_param_t *create_para)
{
    cat_core_t          *core = NULL;
    cat_list_handle_t   *list_handle = NULL;
    
    core = (cat_core_t *)cat_handle;
    list_handle = (cat_list_handle_t *)hexplr;
        
    if((core == NULL) || (list_handle == NULL) || (create_para == NULL))
    {
        return EPDK_FAIL;
    }
    else
    {
        eLIBs_strcpy(create_para->path, list_handle->create_para.path);
        create_para->scan_type = list_handle->create_para.scan_type;
        create_para->media_type = list_handle->create_para.media_type;
        
        return EPDK_OK;
    }
}


/*
**********************************************************************************************************************
*                                               cat_open_extra_dec
*
* author:           terry.zeng
*
* date:             2010-09-08
*
* Description:      打开详细信息的解码功能。此功能主要在图片缩略图解码、音乐专辑解码、视频截图解码或者电子书额外信息解码时调用
*
* parameters:		core:cat句柄;
*
* return:           if success return EPDK_OK
*                   if fail return EPDK_FAIL
* modify history:
**********************************************************************************************************************
*/
__s32 cat_open_extra_dec(HCAT cat_handle, cat_media_type_t type)
{
    cat_core_t *core = (cat_core_t *)cat_handle;
    
    if(core == NULL)
    {
        return EPDK_FAIL;
    }

    //__msg("----type: %d\n", type);
    
    if((type == CAT_MEDIA_TYPE_AUDIO) || (type == CAT_MEDIA_TYPE_PIC))
    {
        if(core->willow == NULL)
        {
            core->willow = AWILLOW_Open();
            if(core->willow == NULL)
            {
                __wrn("----open willow failed\n");
                return EPDK_FAIL;
            }
        }        
    }        
    else if(type == CAT_MEDIA_TYPE_VIDEO)
    { 
        if(core->video_dec == NULL)
        {
            core->video_dec = video_dec_open();
            if(core->video_dec == NULL)
            {
                __wrn("----open video decode error\n");
                return EPDK_FAIL;
            }
        }
    }
    else
    {
        return EPDK_FAIL;
    }
        
    return EPDK_OK;
}


/*
**********************************************************************************************************************
*                                               cat_close_extra_dec
*
* author:           terry.zeng
*
* date:             2010-09-08
*
* Description:      关闭详细信息的解码功能。
*
* parameters:       core:cat句柄;
*
* return:           if success return EPDK_OK
*                   if fail return EPDK_FAIL
* modify history:
**********************************************************************************************************************
*/
__s32 cat_close_extra_dec(HCAT cat_handle)
{
    cat_core_t *core = (cat_core_t *)cat_handle;
    
    if(core == NULL)
    {
        return EPDK_FAIL;
    }
            
    if(core->willow != NULL)
    {
        AWILLOW_Close(core->willow);
        core->willow = NULL;        
    }
        
    if(core->video_dec != NULL)
    {
        video_dec_close(core->video_dec);
        core->video_dec = NULL;
    }
    
    return EPDK_OK;
}

/*
**********************************************************************************************************************
*                                               cat_cache_create
*
* author:           terry.zeng
*
* date:             2010-09-08
*
* Description:      打开缩略图cache功能，每一个浏览信息对应一个cache
*
* parameters:		core:cat句柄;hexplr:浏览信息句柄；cache_para
*
* return:           if success return EPDK_OK
*                   if fail return EPDK_FAIL
* modify history:
**********************************************************************************************************************
*/
__s32 cat_cache_create(HCAT cat_handle, HEXPLR hexplr, cat_cache_para_t cache_para)
{
    cat_core_t          *core = NULL;
    cat_list_handle_t   *list_handle = NULL;
    
    if((cat_handle == NULL) || (hexplr == NULL))
    {
        return EPDK_FAIL;
    }
    else
    {
        core = (cat_core_t *)cat_handle;
        list_handle = (cat_list_handle_t *)hexplr;
    }
    
    list_handle->cache_handle = cat_cache_inner_create(core->db, list_handle, &cache_para);    
        
    
    if(list_handle->cache_handle == NULL)
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
*                                               cat_cache_destroy
*
* author:           terry.zeng
*
* date:             2010-09-08
*
* Description:      销毁cache功能。
*
* parameters:		core:cat句柄;hexplr:浏览信息句柄；
*
* return:           if success return EPDK_OK
*                   if fail return EPDK_FAIL
* modify history:
**********************************************************************************************************************
*/
__s32 cat_cache_destroy(HCAT cat_handle, HEXPLR hexplr)
{
    cat_core_t          *core = NULL;
    cat_list_handle_t   *list_handle = NULL;
    __s32               ret = EPDK_FAIL;
    cat_list_handle_t   *tmp = NULL;
    
    __msg("----cat_handle: %x, hexplr: %x\n", cat_handle, hexplr);
    if((cat_handle == NULL) || (hexplr == NULL))
    {
        return EPDK_FAIL;
    }
    else
    {
        core = (cat_core_t *)cat_handle;
        list_handle = (cat_list_handle_t *)hexplr;
    }    
    
    tmp = core->handle_list;    
    while(tmp != NULL)
    {
        if(tmp == list_handle)
        {
            break;
        }
        else
        {
            tmp = tmp->next;
        }
    }
    if(tmp == NULL)
    {
        __msg("----hexplr has already destroyed\n");
        return EPDK_FAIL;
    }
    
    if(list_handle->cache_handle != NULL)
    {        
        cat_wait_willow_finish(core->willow);     
        ret = cat_cache_inner_delete(list_handle->cache_handle);        
        list_handle->cache_handle = NULL;
    }
    
    return ret;
}


/*
**********************************************************************************************************************
*                                               cat_get_explr_extra
*
* author:           terry.zeng
*
* date:             2010-09-08
*
* Description:      获取额外信息
*
* parameters:		core:cat句柄;hexplr:浏览信息句柄；cat_item:浏览项
*
* return:           if success return EPDK_OK
*                   if fail return EPDK_FAIL
* modify history:
**********************************************************************************************************************
*/
__s32 cat_get_explr_extra(HCAT cat_handle, HEXPLR hexplr, cat_miniature_para_t *para , __u32 id, __cat_item_t *cat_item)
{
    cat_core_t          *core = NULL;
    cat_list_handle_t   *list_handle = NULL;
    __s32               ret = EPDK_OK;
    
    if((cat_handle == NULL) || (hexplr == NULL) || (cat_item == NULL))
    {
        return EPDK_FAIL;
    }
    else
    {
        core = (cat_core_t *)cat_handle;
        list_handle = (cat_list_handle_t *)hexplr;
    }
        
    /*尝试从数据库获取，返回EPDK_OK代表数据库已存储该信息，返回EPDK_FAIL代表没有，需要重新解详细信息，然后添加到数据库*/
    ret = cat_db_get_extra_info(core->db, list_handle, id, cat_item);
    __msg("----ret: %d, id: %d\n", ret, id);
    if(ret == EPDK_OK)
    {
        return EPDK_OK;
    }
    
    /*获取基本信息，然后解析详细信息，添加到数据库*/
    ret = cat_db_get_base_info(core->db, list_handle, id, cat_item);
    if(ret == EPDK_FAIL)
    {
        return EPDK_FAIL;
    }
    
    if(cat_item->base_info.type == CAT_SW_PARTI)
    {
        ret = cat_get_parti_item_detail(cat_item);
    }
    else if(cat_item->base_info.type == CAT_SW_DIR)
    {
        ret = cat_get_dir_item_detail(cat_item, list_handle->create_para.media_type);
    }
    else if(cat_item->base_info.type == CAT_SW_FILE)
    {
        if(cat_item->base_info.item_pri_info.file.media_type == CAT_MEDIA_TYPE_PIC)
        {
            cat_pic_info_t pic_info;
            
            if(list_handle->cache_handle != NULL)
            {
                cat_get_one_cache_buf(list_handle->cache_handle, id, cat_item);
            }
            else
            {
                return EPDK_FAIL;
            }
            
            eLIBs_memset(&pic_info, 0, sizeof(cat_pic_info_t));
            pic_info.miniature.buf = cat_item->extra_info.extra_info.file.miniature.buf;
            eLIBs_strcpy(para->file, cat_item->base_info.fpath);
            eLIBs_strcat(para->file, "\\");
            eLIBs_strcat(para->file, cat_item->base_info.name);
            ret = cat_get_pic_info(core->willow, para, &pic_info);
            cat_item->extra_info.extra_info.file.file_info.pic.width = pic_info.width;
            cat_item->extra_info.extra_info.file.file_info.pic.height = pic_info.height;
            cat_item->extra_info.extra_info.file.miniature.len = pic_info.miniature.len;
            if(cat_item->extra_info.extra_info.file.miniature.len == 0)
            {
                cat_item->extra_info.extra_info.file.miniature.buf = NULL;
                /*还回buf*/
                cat_free_one_cache_buf(list_handle->cache_handle);
            }
        }
        else if(cat_item->base_info.item_pri_info.file.media_type == CAT_MEDIA_TYPE_AUDIO)
        {
            cat_audio_info_t audio_info; 
            
            if(list_handle->cache_handle != NULL)
            {
                cat_get_one_cache_buf(list_handle->cache_handle, id, cat_item);
            }
            else
            {
                return EPDK_FAIL;
            }
            
            eLIBs_memset(&audio_info, 0, sizeof(cat_audio_info_t));
            audio_info.album.buf = cat_item->extra_info.extra_info.file.miniature.buf;
            eLIBs_strcpy(para->file, cat_item->base_info.fpath);
            eLIBs_strcat(para->file, "\\");
            eLIBs_strcat(para->file, cat_item->base_info.name);
            ret = cat_get_album_info(core->willow, para, &audio_info);
            cat_item->extra_info.extra_info.file.file_info.audio.bitrate = audio_info.bitrate;
            cat_item->extra_info.extra_info.file.file_info.audio.sample_rate = audio_info.sample_rate;
            cat_item->extra_info.extra_info.file.file_info.audio.channel = audio_info.channel;
            cat_item->extra_info.extra_info.file.file_info.audio.duration = audio_info.duration;
            cat_item->extra_info.extra_info.file.file_info.audio.bits = audio_info.bits;
            cat_item->extra_info.extra_info.file.miniature.len = audio_info.album.len;
            if(cat_item->extra_info.extra_info.file.miniature.len == 0)
            {
                cat_item->extra_info.extra_info.file.miniature.buf = NULL;
                /*还回buf*/
                cat_free_one_cache_buf(list_handle->cache_handle);
            }
        }
        else if(cat_item->base_info.item_pri_info.file.media_type == CAT_MEDIA_TYPE_VIDEO)
        {
            cat_video_info_t video_info;
            
            if(list_handle->cache_handle != NULL)
            {
                cat_get_one_cache_buf(list_handle->cache_handle,id,cat_item);
            }
            else
            {
                return EPDK_FAIL;
            }
            
            eLIBs_memset(&video_info, 0, sizeof(cat_video_info_t));
            video_info.miniature.buf = cat_item->extra_info.extra_info.file.miniature.buf;
            eLIBs_strcpy(para->file, cat_item->base_info.fpath);
            eLIBs_strcat(para->file, "\\");
            eLIBs_strcat(para->file, cat_item->base_info.name);            
            ret = cat_get_video_info(core->video_dec, para, &video_info);            
            cat_item->extra_info.extra_info.file.file_info.video.width = video_info.width;
            cat_item->extra_info.extra_info.file.file_info.video.height = video_info.height;
            cat_item->extra_info.extra_info.file.file_info.video.frame_rate = video_info.frame_rate;
            cat_item->extra_info.extra_info.file.file_info.video.video_fmt = video_info.video_fmt;
            cat_item->extra_info.extra_info.file.file_info.video.audio_fmt = video_info.audio_fmt;
            cat_item->extra_info.extra_info.file.file_info.video.duration = video_info.duration;
            cat_item->extra_info.extra_info.file.miniature.len = video_info.miniature.len;
            if(cat_item->extra_info.extra_info.file.miniature.len == 0)
            {
                cat_item->extra_info.extra_info.file.miniature.buf = NULL;
                /*还回buf*/
                cat_free_one_cache_buf(list_handle->cache_handle);
            }
        }
        else if(cat_item->base_info.item_pri_info.file.media_type == CAT_MEDIA_TYPE_EBOOK)
        {
        }
    }
    cat_db_add_extra_info(core->db, list_handle, id, cat_item);
    
    return ret;
}


/*
**********************************************************************************************************************
*                                               cat_del_file
*
* author:           terry.zeng
*
* date:             2010-09-08
*
* Description:      删除一个文件
*
* parameters:		core:cat句柄;hexplr:浏览信息句柄；file:文件名
*
* return:           if success return EPDK_OK
*                   if fail return EPDK_FAIL
* modify history:
**********************************************************************************************************************
*/
__s32 cat_del_file(HCAT cat_handle, HEXPLR hexplr, __u32 index)
{
    cat_core_t          *core = NULL;
    cat_list_handle_t   *list_handle = NULL;
    __cat_item_t        cat_item;
    char                file[CAT_MAX_FULL_PATH_LEN];    
    __s32               ret = EPDK_OK;
    
    if((cat_handle == NULL) || (hexplr == NULL))
    {
        return EPDK_FAIL;
    }
    else
    {
        core = (cat_core_t *)cat_handle;
        list_handle = (cat_list_handle_t *)hexplr;
    }
    
    /*get the item information in database by index*/
    ret = cat_db_get_base_info(core->db, list_handle, index, &cat_item);
    if(ret == EPDK_FAIL)
    {
        return EPDK_FAIL;
    }
    
    /*delete file at filedisk*/
    eLIBs_memset(file, 0, CAT_MAX_FULL_PATH_LEN);
    eLIBs_strcpy(file, cat_item.base_info.fpath);
    eLIBs_strcat(file, "\\");
    eLIBs_strcat(file, cat_item.base_info.name);
    ret = eLIBs_remove(file);
    if(ret != 0)
    {
        __msg("----remove file %s failed\n", file);
        return EPDK_FAIL;
    }
    else
    {
        core->file_op_info.percent = 100;
    }
    
    /*delete the item in database*/
    cat_db_del_item(core->db, list_handle, index, &cat_item);   
    
    /*if audio file,update orchid*/
    if(cat_item.base_info.item_pri_info.file.media_type == CAT_MEDIA_TYPE_AUDIO)
    {
        ES_FILE *orchid = eLIBs_fopen(ORCHID_FILE_PATH, "r+");
        if(orchid == NULL)
        {
            return EPDK_FAIL;
        }
        else
        {
            __msg("----ORC_CMD_DELETE_FILE_FROM_ORCHID\n");
            eLIBs_fioctrl(orchid, ORC_CMD_ORCHID_IS_VALID, 0, 0 );
            eLIBs_fioctrl(orchid, ORC_CMD_DELETE_FILE_FROM_ORCHID, 0, file);
            eLIBs_fclose(orchid);
        }
    }
    
    return EPDK_OK;
}


/*
**********************************************************************************************************************
*                                               cat_del_dir
*
* author:           terry.zeng
*
* date:             2010-09-08
*
* Description:      删除一个目录
*
* parameters:		core:cat句柄;hexplr:浏览信息句柄；dir:目录名
*
* return:           if success return EPDK_OK
*                   if fail return EPDK_FAIL
* modify history:
**********************************************************************************************************************
*/
__s32 cat_del_dir(HCAT cat_handle, HEXPLR hexplr, __u32 index)
{
    cat_core_t          *core = NULL;
    cat_list_handle_t   *list_handle = NULL;
    cat_list_handle_t   *cur = NULL;
    cat_list_handle_t   *tmp = NULL;
    char                dir[CAT_MAX_FULL_PATH_LEN];    
    __s32               ret = EPDK_OK;
    __u8                err = 0;
    
    if((cat_handle == NULL) || (hexplr == NULL))
    {
        return EPDK_FAIL;
    }
    else
    {
        core = (cat_core_t *)cat_handle;
        list_handle = (cat_list_handle_t *)hexplr;
    }
    
    /*get the item information in database by index*/
    ret = cat_db_get_base_info(core->db, list_handle, index, &core->file_op_item);
    if(ret == EPDK_FAIL)
    {
        return EPDK_FAIL;
    }
    
    eLIBs_memset(dir, 0, CAT_MAX_FULL_PATH_LEN);
    eLIBs_strcpy(dir, core->file_op_item.base_info.fpath);
    eLIBs_strcat(dir, "\\");
    eLIBs_strcat(dir, core->file_op_item.base_info.name);
    
    __msg("----dir: %s\n", dir);
    
    /*delete dir infomation in database*/
    __msg("----core: %x, db: %x, list_handle: %x\n", core, core->db, list_handle);
    //cat_db_del_item(core->db, list_handle, index, &cat_item);
    
    /*
    tmp= core->handle_list;
    __inf("\n\n\n\n");
    __inf("===================================================\n");
    while(tmp != NULL)
    {
        __inf("====table_name: %s, path: %s\n", tmp->table_name, tmp->create_para.path);
        tmp = tmp->next;
    }
    __inf("===================================================\n\n\n\n");    
    */
    
    cur = core->handle_list;
    while(cur != NULL)
    {
        tmp = cur;
        cur = cur->next;        
        /*该目录的浏览已经存在*/
        __msg("----tmp->create_para.path: %s\n", tmp->create_para.path);
        if(eLIBs_strncmp(tmp->create_para.path, dir, eLIBs_strlen(dir)) == 0)
        {
            cat_destroy_explr(cat_handle, (HEXPLR)tmp);
        }        
    }
    
    
    eLIBs_memset(&(core->file_op_info), 0, sizeof(cat_file_op_info_t));
    core->file_op_info.type = CAT_SW_DIR;
    while(core->cmd_pend_flag == 0x01)
    {
        esKRNL_TimeDly(2);
    }
    core->cmd_pend_flag = 0x01;
    core->cmd = CAT_DEL;
    core->param1 = (__u32)list_handle;
    core->param2 = (__u32)index;
    
    return EPDK_OK;
}


__s32 cat_cancel_del(HCAT cat_handle, HEXPLR hexplr)
{
    cat_core_t          *core = NULL;
    cat_list_handle_t   *list_handle = NULL;
    
    if((cat_handle == NULL) || (hexplr == NULL))
    {
        return EPDK_FAIL;
    }
    else
    {
        core = (cat_core_t *)cat_handle;
        list_handle = (cat_list_handle_t *)hexplr;
    }
    
    if(core->status == CAT_DELETING)
    {
        core->status = CAT_DELOVER;
    }
    
    __msg("----core: %x, list_handle: %x, status: %d\n", core, list_handle, core->status);
    
    return EPDK_OK;
}

/*
**********************************************************************************************************************
*                                               cat_get_paste_del_info
*
* author:           terry.zeng
*
* date:             2010-09-08
*
* Description:      获取复制删除的详细信息
*
* parameters:		core:cat句柄;hexplr:浏览信息句柄；info:复制删除信息
*
* return:           if success return EPDK_OK
*                   if fail return EPDK_FAIL
* modify history:
**********************************************************************************************************************
*/
__s32 cat_get_file_operation_info(HCAT cat_handle, cat_file_op_info_t *info)
{
    cat_core_t  *core = NULL;
    
    if((cat_handle == NULL) || (info == NULL))
    {
        return EPDK_FAIL;
    }
    else
    {
        core = (cat_core_t *)cat_handle;
    }
    
    info->type = core->file_op_info.type;
    info->total_size = core->file_op_info.total_size;
    info->finish_size = core->file_op_info.finish_size;
    info->total_num = core->file_op_info.total_num;
    info->finish_num = core->file_op_info.finish_num;
    info->percent = core->file_op_info.percent;
    info->percent = core->file_op_info.percent;
    
    return EPDK_OK;
}


/*
**********************************************************************************************************************
*                                               cat_copy_file
*
* author:           terry.zeng
*
* date:             2010-09-08
*
* Description:      复制一个文件
*
* parameters:		core:cat句柄;hexplr:浏览信息句柄；file:文件名
*
* return:           if success return EPDK_OK
*                   if fail return EPDK_FAIL
* modify history:
**********************************************************************************************************************
*/
__s32 cat_copy_file(HCAT cat_handle, HEXPLR hexplr, __u32 index)
{
    cat_core_t          *core = NULL;
    cat_list_handle_t   *list_handle = NULL;
    __s32               ret = EPDK_OK;
    
    if((cat_handle == NULL) || (hexplr == NULL))
    {
        return EPDK_FAIL;
    }
    else
    {
        core = (cat_core_t *)cat_handle;
        list_handle = (cat_list_handle_t *)hexplr;
    }
    
    //get the item information in database by index
    ret = cat_db_get_base_info(core->db, list_handle, index, &core->file_op_item);
    if(ret == EPDK_FAIL)
    {
        return EPDK_FAIL;
    }
    
    eLIBs_strcpy(core->src, core->file_op_item.base_info.fpath);
    eLIBs_strcat(core->src, "\\");
    eLIBs_strcat(core->src, core->file_op_item.base_info.name);
    
    return EPDK_OK;
}


/*
**********************************************************************************************************************
*                                               cat_paste_file
*
* author:           terry.zeng
*
* date:             2010-09-08
*
* Description:      粘贴一个文件
*
* parameters:		core:cat句柄;hexplr:浏览信息句柄；des_path:目标路径
*
* return:           if success return EPDK_OK
*                   if fail return EPDK_FAIL
* modify history:
**********************************************************************************************************************
*/
__s32 cat_paste_file(HCAT cat_handle, HEXPLR hexplr, char *src, char *dst)
{
    cat_core_t          *core = NULL;
    cat_list_handle_t   *list_handle = NULL;
    __u8                err = 0;
    
    if((cat_handle == NULL) || (hexplr == NULL) || (src == NULL) || (dst == NULL))
    {
        return EPDK_FAIL;
    }
    else
    {
        core = (cat_core_t *)cat_handle;
        list_handle = (cat_list_handle_t *)hexplr;
    }        
    
    eLIBs_strcpy(core->src, src);
    eLIBs_strcpy(core->dst, dst);
    eLIBs_memset(&(core->file_op_info), 0, sizeof(cat_file_op_info_t));
    core->file_op_info.type = CAT_SW_FILE;
    while(core->cmd_pend_flag == 0x01)
    {
        esKRNL_TimeDly(2);
    }
    
    core->cmd_pend_flag = 0x01;
    core->cmd = CAT_PASTE_FILE;
    core->param1 = (__u32)list_handle;
    
    return EPDK_OK;
}


/*
**********************************************************************************************************************
*                                               cat_cancel_paste
*
* author:           terry.zeng
*
* date:             2010-09-08
*
* Description:      取消粘贴
*
* parameters:		core:cat句柄;hexplr:浏览信息句柄；
*
* return:           if success return EPDK_OK
*                   if fail return EPDK_FAIL
* modify history:
**********************************************************************************************************************
*/
__s32 cat_cancel_paste(HCAT cat_handle, HEXPLR hexplr)
{
    cat_core_t          *core = NULL;
    cat_list_handle_t   *list_handle = NULL;
    
    if((cat_handle == NULL) || (hexplr == NULL))
    {
        return EPDK_FAIL;
    }
    else
    {
        core = (cat_core_t *)cat_handle;
        list_handle = (cat_list_handle_t *)hexplr;
    }
    
    if(core->status == CAT_FILE_PASTING)
    {
        core->status = CAT_FILE_PASTEOVER;
    }
    else if(core->status == CAT_DIR_PASTING)
    {
        core->status = CAT_DIR_PASTEOVER;
    }
    
    __msg("----core: %x, list_handle: %x, core->status: %d\n", core, list_handle, core->status);
    
    return EPDK_OK;
}


/*
**********************************************************************************************************************
*                                               cat_copy_dir
*
* author:           terry.zeng
*
* date:             2010-09-08
*
* Description:      复制一个目录
*
* parameters:		core:cat句柄;hexplr:浏览信息句柄；dir:目录名
*
* return:           if success return EPDK_OK
*                   if fail return EPDK_FAIL
* modify history:
**********************************************************************************************************************
*/
__s32 cat_copy_dir(HCAT cat_handle, HEXPLR hexplr, __u32 index)
{
    cat_core_t          *core = NULL;
    cat_list_handle_t   *list_handle = NULL;
    __s32               ret = EPDK_OK;
    
    if((cat_handle == NULL) || (hexplr == NULL))
    {
        return EPDK_FAIL;
    }
    else
    {
        core = (cat_core_t *)cat_handle;
        list_handle = (cat_list_handle_t *)hexplr;
    }
    
    //get the item information in database by index
    ret = cat_db_get_base_info(core->db, list_handle, index, &core->file_op_item);
    if(ret == EPDK_FAIL)
    {
        return EPDK_FAIL;
    }
    
    eLIBs_strcpy(core->src, core->file_op_item.base_info.fpath);
    eLIBs_strcat(core->src, "\\");
    eLIBs_strcat(core->src, core->file_op_item.base_info.name);
    
    
    __msg("-------------total size: %d\n", eLIBs_GetDirSize(core->src));
    
    return EPDK_OK;
}
/*
**********************************************************************************************************************
*                                               cat_paste_dir
*
* author:           terry.zeng
*
* date:             2010-09-08
*
* Description:      粘贴一个目录
*
* parameters:		core:cat句柄;hexplr:浏览信息句柄；des_path:目标路径
*
* return:           if success return EPDK_OK
*                   if fail return EPDK_FAIL
* modify history:
**********************************************************************************************************************
*/
__s32 cat_paste_dir(HCAT cat_handle, HEXPLR hexplr, char *src, char *dst)
{
    cat_core_t          *core = NULL;
    cat_list_handle_t   *list_handle = NULL;
    __u8                err = 0;
    
    if((cat_handle == NULL) || (hexplr == NULL) || (src == NULL) || (dst == NULL))
    {
        return EPDK_FAIL;
    }
    else
    {
        core = (cat_core_t *)cat_handle;
        list_handle = (cat_list_handle_t *)hexplr;
    }
    
    eLIBs_strcpy(core->src, src);
    eLIBs_strcpy(core->dst, dst);
    eLIBs_memset(&(core->file_op_info), 0, sizeof(cat_file_op_info_t));	
    core->file_op_info.type = CAT_SW_DIR;
    while(core->cmd_pend_flag == 0x01)
    {
        esKRNL_TimeDly(2);
    }
    
    core->cmd_pend_flag = 0x01;
    core->cmd = CAT_PASTE_DIR;
    core->param1 = (__u32)list_handle;
    
    return EPDK_OK;
}

/*
**********************************************************************************************************************
*                                               cat_set_file_for_play
*
* author:           terry.zeng
*
* date:             2010-09-08
*
* Description:      获取当前播放项的ID，
*
* parameters:		core:cat句柄;hexplr:浏览信息句柄；media_type：从cat_set_file_for_play返回的媒体类型；cur_id：当前项的ID。
*
* return:           if success return EPDK_OK
*                   if fail return EPDK_FAIL
* modify history:
**********************************************************************************************************************
*/
__s32 cat_set_file_for_play(HCAT cat_handle, HEXPLR hexplr, cat_media_type_t *media_type, __u32 cur_id)
{
    ES_FILE             *orchid = NULL;
    HNPL                hnpl = NULL;
    orc_arg_seek_t      seek_arg;
    __cat_item_t        cur_item;
    __cat_item_t        play_item;
    char                filename[CAT_MAX_FULL_PATH_LEN]; 
    __u32               true_media_type;
    cat_core_t          *core = NULL;
    cat_list_handle_t   *list_handle = NULL;
    __u32               num = 0;
    __u32               i = 0;
    __u32               index = 0; 
    __s32               result = 0;
    orc_arg_npl_t       arg;
    
    if((cat_handle == NULL) || (hexplr == NULL) || (media_type == NULL))
    {
        return EPDK_FAIL;
    }
    else
    {
        core = (cat_core_t *)cat_handle;
        list_handle = (cat_list_handle_t *)hexplr;
    }
    
    orchid = eLIBs_fopen(ORCHID_FILE_PATH, "r+");
    if(orchid == NULL)
    {
        __wrn("open orchid fail!\n");
        return EPDK_FAIL;
    }
    
    eLIBs_fioctrl(orchid, ORC_CMD_ORCHID_IS_VALID, 0, 0);
    result = cat_db_get_base_info(core->db, list_handle, cur_id, &cur_item);
    if(result == EPDK_FAIL)
    {
        eLIBs_fclose(orchid);     
        return EPDK_FAIL;
    }
    
    if(cat_get_file_format(cur_item.base_info.name, CAT_MEDIA_TYPE_AUDIO, &true_media_type) != -1)
    {
        arg.media_type = ORC_MEDIA_TYPE_AUDIO;
        arg.work_mode  = ORC_NPL_WORK_MODE_MANAGE;
        *media_type = CAT_MEDIA_TYPE_AUDIO;
    }
    else if(cat_get_file_format(cur_item.base_info.name, CAT_MEDIA_TYPE_VIDEO, &true_media_type) != -1)
    {
        arg.media_type = ORC_MEDIA_TYPE_VIDEO;
        arg.work_mode  = ORC_NPL_WORK_MODE_MANAGE;
        *media_type = CAT_MEDIA_TYPE_VIDEO;        
    }
    else if(cat_get_file_format(cur_item.base_info.name, CAT_MEDIA_TYPE_PIC, &true_media_type) != -1)
    {
        arg.media_type = ORC_MEDIA_TYPE_PIC;
        arg.work_mode  = ORC_NPL_WORK_MODE_MANAGE;
        *media_type = CAT_MEDIA_TYPE_PIC;        
    }
    else if(cat_get_file_format(cur_item.base_info.name, CAT_MEDIA_TYPE_EBOOK, &true_media_type) != -1)
    {
        arg.media_type = ORC_MEDIA_TYPE_EBOOK;
        arg.work_mode  = ORC_NPL_WORK_MODE_MANAGE;
        *media_type = CAT_MEDIA_TYPE_EBOOK;        
    }
    else if(cat_get_file_format(cur_item.base_info.name, CAT_MEDIA_TYPE_FLASH, &true_media_type) != -1)
    {
        arg.media_type = ORC_MEDIA_TYPE_FLASH;
        arg.work_mode  = ORC_NPL_WORK_MODE_MANAGE;
        *media_type = CAT_MEDIA_TYPE_FLASH;
    }
    else
    {
        eLIBs_fclose(orchid);
        return EPDK_FAIL;
    }
    
    hnpl = (HNPL)eLIBs_fioctrl(orchid, ORC_CMD_CONSTRUCT_NOW_PLAYLIST, 0, (void *)&arg);
    eLIBs_fioctrl(orchid, ORC_CMD_CLEAR_NPL, 0, (void *)hnpl);
    num = cat_db_get_num(core->db, list_handle);
    for(i = 0; i < num; i++)
    {
        cat_db_get_base_info(core->db, list_handle, i, &play_item);
        if(cat_get_file_format(play_item.base_info.name, *media_type, &true_media_type) != -1)
        {
            eLIBs_memset(filename, 0, CAT_MAX_FULL_PATH_LEN);
            eLIBs_strcpy(filename, play_item.base_info.fpath);
            eLIBs_strcat(filename, "\\");
            eLIBs_strcat(filename, play_item.base_info.name);
            eLIBs_fioctrl(orchid, ORC_CMD_ADD_ITEM_TO_NPL, hnpl, filename);
        }
    }
    
    eLIBs_memset(filename, 0, CAT_MAX_FULL_PATH_LEN);
    eLIBs_strcpy(filename, cur_item.base_info.fpath);
    eLIBs_strcat(filename, "\\");
    eLIBs_strcat(filename, cur_item.base_info.name);
    index = eLIBs_fioctrl(orchid, ORC_CMD_FILE_TO_INDEX_IN_NPL, hnpl, filename);
    seek_arg.hnpl = hnpl;
    seek_arg.offset = index;
    seek_arg.origin = ORC_NPL_SEEK_SET;
    eLIBs_fioctrl(orchid, ORC_CMD_PLAYING_SEEK, 0, (void *)&seek_arg);
    eLIBs_fioctrl(orchid, ORC_CMD_DESTROY_NOW_PLAYLIST, 0, (void *)hnpl);
    
    eLIBs_fclose(orchid);

    return EPDK_OK;
}


/*
**********************************************************************************************************************
*                                               cat_get_cur_playfile
*
* author:           terry.zeng
*
* date:             2010-09-08
*
* Description:      获取当前播放项的ID，
*
* parameters:		core:cat句柄;hexplr:浏览信息句柄；media_type：从cat_set_file_for_play返回的媒体类型；cur_id：当前项的ID。
*
* return:           if success return EPDK_OK
*                   if fail return EPDK_FAIL
* modify history:
**********************************************************************************************************************
*/
__s32 cat_get_cur_playfile(HCAT cat_handle, HEXPLR hexplr, cat_media_type_t media_type, __u32 *cur_id)
{
    cat_core_t                  *core = NULL;
    cat_list_handle_t           *list_handle = NULL;
    ES_FILE                     *orchid = NULL;
    HNPL                        hnpl = NULL;
    __s32                       index = 0;
    orc_arg_index_to_file_t     index_arg;
    orc_arg_npl_t               npl_arg;
    __s32                       ret = EPDK_OK;
    
    if((cat_handle == NULL) || (hexplr == NULL))
    {
        return EPDK_FAIL;
    }
    else
    {
        core = (cat_core_t *)cat_handle;
        list_handle = (cat_list_handle_t *)hexplr;
    }
    
    orchid = eLIBs_fopen(ORCHID_FILE_PATH, "r+");
    if(orchid == 0)
    {
        __wrn("open orchid fail!\n");
        return EPDK_FAIL;
    }
    
    eLIBs_fioctrl(orchid, ORC_CMD_ORCHID_IS_VALID, 0, 0);
    eLIBs_memset(&index_arg, 0, sizeof(orc_arg_index_to_file_t));
    eLIBs_memset(&npl_arg, 0, sizeof(orc_arg_npl_t));
    
    if(media_type == CAT_MEDIA_TYPE_AUDIO)
    {
        npl_arg.media_type = ORC_MEDIA_TYPE_AUDIO;
        npl_arg.work_mode  = ORC_NPL_WORK_MODE_MANAGE;
    }
    else if(media_type == CAT_MEDIA_TYPE_VIDEO)
    {
        npl_arg.media_type = ORC_MEDIA_TYPE_VIDEO;
        npl_arg.work_mode  = ORC_NPL_WORK_MODE_MANAGE;
    }
    else if(media_type == CAT_MEDIA_TYPE_PIC)
    {
        npl_arg.media_type = ORC_MEDIA_TYPE_PIC;
        npl_arg.work_mode  = ORC_NPL_WORK_MODE_MANAGE;
    }
    else if(media_type == CAT_MEDIA_TYPE_EBOOK)
    {
        npl_arg.media_type = ORC_MEDIA_TYPE_EBOOK;
        npl_arg.work_mode  = ORC_NPL_WORK_MODE_MANAGE;
    }
    else if(media_type == CAT_MEDIA_TYPE_FLASH)
    {
        npl_arg.media_type = ORC_MEDIA_TYPE_FLASH;
        npl_arg.work_mode  = ORC_NPL_WORK_MODE_MANAGE;
    }
    else
    {
        eLIBs_fclose(orchid);
        return EPDK_FAIL;
    }

    hnpl = (HNPL)eLIBs_fioctrl(orchid, ORC_CMD_CONSTRUCT_NOW_PLAYLIST, 0, (void *)&npl_arg);
    eLIBs_fioctrl(orchid, ORC_CMD_PLAYING_GET_CUR, hnpl, (void *)&index);
    index_arg.hnpl = hnpl;
    index_arg.index = index;
    index_arg.file = esMEMS_Malloc(0, CAT_MAX_FULL_PATH_LEN);
    eLIBs_fioctrl(orchid, ORC_CMD_INDEX_TO_FILE_IN_NPL, 0, (void *)&index_arg);
    index = cat_db_get_rowid(core->db, list_handle, index_arg.file);
    if(index == -1)
    {        
        ret = EPDK_FAIL;
    }
    else
    {
        *cur_id = index;
        ret = EPDK_OK;
    }
    
    esMEMS_Mfree(0, index_arg.file);
    eLIBs_fioctrl(orchid, ORC_CMD_DESTROY_NOW_PLAYLIST, 0, (void *)hnpl);
    eLIBs_fclose(orchid);
    
    return ret;
}

/*
**********************************************************************************************************************
*                                               cat_search
*
* author:           terry.zeng
*
* date:             2010-09-08
*
* Description:      搜索关键字对应的浏览信息，返回搜索到的浏览信息对应的列表
*
* parameters:		core:cat句柄；hexplr:对应的浏览信息;keyword:关键字
*
* return:           if success return explorer info handle
*                   if fail return NULL
* modify history:
**********************************************************************************************************************
*/
HEXPLR cat_search(HCAT cat_handle, HEXPLR hexplr, char *keyword)
{
    cat_core_t          *core = NULL;
    cat_list_handle_t   *list_handle = NULL;
    //cat_list_handle_t   *tmp = NULL;
    cat_list_handle_t   *search_handle = NULL;
    __s32               ret = EPDK_OK;
    
    if((cat_handle == NULL) || (hexplr == NULL) || (keyword == NULL))
    {
        return NULL;
    }
    else
    {
        core = (cat_core_t *)cat_handle;
        list_handle = (cat_list_handle_t *)hexplr;
    }
    
    search_handle = (cat_list_handle_t *)esMEMS_Malloc(0, sizeof(cat_list_handle_t));
    if(search_handle == NULL)
    {
        return NULL;
    }
    else
    {
        eLIBs_memset(search_handle, 0, sizeof(cat_list_handle_t));
    }
    
    eLIBs_strcpy(search_handle->table_name, list_handle->table_name);
    __msg("----table: %s\n", search_handle->table_name);
    search_handle->attr = SEARCH_DB;
    search_handle->create_para.scan_type = list_handle->create_para.scan_type;
    search_handle->create_para.media_type = list_handle->create_para.media_type;
    eLIBs_strcpy(search_handle->create_para.path, list_handle->create_para.path);
    ret = cat_db_search(core->db, list_handle, keyword, search_handle);
    if(ret == EPDK_FAIL)
    {
        esMEMS_Mfree(0,search_handle);
        return NULL;
    }
    else
    {
        list_handle = core->handle_list;
        while(list_handle->next != NULL)
        {
            list_handle = list_handle->next;				
        }
        list_handle->next = search_handle;
        
        /*
        tmp = core->handle_list;    
        __inf("\n\n\n\n----%s----%d\n", __FILE__, __LINE__);
        __inf("===================================================\n");
        while(tmp != NULL)
        {
            __inf("====core: %x, handle: %x, table_name: %s, path: %s, cache_handle: %x, cursor: %x\n", core, tmp, tmp->table_name, tmp->create_para.path, tmp->cache_handle, tmp->cursor);
            tmp = tmp->next;
        }
        __inf("===================================================\n\n\n\n");    
        */
        return (HEXPLR)search_handle;
    }
}


/*
**********************************************************************************************************************
*                                               cat_core_close
*
* author:           terry.zeng
*
* date:             2009-09-29
*
* Description:      关闭cat core，释放相关资源
*
* parameters:
*
* return:           if success return EPDK_OK
*                   if fail return EPDK_FAIL
* modify history:
**********************************************************************************************************************
*/
__s32 cat_core_close(HCAT cat_handle)
{
    cat_core_t          *core = NULL;
    cat_list_handle_t   *list_handle = NULL;
    cat_list_handle_t   *tmp = NULL;
    
    if(cat_handle == NULL)
    {
        return EPDK_FAIL;
    }
    else
    {
        core = (cat_core_t *)cat_handle;
    }
    /*
    tmp = core->handle_list;    
    __inf("\n\n\n\n----%s----%d\n", __FILE__, __LINE__);
    __inf("===================================================\n");
    while(tmp != NULL)
    {
        __inf("====core: %x, handle: %x, table_name: %s, path: %s, cache_handle: %x, cursor: %x\n", core, tmp, tmp->table_name, tmp->create_para.path, tmp->cache_handle, tmp->cursor);
        tmp = tmp->next;
    }
    __inf("===================================================\n\n\n\n");        
    */
    
    while(1)
    {
        if(esKRNL_TDelReq(core->file_op_thread_id) == OS_TASK_NOT_EXIST)
        {
            break;
        }
        esKRNL_TimeDly(3);
    }
    
    if(core->engine)
    {
        cat_engine_close(core->engine);
        core->engine = NULL;
    }
    
    list_handle = core->handle_list;
    while(list_handle != NULL)
    {
        tmp = list_handle;
        list_handle = list_handle->next;        
        cat_destroy_explr((HCAT)core, (HEXPLR)tmp);  
    }
    core->handle_list = NULL;
    
    if(core->db)
    {
        cat_db_close(core->db);
        core->db = NULL;
    }
    
    cat_close_extra_dec((HCAT)core);
    
    esMEMS_Mfree(0, core);
    core = NULL;
    
    return EPDK_FAIL;
}

