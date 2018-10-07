/*
*******************************************************************************************************************
*                                                         	CAT  Framework
*                                         			the core api of apps support module
*
*                             				 Copyright(C), 2006-2008, Softwinner Microelectronic Co., Ltd.
*											               All Rights Reserved
*
*File Name： 	cat_detail_dec.c
*
*Author： 		Terry Zeng
*
*Version :   	2.0
*
*Date:  		2010-10-11
*
*Description : the api of cat 
*
*Others : 		None at present.
*
* History :
*
* <Author>          <time>         <version>     <description>
*
* Terry.Zeng     2010-10-11         2.0          Create File
*
*******************************************************************************************************************
*/

#include "cat_detail_dec.h"
#include ".\\..\\common\\cat_common.h"

/*
**********************************************************************************************************************
*                                               _convert_2_argb
*
* author:          terry.zeng 
*
* date:             2009-11-15
*
* Description:       数据由bgra格式转化为argb 
*
* parameters:       data：数据  len：数据长度
*
* return:           void
* modify history: 
**********************************************************************************************************************
*/
static void _convert_2_argb(__u32 *data, __u32 len)
{
    __u32 tmp = 0;
    __u32 cnt = 0;
    
    for(cnt = 0; cnt < len; cnt++)
    {
        tmp = *(data + cnt);
        *(data + cnt) = ( ((tmp | 0xff) << 24)
                        | ((tmp & 0xff00) << 8)
                        | ((tmp & 0xff0000) >> 8)
                        | ((tmp & 0xff000000) >> 24) );
    }
}


/*
**********************************************************************************************************************
*                                               wait_willow_ready
*
* author:          terry.zeng 
*
* date:             2009-11-15
*
* Description:       确认willow中间件处于非WILLOW_STATUS_BUSY状态
*
* parameters:      
*
* return:           void
* modify history: 
**********************************************************************************************************************
*/
void wait_willow_ready(__mp *willow_handle)
{
    __u8 status = 0;
    
    if(willow_handle == NULL)
    {
        return;
    }

    /*check willow status*/
    while(1)
    {
        status = esMODS_MIoctrl(willow_handle, WILLOW_CMD_QUERY_STATUS, 0, 0);
        if (status != WILLOW_STATUS_BUSY)
        {
            break;
        }
        esKRNL_TimeDly(1);
    }
}

/*
**********************************************************************************************************************
*                                               wait_willow_finish
*
* author:          terry.zeng 
*
* date:             2009-11-15
*
* Description:       确认willow中间件处于WILLOW_STATUS_FINISH状态
*
* parameters:      
*
* return:           void
* modify history: 
**********************************************************************************************************************
*/
void wait_willow_finish(__mp *willow_handle)
{
    __u8 status = 0;
    
    if(willow_handle == NULL)
    {
        return;
    }

    /*check willow status*/
    while(1)
    {   
        status = esMODS_MIoctrl(willow_handle, WILLOW_CMD_QUERY_STATUS, 0, 0);
        if (status == WILLOW_STATUS_FINISH)
        {
            break;
        }
        esKRNL_TimeDly(1);        
    }
}


/*
**********************************************************************************************************************
*                                               AWILLOW_Open
*
* author:           terry.zeng
*
* date:             2010-10-11
*
* Description:      打开图片解码引擎
*
* parameters:		
*
* return:           if success return willow handle
*                   if fail return NULL
* modify history:
**********************************************************************************************************************
*/
willow_t *AWILLOW_Open(void)
{
    willow_t *willow = NULL;

    willow = (willow_t *)esMEMS_Malloc(0, sizeof(willow_t));
    if(willow == NULL)
    {
        return NULL;
    }
    else
    {
        eLIBs_memset(willow, 0, sizeof(willow_t));
    }
    
    willow->willow_libs = esMODS_MInstall("d:\\mod\\willow\\willow.mod", 0);     
    if(willow->willow_libs == NULL)
    {
        __wrn("install willow fail!\n");
        esMEMS_Mfree(0, willow);
        willow = NULL;
        return NULL;
    }
    
    willow->willow_handle = esMODS_MOpen(willow->willow_libs, 0);    
    if(willow->willow_handle == NULL)
    {
        __wrn("open willow fail!\n");
        esMODS_MUninstall(willow->willow_libs);
        willow->willow_libs = NULL;
        esMEMS_Mfree(0, willow);
        willow = NULL;
        return NULL;
    }
    
    return willow;
}


