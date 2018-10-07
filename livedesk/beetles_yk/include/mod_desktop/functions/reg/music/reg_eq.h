/*
************************************************************************************************************************
*                                            eq in register table
*
*                                  Copyright(C), 2006-2010, AllWinner Technology Co., Ltd.
*											       All Rights Reserved
*
* File Name   : reg_eq.h
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
#ifndef  __REG_EQ_H__
#define  __REG_EQ_H__

#ifdef __cplusplus
extern "C" {
#endif


/*
****************************************************************************************************
*Name        : lion_reg_eq_get
*Prototype   : __cedar_audio_eq_t lion_reg_eq_get( void )
*Arguments   : void
*Return      : the current eq in register table.
*Description : get the current eq in register table.
*Other       :
****************************************************************************************************
*/
extern __cedar_audio_eq_t  lion_reg_eq_get( void );

/*
****************************************************************************************************
*Name        : lion_reg_eq_set
*Prototype   : __s32 lion_reg_eq_set( __cedar_audio_eq_t eq )
*Arguments   : eq  input. eq.
*Return      : EPDK_OK    succeed
*              EPDK_FAIL  fail
*Description : set the current eq in register table.
*Other       :
****************************************************************************************************
*/
extern __s32 lion_reg_eq_set( __cedar_audio_eq_t eq );

/*
****************************************************************************************************
*Name        : lion_reg_eq_get_default
*Prototype   : __cedar_audio_eq_t lion_reg_eq_get_default( void )
*Arguments   : void
*Return      : the default eq in register table.
*Description : get the default eq in register table.
*Other       :
****************************************************************************************************
*/
extern __cedar_audio_eq_t  lion_reg_eq_get_default( void );

/*
****************************************************************************************************
*Name        : lion_reg_eq_restore
*Prototype   : __s32 lion_reg_eq_restore( void )
*Arguments   : void
*Return      : EPDK_OK    succeed
*              EPDK_FAIL  fail
*Description : restore the default value of eq in register table.
*Other       :
****************************************************************************************************
*/
extern __s32 lion_reg_eq_restore( void );



#ifdef __cplusplus
}
#endif

#endif     //  ifndef __REG_EQ_H__

/* end of reg_eq.h */
