/*
*********************************************************************************************************
*                                                    eMOD
*                                   the Easy Portable/Player Operation System
*                                               mod_mmp sub-system
*
*                                (c) Copyright 2006-2007, lszhang China
*                                               All Rights Reserved
*
* File   : sw_audio_format.h
* Version: V1.0
* By     : lszhang	2008-12-19 10:02:07 
*********************************************************************************************************
*/
#ifndef _SW_AUDIO_FORMAT_H_
#define _SW_AUDIO_FORMAT_H_



typedef enum __AW_AUDIO_FORMAT
{
    SW_AUDIO_FORMAT_UNKNOWN = 0,            /* 无法识别 */
    
    SW_AUDIO_FORMAT_AAC,
    SW_AUDIO_FORMAT_AC3,
    SW_AUDIO_FORMAT_APE,
    SW_AUDIO_FORMAT_DTS,
    SW_AUDIO_FORMAT_FLAC,
    SW_AUDIO_FORMAT_MP3,
    SW_AUDIO_FORMAT_OGG,
    SW_AUDIO_FORMAT_RA,
    SW_AUDIO_FORMAT_WAV,    
    SW_AUDIO_FORMAT_WMA,
    SW_AUDIO_FORMAT_AMR,
    SW_AUDIO_FORMAT_ATRC,
    SW_AUDIO_FORMAT_MID,	
    SW_AUDIO_FORMAT_CAF,
        
    SW_AUDIO_FORMAT_ = -1

} __sw_audio_format_e;





#endif  /* _SW_AUDIO_FORMAT_H_ */