/*
**********************************************************************************************************************
*                                               AWILLOW_Close
*
* author:          terry.zeng 
*
* date:             2010-10-11
*
* Description:       关闭willow中间件 
*
* parameters:      
*
* return:           success: EPDK_OK;   failed: EPDK_FAIL
* modify history: 
**********************************************************************************************************************
*/
__s32 AWILLOW_Close(willow_t *handle)
{
    willow_t *willow = NULL;
    
    if(handle == NULL)
    {
        return EPDK_FAIL;
    }
    else
    {
        willow = (willow_t *)handle;
    }
    
    wait_willow_ready(willow->willow_handle);
        
    if(willow->willow_handle != NULL)
    {        
        esMODS_MClose(willow->willow_handle);
        willow->willow_handle = NULL;
    }
        
    if(willow->willow_libs != NULL)
    {
        esMODS_MUninstall(willow->willow_libs);
        willow->willow_libs = NULL;
    }
    
    esMEMS_Mfree(0, willow);
    willow = NULL;
    
    return EPDK_OK;
}


/*
**********************************************************************************************************************
*                                               AWILLOW_GetThumbs
*
* author:          terry.zeng 
*
* date:             2009-11-15
*
* Description:       向中间件获取缩略图
*
* parameters:      
*
* return:           success: EPDK_OK;   failed: EPDK_FAIL
* modify history: 
**********************************************************************************************************************
*/
__s32 AWILLOW_GetThumbs(__mp *willow_handle, __willow_get_thumbs_param_t *param, __u32 mode, __willow_img_info_t *img_info)
{
    __s32 result = -1;
        
    if((willow_handle == NULL) || (param == NULL) || (img_info == NULL))
    {
        return EPDK_FAIL;
    }
    
    wait_willow_finish(willow_handle);

    result = esMODS_MIoctrl(willow_handle, WILLOW_CMD_GET_THUMBS, 0, param);
    if(result == -1)
    {
        return EPDK_FAIL;
    }
        
    wait_willow_finish(willow_handle);
    
    if(param->filename)
    {
        if(esMODS_MIoctrl(willow_handle, WILLOW_CMD_CHECK_IMG, 0, param) == EPDK_FAIL)
        {
            return EPDK_FAIL;
        }
        
        wait_willow_ready(willow_handle);
        result = esMODS_MIoctrl(willow_handle, WILLOW_CMD_GET_IMG_INFO, 0, img_info);
        if(result == -1)
        {
            return EPDK_FAIL;
        }
        
        result = esMODS_MIoctrl(willow_handle, WILLOW_CMD_START_DEC, 0, 0);
        if(result == -1)
        {
            return EPDK_FAIL;
        }
        
        result = esMODS_MIoctrl(willow_handle, WILLOW_CMD_START_SHOW_EXT, 0, (void *)mode);
        if(result == -1)
        {
            return EPDK_FAIL;
        }
        wait_willow_finish(willow_handle);
        
        return EPDK_OK;
    }
    else
    {    
        return EPDK_FAIL;
    }
}


