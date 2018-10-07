/*
************************************************************************************************************************
*                                            zoom in register table
*
*                                  Copyright(C), 2006-2010, AllWinner Technology Co., Ltd.
*											       All Rights Reserved
*
* File Name   : reg_zoom.h
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
#ifndef  __REG_ZOOM_H__
#define  __REG_ZOOM_H__

#ifdef __cplusplus
extern "C" {
#endif


/*
****************************************************************************************************
*Name        : lion_reg_zoom_get
*Prototype   : robin_zoom_e lion_reg_zoom_get( void )
*Arguments   : void
*Return      : the current zoom in register table.
*Description : get the current zoom in register table.
*Other       :
****************************************************************************************************
*/
extern robin_zoom_e  lion_reg_zoom_get( void );

/*
****************************************************************************************************
*Name        : lion_reg_zoom_set
*Prototype   : __s32 lion_reg_zoom_set( robin_zoom_e zoom )
*Arguments   : zoom  input. zoom.
*Return      : EPDK_OK    succeed
*              EPDK_FAIL  fail
*Description : set the current zoom in register table.
*Other       :
****************************************************************************************************
*/
extern __s32 lion_reg_zoom_set( robin_zoom_e zoom );

/*
****************************************************************************************************
*Name        : lion_reg_zoom_get_default
*Prototype   : robin_zoom_e lion_reg_zoom_get_default( void )
*Arguments   : void
*Return      : the default zoom in register table.
*Description : get the default zoom in register table.
*Other       :
****************************************************************************************************
*/
extern robin_zoom_e  lion_reg_zoom_get_default( void );

/*
****************************************************************************************************
*Name        : lion_reg_zoom_restore
*Prototype   : __s32 lion_reg_zoom_restore( void )
*Arguments   : void
*Return      : EPDK_OK    succeed
*              EPDK_FAIL  fail
*Description : restore the default value of zoom in register table.
*Other       :
****************************************************************************************************
*/
extern __s32 lion_reg_zoom_restore( void );



#ifdef __cplusplus
}
#endif

#endif     //  ifndef __REG_ZOOM_H__

/* end of reg_zoom.h */
