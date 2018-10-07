/*
************************************************************************************************************************
*                                           switch time in register table
*
*                                  Copyright(C), 2006-2010, AllWinner Technology Co., Ltd.
*											       All Rights Reserved
*
* File Name   : reg_switch_time.h
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
#ifndef  __REG_SWITCH_TIME_H__
#define  __REG_SWITCH_TIME_H__

#ifdef __cplusplus
extern "C" {
#endif



/*
****************************************************************************************************
*Name        : lion_reg_switch_time_get
*Prototype   : __s32 lion_reg_switch_time_get( void )
*Arguments   : void
*Return      : the current switch time in register table. unit:s.
*Description : get the current switch time in register table.
*Other       :
****************************************************************************************************
*/
extern __s32  lion_reg_switch_time_get( void );

/*
****************************************************************************************************
*Name        : lion_reg_switch_time_set
*Prototype   : __s32 lion_reg_switch_time_set( __s32 time )
*Arguments   : time         input. switch time, unit:s.
*Return      : EPDK_OK    succeed
*              EPDK_FAIL  fail
*Description : set the current switch time in register table.
*Other       :
****************************************************************************************************
*/
extern __s32 lion_reg_switch_time_set( __s32 time );

/*
****************************************************************************************************
*Name        : lion_reg_switch_time_get_default
*Prototype   : __s32 lion_reg_switch_time_get_default( void )
*Arguments   : void
*Return      : the default switch time in register table. unit:s.
*Description : get the default switch time in register table.
*Other       :
****************************************************************************************************
*/
extern __s32  lion_reg_switch_time_get_default( void );

/*
****************************************************************************************************
*Name        : lion_reg_switch_time_restore
*Prototype   : __s32 lion_reg_switch_time_restore( void )
*Arguments   : void
*Return      : EPDK_OK    succeed
*              EPDK_FAIL  fail
*Description : restore the default value of switch time in register table.
*Other       :
****************************************************************************************************
*/
extern __s32 lion_reg_switch_time_restore( void );



#ifdef __cplusplus
}
#endif

#endif     //  ifndef __REG_SWITCH_TIME_H__

/* end of reg_switch_time.h */