/*
**********************************************************************************************************************
*                                               AWILLOW_GetAlbumPic
*
* author:          terry.zeng 
*
* date:             2009-11-15
*
* Description:       向中间件获取缩略图
*
* parameters:      
*
* return:           success: EPDK_OK;   failed: EPDK_FAIL
* modify history: 
**********************************************************************************************************************
*/
__s32 AWILLOW_GetAlbumPic(__mp *willow_handle, __willow_get_albumart_param_t *para, __u32 mode)
{
    __s32                       result = -1;
    __willow_show_file_param_t  check_para;
    
    if((willow_handle == NULL) || (para == NULL))
    {
        return EPDK_FAIL;
    }
    
    eLIBs_memset(&check_para, 0, sizeof(__willow_show_file_param_t));
    
    wait_willow_finish(willow_handle);
    
    result = esMODS_MIoctrl(willow_handle, WILLOW_CMD_SET_ALBUM_ART, 0, para);
    if(result == -1)
    {
        return EPDK_FAIL;
    }
    
    wait_willow_finish(willow_handle);
    
    if(para->thumbs.filename)
    {
        check_para.filename = para->thumbs.filename;
        check_para.img_no = 0;
        result = esMODS_MIoctrl(willow_handle, WILLOW_CMD_CHECK_IMG, 0, &check_para);
        if(result == -1)
        {
            return EPDK_FAIL;
        }
        
        wait_willow_ready(willow_handle);
        result = esMODS_MIoctrl(willow_handle, WILLOW_CMD_START_DEC, 0, 0);
        if(result == -1)
        {
            return EPDK_FAIL;
        }
        
        result = esMODS_MIoctrl(willow_handle, WILLOW_CMD_START_SHOW_EXT, 0, (void *)mode);
        if(result == -1)
        {
            return EPDK_FAIL;
        }
        
        wait_willow_finish(willow_handle);
    
        return EPDK_OK;
    }
    else
    {    
        return EPDK_FAIL;
    }
}


/*
**********************************************************************************************************************
*                                               cat_get_pic_info
*
* author:          terry.zeng 
*
* date:             2010-10-11
*
* Description:       获取缩略图 
*
* parameters:      
*
* return:           success: EPDK_OK;   failed: EPDK_FAIL     
* modify history: 
**********************************************************************************************************************
*/
__s32 cat_get_pic_info(willow_t *willow, cat_miniature_para_t *miniature, cat_pic_info_t *pic_info)
{
    __willow_get_thumbs_param_t     thumbs;
    __willow_img_info_t             img_info;    
    __s32                           result = 0;
    
    if((willow == NULL) || (miniature == NULL) || (pic_info == NULL))
    {
        return EPDK_FAIL;
    }
    
    eLIBs_memset(&thumbs, 0, sizeof(__willow_get_thumbs_param_t)); 
    eLIBs_memset(&img_info, 0, sizeof(__willow_img_info_t)); 
    
    thumbs.filename = miniature->file;
    thumbs.format = miniature->format;
    thumbs.size.width = miniature->width;
    thumbs.size.height = miniature->height;
    thumbs.buf = (__u8 *)pic_info->miniature.buf;
    //__msg("file=%s, format=%d, width=%d, height=%d, buf=%x\n",thumbs.filename, thumbs.format, thumbs.size.width, thumbs.size.height, thumbs.buf);
    result = AWILLOW_GetThumbs(willow->willow_handle, &thumbs, miniature->mode, &img_info);
    if(result != EPDK_OK)
    {
        return EPDK_FAIL;
    }
    
    pic_info->miniature.len = miniature->width * miniature->height * 4;
    _convert_2_argb((__u32 *)pic_info->miniature.buf, miniature->width * miniature->height);
    //__msg("width=%d, height=%d\n",img_info.size.width, img_info.size.height);
    pic_info->width = img_info.size.width;
    pic_info->height = img_info.size.height;
    pic_info->format = (cat_pic_format_t)img_info.format;
    
    return EPDK_OK;
}


