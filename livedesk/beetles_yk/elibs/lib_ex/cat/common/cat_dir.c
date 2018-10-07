/*
*******************************************************************************************************************
*                                                         	CAT  Framework
*                                         			the core of apps support module
*
*                             				 Copyright(C), 2006-2008, Softwinner Microelectronic Co., Ltd.
*											               All Rights Reserved
*
*File Name： 	cat_dir.c
*
*Author： 		Terry Zeng
*
*Version :   	1.1.0
*
*Date:  		2009-09-30
*
*Description : the dir part of cat 
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

#define CAT_IGNORE_PATH         "e:"
#define CAT_IGNORE_PATH_LEN     2
#define CAT_IGNORE_DIR          "gps"
#define CAT_IGNORE_DIR_LEN      3

__u32 has_read=0;

static __s32 _change_audio_format_to_cat(__s32 file_format)
{
    __s32 format = -1;
    
    if(file_format == SW_AUDIO_FORMAT_AAC)
    {
        format = CAT_AUDIO_FORMAT_AAC;
    }
    else if(file_format == SW_AUDIO_FORMAT_AC3)
    {
        format = CAT_AUDIO_FORMAT_AC3;
    }
    else if(file_format == SW_AUDIO_FORMAT_APE)
    {
        format = CAT_AUDIO_FORMAT_APE;
    }
    else if(file_format == SW_AUDIO_FORMAT_DTS)
    {
        format = CAT_AUDIO_FORMAT_DTS;
    }
    else if(file_format == SW_AUDIO_FORMAT_FLAC)
    {
        format = CAT_AUDIO_FORMAT_FLAC;
    }
    else if(file_format == SW_AUDIO_FORMAT_MP3)
    {
        format = CAT_AUDIO_FORMAT_MP3;
    }
    else if(file_format == SW_AUDIO_FORMAT_OGG)
    {
        format = CAT_AUDIO_FORMAT_OGG;
    }
    else if(file_format == SW_AUDIO_FORMAT_RA)
    {
        format = CAT_AUDIO_FORMAT_RA;
    }
    else if(file_format == SW_AUDIO_FORMAT_WAV)
    {
        format = CAT_AUDIO_FORMAT_WAV;
    }
    else if(file_format == SW_AUDIO_FORMAT_WMA)
    {
        format = CAT_AUDIO_FORMAT_WMA;
    }
    else if(file_format == SW_AUDIO_FORMAT_AMR)
    {
        format = CAT_AUDIO_FORMAT_AMR;
    }
    else if(file_format == SW_AUDIO_FORMAT_ATRC)
    {
        format = CAT_AUDIO_FORMAT_ATRC;
    }
    /*
    else if(file_format == SW_AUDIO_FORMAT_MID)
    {
        format = AUDIO_FORMAT_MID;
    }
    */
    else
    {
        format = -1;
    }
    
    return format;
}


static __s32 _change_photo_format_to_cat(__s32 file_format)
{
    __s32 format = -1;
    
    if(file_format == SW_PIC_FORMAT_BMP)
    {
        format = CAT_PIC_FORMAT_BMP;
    }
    else if(file_format == SW_PIC_FORMAT_GIF)
    {
        format = CAT_PIC_FORMAT_GIF;
    }
    else if(file_format == SW_PIC_FORMAT_PNG)
    {
        format = CAT_PIC_FORMAT_PNG;
    }
    else if(file_format == SW_PIC_FORMAT_JPG)
    {
        format = CAT_PIC_FORMAT_JPG;
    }
    else 
    {
        format = -1;
    }
    
    return format;
}


