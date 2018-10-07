/*
*********************************************************************************************************
*											        ePDK
*						            the Easy Portable/Player Develop Kits
*									           home app sample
*
*						        	(c) Copyright 2006-2009, China
*											 All Rights Reserved
*
* File    : dv_uipara.h
* By      : lincoln
* Version : V1.00
*********************************************************************************************************
*/

#ifndef __DV_FRMWIN_H__
#define __DV_FRMWIN_H__

typedef struct tag_frmwin_para
{
	H_WIN				frmwin_hdl ;
	GUI_FONT 			*font;
	dv_state_e			cur_state;
	__s32				rec_status;
	__krnl_stmr_t  *	os_timer_hdl;
	__u8				delete_file_tsk ;
	__time_t			sys_time;
	__date_t			sys_date;
	__time_t			cur_rec_time;
	__lotus_overlay_dis_par_t   water_mark_para;
	reg_dv_para_t*     user_data;      // 用户存储数据
	__bool				recording_flash_flag ;	// 录像时图标更新
	H_LYR				frm_lyr_hdl ;
	dv_uipara_para_t 	*uipara ;
	H_WIN				h_parent ;
	H_LYR				dialoag_lyr_hdl ;
	__u32				txt_color ;
	
}dv_frmwin_para_t;

typedef enum
{
	CMD_DV_FRM_POP_REC_SETTING_MENU,
	CMD_DV_FRM_POP_IMAGE_SETTING_MENU,
}dv_frm_to_mset_cmd_t;

typedef enum
{
	CMD_DV_FRM_SET_RECORD_RESOLUTION = GUI_MSG_USER_DEF,
	CMD_DV_FRM_SETTING_MENU_EXIT,	
}dv_man_to_frm_cmd_t;

__s32 dv_frm_create(dv_frmwin_para_t *para);
__s32 dv_frm_destroy( H_WIN h_win );


#endif /* __DV_FRMWIN_H__ */
