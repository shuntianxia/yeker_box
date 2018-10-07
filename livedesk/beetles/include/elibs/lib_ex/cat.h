/*
*********************************************************************************************************
*											        ePDK
*						            the Easy Portable/Player Develop Kits
*									           cat  sample
*
*						        (c) Copyright 2006-2007, terry, China
*											All	Rights Reserved
*
* File    : cat.h
* By      : terry.zeng
* Version : V2.00
* Date	  : 2010-09-08
*********************************************************************************************************
*/

#ifndef __CAT_H__
#define __CAT_H__


#define CAT_MAX_FULL_PATH_LEN  1024


typedef unsigned int HEXPLR;
typedef unsigned int HCAT;


#define CAT_USB_DISK    "ExternalDisk1"
#define CAT_TF_CARD     "ExternalDisk0"
#define CAT_MEDIALIB    "media_lib_root"
#define CAT_LOCAL_DISK  "LocalDisk"
#define CAT_CDROM       "ExternalDisk2"


/*!
  * \brief
  *     AUDIO文件格式
*/
typedef enum
{
    CAT_AUDIO_FORMAT_AAC = 0x00,
    CAT_AUDIO_FORMAT_AC3,
    CAT_AUDIO_FORMAT_AMR,
    CAT_AUDIO_FORMAT_APE,
    CAT_AUDIO_FORMAT_ATRC,
    CAT_AUDIO_FORMAT_DTS,
    CAT_AUDIO_FORMAT_FLAC,
    CAT_AUDIO_FORMAT_MID,    
    CAT_AUDIO_FORMAT_MP3, 
    CAT_AUDIO_FORMAT_OGG,   
    CAT_AUDIO_FORMAT_RA,
    CAT_AUDIO_FORMAT_WAV,
    CAT_AUDIO_FORMAT_WMA,    
    CAT_AUDIO_FORMAT_UNKNOWN,
}cat_audio_format_t;


/*!
  * \brief
  *     VIDEO文件格式
*/
typedef enum
{
    CAT_VIDEO_FORMAT_3GP = 0x20,
    CAT_VIDEO_FORMAT_ASF,
    CAT_VIDEO_FORMAT_AVI,
    CAT_VIDEO_FORMAT_DAT,
    CAT_VIDEO_FORMAT_FLV,
    CAT_VIDEO_FORMAT_MKV,
    CAT_VIDEO_FORMAT_MOV,
    CAT_VIDEO_FORMAT_MP4,
    CAT_VIDEO_FORMAT_MPG,
    CAT_VIDEO_FORMAT_PMP,
    CAT_VIDEO_FORMAT_PSR,
    CAT_VIDEO_FORMAT_RM,
    CAT_VIDEO_FORMAT_RMVB,
    CAT_VIDEO_FORMAT_TP,
    CAT_VIDEO_FORMAT_TS,    
    CAT_VIDEO_FORMAT_VOB,    
    CAT_VIDEO_FORMAT_WMV,    
    CAT_VIDEO_FORMAT_UNKNOWN
}cat_video_format_t;


/*!
  * \brief
  *     图片文件格式
*/
typedef enum
{
    CAT_PIC_FORMAT_BMP = 0x40,
    CAT_PIC_FORMAT_GIF,
    CAT_PIC_FORMAT_JPG,
    CAT_PIC_FORMAT_PNG,	
    CAT_PIC_FORMAT_UNKNOWN
}cat_pic_format_t;


/*!
  * \brief
  *     EBOOK文件格式
*/
typedef enum
{
    CAT_EBOOK_FORMAT_TXT = 0x50,
    CAT_EBOOK_FORMAT_C,
    CAT_EBOOK_FORMAT_H,
    CAT_EBOOK_FORMAT_INI,
    CAT_EBOOK_FORMAT_LRC,
    CAT_EBOOK_FORMAT_SRT,
    CAT_EBOOK_FORMAT_PDF,
    CAT_EBOOK_FORMAT_EPUB,
    CAT_EBOOK_FORMAT_FB2,
    CAT_EBOOK_FORMAT_PDB,
    CAT_EBOOK_FORMAT_RTF,
    CAT_EBOOK_FORMAT_HTML,
    CAT_EBOOK_FORMAT_CHM,
    CAT_EBOOK_FORMAT_HTM,
    
    CAT_EBOOK_FORMAT_UNKNOWN
}cat_ebook_format_t;


/*!
  * \brief
  *     GAME文件格式
*/
typedef enum
{
    CAT_GAME_FORMAT_EPOS = 0x70,
    CAT_GAME_FORMAT_UNKNOWN
}cat_game_format_t;


