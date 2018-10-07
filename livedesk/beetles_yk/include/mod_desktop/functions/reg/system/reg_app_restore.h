/*
************************************************************************************************************************
*                                            app restore flag in register table
*
*                                  Copyright(C), 2006-2010, AllWinner Technology Co., Ltd.
*											       All Rights Reserved
*
* File Name   : reg_app_restore.h
*
* Author      : Jackie.chen
*
* Version     : 1.0
*
* Date        : 2010.11.25
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
* Jackie.chen     2010.11.25        1.0         build the file
*
************************************************************************************************************************
*/

#ifndef  __REG_APP_RESTORE_H__
#define  __REG_APP_RESTORE_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    LION_APP_HOME = 0,
    LION_APP_EXPLORER,
    LION_APP_MOVIE   ,
    LION_APP_MUSIC   ,
    LION_APP_PHOTO   ,
    LION_APP_EBOOK   ,
    LION_APP_DVD     ,
    LION_APP_FM      ,
    LION_APP_GPS     ,
    LION_APP_DV      ,
    LION_APP_RECORDER,
    
    LION_APP_UNKNOWN = -1
}lion_app_e;

__bool					lion_get_app_restore_flag( lion_app_e app );
__s32					lion_set_app_restore_flag( lion_app_e app, __bool flag );
__s32					lion_set_all_app_restore( void );


#ifdef __cplusplus
}
#endif

#endif  
