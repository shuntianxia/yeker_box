/*
************************************************************************************************************************
*                                            lyric mode in register table
*
*                                  Copyright(C), 2006-2010, AllWinner Technology Co., Ltd.
*											       All Rights Reserved
*
* File Name   : reg_lyric_mode.h
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
#ifndef  __REG_LYRIC_MODE_H__
#define  __REG_LYRIC_MODE_H__

#ifdef __cplusplus
extern "C" {
#endif



/*
****************************************************************************************************
*Name        : lion_reg_lyric_mode_get
*Prototype   : __txtbox_mode_t lion_reg_lyric_mode_get( void )
*Arguments   : void
*Return      : the current lyric mode in register table.
*Description : get the current lyric mode in register table.
*Other       :
****************************************************************************************************
*/
extern __txtbox_mode_t  lion_reg_lyric_mode_get( void );

/*
****************************************************************************************************
*Name        : lion_reg_lyric_mode_set
*Prototype   : __s32 lion_reg_lyric_mode_set( __txtbox_mode_t lyric_mode )
*Arguments   : lyric_mode  input. lyric mode.
*Return      : EPDK_OK    succeed
*              EPDK_FAIL  fail
*Description : set the current lyric mode in register table.
*Other       :
****************************************************************************************************
*/
extern __s32 lion_reg_lyric_mode_set( __txtbox_mode_t lyric_mode );

/*
****************************************************************************************************
*Name        : lion_reg_lyric_mode_get_default
*Prototype   : __txtbox_mode_t lion_reg_lyric_mode_get_default( void )
*Arguments   : void
*Return      : the default lyric mode in register table.
*Description : get the default lyric mode in register table.
*Other       :
****************************************************************************************************
*/
extern __txtbox_mode_t  lion_reg_lyric_mode_get_default( void );

/*
****************************************************************************************************
*Name        : lion_reg_lyric_mode_restore
*Prototype   : __s32 lion_reg_lyric_mode_restore( void )
*Arguments   : void
*Return      : EPDK_OK    succeed
*              EPDK_FAIL  fail
*Description : restore the default value of lyric mode in register table.
*Other       :
****************************************************************************************************
*/
extern __s32 lion_reg_lyric_mode_restore( void );



#ifdef __cplusplus
}
#endif

#endif     //  ifndef __REG_LYRIC_MODE_H__

/* end of reg_lyric_mode.h */
