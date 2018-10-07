/*
************************************************************************************************************************
*                                                        gate map
*
*                                  Copyright(C), 2006-2010, AllWinner Technology Co., Ltd.
*											       All Rights Reserved
*
* File Name   : gatemap.h
*
* Author      : Gary.Wang
*
* Version     : 1.0
*
* Date        : 2010.10.12
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
* Gary.Wang      2010.10.12        1.0         build the file
*
************************************************************************************************************************
*/
#ifndef  __GATEMAP_H__
#define  __GATEMAP_H__

#ifdef __cplusplus
extern "C" {
#endif


/*
****************************************************************************************************
*Name        : gatemap_name2enm
*Prototype   : __bool  gatemap_name2enm( const char *gatemap_name )
*Arguments   : gatemap_name   input. on or off.
*Return      : the enum value of the gate.
*Description : get the enum value of the gate through its name
*Other       :
****************************************************************************************************
*/
extern __bool  gatemap_name2enm( const char *gatemap_name );

/*
****************************************************************************************************
*Name        : gatemap_enm2name
*Prototype   : __s32  gatemap_enm2name( __bool gatemap_enm, 
*                                              char *gatemap_name, __u32 size )
*Arguments   : gatemap_enm    input. the enum value of a gate
*              gatemap_name   output. a buffert to store the name of gate.
*              size           input. the buffer's size.
*Return      : ==0      succeed
*              !=0      fail
*Description : get the name of the gate through its enum value
*Other       :
****************************************************************************************************
*/
extern __s32  gatemap_enm2name( __bool gatemap_enm, char *gatemap_name, __u32 size );



#ifdef __cplusplus
}
#endif

#endif     //  ifndef __GATEMAP_H__

/* end of gatemap.h */
