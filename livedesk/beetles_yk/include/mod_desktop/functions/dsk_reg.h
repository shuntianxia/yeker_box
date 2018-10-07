/*
************************************************************************************************************************
*                                             information in register table
*
*                                  Copyright(C), 2006-2010, AllWinner Technology Co., Ltd.
*											       All Rights Reserved
*
* File Name   : dsk_reg.h
*
* Author      : Gary.Wang
*
* Version     : 1.0
*
* Date        : 2010.09.01
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
* Gary.Wang      2010.09.01        1.0         build the file
*
************************************************************************************************************************
*/
#ifndef  __DSK_REG_H__
#define  __DSK_REG_H__

#ifdef __cplusplus
extern "C" {
#endif


#include "reg\\system\\reg_audio_output.h"
#include "reg\\system\\reg_auto_off.h"
#include "reg\\system\\reg_channel.h"
#include "reg\\system\\reg_charset.h"
#include "reg\\system\\reg_close_scn.h"
#include "reg\\system\\reg_detect_gate.h"
#include "reg\\system\\reg_display.h"
#include "reg\\system\\reg_gamma.h"
#include "reg\\system\\reg_keytone_gate.h"
#include "reg\\system\\reg_language.h"
#include "reg\\system\\reg_volume.h"
#include "reg\\system\\reg_app_restore.h"
#include "reg\\system\\reg_lcd_bright.h"
#include "reg\\system\\reg_lcd_contrast.h"
#include "reg\\system\\reg_lcd_saturation.h"
#include "reg\\system\\reg_tp_adjust.h"

#include "reg\\movie\\reg_movie_repeat.h"
#include "reg\\movie\\reg_sub_charset.h"
#include "reg\\movie\\reg_sub_color.h"
#include "reg\\movie\\reg_sub_delay.h"
#include "reg\\movie\\reg_sub_frame_color.h"
#include "reg\\movie\\reg_sub_gate.h"
#include "reg\\movie\\reg_sub_size.h"
#include "reg\\movie\\reg_zoom.h"
#include "reg\\movie\\reg_bright.h"
#include "reg\\movie\\reg_contrast.h"
#include "reg\\movie\\reg_saturation.h"
#include "reg\\movie\\reg_color_temperature.h"
#include "reg\\movie\\reg_sub_position.h"

#include "reg\\music\\reg_eq.h"
#include "reg\\music\\reg_lyric_mode.h"
#include "reg\\music\\reg_lyric_focus_color.h"
#include "reg\\music\\reg_lyric_unfocus_color.h"
#include "reg\\music\\reg_music_repeat.h"
#include "reg\\music\\reg_backgrd.h"
#include "reg\\music\\reg_album.h"

#include "reg\\photo\\reg_switch_time.h"
#include "reg\\photo\\reg_switch_mode.h"
#include "reg\\photo\\reg_photo_repeat.h"

#include "reg\\explorer\\reg_exp_sort.h"

#include "reg\\ebook\\reg_ebook_backgrd.h"
#include "reg\\ebook\\reg_ebook_color.h"
#include "reg\\ebook\\reg_ebook_switch_time.h"
#include "reg\\ebook\\reg_ebook_bright.h"

#include "reg\\fm\\reg_fm_area.h"
#include "reg\\fm\\reg_fm_freq.h"
#include "reg\\fm\\reg_fm_send.h"

#ifdef __cplusplus
}
#endif

#endif     //  ifndef __DSK_REG_H__

/* end of dsk_reg.h */
