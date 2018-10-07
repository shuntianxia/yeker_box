#ifndef    __AUINPA_H_
#define    __AUINPA_H_
#include "osPlatform.h"
#include "sw_audio_format.h"
#ifndef __OS_LINUX
	#ifndef __u32
	typedef unsigned int        __u32;
	#endif

	#ifndef __u16
	typedef unsigned short      __u16;
	#endif

	#ifndef __u8
	typedef unsigned char       __u8;
	#endif

	#ifndef __s32
	typedef signed int          __s32;
	#endif

	#ifndef __s16
	typedef signed short        __s16;
	#endif

	#ifndef __s8
	typedef signed char         __s8;
	#endif

	
#endif
	#ifdef __OS_ANDROID
	#ifndef __bool
	typedef signed char         __bool;
	#endif
	#endif
#ifdef DOS
	#ifndef __bool
	typedef signed char         __bool;
	#endif
#endif

#include "GetAudio_format.h"


/*******
audio format infor parser  1: succeed 0: defeat
********/
int GetInfo_MP3(FILE *bs,__audio_file_info_t *AIF);
int GetInfo_WMA(FILE *bs,__audio_file_info_t *AIF);
int GetInfo_AC3(FILE *bs,__audio_file_info_t *AIF); //return 1 :succed 0 :fail
int GetInfo_FLAC(FILE *bs,__audio_file_info_t *AIF);  //return 1 :succed 0 :fail
int GetInfo_DTS(FILE *bs,__audio_file_info_t *AIF);  //return 1 :succed 0 :fail
int GetInfo_APE(FILE *bs,__audio_file_info_t *AIF);  //return 1 :succed 0 :fail
int GetInfo_RA(FILE *bs,__audio_file_info_t *AIF);  //return 1 :succed 0 :fail
int GetInfo_WAV(FILE *bs,__audio_file_info_t *AIF);  //return 1 :succed 0 :fail
int GetInfo_AAC(FILE *bs,__audio_file_info_t *AIF);  //return 1 :succed 0 :fail
int GetInfo_OGG(FILE *bs,__audio_file_info_t *AIF);  //return 1 :succed 0 :fail
int GetInfo_AMR(FILE *bs,__audio_file_info_t *AIF);  //return 1 :succed 0 :fail
int GetInfo_ATRAC(FILE *bs,__audio_file_info_t *AIF);  //return 1 :succed 0 :fail
int GetInfo_MID(FILE *bs,__audio_file_info_t *AIF);  //return 1 :succed 0 :fail
int GetInfo_CAF(FILE *bs,__audio_file_info_t *AIF);  //return 1 :succed 0 :fail
#endif