/*
**********************************************************************************************************************
*                                               cat_get_album_info
*
* author:          terry.zeng 
*
* date:             2010-10-11
*
* Description:       获取缩略图 
*
* parameters:      
*
* return:           success: EPDK_OK;   failed: EPDK_FAIL
* modify history: 
**********************************************************************************************************************
*/
__s32 cat_get_album_info(willow_t *willow, cat_miniature_para_t *miniature, cat_audio_info_t *audio_info)
{
    __willow_get_albumart_param_t   album_para;
    __audio_file_info_t             file_info;
    __willow_img_info_t             img_info;    
    char                            info_buf[1024];    
    __s32                           result = 0;
    
    if((willow == NULL) || (miniature == NULL) || (audio_info == NULL))
    {
        return EPDK_FAIL;
    }
    
    if((miniature->file == NULL) || (audio_info->album.buf == NULL))
    {
        return EPDK_FAIL;
    }
    
    eLIBs_memset(&album_para, 0, sizeof(__willow_get_albumart_param_t));
    eLIBs_memset(&file_info, 0, sizeof(__audio_file_info_t));
    eLIBs_memset(&img_info, 0, sizeof(__willow_img_info_t));
    eLIBs_memset(info_buf, 0, 1024);
    
    file_info.InforBufLeftLength = 1024;
    file_info.InforBuf = info_buf;
    if(!eLIBs_GetAudioInfo(miniature->file, &file_info)) // fail in getting information
    {
        __wrn("Fail in extracting audio info of file %s.\n", miniature->file);
        return EPDK_FAIL;
    }
    
    audio_info->bitrate = file_info.ulBitRate;
    audio_info->sample_rate = file_info.ulSampleRate;
    audio_info->channel = file_info.ulChannels;
    audio_info->duration = file_info.ulDuration;
    audio_info->bits = file_info.ulBitsSample;
    if(file_info.ulAPic_sz == 0)
    {
        __wrn("no album info in  %s. \n", miniature->file);
        return EPDK_FAIL;
    }
    
    album_para.album_art_info.enable = 1;
    if(file_info.ulAPic->img_format == ID3_IMG_FORMAT_BMP)
    {
        album_para.album_art_info.img_type = IMG_FORMAT_BMP;
    }
    else if(file_info.ulAPic->img_format == ID3_IMG_FORMAT_GIF)
    {
        album_para.album_art_info.img_type = IMG_FORMAT_GIF;
    }
    else if(file_info.ulAPic->img_format == ID3_IMG_FORMAT_PNG)
    {
        album_para.album_art_info.img_type = IMG_FORMAT_PNG;
    }
    else if(file_info.ulAPic->img_format == ID3_IMG_FORMAT_JPG)
    {
        album_para.album_art_info.img_type = IMG_FORMAT_JPG;
    }
    else if(file_info.ulAPic->img_format == ID3_IMG_FORMAT_UNSUPPORT)
    {
        __wrn("image format not support\n");
        return EPDK_FAIL;
    }
    else
    {
        return EPDK_FAIL;
    }
    
    album_para.album_art_info.offset = file_info.ulAPic->FileLocation;
    album_para.album_art_info.length = file_info.ulAPic->length;
    album_para.thumbs.format = miniature->format;
    album_para.thumbs.filename = miniature->file;
    album_para.thumbs.size.width = miniature->width;
    album_para.thumbs.size.height = miniature->height;
    album_para.thumbs.buf = (__u8 *)audio_info->album.buf;
    result = AWILLOW_GetAlbumPic(willow->willow_handle, &album_para, miniature->mode);
    if(result != EPDK_OK)
    {
        return EPDK_FAIL;
    }
    audio_info->album.len = miniature->width * miniature->height * 4;
    _convert_2_argb((__u32 *)audio_info->album.buf, miniature->width * miniature->height);
    
    return EPDK_OK;
}


/*
**********************************************************************************************************************
*                                               cat_get_parti_item_detail
*
* author:          terry.zeng 
*
* date:             2010-10-12
*
* Description:       获取信息项具体信息 
*
* parameters:       __cat_item_t *item
*
* return:           success: EPDK_OK;   failed: EPDK_FAIL
* modify history: 
**********************************************************************************************************************
*/
__s32 cat_get_parti_item_detail(__cat_item_t *item)
{
    if(item == NULL)
    {
        return EPDK_FAIL;
    }
    else
    {
        item->extra_info.extra_info.pati.size = eLIBs_GetVolTSpace(item->base_info.name);
        item->extra_info.extra_info.pati.space_left = eLIBs_GetVolFSpace(item->base_info.name);
        
        return EPDK_OK;
    }
}


/*
**********************************************************************************************************************
*                                               cat_get_dir_item_detail
*
* author:          terry.zeng 
*
* date:             2010-10-12
*
* Description:       获取信息项具体信息 
*
* parameters:       __cat_item_t *item
*
* return:           success: EPDK_OK;   failed: EPDK_FAIL
* modify history: 
**********************************************************************************************************************
*/
__s32 cat_get_dir_item_detail(__cat_item_t *item, cat_media_type_t media_type)
{
    if(item == NULL)
    {
        return EPDK_FAIL;
    }
    else
    {
        char fpath[CAT_MAX_FULL_PATH_LEN];
        
        eLIBs_strcpy(fpath, item->base_info.fpath);
        eLIBs_strcat(fpath, "\\");
        eLIBs_strcat(fpath, (const char *)item->base_info.name);
        item->extra_info.extra_info.dir.item_cnt = cat_get_file_count_in_dir(fpath, (u32)media_type);
        item->extra_info.extra_info.dir.size = 0;//eLIBs_GetDirSize(fpath);
        
        return EPDK_OK;
    }
}


