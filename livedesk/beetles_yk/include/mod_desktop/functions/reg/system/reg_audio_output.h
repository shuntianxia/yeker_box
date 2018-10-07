/*
************************************************************************************************************************
*                                            audio output format in register table
*
*                                  Copyright(C), 2006-2010, AllWinner Technology Co., Ltd.
*											       All Rights Reserved
*
* File Name   : reg_audio_output.h
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
#ifndef  __REG_AUDIO_OUTPUT_H__
#define  __REG_AUDIO_OUTPUT_H__

#ifdef __cplusplus
extern "C" {
#endif


typedef enum tag_LION_AUDIO_OUTPUT
{
	LION_AUDIO_OUTPUT_NORMAL = 0,
	LION_AUDIO_OUTPUT_SPDIF_LPCM,
	LION_AUDIO_OUTPUT_SPDIF_RAWDATA,
	  	
	LION_AUDIO_OUTPUT_UNKNOWN = -1
}__lion_audio_output_type_e;


/*
****************************************************************************************************
*Name        : lion_reg_audio_output_get
*Prototype   : __lion_audio_output_type_e lion_reg_audio_output_get( void )
*Arguments   : void
*Return      : the current audio output in register table.
*Description : get the current audio output in register table.
*Other       :
****************************************************************************************************
*/
extern __lion_audio_output_type_e  lion_reg_audio_output_get( void );

/*
****************************************************************************************************
*Name        : lion_reg_audio_output_set
*Prototype   : __s32 lion_reg_audio_output_set( __lion_audio_output_type_e audio_output )
*Arguments   : audio_output  input. audio output.
*Return      : EPDK_OK    succeed
*              EPDK_FAIL  fail
*Description : set the current audio output in register table.
*Other       :
****************************************************************************************************
*/
extern __s32 lion_reg_audio_output_set( __lion_audio_output_type_e audio_output );

/*
****************************************************************************************************
*Name        : lion_reg_audio_output_get_default
*Prototype   : __lion_audio_output_type_e lion_reg_audio_output_get_default( void )
*Arguments   : void
*Return      : the default audio output in register table.
*Description : get the default audio output in register table.
*Other       :
****************************************************************************************************
*/
extern __lion_audio_output_type_e  lion_reg_audio_output_get_default( void );

/*
****************************************************************************************************
*Name        : lion_reg_audio_output_restore
*Prototype   : __s32 lion_reg_audio_output_restore( void )
*Arguments   : void
*Return      : EPDK_OK    succeed
*              EPDK_FAIL  fail
*Description : restore the default value of audio output in register table.
*Other       :
****************************************************************************************************
*/
extern __s32 lion_reg_audio_output_restore( void );



#ifdef __cplusplus
}
#endif

#endif     //  ifndef __REG_AUDIO_OUTPUT_H__

/* end of reg_audio_output.h */
