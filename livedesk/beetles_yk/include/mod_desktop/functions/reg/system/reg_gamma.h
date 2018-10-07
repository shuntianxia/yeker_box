/*
************************************************************************************************************************
*                                           gamma in register table
*
*                                  Copyright(C), 2006-2010, AllWinner Technology Co., Ltd.
*											       All Rights Reserved
*
* File Name   : reg_gamma.h
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
#ifndef  __REG_GAMMA_H__
#define  __REG_GAMMA_H__

#ifdef __cplusplus
extern "C" {
#endif



/*
****************************************************************************************************
*Name        : lion_reg_gamma_get
*Prototype   : __u32 lion_reg_gamma_get( void )
*Arguments   : void
*Return      : the current gamma in register table.
*Description : get the current gamma in register table.
*Other       :
****************************************************************************************************
*/
extern __u32  lion_reg_gamma_get( void );

/*
****************************************************************************************************
*Name        : lion_reg_gamma_set
*Prototype   : __s32 lion_reg_gamma_set( __u32 value )
*Arguments   : value         input. gamma value.
*Return      : EPDK_OK    succeed
*              EPDK_FAIL  fail
*Description : set the current gamma in register table.
*Other       :
****************************************************************************************************
*/
extern __s32 lion_reg_gamma_set( __u32 value );

/*
****************************************************************************************************
*Name        : lion_reg_gamma_get_default
*Prototype   : __u32 lion_reg_gamma_get_default( void )
*Arguments   : void
*Return      : the default gamma in register table.
*Description : get the default gamma in register table.
*Other       :
****************************************************************************************************
*/
extern __u32  lion_reg_gamma_get_default( void );

/*
****************************************************************************************************
*Name        : lion_reg_gamma_restore
*Prototype   : __s32 lion_reg_gamma_restore( void )
*Arguments   : void
*Return      : EPDK_OK    succeed
*              EPDK_FAIL  fail
*Description : restore the default value of gamma in register table.
*Other       :
****************************************************************************************************
*/
extern __s32 lion_reg_gamma_restore( void );



#ifdef __cplusplus
}
#endif

#endif     //  ifndef __REG_GAMMA_H__

/* end of reg_gamma.h */