__s32 cat_wait_willow_finish(willow_t *handle)
{
    if(handle == NULL)
    {
        return EPDK_FAIL;
    }
    
    wait_willow_finish(handle->willow_handle);
    
    return EPDK_OK;
}



vdec_t *video_dec_open(void)
{
    vdec_t *vdec = NULL;
    /*
    vdec = (vdec_t *)esMEMS_Malloc(0, sizeof(vdec_t));
    if(vdec == NULL)
    {
        return NULL;
    }
        
    eLIBs_memset(vdec, 0, sizeof(vdec_t));
    vdec->duckweed_libs = esMODS_MInstall("d:\\mod\\duckweed.mod", 0);
    if(vdec->duckweed_libs == NULL)
    {
        __msg("install duckweed fail!\n");
        esMEMS_Mfree(0, vdec);
        vdec = NULL;
        return NULL;
    }
    
    vdec->duckweed_handle = esMODS_MOpen(vdec->duckweed_libs, 0);
    if(vdec->duckweed_handle == NULL)
    {
        __msg("duckweed mod open fail!\n");
        esMODS_MUninstall(vdec->duckweed_libs);
        vdec->duckweed_libs = 0;
        esMEMS_Mfree(0, vdec);
        vdec = NULL;
        return NULL;
    }
    */
    return vdec;
}


__s32 video_dec_close(vdec_t *handle)
{
    vdec_t *vdec = NULL;
    /*
    if(handle == NULL)
    {
        return EPDK_FAIL;
    }
    else
    {
        vdec = (vdec_t *) handle;
    }
    
    if(vdec->duckweed_handle != NULL)
    {
        esMODS_MClose(vdec->duckweed_handle);
        vdec->duckweed_handle = NULL;
    }
    
    if(vdec->duckweed_libs != 0)
    {
        esMODS_MUninstall(vdec->duckweed_libs);
        vdec->duckweed_libs = 0;
    }
    
    esMEMS_Mfree(0, vdec);
    vdec = NULL;
    */
    return EPDK_OK;
}

