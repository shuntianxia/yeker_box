/*
************************************************************************************************************************
*                                           auto off time in register table
*
*                                  Copyright(C), 2006-2010, AllWinner Technology Co., Ltd.
*											       All Rights Reserved
*
* File Name   : reg_auto_off.h
*
* Author      : Gary.Wang
*
* Version     : 1.0
*
* Date        : 2010.08.28
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
* Gary.Wang      2010.08.28        1.0         build the file
*
************************************************************************************************************************
*/
#ifndef  __REG_AUTO_OFF_H__
#define  __REG_AUTO_OFF_H__

#ifdef __cplusplus
extern "C" {
#endif



/*
****************************************************************************************************
*Name        : lion_reg_auto_off_get
*Prototype   : __u32 lion_reg_auto_off_get( void )
*Arguments   : void
*Return      : the current auto off time in register table. unit: s, 0 means "forbid closing 
*              screen".
*Description : get the current auto off time in register table.
*Other       :
****************************************************************************************************
*/
extern __u32  lion_reg_auto_off_get( void );

/*
****************************************************************************************************
*Name        : lion_reg_auto_off_set
*Prototype   : __s32 lion_reg_auto_off_set( __u32 time )
*Arguments   : time         input. auto off time, unit: s, 0 means "forbid closing screen".
*Return      : EPDK_OK    succeed
*              EPDK_FAIL  fail
*Description : set the current auto off time in register table.
*Other       :
****************************************************************************************************
*/
extern __s32 lion_reg_auto_off_set( __u32 time );

/*
****************************************************************************************************
*Name        : lion_reg_auto_off_get_default
*Prototype   : __u32 lion_reg_auto_off_get_default( void )
*Arguments   : void
*Return      : the default auto off time in register table. unit: s, 0 means "forbid closing 
*              screen".
*Description : get the default auto off time in register table.
*Other       :
****************************************************************************************************
*/
extern __u32  lion_reg_auto_off_get_default( void );

/*
****************************************************************************************************
*Name        : lion_reg_auto_off_restore
*Prototype   : __s32 lion_reg_auto_off_restore( void )
*Arguments   : void
*Return      : EPDK_OK    succeed
*              EPDK_FAIL  fail
*Description : restore the default value of auto off time in register table.
*Other       :
****************************************************************************************************
*/
extern __s32 lion_reg_auto_off_restore( void );



#ifdef __cplusplus
}
#endif

#endif     //  ifndef __REG_AUTO_OFF_H__

/* end of reg_auto_off.h */
