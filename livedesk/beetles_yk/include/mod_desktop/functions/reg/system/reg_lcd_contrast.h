/*
************************************************************************************************************************
*                                           lcd_contrast in register table
*
*                                  Copyright(C), 2006-2010, AllWinner Technology Co., Ltd.
*											       All Rights Reserved
*
* File Name   : reg_lcd_contrast.h
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
#ifndef  __REG_LCD_CONTRAST_H__
#define  __REG_LCD_CONTRAST_H__

#ifdef __cplusplus
extern "C" {
#endif



/*
****************************************************************************************************
*Name        : lion_reg_lcd_contrast_get
*Prototype   : __u32 lion_reg_lcd_contrast_get( void )
*Arguments   : void
*Return      : the current lcd_contrast in register table.
*Description : get the current lcd_contrast in register table.
*Other       :
****************************************************************************************************
*/
extern __u32  lion_reg_lcd_contrast_get( void );

/*
****************************************************************************************************
*Name        : lion_reg_lcd_contrast_set
*Prototype   : __s32 lion_reg_lcd_contrast_set( __u32 value )
*Arguments   : value         input. lcd_contrast value.
*Return      : EPDK_OK    succeed
*              EPDK_FAIL  fail
*Description : set the current lcd_contrast in register table.
*Other       :
****************************************************************************************************
*/
extern __s32 lion_reg_lcd_contrast_set( __u32 value );

/*
****************************************************************************************************
*Name        : lion_reg_lcd_contrast_get_default
*Prototype   : __u32 lion_reg_lcd_contrast_get_default( void )
*Arguments   : void
*Return      : the default lcd_contrast in register table.
*Description : get the default lcd_contrast in register table.
*Other       :
****************************************************************************************************
*/
extern __u32  lion_reg_lcd_contrast_get_default( void );

/*
****************************************************************************************************
*Name        : lion_reg_lcd_contrast_restore
*Prototype   : __s32 lion_reg_lcd_contrast_restore( void )
*Arguments   : void
*Return      : EPDK_OK    succeed
*              EPDK_FAIL  fail
*Description : restore the default value of lcd_contrast in register table.
*Other       :
****************************************************************************************************
*/
extern __s32 lion_reg_lcd_contrast_restore( void );



#ifdef __cplusplus
}
#endif

#endif     //  ifndef __REG_LCD_CONTRAST_H__

/* end of reg_lcd_contrast.h */