__s32 cat_get_video_info(vdec_t *video_dec, cat_miniature_para_t *para, cat_video_info_t *video_info)
{
#if 0
    __duckweed_media_file_inf       media_file;
    __duckweed_vidstream_profile_t  vstream_attr;
    __duckweed_audstream_profile_t  astream_attr;
    FB                              fb;
    __s32                           ret = EPDK_FAIL;
    
    //__msg("----%s\n", __func__);
    
    if((video_dec == NULL) || (para == NULL) || (video_info == NULL))
    {
        __msg("----param is error\n");
        return EPDK_FAIL;
    }    
    
    eLIBs_memset(&media_file, 0, sizeof(__duckweed_media_file_inf));
    eLIBs_strcpy(media_file.file_path, para->file);    
    ret = esMODS_MIoctrl(video_dec->duckweed_handle, DUCKWEED_CMD_OPEN_MEDIAFILE, 0, &media_file);
    //__msg("----ret: %d\n", ret);
    if(ret == EPDK_FAIL)
    {        
        return EPDK_FAIL;
    }
    
    //video_info->format = esMODS_MIoctrl(video_dec->duckweed_handle, DUCKWEED_CMD_GET_FILE_FORMAT, 0, NULL);
    video_info->duration = esMODS_MIoctrl(video_dec->duckweed_handle, DUCKWEED_CMD_GET_TOTAL_TIME, 0, NULL);    
    //__msg("----video_info->format: %d, video_info->duration: %d\n", video_info->format, video_info->duration);
    
    eLIBs_memset(&vstream_attr, 0, sizeof(__duckweed_vidstream_profile_t));
    vstream_attr.nVidStrmMaxCnt = 1;
    vstream_attr.VidStrmArray = esMEMS_Malloc(0, sizeof(__duckweed_video_bs_info_t) * vstream_attr.nVidStrmMaxCnt);    
    eLIBs_memset(vstream_attr.VidStrmArray, 0, sizeof(__duckweed_video_bs_info_t) * vstream_attr.nVidStrmMaxCnt);    
    ret = esMODS_MIoctrl(video_dec->duckweed_handle, DUCKWEED_CMD_GET_VIDSTREAM_PROFILE, 0, &vstream_attr);
    //__msg("----ret: %d\n", ret);
    if(ret == EPDK_FAIL)
    {
        esMEMS_Mfree(0, vstream_attr.VidStrmArray);
        vstream_attr.VidStrmArray = NULL;
        esMODS_MIoctrl(video_dec->duckweed_handle, DUCKWEED_CMD_CLOSE_MEDIAFILE, 0, NULL);
        return EPDK_FAIL;
    }
    else
    {
        video_info->width = vstream_attr.VidStrmArray->width;
        video_info->height = vstream_attr.VidStrmArray->height;
        video_info->frame_rate = vstream_attr.VidStrmArray->frame_rate;
        video_info->video_fmt = (__cedar_video_fmt_t)vstream_attr.VidStrmArray->codec_type;
    }
    
    eLIBs_memset(&astream_attr, 0, sizeof(__duckweed_audstream_profile_t));
    astream_attr.nAudStrmMaxCnt = 1;
    astream_attr.AudStrmArray = esMEMS_Malloc(0, sizeof(__duckweed_audio_bs_info_t) * astream_attr.nAudStrmMaxCnt);
    eLIBs_memset(astream_attr.AudStrmArray, 0, sizeof(__duckweed_audio_bs_info_t) * astream_attr.nAudStrmMaxCnt);
    ret = esMODS_MIoctrl(video_dec->duckweed_handle, DUCKWEED_CMD_GET_AUDSTREAM_PROFILE, 0, &astream_attr);
    //__msg("----ret: %d\n", ret);
    if(ret == EPDK_FAIL)
    {
        esMEMS_Mfree(0, vstream_attr.VidStrmArray);
        vstream_attr.VidStrmArray = NULL;
        esMEMS_Mfree(0, astream_attr.AudStrmArray);
        astream_attr.AudStrmArray = NULL;
        esMODS_MIoctrl(video_dec->duckweed_handle, DUCKWEED_CMD_CLOSE_MEDIAFILE, 0, NULL);
        return EPDK_FAIL;
    }
    else
    {
        video_info->audio_fmt = (__cedar_audio_fmt_t)astream_attr.AudStrmArray->codec_type;
    }
    
    eLIBs_memset(&fb, 0, sizeof(FB));
    fb.size.width = para->width;
    fb.size.height = para->height;
    fb.addr[0] = video_info->miniature.buf;
    fb.addr[1] = NULL;
    fb.addr[2] = NULL;
    fb.fmt.type = FB_TYPE_RGB;
    fb.fmt.cs_mode = BT601;
    fb.fmt.fmt.rgb.pixelfmt = (__pixel_rgbfmt_t)para->format;
    fb.fmt.fmt.rgb.br_swap = 0;
    fb.fmt.fmt.rgb.pixseq = RGB_SEQ_ARGB;
    fb.fmt.fmt.rgb.palette.addr = NULL;
    fb.fmt.fmt.rgb.palette.size = 0;    
    esMODS_MIoctrl(video_dec->duckweed_handle, DUCKWEED_CMD_GET_PREVIEW_FB, 0, &fb);
    video_info->miniature.len = para->width * para->height * 4;
    
    esMEMS_Mfree(0, vstream_attr.VidStrmArray);
    vstream_attr.VidStrmArray = NULL;
    esMEMS_Mfree(0, astream_attr.AudStrmArray);
    astream_attr.AudStrmArray = NULL;
    esMODS_MIoctrl(video_dec->duckweed_handle, DUCKWEED_CMD_CLOSE_MEDIAFILE, 0, NULL);
    
    //__msg("----width: %d, height: %d, video_fmt: %d, audio_fmt: %d, frame_rate: %d\n", video_info->width, video_info->height, video_info->video_fmt, video_info->audio_fmt, video_info->frame_rate);
    return EPDK_OK;
#endif
}


