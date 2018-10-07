/*
************************************************************************************************************************
*                                            channel in register table
*
*                                  Copyright(C), 2006-2010, AllWinner Technology Co., Ltd.
*											       All Rights Reserved
*
* File Name   : reg_channel.h
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
#ifndef  __REG_CHANNEL_H__
#define  __REG_CHANNEL_H__

#ifdef __cplusplus
extern "C" {
#endif



/*
****************************************************************************************************
*Name        : lion_reg_channel_get
*Prototype   : __audio_dev_chan_mode_t lion_reg_channel_get( void )
*Arguments   : void
*Return      : the current channel in register table.
*Description : get the current channel in register table.
*Other       :
****************************************************************************************************
*/
extern __audio_dev_chan_mode_t  lion_reg_channel_get( void );

/*
****************************************************************************************************
*Name        : lion_reg_channel_set
*Prototype   : __s32 lion_reg_channel_set( __audio_dev_chan_mode_t channel )
*Arguments   : channel  input. channel.
*Return      : EPDK_OK    succeed
*              EPDK_FAIL  fail
*Description : set the current channel in register table.
*Other       :
****************************************************************************************************
*/
extern __s32 lion_reg_channel_set( __audio_dev_chan_mode_t channel );

/*
****************************************************************************************************
*Name        : lion_reg_channel_get_default
*Prototype   : __audio_dev_chan_mode_t lion_reg_channel_get_default( void )
*Arguments   : void
*Return      : the default channel in register table.
*Description : get the default channel in register table.
*Other       :
****************************************************************************************************
*/
extern __audio_dev_chan_mode_t  lion_reg_channel_get_default( void );

/*
****************************************************************************************************
*Name        : lion_reg_channel_restore
*Prototype   : __s32 lion_reg_channel_restore( void )
*Arguments   : void
*Return      : EPDK_OK    succeed
*              EPDK_FAIL  fail
*Description : restore the default value of channel in register table.
*Other       :
****************************************************************************************************
*/
extern __s32 lion_reg_channel_restore( void );



#ifdef __cplusplus
}
#endif

#endif     //  ifndef __REG_CHANNEL_H__

/* end of reg_channel.h */
