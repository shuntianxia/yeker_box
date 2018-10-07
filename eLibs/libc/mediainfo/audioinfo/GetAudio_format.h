/******************************************************************************
* file:GetAudio_format.h 2008-12-24 10:47:12
*
*author lszhang
*
*
*
*******************************************************************************/
#ifndef _GETAUFIO_FORMAT_H_
#define _GETAUFIO_FORMAT_H_

#ifdef __cplusplus
extern "C"
{
#endif

typedef enum
{
	IMG_FORMAT_BMP =0,
	IMG_FORMAT_JPG,
	IMG_FORMAT_GIF,
	IMG_FORMAT_PNG,
	IMG_FORMAT_UNSUPPORT = -1
}__w_img_format_t;

typedef enum __A_AUDIO_FONTTYPE
{
	  A_AUDIO_FONTTYPE_ISOIEC8859_1  = 0,       //ISO/IEC 8859-1
	  A_AUDIO_FONTTYPE_UTF_16LE,//
	  A_AUDIO_FONTTYPE_UTF_16BE,
	  A_AUDIO_FONTTYPE_UTF_8,//
	  A_AUDIO_FONTTYPE_ISOIEC8859_2,//
	  A_AUDIO_FONTTYPE_ISOIEC8859_3,//
	  A_AUDIO_FONTTYPE_ISOIEC8859_4,//
	  A_AUDIO_FONTTYPE_ISOIEC8859_5,//
	  A_AUDIO_FONTTYPE_ISOIEC8859_6,
	  A_AUDIO_FONTTYPE_ISOIEC8859_7,
	  A_AUDIO_FONTTYPE_ISOIEC8859_8,
	  A_AUDIO_FONTTYPE_ISOIEC8859_9,
	  A_AUDIO_FONTTYPE_ISOIEC8859_10,
	  A_AUDIO_FONTTYPE_ISOIEC8859_11,
	  A_AUDIO_FONTTYPE_ISOIEC8859_12,
	  A_AUDIO_FONTTYPE_ISOIEC8859_13,
	  A_AUDIO_FONTTYPE_ISOIEC8859_14,
	  A_AUDIO_FONTTYPE_ISOIEC8859_15,
	  A_AUDIO_FONTTYPE_ISOIEC8859_16,
	  A_AUDIO_FONTTYPE_WINDOWS_1250,
	  A_AUDIO_FONTTYPE_WINDOWS_1251,//
	  A_AUDIO_FONTTYPE_WINDOWS_1252,
	  A_AUDIO_FONTTYPE_WINDOWS_1253,
	  A_AUDIO_FONTTYPE_WINDOWS_1254,
	  A_AUDIO_FONTTYPE_WINDOWS_1255,
	  A_AUDIO_FONTTYPE_WINDOWS_1256,
	  A_AUDIO_FONTTYPE_WINDOWS_1257,
	  A_AUDIO_FONTTYPE_WINDOWS_1258,
	  A_AUDIO_FONTTYPE_KOI8_R,
	  A_AUDIO_FONTTYPE_KOI8_U,
	  A_AUDIO_FONTTYPE_GB2312,
	  A_AUDIO_FONTTYPE_GBK,
	  A_AUDIO_FONTTYPE_BIG5,
		
	  
	  A_AUDIO_FONTTYPE_ = -1
}__a_audio_fonttype_e;

typedef struct __ID3_IMAGE_INFO
{
    __s32				length;         //数据长度
    __s32				FileLocation;   //文件偏移位置
    __w_img_format_t	img_format;     //图片格式
    __s32				pic_type;       //picture type;
    __s32				img_high;       //预留，图片高度
    __s32				img_width;      //预留，图片宽度
    __s32				otherdata;      //预留

}__id3_image_info_t;


typedef struct __AUDIO_FILE_INFO
{
    __u32					ulSampleRate;   // 采样率 sample rate
    __u32					ulBitRate;      // 比特率 bit rate单位：BPS
    __u32					ulChannels;     // 声道数 channel
    __u32					ulDuration;     // 播放时间 duration 单位：ms
    __u32					ulBitsSample;   // 采样点位数 sample 8/16/24/32
    __u32					ulCharEncode;   // 0:GB2312.1:UNICODE

    __s32					ulAudio_name_sz;        // 音频格式说明
    __s8					*ulAudio_name;          // mp3 /RealAudio Cook.sipo. / aac-lc....
    __a_audio_fonttype_e	ulAudio_nameCharEncode; // 

    __s32					ulGenre_sz;             // 流派
    __s8					*ulGenre;               // pop soft...
    __a_audio_fonttype_e	ulGenreCharEncode;

    __s32					ultitle_sz;             // 歌曲名
    __s8					*ultitle;
   __a_audio_fonttype_e		ultitleCharEncode;

    __s32					ulauthor_sz;            // 演唱者
    __s8					*ulauthor;
    __a_audio_fonttype_e	ulauthorCharEncode;

    __sw_audio_format_e		ulFormat;
    __s32					ulAlbum_sz;             // 专集
    __s8					*ulAlbum;
    __a_audio_fonttype_e	ulAlbumCharEncode;

    __s32					ulYear_sz;              // 出品年代
    __s8					*ulYear;
    __a_audio_fonttype_e	ulYearCharEncode;

    __s32					ulAPic_sz;             // attached picture
    __id3_image_info_t		*ulAPic;
    __a_audio_fonttype_e	ulAPicCharEncode;

    __s32					ulUslt_sz;             // 非同步的歌词/文本 抄本
    __s8*					ulUslt;                // __s32 ulFileLocation；
    __a_audio_fonttype_e	ulUsltCharEncode;

    __s32					ulSylt_sz;             // 同步的歌词/文本
    __s8*					ulSylt;                // __s32 ulFileLocation；
    __a_audio_fonttype_e	ulSyltCharEncode;

    __s32                   ulbufflag;				//0:对文件进行分析，否则对buf里面的数据进行分析
	__s32                   ulbuf_length;			//buf length;
	char*					ulbuf;					//分析buf里面的数据.
//__s32					data[9];
	__s32         data[3];       //for  没有用到，只是为了64位对齐
	__s64       offset;        //audio data 在文件中的偏移量，id3也属于audio data这三个变量增加是为了解码一个文件中某个部分是audio用
	__s64       length;        //audio data lenth；有效audio数据的长度。可以赋值0，意思是整个都是有效数据
	__s64       readlen;       //当前读取多少个有效的audio 数据，相当于文件的偏移位置减去audio data在文件中的偏移量。


    __s8					*ulBSINFO;              // temporary buffer of read data
    __s32					InforBufLeftLength;
    __s8					*InforBuf;              // 用于存储 ulAudio_name  ultitle ulauthor等信息的buffer
    __s32                    stream_num;                       //音轨数量 目前只有midi音乐有效
}__audio_file_info_t;

/***************************************************
!!!!!!!!!!!!1对于android来说已经定义为cdx_read等，所以不能通过文件名调用方式，必须通过文件指针的方式。
***************************************************/
/*
function :GetAudioFormat
说明：根据文件后缀判断文件格式。
参数说明：pFilename ：文件名，是真实的文件名，.后面不能超过4位，否则范围失败。识别大小写。
			A_Audio_Format：返回文件格式
返回值：1 :succed 0 :fail
*/
int GetAudioFormat(const char *pFilename,int *A_Audio_Format);//return 1 :succed 0 :fail
/*
function :GetAudioInfo
说明：通过文件名获得文件信息。
	首先会调用GetAudioFormat，根据文件后缀初步得到文件格式。
	然后根据文件名，打开文件，通过文件指针得到文件信息。
	如果初次得到信息失败，会根据文件内容循环判断文件格式。
	关闭文件指针，并退出
参数说明：pFilename ：文件名，是真实的文件名，.后面不能超过4位，否则范围失败。识别大小写。
			AIF：返回文件信息
返回值：1 :succed 0 :fail
*/
int GetAudioInfo(const char *pFilename,__audio_file_info_t *AIF);  //return 1 :succed 0 :fail
/*
function :GetAudioFormatData
说明：根据文件前几个字节快速判断文件类型。
参数说明：ptr ：文件数据首地址。
			buflen：数据长度。
			A_Audio_Format：返回文件文件格式
返回值：1 :succed 0 :fail
*/
//simple format from first 16bytes
int GetAudioFormatData(unsigned char *ptr,int buflen,int *A_Audio_Format);//return 1 :succed 0 :fail
/*
function :GetAudioDataInfo
说明：通过文件数据获得文件信息。
	本函数第一个参数文件名可以为0。
	首先会调用GetAudioFormat，根据文件后缀初步得到文件格式。文件后缀必须正确。如果没有文件名，知道那种格式，可赋值.***,例如.aac,.mp3等。
	如果不知道格式，赋值文件名为0，会调用GetAudioFormatData根据内容判断文件格式。
	然后根据文件类型，通过给出的buffer和长度，获得文件信息。
	如果初次得到信息失败，直接返回，不会根据文件内容循环判断文件格式。
参数说明：pFilename ：文件名，是真实的文件类型后缀，.后面不能超过4位，否则范围失败。识别大小写。
			AIF：返回文件信息
			buf：数据首地址
			datalen：数据长度。
返回值：1 :succed 0 :fail
*/
//*pFilename must get right audio_format suffix
int GetAudioDataInfo(const char *pFilename,__audio_file_info_t *AIF,__s8* buf,__s32 datalen);  //return 1 :succed 0 :fail
/*
function :GetAudioFileInfo
说明：通过文件指针获得文件信息。
	首先会跳过id3信息，然后调用GetAudioFormatData，根据文件数据前几个字节得到文件格式。
	然后通过文件指针得到文件信息。
	如果初次得到信息失败，会根据文件内容循环判断文件格式。
	！！！！！文件指针位置可能不是文件开始，需要调用完本函数后把文件指针移到文件开始处。
参数说明：Bitstream ：文件指针。
			AIF：返回文件信息
返回值：1 :succed 0 :fail
*/
int GetAudioFileInfo(FILE *Bitstream,__audio_file_info_t *AIF);  //return 1 :succed 0 :fail 

/*
function :GetAudioFileInfoOffset
说明：通过文件指针获得文件信息。增加支持偏移量和长度进行判断，为音频数据只在文件中间某个地方使用。
offset:有效数据在文件中的偏移量，length：有效数据长度，其中id3是有效数据，默认为0，即文件实际长度即为length
如果offset =0,&& length =0;等于GetAudioFileInfo函数。因此此函数可以替代这个函数。
	首先会跳过id3信息，然后调用GetAudioFormatData，根据文件数据前几个字节得到文件格式。
	然后通过文件指针得到文件信息。
	如果初次得到信息失败，会根据文件内容循环判断文件格式。
	！！！！！文件指针位置可能不是文件开始，需要调用完本函数后把文件指针移到文件开始处。
参数说明：Bitstream ：文件指针。
			AIF：返回文件信息
返回值：1 :succed 0 :fail
*/
int GetAudioFileInfoOffset(FILE *Bitstream,int64_t offset,int64_t length,__audio_file_info_t *AIF);  //return 1 :succed 0 :fail 
#define FFMIN(a,b) ((a) > (b) ? (b) : (a))

#ifdef __cplusplus
}
#endif

#endif //_GETAUFIO_FORMAT_H_

