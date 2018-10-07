/*
************************************************************************************************************************
*                                            app restore flag in register table
*
*                                  Copyright(C), 2006-2010, AllWinner Technology Co., Ltd.
*											       All Rights Reserved
*
* File Name   : reg_lcd_bright.h
*
* Author      : Jackie.chen
*
* Version     : 1.0
*
* Date        : 2010.11.25
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
* Jackie.chen     2010.11.25        1.0         build the file
*
************************************************************************************************************************
*/

#ifndef  __REG_BRIGHT_H__
#define  __REG_BRIGHT_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef enum tag_lion_bright
{
    LION_BRIGHT_LEVEL1=0,
    LION_BRIGHT_LEVEL2,
    LION_BRIGHT_LEVEL3,
    LION_BRIGHT_LEVEL4,
    LION_BRIGHT_LEVEL5,
    LION_BRIGHT_LEVEL6,
    LION_BRIGHT_LEVEL7,
    LION_BRIGHT_LEVEL8,
    LION_BRIGHT_LEVEL9,
    LION_BRIGHT_LEVEL10,
    LION_BRIGHT_LEVEL11,
    LION_BRIGHT_LEVEL12,
    LION_BRIGHT_LEVEL13,
    LION_BRIGHT_LEVEL14,
    LION_BRIGHT_LEVEL15,
    LION_BRIGHT_LEVEL16,
    
    LION_BRIGHT_= -1
}__lion_bright_t;

__lion_bright_t lion_reg_lcd_bright_get(void);

__s32 lion_reg_lcd_bright_set(__lion_bright_t bright);

__lion_bright_t lion_reg_lcd_bright_get_default(void);

__s32 lion_reg_lcd_bright_restore( void );

#ifdef __cplusplus
}
#endif

#endif

