/*
************************************************************************************************************************
*                                           subtitle size in register table
*
*                                  Copyright(C), 2006-2010, AllWinner Technology Co., Ltd.
*											       All Rights Reserved
*
* File Name   : reg_bright.h
*
* Author      : Jackie.Chen
*
* Version     : 1.0
*
* Date        : 2010.12.17
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
* Jackie.Chen      2010.12.17        1.0         build the file

************************************************************************************************************************
*/
#ifndef  __REG_MOVIE_BRIGHT_H__
#define  __REG_MOVIE_BRIGHT_H__

#ifdef __cplusplus
extern "C" {
#endif

extern __u32 lion_reg_bright_get(void);


/*
**********************************************************************************************************************
*                                               lion_reg_movie_bright_set
*
* Description:
*
* Arguments  :
*
* Returns    :
*
* Notes      :
*
**********************************************************************************************************************
*/
extern __s32 lion_reg_bright_set(__u32 bright);


/*
**********************************************************************************************************************
*                                               lion_reg_movie_bright_get_default
*
* Description:
*
* Arguments  :
*
* Returns    :
*
* Notes      :
*
**********************************************************************************************************************
*/
extern __u32 lion_reg_bright_get_default(void);


/*
**********************************************************************************************************************
*                                               lion_reg_movie_bright_restore
*
* Description:
*
* Arguments  :
*
* Returns    :
*
* Notes      :
*
**********************************************************************************************************************
*/
extern __s32 lion_reg_bright_restore( void );

#ifdef __cplusplus
}
#endif

#endif     //  ifndef __REG_BRIGHT_H__

/* end of reg_bright.h */
