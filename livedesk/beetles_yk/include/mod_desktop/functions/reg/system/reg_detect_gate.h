/*
************************************************************************************************************************
*                                            detect gate in register table
*
*                                  Copyright(C), 2006-2010, AllWinner Technology Co., Ltd.
*											       All Rights Reserved
*
* File Name   : reg_detect_gate.h
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
#ifndef  __REG_DETECT_GATE_H__
#define  __REG_DETECT_GATE_H__

#ifdef __cplusplus
extern "C" {
#endif


/*
****************************************************************************************************
*Name        : lion_reg_detect_gate_get
*Prototype   : __bool lion_reg_detect_gate_get( void )
*Arguments   : void
*Return      : the current detect gate in register table.
*Description : get the current detect gate in register table.
*Other       :
****************************************************************************************************
*/
extern __bool  lion_reg_detect_gate_get( void );

/*
****************************************************************************************************
*Name        : lion_reg_detect_gate_set
*Prototype   : __s32 lion_reg_detect_gate_set( __bool detect_gate )
*Arguments   : detect_gate  input. detect gate.
*Return      : EPDK_OK    succeed
*              EPDK_FAIL  fail
*Description : set the current detect gate in register table.
*Other       :
****************************************************************************************************
*/
extern __s32 lion_reg_detect_gate_set( __bool detect_gate );

/*
****************************************************************************************************
*Name        : lion_reg_detect_gate_get_default
*Prototype   : __bool lion_reg_detect_gate_get_default( void )
*Arguments   : void
*Return      : the default detect gate in register table.
*Description : get the default detect gate in register table.
*Other       :
****************************************************************************************************
*/
extern __bool  lion_reg_detect_gate_get_default( void );

/*
****************************************************************************************************
*Name        : lion_reg_detect_gate_restore
*Prototype   : __s32 lion_reg_detect_gate_restore( void )
*Arguments   : void
*Return      : EPDK_OK    succeed
*              EPDK_FAIL  fail
*Description : restore the default value of detect gate in register table.
*Other       :
****************************************************************************************************
*/
extern __s32 lion_reg_detect_gate_restore( void );



#ifdef __cplusplus
}
#endif

#endif     //  ifndef __REG_DETECT_GATE_H__

/* end of reg_detect_gate.h */