typedef enum
{
    CAT_FLASH_FORMAT_SWF = 0x80,
    CAT_FLASH_FORMAT_UNKNOWN
}cat_flash_format_t;


typedef enum
{
    CAT_FIRMWARE_FORMAT_IMG = 0x90,
    CAT_FIRMWARE_FORMAT_UNKNOWN	
}cat_firmware_format_t;


/*!
  * \brief
  *     浏览扫描的类型
*/
typedef enum
{
    CAT_SCAN_ALL_DEVICE,    //根扫描，扫描结果有CAT_USB_DISK，CAT_TF_CARD，CAT_MEDIALIB，CAT_LOCAL_DISK,CAT_CDROM
    CAT_SCAN_ALL_PATI,      //根扫描，但以分区为根，即返回CAT_MEDIALIB，e,f...等等
    CAT_SCAN_PARTI,         //分区扫描，只扫描某个设备的分区
    CAT_SCAN_DIR,           //普通目录扫描，只扫描当前路径，不递归
    CAT_SCAN_MEIDA_ALL,     //目录全扫描（指定路径下所有文件扫描，包括子目录下的文件）
}cat_scan_type_t;


/*!
  * \brief
  *     浏览的媒体类型
*/
typedef enum
{
    CAT_MEDIA_TYPE_ALL,         //所有媒体类型
    CAT_MEDIA_TYPE_PIC,         //所有图片文件
    CAT_MEDIA_TYPE_AUDIO,       //所有音频文件
    CAT_MEDIA_TYPE_VIDEO,       //所有视频文件
    CAT_MEDIA_TYPE_EBOOK,       //所有电子书文件
    CAT_MEDIA_TYPE_GAME,        //所有游戏文件
    CAT_MEDIA_TYPE_FLASH,       //所有SWF文件
    CAT_MEDIA_TYPE_FIRMWARE,    //所有固件文件
    CAT_MEDIA_TYPE_UNKNOWN,     //所有系统不支持的文件
}cat_media_type_t;


/*!
  * \brief
  *     浏览的结果中每一项的类型（文件或目录）
*/
typedef enum
{
    CAT_SW_FILE = 0x01,     //文件
    CAT_SW_DIR,             //目录
    CAT_SW_PARTI,           //分区
    CAT_SW_DEVICE,          //根
    CAT_SW_EMPTY            //无法识别
}cat_item_type_t;



/*!
  * \brief
  *     排序的方式
*/
typedef enum
{
    CAT_UNSORT,     //无序
    CAT_ASCEND,     //升序
    CAT_DESCEND     //降序
}cat_sort_mode_t;


/*!
  * \brief
  *     排序的类型
*/
typedef enum
{
    CAT_SORT_NAME,      //针对名字进行排序
    CAT_SORT_TIME,      //针对时间进行排序
    CAT_SORT_SIZE,      //针对大小进行排序
    CAT_SORT_FORMAT     //针对格式进行排序
}cat_sort_type_t;


/*!
  * \brief
  *     排序参数
*/
typedef struct
{
    cat_sort_mode_t     mode;       //排序方式
    cat_sort_type_t     type;       //排序的类型
}cat_sort_para_t;


/*!
  * \brief
  *     缩略图输入参数
*/
typedef struct
{
    char file[CAT_MAX_FULL_PATH_LEN];   //文件全路径
    __u8 format;                        //数据格式
    __u32 width;                        //宽度
    __u32 height;                       //高度
    __u32 mode;                         //缩放模式，0非拉伸，1拉伸
}cat_miniature_para_t;


/*!
  * \brief
  *     返回的缩略图数据参数
*/
typedef struct
{
    __u32 len;      //数据长度
    char *buf;      //数据
}cat_miniature_t;


/*!
  * \brief
  *     创建浏览数据时的传入参数
*/
typedef struct
{
    char                path[CAT_MAX_FULL_PATH_LEN];    // 待浏览的路径
    cat_scan_type_t     scan_type;                      //浏览扫描的类型
    cat_media_type_t    media_type;                     //浏览的媒体类型
}cat_create_param_t;


/*!
  * \brief
  *     创建浏览信息后的cat状态
*/
typedef enum
{
    CAT_CREATING = 0x01,    //创建中
    CAT_CREATE_OVER,        //创建完毕
    CAT_CREATE_UNKONWN,
}cat_create_status_t;


/*!
  * \brief
  *     分区对应的设备
*/
typedef enum
{
    CAT_UNKNOWN,        //未知
    CAT_LOCAL,          //本地磁盘
    CAT_TF,             //TF卡
    CAT_USB,            //USB设备
    CAT_CD,             //CD-ROM
    CAT_MEDIA           //媒体库
}cat_root_t;


