/*
*********************************************************************************************************
*											        ePDK
*						            the Easy Portable/Player Develop Kits
*									           home app sample
*
*						        	(c) Copyright 2006-2009, China
*											 All Rights Reserved
*
* File    : dv_uipara.c
* By      : lincoln
* Version : V1.00
*********************************************************************************************************
*/
#include "dv_i.h"
#include "dv_uipara.h"

static dv_uipara_para_t dv_para ;

void Dv_Uipara_Init( void )
{
	ES_FILE* pfile ;
	
	pfile = eLIBs_fopen("b:\\DISP\\DISPLAY", "r+");
	if( !pfile )
	{
		__err("open display fail..........\n");
	}
	dv_para.scn_w = eLIBs_fioctrl( pfile, DISP_CMD_SCN_GET_WIDTH, 0, 0 );
	dv_para.scn_h = eLIBs_fioctrl( pfile, DISP_CMD_SCN_GET_HEIGHT, 0, 0 );
	eLIBs_fclose(pfile);

	// menu bar size =[320,40] , show at left bottom
	dv_para.menu_bar_bg_rect.width = 320 ;
	dv_para.menu_bar_bg_rect.height = 40 ;
	dv_para.menu_bar_bg_rect.x = 0 ;
	dv_para.menu_bar_bg_rect.y = dv_para.scn_h - dv_para.menu_bar_bg_rect.height  ;
	dv_para.menu_bar_bg_handle= dsk_theme_open(ID_DV_MENU_BACK_BMP );
	
	//record setting backgroud 
	dv_para.rec_setting_bk_rect.width = 128 ;
	dv_para.rec_setting_bk_rect.height = 84;  
	dv_para.rec_setting_bk_rect.x = 3 ;
	dv_para.rec_setting_bk_rect.y = dv_para.scn_h - dv_para.menu_bar_bg_rect.height - dv_para.rec_setting_bk_rect.height - 5  ;
	dv_para.rec_setting_bk_handle= dsk_theme_open(ID_DV_REC_SETTING_BK_BMP) ;
	
	//picture setting backgroud 
	dv_para.image_setting_bk_rect.width = 128 ;
	dv_para.image_setting_bk_rect.height = 109;  
	dv_para.image_setting_bk_rect.x = 3 ;
	dv_para.image_setting_bk_rect.y = dv_para.scn_h - dv_para.menu_bar_bg_rect.height - dv_para.image_setting_bk_rect.height - 5  ;
	dv_para.image_setting_bk_handle= dsk_theme_open(ID_DV_IMAGE_SETTING_BK_BMP) ;

	// select  item
	dv_para.select_item_size.width = 124 ;
	dv_para.select_item_size.height = 25 ;
	dv_para.select_focus_item_handle= dsk_theme_open(ID_DV_ITEM_FCS_BMP) ;
	dv_para.select_unfocus_item_handle= dsk_theme_open(ID_DV_ITEM_UFCS_BMP) ;

	// option item
	dv_para.opt_item_size.width = 16 ;
	dv_para.opt_item_size.height = 16 ;
	dv_para.opt_item_handle = dsk_theme_open(ID_DV_OPT_ITEM_BMP) ;

	dv_para.mp_fcs_handle = dsk_theme_open(ID_DV_MP_FCS_BMP);
	dv_para.rec_setting_item_num = 2 ;
	dv_para.image_setting_item_num = 3;

	dv_para.work_mode_icon_pos.x = 10 ;
	dv_para.work_mode_icon_pos.y = 0 ;

	dv_para.quantity_text_pos.x = 75 ;
	dv_para.quantity_text_pos.y = 2 ;	
	dv_para.hbar_back_handle = dsk_theme_open(ID_DV_HBAR_BACK_BMP);
	dv_para.hbar_bg_rect.x = 0 ;
	dv_para.hbar_bg_rect.y = 0 ;
	dv_para.rec_flash1_handle = dsk_theme_open(ID_DV_REC_FLASH1_BMP);
	dv_para.rec_flash2_handle = dsk_theme_open(ID_DV_REC_FLASH2_BMP);
	dv_para.work_rec_handle = dsk_theme_open(ID_DV_WORK_REC_BMP);
	dv_para.work_cam_handle = dsk_theme_open(ID_DV_WORK_CAM_BMP);
	
	dv_para.msg_box_bg = dsk_theme_open(ID_DV_MSG_BACK_BMP);
	dv_para.msg_box_size.width = 220 ;
	dv_para.msg_box_size.height = 120 ;
		
	dv_para.no_sd_id = STRING_NO_SD ;
	dv_para.disk_full_id = STRING_DISK_FULL ;

	dv_para.rec_quantity_strid[0] = STRING_REC_MP1 ;
	dv_para.rec_quantity_strid[1] = STRING_REC_MP1 ;
	dv_para.image_quantity_strid[0] = STRING_CAM_MP1;
	dv_para.image_quantity_strid[1] = STRING_CAM_MP2 ;
	dv_para.image_quantity_strid[2] = STRING_CAM_MP3;

}

void Dv_Uipara_UnInit( void )
{
	if(dv_para.menu_bar_bg_handle)
	{
		dsk_theme_close(dv_para.menu_bar_bg_handle);
		dv_para.menu_bar_bg_handle = NULL ;
	}
	if(dv_para.rec_setting_bk_handle)
	{
		dsk_theme_close(dv_para.rec_setting_bk_handle);
		dv_para.rec_setting_bk_handle = NULL ;
	}
	if(dv_para.image_setting_bk_handle)
	{
		dsk_theme_close(dv_para.image_setting_bk_handle);
		dv_para.image_setting_bk_handle = NULL ;
	}
	if(dv_para.select_focus_item_handle)
	{
		dsk_theme_close(dv_para.select_focus_item_handle);
		dv_para.select_focus_item_handle = NULL ;
	}
	if(dv_para.select_unfocus_item_handle)
	{
		dsk_theme_close(dv_para.select_unfocus_item_handle);
		dv_para.select_unfocus_item_handle = NULL ;
	}
	if(dv_para.opt_item_handle)
	{
		dsk_theme_close(dv_para.opt_item_handle);
		dv_para.opt_item_handle = NULL ;
	}
	if(dv_para.mp_fcs_handle)
	{
		dsk_theme_close(dv_para.mp_fcs_handle);
		dv_para.mp_fcs_handle = NULL ;
	}
	if(dv_para.rec_flash1_handle)
	{
		dsk_theme_close(dv_para.rec_flash1_handle);
		dv_para.rec_flash1_handle = NULL ;
	}
	if(dv_para.rec_flash2_handle)
	{
		dsk_theme_close(dv_para.rec_flash2_handle);
		dv_para.rec_flash2_handle = NULL ;
	}
	if(dv_para.work_rec_handle)
	{
		dsk_theme_close(dv_para.work_rec_handle);
		dv_para.work_rec_handle = NULL ;
	}
	if(dv_para.work_cam_handle)
	{
		dsk_theme_close(dv_para.work_cam_handle);
		dv_para.work_cam_handle = NULL ;
	}
	if(dv_para.msg_box_bg)
	{
		dsk_theme_close(dv_para.msg_box_bg);
		dv_para.msg_box_bg = NULL ;
	}
	if(dv_para.hbar_back_handle)
	{
		dsk_theme_close(dv_para.hbar_back_handle);
		dv_para.hbar_back_handle = NULL ;
	}

}

dv_uipara_para_t * Dv_GetUipara( void )
{
    return &dv_para;
}


