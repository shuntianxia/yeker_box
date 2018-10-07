/*
*********************************************************************************************************
*											        ePDK
*						            the Easy Portable/Player Develop Kits
*									           cat  sample
*
*						        (c) Copyright 2006-2007, terry, China
*											All	Rights Reserved
*
* File    : cat_cache.h
* By      : terry.zeng
* Version : V2.00
* date    :2010-09-06
*********************************************************************************************************
*/

#include ".\\..\\cat_p.h"


typedef struct
{
    __u8    willow_libs;
    __mp    *willow_handle;
}willow_t;


typedef struct
{
    __u8    duckweed_libs;
    __mp    *duckweed_handle;
}vdec_t;


typedef struct
{
    cat_pic_format_t    format;
    __u32               width;
    __u32               height;
    cat_miniature_t     miniature;
}cat_pic_info_t;


typedef struct
{
    cat_pic_format_t    format;
    __u32               bitrate;
    __u32               sample_rate;
    __u32               channel;
    __u32               duration;
    __u32               bits;
    cat_miniature_t     album;
}cat_audio_info_t;


typedef struct
{
    cat_video_format_t  format;    
    __u32               width;
    __u32               height;
    __u32               frame_rate;
    __cedar_video_fmt_t video_fmt;
    __cedar_audio_fmt_t audio_fmt;
    __u32               duration;
    cat_miniature_t     miniature;
}cat_video_info_t;


typedef struct
{
    cat_ebook_format_t  format;
    char                author[64];
    char                theme[256];
}cat_ebook_info_t;


willow_t *AWILLOW_Open(void); 

__s32 AWILLOW_Close(willow_t *handle);

vdec_t *video_dec_open(void);

__s32 video_dec_close(vdec_t *handle);

__s32 cat_get_pic_info(willow_t *willow, cat_miniature_para_t *para, cat_pic_info_t *pic_info);

__s32 cat_get_album_info(willow_t *willow, cat_miniature_para_t *para, cat_audio_info_t *audio_info);

__s32 cat_get_video_info(vdec_t	*video_dec, cat_miniature_para_t *para, cat_video_info_t *video_info);

__s32 cat_get_ebook_info(char *file_name, cat_ebook_info_t *ebook_info);

__s32 cat_get_parti_item_detail(__cat_item_t *item);

__s32 cat_get_dir_item_detail(__cat_item_t *item, cat_media_type_t media_type);

__s32 cat_get_file_item_detail(__cat_item_t *item);

__s32 cat_wait_willow_finish(willow_t *handle);