/*!
  * \brief
  *     分区属性
*/
typedef enum
{
    CAT_READONLY = 0x01,    //只读
    CAT_READWRITE           //可读写
}cat_pati_attr_t;


/*!
  * \brief
  *     浏览项的基本信息
*/
typedef struct
{
    char                        fpath[CAT_MAX_FULL_PATH_LEN];           //该item的路径
    cat_item_type_t             type;                                   //该项的类型
    char                        name[CAT_MAX_FULL_PATH_LEN];            //该项的名字
    __epdk_charset_enm_e        char_encoding;                          //编码格式
    union
    {
        struct
        {
            cat_pati_attr_t     pati_attr;                          //分区属性，只读或者可读写
            cat_root_t          root_type;                          //根类型，本地盘或者TF卡或者其他
        }pati;
        
        struct
        {
            ES_TIMESTAMP        m_time;                             //创建时间
        }dir;
        
        struct
        {
            __s64               size;
            ES_TIMESTAMP        m_time;                             //创建时间
            cat_media_type_t    media_type;                         //媒体类型，音乐、视频、图片等等
            __u8                format;                             //格式，MP3\WAV等
        }file;
    }item_pri_info;    
}cat_item_base_info_t;


/*!
  * \brief
  *     浏览项的额外信息
*/
typedef struct
{
    union
    {
        struct
        {
            __s64   size;
            __s64   space_left;     //分区的剩余空间
        }pati;
        
        struct
        {
            __s64   size;
            __u32   item_cnt;       //包含的相应媒体类型的子项个数
        }dir;
        
        struct
        {
            cat_miniature_t miniature;  //缩略图信息
            union
            {
                struct
                {
                    __u32   bitrate;        //比特率
                    __u32   sample_rate;    //采样率
                    __u32   channel;        //声道数
                    __u32   duration;       //总时间
                    __u32   bits;           //采样精度
                }audio;
                
                struct
                {
                    __u32   width;          //宽
                    __u32   height;         //高
                }pic;
                
                struct
                {
                    __u32               width;      //宽
                    __u32               height;     //高
                    __u32               frame_rate; //帧率
                    __cedar_video_fmt_t video_fmt;  //视频格式
                    __cedar_audio_fmt_t audio_fmt;  //音频格式
                    __u32               duration;   //总时间 
                }video;
                
                struct
                {
                    char    author[64];     //作者
                    char    theme[256];     //主题
                }ebook;
            }file_info;
        }file;
    }extra_info;
}cat_item_extra_info_t;


/*!
  * \brief
  *     创建浏览后，每一条目的信息
*/
typedef struct __cat_item_t
{
    cat_item_base_info_t    base_info;      //该项的基本信息
    cat_item_extra_info_t   extra_info;     //该项的额外信息
}__cat_item_t;


/*!
  * \brief
  *     删除拷贝信息
*/
typedef struct
{
    cat_item_type_t     type;               //条目类型，文件或者目录
    __u64               total_size;         //需要拷贝的总容量;
    __u64               finish_size;        //已拷贝的总容量;
    __u32               total_num;          //目录时，需拷贝/删除的文件总数
    __u32               finish_num;         //目录时，已完成拷贝/删除的文件数    
    __u32               percent;            //已拷贝/删除数据的百分比，公式=已拷贝数据/需拷贝数据总量
}cat_file_op_info_t;


/*!
  * \brief
  *     缩略图需要cache时的参数
*/
typedef struct
{
    __u32 cache_num;    //需要cache的个数
    __u32 buf_size;     //cache时每个buffer的大小
}cat_cache_para_t;


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
HCAT cat_core_open(void);


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
* parameters:		cat_handle:cat句柄；create_para创建列表的参数
*
* return:           if success return explorer info handle
*                   if fail return NULL
* modify history:
**********************************************************************************************************************
*/
HEXPLR cat_create_explr(HCAT cat_handle, cat_create_param_t *create_para);


