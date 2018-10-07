/*
************************************************************************************************************************
*                                            switch mode in register table
*
*                                  Copyright(C), 2006-2010, AllWinner Technology Co., Ltd.
*											       All Rights Reserved
*
* File Name   : reg_switch_mode.h
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
#ifndef  __REG_SWITCH_MODE_H__
#define  __REG_SWITCH_MODE_H__

#ifdef __cplusplus
extern "C" {
#endif



/*
****************************************************************************************************
*Name        : lion_reg_switch_mode_get
*Prototype   : __willow_switch_mode_t lion_reg_switch_mode_get( void )
*Arguments   : void
*Return      : the current switch mode in register table.
*Description : get the current switch mode in register table.
*Other       :
****************************************************************************************************
*/
extern __willow_switch_mode_t  lion_reg_switch_mode_get( void );

/*
****************************************************************************************************
*Name        : lion_reg_switch_mode_set
*Prototype   : __s32 lion_reg_switch_mode_set( __willow_switch_mode_t switch_mode )
*Arguments   : switch_mode  input. switch mode.
*Return      : EPDK_OK    succeed
*              EPDK_FAIL  fail
*Description : set the current switch mode in register table.
*Other       :
****************************************************************************************************
*/
extern __s32 lion_reg_switch_mode_set( __willow_switch_mode_t switch_mode );

/*
****************************************************************************************************
*Name        : lion_reg_switch_mode_get_default
*Prototype   : __willow_switch_mode_t lion_reg_switch_mode_get_default( void )
*Arguments   : void
*Return      : the default switch mode in register table.
*Description : get the default switch mode in register table.
*Other       :
****************************************************************************************************
*/
extern __willow_switch_mode_t  lion_reg_switch_mode_get_default( void );

/*
****************************************************************************************************
*Name        : lion_reg_switch_mode_restore
*Prototype   : __s32 lion_reg_switch_mode_restore( void )
*Arguments   : void
*Return      : EPDK_OK    succeed
*              EPDK_FAIL  fail
*Description : restore the default value of switch mode in register table.
*Other       :
****************************************************************************************************
*/
extern __s32 lion_reg_switch_mode_restore( void );



#ifdef __cplusplus
}
#endif

#endif     //  ifndef __REG_SWITCH_MODE_H__

/* end of reg_switch_mode.h */
