/*
************************************************************************************************************************
*                                            repeat mode in register table
*
*                                  Copyright(C), 2006-2010, AllWinner Technology Co., Ltd.
*											       All Rights Reserved
*
* File Name   : reg_movie_repeat.h
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
#ifndef  __REG_MOVIE_REPEAT_H__
#define  __REG_MOVIE_REPEAT_H__

#ifdef __cplusplus
extern "C" {
#endif



/*
****************************************************************************************************
*Name        : lion_reg_movie_repeat_get
*Prototype   : orc_play_mode_e lion_reg_movie_repeat_get( void )
*Arguments   : void
*Return      : the current repeat in register table.
*Description : get the current repeat in register table.
*Other       :
****************************************************************************************************
*/
extern orc_play_mode_e  lion_reg_movie_repeat_get( void );

/*
****************************************************************************************************
*Name        : lion_reg_movie_repeat_set
*Prototype   : __s32 lion_reg_movie_repeat_set( orc_play_mode_e repeat )
*Arguments   : repeat  input. repeat.
*Return      : EPDK_OK    succeed
*              EPDK_FAIL  fail
*Description : set the current repeat in register table.
*Other       :
****************************************************************************************************
*/
extern __s32 lion_reg_movie_repeat_set( orc_play_mode_e repeat );

/*
****************************************************************************************************
*Name        : lion_reg_movie_repeat_get_default
*Prototype   : orc_play_mode_e lion_reg_movie_repeat_get_default( void )
*Arguments   : void
*Return      : the default repeat in register table.
*Description : get the default repeat in register table.
*Other       :
****************************************************************************************************
*/
extern orc_play_mode_e  lion_reg_movie_repeat_get_default( void );

/*
****************************************************************************************************
*Name        : lion_reg_movie_repeat_restore
*Prototype   : __s32 lion_reg_movie_repeat_restore( void )
*Arguments   : void
*Return      : EPDK_OK    succeed
*              EPDK_FAIL  fail
*Description : restore the default value of repeat in register table.
*Other       :
****************************************************************************************************
*/
extern __s32 lion_reg_movie_repeat_restore( void );



#ifdef __cplusplus
}
#endif

#endif     //  ifndef __REG_MOVIE_REPEAT_H__

/* end of reg_movie_repeat.h */