/*
**********************************************************************************************************************
*                                               cat_get_create_status
*
* author:           terry.zeng
*
* date:             2010-09-08
*
* Description:      获取目录信息创建的状态
*
* parameters:		cat_handle:cat句柄；hexplr：浏览信息句柄
*
* return:           略
* modify history:
**********************************************************************************************************************
*/
cat_create_status_t cat_get_create_status(HCAT cat_handle, HEXPLR hexplr);


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
* parameters:		cat_handle:cat句柄;hexplr:浏览信息句柄；
*
* return:           if success return EPDK_OKt
*                   if fail return EPDK_FAIL
* modify history:
**********************************************************************************************************************
*/
__s32 cat_update_root(HCAT cat_handle, HEXPLR hexplr, __s32 plug_in, char disk);


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
__s32 cat_destroy_explr(HCAT cat_handle, HEXPLR hexplr);


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
__s32 cat_get_explr_num(HCAT cat_handle, HEXPLR hexplr);


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
* parameters:		core:cat句柄;hexplr:浏览信息句柄；id:需要获取的项ID cat_item  获取到的浏览信息，由用户申请相应资源传递给cat
*
* return:           if success return EPDK_OK
*                   if fail return EPDK_FAIL
* modify history:
**********************************************************************************************************************
*/
__s32 cat_get_explr_item(HCAT cat_handle, HEXPLR hexplr, __u32 id, __cat_item_t *cat_item);


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
* parameters:		core:cat句柄;hexplr:浏览信息句柄；sort_para	排序参数
*
* return:           if success return EPDK_OK
*                   if fail return EPDK_FAIL
* modify history:
**********************************************************************************************************************
*/
__s32 cat_sort_explr(HCAT cat_handle, HEXPLR hexplr, cat_sort_para_t sort_para);


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
__s32 cat_get_explr_para(HCAT cat_handle, HEXPLR hexplr, cat_create_param_t *create_para);


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
__s32 cat_open_extra_dec(HCAT cat_handle, cat_media_type_t type);


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
* parameters:		core:cat句柄;
*
* return:           if success return EPDK_OK
*                   if fail return EPDK_FAIL
* modify history:
**********************************************************************************************************************
*/
__s32 cat_close_extra_dec(HCAT cat_handle);


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
__s32 cat_cache_create(HCAT cat_handle, HEXPLR hexplr, cat_cache_para_t cache_para);


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
__s32 cat_cache_destroy(HCAT cat_handle, HEXPLR hexplr);


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
* parameters:		core:cat句柄;hexplr:浏览信息句柄；para：缩略图参数；id：浏览项编号；cat_item:浏览项
*
* return:           if success return EPDK_OK
*                   if fail return EPDK_FAIL
* modify history:
**********************************************************************************************************************
*/
__s32 cat_get_explr_extra(HCAT cat_handle, HEXPLR hexplr, cat_miniature_para_t *para, __u32 id, __cat_item_t *cat_item);


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
* parameters:		core:cat句柄;hexplr:浏览信息句柄；index:浏览项对应的id
*
* return:           if success return EPDK_OK
*                   if fail return EPDK_FAIL
* modify history:
**********************************************************************************************************************
*/
__s32 cat_del_file(HCAT cat_handle, HEXPLR hexplr, __u32 index);


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
__s32 cat_del_dir(HCAT cat_handle, HEXPLR hexplr, __u32 index);


/*
**********************************************************************************************************************
*                                               cat_cancel_del
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
__s32 cat_cancel_del(HCAT cat_handle, HEXPLR hexplr);
 
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
__s32 cat_copy_file(HCAT cat_handle, HEXPLR hexplr, __u32 index);


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
__s32 cat_paste_file(HCAT cat_handle, HEXPLR hexplr, char *src, char *dst);


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
__s32 cat_copy_dir(HCAT cat_handle, HEXPLR hexplr, __u32 index);


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
__s32 cat_paste_dir(HCAT cat_handle, HEXPLR hexplr, char *src, char *dst);


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
__s32 cat_get_file_operation_info(HCAT cat_handle, cat_file_op_info_t *info);


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
__s32 cat_cancel_paste(HCAT cat_handle, HEXPLR hexplr);


/*
**********************************************************************************************************************
*                                               cat_set_file_for_play
*
* author:           terry.zeng
*
* date:             2010-09-08
*
* Description:      设置当前需要播放的文件
*
* parameters:		core:cat句柄;hexplr:浏览信息句柄；cur_id：需要播放的item；media_type：播放的媒体类型，用于后续获取当前播放时使用。
*
* return:           if success return EPDK_OK
*                   if fail return EPDK_FAIL
* modify history:
**********************************************************************************************************************
*/
__s32 cat_set_file_for_play(HCAT cat_handle, HEXPLR hexplr, cat_media_type_t *media_type, __u32 cur_id);


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
__s32 cat_get_cur_playfile(HCAT cat_handle, HEXPLR hexplr, cat_media_type_t media_type, __u32 *cur_id);


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
HEXPLR cat_search(HCAT cat_handle, HEXPLR hexplr, char* keyword);


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
__s32 cat_core_close(HCAT cat_handle);



#endif
