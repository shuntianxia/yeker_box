/*
************************************************************************************************************************
*                                            repeat mode in register table
*
*                                  Copyright(C), 2006-2010, AllWinner Technology Co., Ltd.
*											       All Rights Reserved
*
* File Name   : reg_fm_area.h
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
#ifndef  __REG_FM_AREA_H__
#define  __REG_FM_AREA_H__

#ifdef __cplusplus
extern "C" {
#endif


typedef enum tag_lion_fm_area
{
    LION_FM_AREA_JAPAN=0,
    LION_FM_AREA_SCHOOL,
    LION_FM_AREA_US_EUP,
    
    LION_FM_AREA_= -1
}__lion_fm_area_t;


/*
****************************************************************************************************
*Name        : lion_reg_fm_area_get
*Prototype   : __lion_fm_area_t lion_reg_fm_area_get( void )
*Arguments   : void
*Return      : the current repeat in register table.
*Description : get the current repeat in register table.
*Other       :
****************************************************************************************************
*/
extern __lion_fm_area_t  lion_reg_fm_area_get( void );

/*
****************************************************************************************************
*Name        : lion_reg_fm_area_set
*Prototype   : __s32 lion_reg_fm_area_set( __lion_fm_area_t repeat )
*Arguments   : repeat  input. repeat.
*Return      : EPDK_OK    succeed
*              EPDK_FAIL  fail
*Description : set the current repeat in register table.
*Other       :
****************************************************************************************************
*/
extern __s32 lion_reg_fm_area_set( __lion_fm_area_t area );

/*
****************************************************************************************************
*Name        : lion_reg_fm_area_get_default
*Prototype   : __lion_fm_area_t lion_reg_fm_area_get_default( void )
*Arguments   : void
*Return      : the default repeat in register table.
*Description : get the default repeat in register table.
*Other       :
****************************************************************************************************
*/
extern __lion_fm_area_t  lion_reg_fm_area_get_default( void );

/*
****************************************************************************************************
*Name        : lion_reg_fm_area_restore
*Prototype   : __s32 lion_reg_fm_area_restore( void )
*Arguments   : void
*Return      : EPDK_OK    succeed
*              EPDK_FAIL  fail
*Description : restore the default value of repeat in register table.
*Other       :
****************************************************************************************************
*/
extern __s32 lion_reg_fm_area_restore( void );



#ifdef __cplusplus
}
#endif

#endif     //  ifndef __REG_FM_AREA_H__

/* end of reg_fm_area.h */
