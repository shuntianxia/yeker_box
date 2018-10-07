/*
**************************************************************************************************************
*											         ePDK
*						            the Easy Portable/Player Develop Kits
*									           desktop system 
*
*						        	 (c) Copyright 2007-2010, ANDY, China
*											 All Rights Reserved
*
* File    	: app_settings.c
* By      	: Andy.zhang
* Func		: desk main thread
* Version	: v1.0
* ============================================================================================================
* 2009-7-20 8:51:52  andy.zhang  create this file, implements the fundemental interface;
**************************************************************************************************************
*/

#ifndef __APP_SETTINGS_H__
#define  __APP_SETTINGS_H__
#include "app_setting_i.h"

typedef enum {
	ITEM_MSG_CODE_RETURN,
	ITEM_MSG_CODE_OK,
	ITEM_MSG_SELECTED,
	ITEM_MSG_CLICKED,
	ITEM_MSG_VALUE_UPDATE,
	ITEM_MSG_TIME_EXHAUSTED,
}item_msg_t;

typedef enum setting_win_id{
	SETTING_WIN_ID_MAIN,
	SETTING_WIN_ID_LOCK,
	SETTING_WIN_ID_RESET_PASSWD,
	SETTING_WIN_ID_TIMER,
	SETTING_WIN_ID_SCREEN_SAVER,
	SETTING_WIN_ID_SEASON,
	SETTING_WIN_ID_BASIC,
	SETTING_WIN_ID_NETWORK,
	SETTING_WIN_ID_UPGRADE,
	SETTING_WIN_ID_LANGUAGE,
}setting_win_id_t;

typedef enum
{
	SETTING_GENERAL = 0,
	SETTING_POWER_MANAGE,
	SETTING_TIPS,	
}setting_root_type_t;
typedef struct tag_setting_para
{
	GUI_FONT *setting_font;

	H_LYR layer;
	setting_root_type_t root_type;
	__s32 focus_id;
	__s32 old_focus;
	
}setting_para_t;

typedef struct tag_setting_ctrl
{
	H_WIN h_setting_man;
	H_WIN h_setting_main;
	H_WIN h_frm_general;		//通用设置窗口句柄
	H_WIN h_frm_power;		// 电源管理窗口句柄
	H_WIN h_frm_tips;		//产品信息提示框句柄
	H_WIN h_dialoag_win ;		//恢复出厂设置对话框句柄
	H_WIN lyr_setting;

	setting_root_type_t root_type;
	GUI_FONT *setting_font;
	
}setting_ctrl_t;



H_WIN app_setting_create(root_para_t *para);

#endif
