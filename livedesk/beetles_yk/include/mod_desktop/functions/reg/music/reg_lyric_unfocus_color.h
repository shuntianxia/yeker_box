/*
************************************************************************************************************************
*                                            lyric unfocus color in register table
*
*                                  Copyright(C), 2006-2010, AllWinner Technology Co., Ltd.
*											       All Rights Reserved
*
* File Name   : reg_lyric_unfocus_color.h
*
* Author      : Gary.Wang
*
* Version     : 1.0
*
* Date        : 2010.08.31
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
* Gary.Wang      2010.08.31        1.0         build the file
*
************************************************************************************************************************
*/
#ifndef  __REG_LYRIC_UNFOCUS_COLOR_H__
#define  __REG_LYRIC_UNFOCUS_COLOR_H__

#ifdef __cplusplus
extern "C" {
#endif



/*
****************************************************************************************************
*Name        : lion_reg_lyric_unfocus_color_get
*Prototype   : __u32 lion_reg_lyric_unfocus_color_get( void )
*Arguments   : void
*Return      : the current lyric unfocus color in register table. The color follows GUI's color format.
*Description : get the current lyric unfocus color in register table.
*Other       :
****************************************************************************************************
*/
extern __u32  lion_reg_lyric_unfocus_color_get( void );

/*
****************************************************************************************************
*Name        : lion_reg_lyric_unfocus_color_set
*Prototype   : __s32 lion_reg_lyric_unfocus_color_set( __u32 lyric_unfocus_color )
*Arguments   : lyric_unfocus_color  input. lyric unfocus color. The color follows GUI's color format.
*Return      : EPDK_OK    succeed
*              EPDK_FAIL  fail
*Description : set the current lyric unfocus color in register table.
*Other       :
****************************************************************************************************
*/
extern __s32 lion_reg_lyric_unfocus_color_set( __u32 lyric_unfocus_color );

/*
****************************************************************************************************
*Name        : lion_reg_lyric_unfocus_color_get_default
*Prototype   : __u32 lion_reg_lyric_unfocus_color_get_default( void )
*Arguments   : void
*Return      : the default lyric unfocus color in register table. The color follows GUI's color format.
*Description : get the default lyric unfocus color in register table.
*Other       :
****************************************************************************************************
*/
extern __u32  lion_reg_lyric_unfocus_color_get_default( void );

/*
****************************************************************************************************
*Name        : lion_reg_lyric_unfocus_color_restore
*Prototype   : __s32 lion_reg_lyric_unfocus_color_restore( void )
*Arguments   : void
*Return      : EPDK_OK    succeed
*              EPDK_FAIL  fail
*Description : restore the default value of lyric unfocus color in register table.
*Other       :
****************************************************************************************************
*/
extern __s32 lion_reg_lyric_unfocus_color_restore( void );



#ifdef __cplusplus
}
#endif

#endif     //  ifndef __REG_LYRIC_UNFOCUS_COLOR_H__

/* end of reg_lyric_unfocus_color.h */
