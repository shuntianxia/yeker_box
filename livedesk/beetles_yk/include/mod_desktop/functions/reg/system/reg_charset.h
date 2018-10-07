/*
************************************************************************************************************************
*                                                charset in register table
*
*                                  Copyright(C), 2006-2010, AllWinner Technology Co., Ltd.
*											       All Rights Reserved
*
* File Name   : reg_charset.h
*
* Author      : Gary.Wang
*
* Version     : 1.0
*
* Date        : 2010.08.26
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
* Gary.Wang      2010.08.26        1.0         build the file
*
************************************************************************************************************************
*/
#ifndef  __REG_CHARSET_H__
#define  __REG_CHARSET_H__

#ifdef __cplusplus
extern "C" {
#endif



/*
****************************************************************************************************
*Name        : lion_reg_charset_get
*Prototype   : __epdk_charset_enm_e lion_reg_charset_get( void )
*Arguments   : void
*Return      : the current charset in register table.
*Description : get the current charset in register table.
*Other       :
****************************************************************************************************
*/
extern __epdk_charset_enm_e  lion_reg_charset_get( void );

/*
****************************************************************************************************
*Name        : lion_reg_charset_set
*Prototype   : __s32 lion_reg_charset_set( __epdk_charset_enm_e charset )
*Arguments   : charset  input. charset.
*Return      : EPDK_OK    succeed
*              EPDK_FAIL  fail
*Description : set the current charset in register table.
*Other       :
****************************************************************************************************
*/
extern __s32 lion_reg_charset_set( __epdk_charset_enm_e charset );

/*
****************************************************************************************************
*Name        : lion_reg_charset_get_default
*Prototype   : __epdk_charset_enm_e lion_reg_charset_get_default( void )
*Arguments   : void
*Return      : the default charset in register table.
*Description : get the default charset in register table.
*Other       :
****************************************************************************************************
*/
extern __epdk_charset_enm_e  lion_reg_charset_get_default( void );

/*
****************************************************************************************************
*Name        : lion_reg_charset_restore
*Prototype   : __s32 lion_reg_charset_restore( void )
*Arguments   : void
*Return      : EPDK_OK    succeed
*              EPDK_FAIL  fail
*Description : restore the default value of charset in register table.
*Other       :
****************************************************************************************************
*/
extern __s32 lion_reg_charset_restore( void );



#ifdef __cplusplus
}
#endif

#endif     //  ifndef __REG_CHARSET_H__

/* end of reg_charset.h */
