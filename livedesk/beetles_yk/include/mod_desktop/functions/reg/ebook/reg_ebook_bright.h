/*
************************************************************************************************************************
*                                           bright in register table
*
*                                  Copyright(C), 2006-2010, AllWinner Technology Co., Ltd.
*											       All Rights Reserved
*
* File Name   : reg_ebook_bright.h
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
#ifndef  __REG_EBOOK_BRIGHT_H__
#define  __REG_EBOOK_BRIGHT_H__

#ifdef __cplusplus
extern "C" {
#endif



/*
****************************************************************************************************
*Name        : lion_reg_ebook_bright_get
*Prototype   : __s32 lion_reg_ebook_bright_get( void )
*Arguments   : void
*Return      : the current bright in register table. unit:s.
*Description : get the current bright in register table.
*Other       :
****************************************************************************************************
*/
extern __lion_bright_t lion_reg_ebook_bright_get( void );

/*
****************************************************************************************************
*Name        : lion_reg_ebook_bright_set
*Prototype   : __s32 lion_reg_ebook_bright_set( __lion_bright_t bright)
*Arguments   : time         input. bright, unit:s.
*Return      : EPDK_OK    succeed
*              EPDK_FAIL  fail
*Description : set the current bright in register table.
*Other       :
****************************************************************************************************
*/
extern __s32 lion_reg_ebook_bright_set( __lion_bright_t bright );

/*
****************************************************************************************************
*Name        : lion_reg_ebook_bright_get_default
*Prototype   : __s32 lion_reg_ebook_bright_get_default( void )
*Arguments   : void
*Return      : the default bright in register table. unit:s.
*Description : get the default bright in register table.
*Other       :
****************************************************************************************************
*/
extern __lion_bright_t lion_reg_ebook_bright_get_default( void );

/*
****************************************************************************************************
*Name        : lion_reg_ebook_bright_restore
*Prototype   : __s32 lion_reg_ebook_bright_restore( void )
*Arguments   : void
*Return      : EPDK_OK    succeed
*              EPDK_FAIL  fail
*Description : restore the default value of bright in register table.
*Other       :
****************************************************************************************************
*/
extern __s32 lion_reg_ebook_bright_restore( void );



#ifdef __cplusplus
}
#endif

#endif     //  ifndef __REG_EBOOK_BRIGHT_H__

/* end of reg_ebook_bright.h */