static __s32 _change_video_format_to_cat(__s32 file_format)
{
    __s32 format = -1;
    
    if(file_format == SW_VIDEO_FORMAT_AVI)
    {
        format = CAT_VIDEO_FORMAT_AVI;
    }
    else if(file_format == SW_VIDEO_FORMAT_MP4)
    {
        format = CAT_VIDEO_FORMAT_MP4;
    }
    else if(file_format == SW_VIDEO_FORMAT_MPG)
    {
        format = CAT_VIDEO_FORMAT_MPG;
    }
    else if(file_format == SW_VIDEO_FORMAT_RM)
    {
        format = CAT_VIDEO_FORMAT_RM;
    }
    else if(file_format == SW_VIDEO_FORMAT_FLV)
    {
        format = CAT_VIDEO_FORMAT_FLV;
    }
    else if(file_format == SW_VIDEO_FORMAT_PMP)
    {
        format = CAT_VIDEO_FORMAT_PMP;
    }
    else if(file_format == SW_VIDEO_FORMAT_MOV)
    {
        format = CAT_VIDEO_FORMAT_MOV;
    }
    else if(file_format == SW_VIDEO_FORMAT_3GP)
    {
        format = CAT_VIDEO_FORMAT_3GP;
    }
    else if(file_format == SW_VIDEO_FORMAT_VOB)
    {
        format = CAT_VIDEO_FORMAT_VOB;
    }
    else if(file_format == SW_VIDEO_FORMAT_DAT)
    {
        format = CAT_VIDEO_FORMAT_DAT;
    }
    else if(file_format == SW_VIDEO_FORMAT_RMVB)
    {
        format = CAT_VIDEO_FORMAT_RMVB;
    }
    else if(file_format == SW_VIDEO_FORMAT_MKV)
    {
        format = CAT_VIDEO_FORMAT_MKV;
    }
    /*
    else if(file_format == SW_VIDEO_FORMAT_WMV)
    {
        format = CAT_VIDEO_FORMAT_WMV;
    }
    */
    else if(file_format == SW_VIDEO_FORMAT_ASF)
    {
        format = CAT_VIDEO_FORMAT_ASF;
    }
    else if(file_format == SW_VIDEO_FORMAT_PSR)
    {
        format = CAT_VIDEO_FORMAT_PSR;
    }
    else if(file_format == SW_VIDEO_FORMAT_TP)
    {
        format = CAT_VIDEO_FORMAT_TP;
    }
    else if(file_format == SW_VIDEO_FORMAT_TS)
    {
        format = CAT_VIDEO_FORMAT_TS;
    }
    else 
    {
        format = -1;
    }
    
    return format;
}


static __s32 _change_ebook_format_to_cat(__s32 file_format)
{
    __s32 format = -1;
    
    if(file_format == SW_EBOOK_FORMAT_TXT)
    {
        format = CAT_EBOOK_FORMAT_TXT;
    }
    /*
    else if(file_format == SW_EBOOK_FORMAT_C)
    {
        format = CAT_EBOOK_FORMAT_C;
    }
    else if(file_format == SW_EBOOK_FORMAT_H)
    {
        format = CAT_EBOOK_FORMAT_H;
    }
    else if(file_format == SW_EBOOK_FORMAT_INI)
    {
        format = CAT_EBOOK_FORMAT_INI;
    }
    else if(file_format == SW_EBOOK_FORMAT_LRC)
    {
        format = CAT_EBOOK_FORMAT_LRC;
    }
    else if(file_format == SW_EBOOK_FORMAT_SRT)
    {
        format = CAT_EBOOK_FORMAT_SRT;
    }
    else if(file_format == SW_EBOOK_FORMAT_PDF)
    {
        format = CAT_EBOOK_FORMAT_PDF;
    }
    else if(file_format == SW_EBOOK_FORMAT_EPUB)
    {
        format = CAT_EBOOK_FORMAT_EPUB;
    }
    else if(file_format == SW_EBOOK_FORMAT_FB2)
    {
        format = CAT_EBOOK_FORMAT_FB2;
    }
    else if(file_format == SW_EBOOK_FORMAT_PDB)
    {
        format = CAT_EBOOK_FORMAT_PDB;
    }
    else if(file_format == SW_EBOOK_FORMAT_RTF)
    {
        format = CAT_EBOOK_FORMAT_RTF;
    }
    else if(file_format == SW_EBOOK_FORMAT_HTML)
    {
        format = CAT_EBOOK_FORMAT_HTML;
    }
    else if(file_format == SW_EBOOK_FORMAT_CHM)
    {
        format = CAT_EBOOK_FORMAT_CHM;
    }
    else if(file_format == SW_EBOOK_FORMAT_HTM)
    {
        format = CAT_EBOOK_FORMAT_HTM;
    }
    */
    else 
    {
        format = -1;
    }
    
    return format;
}


static __s32 _change_game_format_to_cat(__s32 file_format)
{
    __s32 format = -1;
    
    if(file_format == SW_GAME_FORMAT_EPOS)
    {
        format = CAT_GAME_FORMAT_EPOS;
    }
    else 
    {
        format = -1;
    }
    
    return format;
}


