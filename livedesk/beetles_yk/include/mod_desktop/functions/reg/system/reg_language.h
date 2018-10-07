/*
************************************************************************************************************************
*                                                language in register table
*
*                                  Copyright(C), 2006-2010, AllWinner Technology Co., Ltd.
*											       All Rights Reserved
*
* File Name   : reg_language.h
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
#ifndef  __REG_LANGUAGE_H__
#define  __REG_LANGUAGE_H__

#ifdef __cplusplus
extern "C" {
#endif



/*
****************************************************************************************************
*Name        : lion_reg_language_get
*Prototype   : __epdk_language_enm_e lion_reg_language_get( void )
*Arguments   : void
*Return      : the current language in register table.
*Description : get the current language in register table.
*Other       :
****************************************************************************************************
*/
extern __epdk_language_enm_e  lion_reg_language_get( void );

/*
****************************************************************************************************
*Name        : lion_reg_language_set
*Prototype   : __s32 lion_reg_language_set( __epdk_language_enm_e language )
*Arguments   : language  input. language.
*Return      : EPDK_OK    succeed
*              EPDK_FAIL  fail
*Description : set the current language in register table.
*Other       :
****************************************************************************************************
*/
extern __s32 lion_reg_language_set( __epdk_language_enm_e language );

/*
****************************************************************************************************
*Name        : lion_reg_language_get_default
*Prototype   : __epdk_language_enm_e lion_reg_language_get_default( void )
*Arguments   : void
*Return      : the default language in register table.
*Description : get the default language in register table.
*Other       :
****************************************************************************************************
*/
extern __epdk_language_enm_e  lion_reg_language_get_default( void );

/*
****************************************************************************************************
*Name        : lion_reg_language_restore
*Prototype   : __s32 lion_reg_language_restore( void )
*Arguments   : void
*Return      : EPDK_OK    succeed
*              EPDK_FAIL  fail
*Description : restore the default value of language in register table.
*Other       :
****************************************************************************************************
*/
extern __s32 lion_reg_language_restore( void );



#ifdef __cplusplus
}
#endif

#endif     //  ifndef __REG_LANGUAGE_H__

/* end of reg_language.h */
