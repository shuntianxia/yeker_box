/*
************************************************************************************************************************
*                                                     display output
*
*                                  Copyright(C), 2006-2010, AllWinner Technology Co., Ltd.
*											       All Rights Reserved
*
* File Name   : dsk_display_enhance.h
*
* Author      : Gary.Wang
*
* Version     : 1.0
*
* Date        : 2010.09.02
*
* Description :
* 
* Others      : None at present.
*
*
* History     :
*
*  <Author>        <time>       <version>      <description>
*
* Gary.Wang      2010.09.02        1.0         build the file
*
************************************************************************************************************************
*/
#ifndef  __DSK_DISPLAY_ENHANCE_H__
#define  __DSK_DISPLAY_ENHANCE_H__

#ifdef __cplusplus
extern "C" {
#endif


__s32 dsk_display_enhance_on(void);

__s32 dsk_display_enhance_off(void);

__s32 dsk_display_set_bright(__u32 bright);

__s32 dsk_display_get_bright(void);

__s32 dsk_display_set_contrast(__u32 contrast);

__s32 dsk_display_get_contrast(void);

__s32 dsk_display_set_saturation(__u32 saturation);

__s32 dsk_display_get_saturation(void);



#ifdef __cplusplus
}
#endif

#endif     //  ifndef __DSK_DISPLAY_ENHANCE_H__

/* end of dsk_display_enhance.h */