static __s32 _change_flash_format_to_cat(__s32 file_format)
{
    __s32 format = -1;
    
    if(file_format == SW_FLASH_FORMAT_SWF)
    {
        format = CAT_FLASH_FORMAT_SWF;
    }
    else 
    {
        format = -1;
    }
    
    return format;
}


/*
**********************************************************************************************************************
*                                               cat_get_file_format
*
* author:          terry.zeng 
*
* date:             2009-09-30
*
* Description:       获取文件格式 
*
* parameters:       filename 文件名  media_type：媒体类型
*
* return:           
* modify history: 
**********************************************************************************************************************
*/
__s32 cat_get_file_format(char *filename, __u32 check_type, __u32 *media_type)
{
    __s32 file_format = 0;
    __s32 cat_format = -1;
    
    if((filename == NULL) || (media_type == NULL))
    {
        return -1;
    }
    
    *media_type = CAT_MEDIA_TYPE_UNKNOWN;
    
    switch(check_type)
    {
        case CAT_MEDIA_TYPE_PIC:
        {
            if(!eLIBs_GetPicFormat(filename, &file_format))
            {
                file_format = 0;
            }
            if(file_format != 0)
            {
                cat_format = _change_photo_format_to_cat(file_format);
                *media_type = CAT_MEDIA_TYPE_PIC;
            }            
            break;
        }
        case CAT_MEDIA_TYPE_EBOOK:
        {
            if(!eLIBs_GetEbookFormat(filename, &file_format))
            {
                file_format = 0;
            }
            if(file_format != 0)
            {
                cat_format = _change_ebook_format_to_cat(file_format);
                *media_type = CAT_MEDIA_TYPE_EBOOK;
            }            
            break;
        }
        case CAT_MEDIA_TYPE_AUDIO:
        {
            if(!eLIBs_GetAudioFormat(filename, &file_format))
            {
                file_format = 0;
            }
            if(file_format != 0)
            {
                cat_format = _change_audio_format_to_cat(file_format);
                *media_type = CAT_MEDIA_TYPE_AUDIO;
            }            
            break;
        }
        case CAT_MEDIA_TYPE_VIDEO:
        {
            if(!eLIBs_GetVideoFormat(filename, &file_format))
            {
                file_format = 0;
            }
            if(file_format != 0)
            {
                cat_format = _change_video_format_to_cat(file_format);
                *media_type = CAT_MEDIA_TYPE_VIDEO;
            }
            break;
        }
        /*
        case CAT_MEDIA_TYPE_GAME:
        {
            if(!eLIBs_GetGameFormat(filename, &file_format))
            {
                file_format = 0;
            }
            if(file_format != 0)
            {
                cat_format = _change_game_format_to_cat(file_format);
                *media_type = CAT_MEDIA_TYPE_GAME;
            }
            break;
        }
        case CAT_MEDIA_TYPE_FLASH:
        {
            if(!eLIBs_GetFlashFormat(filename, &file_format))
            {
                file_format = 0;
            }
            if(file_format != 0)
            {
                cat_format = _change_flash_format_to_cat(file_format);
                *media_type = CAT_MEDIA_TYPE_FLASH;
            }
            break;
        }
        case CAT_MEDIA_TYPE_FIRMWARE:
        {
            if((eLIBs_strncmp(eLIBs_strchrlast(filename, '.'), ".img", 4) == 0)
                ||(eLIBs_strncmp(eLIBs_strchrlast(filename, '.'), ".IMG", 4) == 0))
            {
                cat_format = CAT_FIRMWARE_FORMAT_IMG;
                *media_type = CAT_MEDIA_TYPE_FIRMWARE;
            }
            break;
        }
        */
        case CAT_MEDIA_TYPE_ALL:
        {
            if(eLIBs_GetAudioFormat(filename, &file_format))
            {
                if(file_format != 0)
                {
                    cat_format = _change_audio_format_to_cat(file_format);
                    *media_type = CAT_MEDIA_TYPE_AUDIO;
                }
            }
            else if(eLIBs_GetVideoFormat(filename, &file_format))
            {
                if(file_format != 0)
                {
                    cat_format = _change_video_format_to_cat(file_format);
                    *media_type = CAT_MEDIA_TYPE_VIDEO;
                }
            }
            else if(eLIBs_GetPicFormat(filename, &file_format))
            {
                if(file_format != 0)
                {
                    cat_format = _change_photo_format_to_cat(file_format);
                    *media_type = CAT_MEDIA_TYPE_PIC;
                }
            }
            else if(eLIBs_GetEbookFormat(filename, &file_format))
            {
                if(file_format != 0)
                {
                    cat_format = _change_ebook_format_to_cat(file_format);
                    *media_type = CAT_MEDIA_TYPE_EBOOK;
                }
            }
            /*
            else if(eLIBs_GetGameFormat(filename, &file_format))
            {
                if(file_format != 0)
                {
                    cat_format = _change_game_format_to_cat(file_format);
                    *media_type = CAT_MEDIA_TYPE_GAME;
                }
            }
            else if(eLIBs_GetFlashFormat(filename,&file_format))
            {
                if(file_format != 0)
                {
                    cat_format = _change_flash_format_to_cat(file_format);
                    *media_type = CAT_MEDIA_TYPE_FLASH;
                }                
            }
            else if ((eLIBs_strncmp(eLIBs_strchrlast(filename, '.'), ".img", 4) == 0)
                    ||(eLIBs_strncmp(eLIBs_strchrlast(filename, '.'), ".IMG", 4) == 0))
            {
                cat_format = CAT_FIRMWARE_FORMAT_IMG;
                *media_type = CAT_MEDIA_TYPE_FIRMWARE;                
            }
            */
            else
            {
                cat_format = -1;
                *media_type = CAT_MEDIA_TYPE_UNKNOWN;  
            }              
            break;
        }
        default:
        {
            cat_format = -1;
            *media_type = CAT_MEDIA_TYPE_UNKNOWN;  
            break;
        }
    }
    
    return cat_format;    
}


static __bool _has_file_type_in_dir(char *dirpath, __u32 check_type)
{
    __s32           format = 0;
    __bool          ret = EPDK_FALSE;
    __u32           media_type;
    ES_DIR          *dirp = NULL;
    ES_DIRENT       *direntp = NULL;
    char            sub_dir[CAT_MAX_FULL_PATH_LEN];        
    static __s32    dir_level = 0;
    
    if(dirpath == NULL)
    {
        return EPDK_FALSE;
    }
    else if(dir_level >= MAX_DIR_SCAN_LEVEL)
    {
        return EPDK_FALSE;
    }

    dirp = (ES_DIR *)eLIBs_opendir(dirpath);
    if (dirp == NULL)
    {
        return EPDK_FALSE;
    }
    else
    {
        dir_level++;
    }     

  //  __msg("-------open path: %s, dir_level: %d\n", dirpath, dir_level);
    
    while((direntp = (ES_DIRENT *)eLIBs_readdir(dirp)) != NULL)
    {
//        __msg("---------------------------name: %s, \n", (char *)direntp->d_name);
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
            if(_has_file_type_in_dir(sub_dir, check_type) == EPDK_FALSE)
            {
                ret = EPDK_FALSE;
                __msg("-----there is no file of this type in dir [%s]\n", sub_dir);
                //break;
            }
            else
            {
                ret = EPDK_TRUE;
                break;
            }     
        }
        else
        {
            format = cat_get_file_format((char *)direntp->d_name, check_type, &media_type);            
            if (format != -1)
            {
                ret = EPDK_TRUE;
                break;
            }            
        }
    }
    
    __msg("---close dirpath: %s, dir_level: %d\n", dirpath, dir_level);
    
    dir_level--;            
    eLIBs_closedir(dirp);        
    
    return ret;
}



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
* return:           文件数目   
* modify history: 
**********************************************************************************************************************
*/
__u32 cat_get_file_count_in_dir(char *path, __u32 check_type)
{
    ES_DIR      *dir = NULL;
    ES_DIRENT   *dirent = NULL;
    __u32       cnt = 0;
    __u32       media_type;
    __s32       format = 0;
    
    if(path == NULL)
    {
        return 0;
    }
    
    /*open dir*/
    dir = eLIBs_opendir(path);
    if(dir == NULL)
    {
        return 0;
    }
        
    while((dirent = eLIBs_readdir(dir)) != NULL)
    {
        if (dirent->d_name[0] == '.')
        {
            continue;
        }
        
        if(dirent->fatdirattr & FSYS_ATTR_DIRECTORY) /*is a dir*/
        {        
            cnt++;
        }
        else /*is a file*/
        {
            format = cat_get_file_format((char *)dirent->d_name, check_type, &media_type);
            if (format != -1)
            {
                cnt++;
            }	
            else
            {
                if(check_type == CAT_MEDIA_TYPE_ALL)
                {
                    cnt++;
                }
            }
        }
    }
    /*close dir*/
    eLIBs_closedir(dir);
    	
    return cnt;
}


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
* return:           success: EPDK_OK;    failed: EPDK_FAIL;
* modify history: 
**********************************************************************************************************************
*/
#if 1
__s32 cat_create_dir_info(cat_create_param_t *param, cat_list_t *cat_list, cat_engine_t *engine, __u32 start_pos)
{
    ES_DIR                  *dir = NULL;
    ES_DIRENT               *dirent = NULL;     
    char                    *fpath = NULL;      //为避免多级目录递归时，stack溢出，特使用malloc
    cat_item_node_t         *item = NULL;    
    static cat_item_node_t  *cur_item = NULL;   //由于存在目录递归，特声明为static
    __s32                   format = -1;
    __u32                   media_type = CAT_MEDIA_TYPE_UNKNOWN;
    __u32                   font_code=0;
    cat_create_param_t      *tmp_param = NULL;
    __s32                   need_sync = 0;
    __s32                   is_dir = 0;
    __s32                   dir_has_ignored = 0;//提高效率 
    static __s32            dir_level = 0;    
    
    if((param == NULL) || (cat_list == NULL) || (engine == NULL))
    {
        return EPDK_FAIL;
    }
    else if(dir_level >= MAX_DIR_SCAN_LEVEL)
    {
        __msg("----dir_level is over than MAX_DIR_SCAN_LEVEL, failed\n");        
        return EPDK_FAIL;
    }
    
    /*open dir*/
    dir = eLIBs_opendir(param->path);
    
    if(dir == NULL)
    {    
        __wrn("----open dir %s failed\n", param->path);
        return EPDK_FAIL;
    } 
    else
    {
        dir_level++;
    } 
               
    fpath = (char *)esMEMS_Malloc(0, CAT_MAX_FULL_PATH_LEN);
    if(fpath == NULL)
    {
        __msg("malloc fpath failed!\n");
        dir_level--;
        eLIBs_closedir(dir);        
        return EPDK_FAIL;
    }
    else
    {
        eLIBs_memset(fpath, 0, CAT_MAX_FULL_PATH_LEN);
    }   
    eLIBs_GetFSCharset(param->path, (__s32 *)&font_code);    
    while(((dirent = eLIBs_readdir(dir)) != NULL) && (engine->status != ENGINE_STOPPED))
    { 
        //__msg("----name: %s\n", (char *)dirent->d_name);
        if(dirent->fatdirattr & FSYS_ATTR_DIRECTORY) /*is a dir*/
        {            
            if (dirent->d_name[0] == '.')
            {
                continue;
            }
            /*
            else if(dir_has_ignored == 0)
            {
                if((eLIBs_strnicmp(param->path, CAT_IGNORE_PATH, CAT_IGNORE_PATH_LEN) == 0) && (eLIBs_strnicmp((char *)dirent->d_name, CAT_IGNORE_DIR, CAT_IGNORE_DIR_LEN) == 0))
                {
                    __msg("----ignore path: %s  dir: %s\n", param->path, (char *)dirent->d_name);
                    dir_has_ignored = 1;
                    continue;
                }
            }
            */
            eLIBs_memset(fpath, 0, CAT_MAX_FULL_PATH_LEN);
            eLIBs_strcpy(fpath, param->path);
            eLIBs_strcat(fpath, "\\");
            eLIBs_strcat(fpath, (const char *)dirent->d_name);
            
            
            if(param->scan_type == CAT_SCAN_MEIDA_ALL)
            {
                tmp_param = (cat_create_param_t *)esMEMS_Malloc(0, sizeof(cat_create_param_t));
                if(tmp_param == NULL)
                {
                    dir_level--;
                    eLIBs_closedir(dir);
                    esMEMS_Mfree(0, fpath);
                    return EPDK_FAIL;
                }
                else
                {
                    eLIBs_memset(tmp_param, 0, sizeof(cat_create_param_t));
                }
                
                eLIBs_strcpy(tmp_param->path, fpath);
                tmp_param->scan_type = param->scan_type;
                tmp_param->media_type = param->media_type;
                cat_create_dir_info(tmp_param, cat_list, engine, start_pos);                
                
                esMEMS_Mfree(0, tmp_param);
            }
            else if(param->media_type == CAT_MEDIA_TYPE_ALL)
            {
                need_sync = 1;
                is_dir = 1;
            }
            else if(_has_file_type_in_dir(fpath, param->media_type) == EPDK_TRUE)
            {
                need_sync = 1;
                is_dir = 1;
            }
        }
        else /*is a file*/
        {            
            format = cat_get_file_format((char *)dirent->d_name, param->media_type, &media_type);
            //__msg("----format: %d, media_type: %d\n", format, param->media_type);
            if(format != -1)
            {
                need_sync = 1;
            }
            /*
            else if(param->media_type == CAT_MEDIA_TYPE_ALL)
            {
                need_sync = 1;
            }
            */
            else
            {
                continue;
            }
        }
        
        if(need_sync == 1)
        {
            has_read++;
            if(has_read <= start_pos)
            {
                continue;
            }
            
            item = esMEMS_Malloc(0, sizeof(cat_item_node_t));
            if(item == NULL)
            {
                __msg("malloc item failed!\n");
                dir_level--;
                eLIBs_closedir(dir); 
                esMEMS_Mfree(0, fpath);
                return EPDK_FAIL;
            }
            else
            {
                eLIBs_memset(item, 0, sizeof(cat_item_node_t));
            }
            
            
            eLIBs_strcpy(item->cat_item.base_info.fpath, param->path);
            eLIBs_strcpy(item->cat_item.base_info.name, (const char *)dirent->d_name);
            item->cat_item.base_info.char_encoding = (__epdk_charset_enm_e)font_code;
            
            
            eLIBs_memset(fpath, 0, CAT_MAX_FULL_PATH_LEN);
            eLIBs_strcpy(fpath, item->cat_item.base_info.fpath);
            eLIBs_strcat(fpath, "\\");
            eLIBs_strcat(fpath, (const char *)item->cat_item.base_info.name);
            
            if(is_dir == 1)
            {
                item->cat_item.base_info.type = CAT_SW_DIR;
                eLIBs_GetDirMTime(fpath, &(item->cat_item.base_info.item_pri_info.dir.m_time));
            }
            else 
            {
                if(format != -1)
                {
                    item->cat_item.base_info.type = CAT_SW_FILE;
                    item->cat_item.base_info.item_pri_info.file.format = format;
                    item->cat_item.base_info.item_pri_info.file.media_type = (cat_media_type_t)media_type;
                }
                else
                {
                    item->cat_item.base_info.type = CAT_SW_EMPTY;
                }
                item->cat_item.base_info.item_pri_info.file.size = eLIBs_GetFileSize(fpath);
                eLIBs_GetFileMTime(fpath, &(item->cat_item.base_info.item_pri_info.file.m_time));
            }
            
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
            
            need_sync = 0;
            is_dir = 0;
            format = -1;
        }
        
        if(cat_list->nitem % 100 == 0)
        {
            /*添加到数据库*/
            cat_db_add_base_info(engine->db, engine->list_handle);
            cat_destroy_dir_info(cat_list);
            cur_item = NULL;
            //cat_destroy_dir_info(engine->list_handle->bg_list);
            esKRNL_TimeDly(5);
        }
    }
    /*添加到数据库*/	
    //cat_db_add_base_info(engine->db, engine->list_handle);	
    //cat_destroy_dir_info(cat_list);
    
    /*close dir*/
    dir_level--;
    eLIBs_closedir(dir);    
    esMEMS_Mfree(0, fpath);    
    if(engine->status == ENGINE_STOPPED)
    {
        return EPDK_FAIL;
    }
    
    return EPDK_OK;
}

#else
__s32 cat_create_dir_info(cat_create_param_t *param, cat_list_t *cat_list, cat_engine_t *engine, __u32 start_pos)
{
    ES_DIR                  *dir = NULL;
    ES_DIRENT               *dirent = NULL;     
    char                    *fpath = NULL;      //为避免多级目录递归时，stack溢出，特使用malloc
    cat_item_node_t         *item = NULL;    
    static cat_item_node_t  *cur_item = NULL;   //由于存在目录递归，特声明为static
    __s32                   format = -1;
    __u32                   media_type = CAT_MEDIA_TYPE_UNKNOWN;
    __u32                   font_code=0;
    cat_create_param_t      *tmp_param = NULL;
    __s32                   need_sync = 0;
    __s32                   is_dir = 0;
    __s32                   dir_has_ignored = 0;//提高效率 
       
    
    if((param == NULL) || (cat_list == NULL) || (engine == NULL))
    {
        return EPDK_FAIL;
    }            
    
    /*open dir*/
    dir = eLIBs_opendir(param->path);
    if(dir == NULL)
    {        
        __wrn("----open dir %s failed\n", param->path);
        return EPDK_FAIL;
    }    
    
    fpath = (char *)esMEMS_Malloc(0, CAT_MAX_FULL_PATH_LEN);
    if(fpath == NULL)
    {
        __msg("malloc fpath failed!\n");
        return EPDK_FAIL;
    }
    else
    {
        eLIBs_memset(fpath, 0, CAT_MAX_FULL_PATH_LEN);
    }
    
    eLIBs_GetFSCharset(param->path, (__s32 *)&font_code);
  // param->scan_type=CAT_SCAN_MEIDA_ALL;  
    while(((dirent = eLIBs_readdir(dir)) != NULL) && (engine->status != ENGINE_STOPPED))
    {        
        if(dirent->fatdirattr & FSYS_ATTR_DIRECTORY) /*is a dir*/
        {
        	continue;
        }
        else /*is a file*/
        {            
            format = cat_get_file_format((char *)dirent->d_name, param->media_type, &media_type);
            //__msg("----format: %d, media_type: %d\n", format, param->media_type);
            if(format != -1)
            {
                need_sync = 1;
            }
            else if(param->media_type == CAT_MEDIA_TYPE_ALL)
            {
                need_sync = 1;
            }
            else
            {
                continue;
            }
        }
         
        if(need_sync == 1)
        {
            has_read++;
            if(has_read <= start_pos)
            {
                continue;
            }
            
            item = esMEMS_Malloc(0, sizeof(cat_item_node_t));
            if(item == NULL)
            {
                __msg("malloc item failed!\n");
                eLIBs_closedir(dir); 
                esMEMS_Mfree(0, fpath);
                return EPDK_FAIL;
            }
            else
            {
                eLIBs_memset(item, 0, sizeof(cat_item_node_t));
            }
            
            
            eLIBs_strcpy(item->cat_item.base_info.fpath, param->path);
            eLIBs_strcpy(item->cat_item.base_info.name, (const char *)dirent->d_name);
            item->cat_item.base_info.char_encoding = (__epdk_charset_enm_e)font_code;
            
            
            eLIBs_memset(fpath, 0, CAT_MAX_FULL_PATH_LEN);
            eLIBs_strcpy(fpath, item->cat_item.base_info.fpath);
            eLIBs_strcat(fpath, "\\");
            eLIBs_strcat(fpath, (const char *)item->cat_item.base_info.name);
            __msg("fpath is %s\n",fpath);
            if(is_dir == 1)
            {
                item->cat_item.base_info.type = CAT_SW_DIR;
                eLIBs_GetDirMTime(fpath, &(item->cat_item.base_info.item_pri_info.dir.m_time));
            }
            else 
            {
                if(format != -1)
                {
                    item->cat_item.base_info.type = CAT_SW_FILE;
                    item->cat_item.base_info.item_pri_info.file.format = format;
                    item->cat_item.base_info.item_pri_info.file.media_type = (cat_media_type_t)media_type;
                }
                else
                {
                    item->cat_item.base_info.type = CAT_SW_EMPTY;
                }
                item->cat_item.base_info.item_pri_info.file.size = eLIBs_GetFileSize(fpath);
                eLIBs_GetFileMTime(fpath, &(item->cat_item.base_info.item_pri_info.file.m_time));
            }
            
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
            
            need_sync = 0;
            is_dir = 0;
            format = -1;
        }
        
        //if(cat_list->nitem % 100 == 0) amend for loading slow
		if(cat_list->nitem % 10 == 0)
        {
            /*添加到数据库*/
            cat_db_add_base_info(engine->db, engine->list_handle);
            cat_destroy_dir_info(cat_list);
            cur_item = NULL;
            //cat_destroy_dir_info(engine->list_handle->bg_list);
            esKRNL_TimeDly(5);
        }
    }
	eLIBs_rewinddir(dir);
	while(((dirent = eLIBs_readdir(dir)) != NULL) && (engine->status != ENGINE_STOPPED))
    {        
        if(dirent->fatdirattr & FSYS_ATTR_DIRECTORY) /*is a dir*/
        {
        	
            if (dirent->d_name[0] == '.')
            {
                continue;
            }
            else if(dir_has_ignored == 0)
            {
                if((eLIBs_strnicmp(param->path, CAT_IGNORE_PATH, CAT_IGNORE_PATH_LEN) == 0) && (eLIBs_strnicmp((char *)dirent->d_name, CAT_IGNORE_DIR, CAT_IGNORE_DIR_LEN) == 0))
                {
                    __msg("----ignore path: %s  dir: %s\n", param->path, (char *)dirent->d_name);
                    dir_has_ignored = 1;
                    continue;
                }
            }
            
            if(param->scan_type == CAT_SCAN_MEIDA_ALL)
            {
                tmp_param = (cat_create_param_t *)esMEMS_Malloc(0, sizeof(cat_create_param_t));
                if(tmp_param == NULL)
                {
                    eLIBs_closedir(dir);
                    esMEMS_Mfree(0, fpath);
                    return EPDK_FAIL;
                }
                else
                {
                    eLIBs_memset(tmp_param, 0, sizeof(cat_create_param_t));
                }
                
                eLIBs_strcpy(tmp_param->path, param->path);
                eLIBs_strcat(tmp_param->path, "\\");
                eLIBs_strcat(tmp_param->path, (const char *)dirent->d_name);
                tmp_param->scan_type = param->scan_type;
                tmp_param->media_type = param->media_type;
                cat_create_dir_info(tmp_param, cat_list, engine, start_pos);
                
                esMEMS_Mfree(0, tmp_param);
            }
            else
			{
                need_sync = 1;
                is_dir = 1;
            }
			
        }
        
     
        if(need_sync == 1)
        {
            has_read++;
            if(has_read <= start_pos)
            {
                continue;
            }
            
            item = esMEMS_Malloc(0, sizeof(cat_item_node_t));
            if(item == NULL)
            {
                __msg("malloc item failed!\n");
                eLIBs_closedir(dir); 
                esMEMS_Mfree(0, fpath);
                return EPDK_FAIL;
            }
            else
            {
                eLIBs_memset(item, 0, sizeof(cat_item_node_t));
            }
            
            
            eLIBs_strcpy(item->cat_item.base_info.fpath, param->path);
            eLIBs_strcpy(item->cat_item.base_info.name, (const char *)dirent->d_name);
            item->cat_item.base_info.char_encoding = (__epdk_charset_enm_e)font_code;
            
            
            eLIBs_memset(fpath, 0, CAT_MAX_FULL_PATH_LEN);
            eLIBs_strcpy(fpath, item->cat_item.base_info.fpath);
            eLIBs_strcat(fpath, "\\");
            eLIBs_strcat(fpath, (const char *)item->cat_item.base_info.name);
            __msg("fpath is %s\n",fpath);
            if(is_dir == 1)
            {
                item->cat_item.base_info.type = CAT_SW_DIR;
                eLIBs_GetDirMTime(fpath, &(item->cat_item.base_info.item_pri_info.dir.m_time));
            }
                     
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
            
            need_sync = 0;
            is_dir = 0;
            format = -1;
        }
        
        if(cat_list->nitem % 100 == 0)
        {
            /*添加到数据库*/
            cat_db_add_base_info(engine->db, engine->list_handle);
            cat_destroy_dir_info(cat_list);
            cur_item = NULL;
            esKRNL_TimeDly(5);
        }
    }
   	eLIBs_closedir(dir);
    esMEMS_Mfree(0, fpath);
    if(engine->status == ENGINE_STOPPED)
    {
        return EPDK_FAIL;
    }
    
    return EPDK_OK;
}
#endif

/*
**********************************************************************************************************************
*                                               cat_destroy_dir_info
*
* author:          terry.zeng 
*
* date:             2009-09-30
*
* Description:       销毁目录浏览信息 
*
* parameters:       cat_list 浏览信息
*
* return:           success 0;	failed:-1
* modify history: 
**********************************************************************************************************************
*/
__s32 cat_destroy_dir_info(cat_list_t *cat_list)
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


