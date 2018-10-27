/*
**************************************************************************************************************
*											         ePDK
*						            the Easy Portable/Player Develop Kits
*									           desktop system 
*
*						        	 (c) Copyright 2007-2010, ANDY, China
*											 All Rights Reserved
*
* File    	: app_main.c
* By      	: Andy.zhang
* Func		: desk main thread
* Version	: v1.0
* ============================================================================================================
* 2009-7-20 8:51:52  andy.zhang  create this file, implements the fundemental interface;
**************************************************************************************************************
*/

#include "beetles_app.h"
#include "app_home.h"
#include "msgbox.h"
#include "app_home_i.h"

typedef struct tag_home_para
{
	H_WIN	h_app_main;
	H_WIN 	h_mmenu;
	H_WIN	h_calendar;
	H_WIN	h_msgbox;
	H_WIN	lyr_mmenu;

	GUI_FONT *main_font;

	void		*core;
	__u32	root_type; 
}home_para_t;

/***********************************************************************************************************
	建立图层
************************************************************************************************************/
static H_LYR home_8bpp_layer_create(RECT *rect, __s32 pipe)
{
	H_LYR layer = NULL;
	FB  fb =
	{
	    {0, 0},                                   		/* size      */
	    {0, 0, 0},                                      /* buffer    */
	    {FB_TYPE_RGB, {PIXEL_MONO_8BPP, 0, (__rgb_seq_t)0}},    /* fmt       */
	};

	__disp_layer_para_t lstlyr =
	{
	    DISP_LAYER_WORK_MODE_PALETTE,                    /* mode      */
	    0,                                              /* ck_mode   */
	    0,                                              /* alpha_en  */
	    0,                                              /* alpha_val */
	    1,                                              /* pipe      */
	    0xff,                                           /* prio      */
	    {0, 0, 0, 0},                           		/* screen    */
	    {0, 0, 0, 0},                               	/* source    */
	    DISP_LAYER_OUTPUT_CHN_DE_CH1,                   /* channel   */
	    NULL                                            /* fb        */
	};

	__layerwincreate_para_t lyrcreate_info =
	{
	    "sub menu layer",
	    NULL,
	    GUI_LYRWIN_STA_SUSPEND,
	    GUI_LYRWIN_NORMAL
	};
	
	fb.size.width		= rect->width;
	fb.size.height		= rect->height;	
	
	lstlyr.src_win.x  		= 0;
	lstlyr.src_win.y  		= 0;
	lstlyr.src_win.width 	= rect->width;
	lstlyr.src_win.height 	= rect->height;
	
	lstlyr.scn_win.x		= rect->x;
	lstlyr.scn_win.y		= rect->y;
	lstlyr.scn_win.width  	= rect->width;
	lstlyr.scn_win.height 	= rect->height;
	
	lstlyr.pipe = pipe;
	lstlyr.fb = &fb;
	lyrcreate_info.lyrpara = &lstlyr;
	
	layer = GUI_LyrWinCreate(&lyrcreate_info);
	if( !layer )
	{
		__err("app bar layer create error !\n");
	} 
		
	return layer;	
}

//应用退出到桌面时设置回当前背景
void __app_home_change_to_home_bg(void)
{
    reg_init_para_t* para;
    para = dsk_reg_get_para_by_app(REG_APP_INIT);
    if(para)
    {
        __s32 ret;

        ret = gscene_bgd_get_flag();
        if(ret != para->bgd_flag_in_main)
        {
            gscene_bgd_set_flag(para->bgd_flag_in_main);
            gscene_bgd_refresh();                                                      
        }   
        else
        {
            __msg("ret == para->bgd_flag_in_main, not need change bg...\n");
        }
    }
    else
    {
        __msg("dsk_reg_get_para_by_app fail...\n");
    }

    gscene_bgd_set_state(BGD_STATUS_SHOW);
    
}

void  main_cmd2parent(H_WIN hwin, __s32 id, __s32 data1, __s32 data2)
{
	__gui_msg_t msg;

	msg.h_deswin = GUI_WinGetParent(hwin);
	msg.h_srcwin = NULL;
	msg.id = GUI_MSG_COMMAND;
	msg.dwAddData1 = MAKELONG(GUI_WinGetItemId(hwin), id);
	msg.dwAddData2 = data1;
	msg.dwReserved = data2;

	GUI_SendNotifyMessage(&msg);
}

static void check_disk(home_para_t *home_para)
{
    __s32 i;
    __s32 ret;
    __s32 cnt;
    char diskname[RAT_MAX_PARTITION][4];

	if(0)
    {
        __target_para_t target;
        esKSRV_GetTargetPara(&target);
        if(target.debugenable == EPDK_TRUE)////如果是调试，则默认为有外设//112350
        {
            home_para->root_type = RAT_TF<<8;
            return ;        
        }
    }   

	__msg("**********check_disk**********\n");
    cnt = 0;
    home_para->root_type = RAT_UNKNOWN;    

    ret = rat_get_partition_name(RAT_USB_DISK, diskname);
    if(EPDK_OK == ret)
    {
    	i = 0;    
    	//while (i < RAT_MAX_PARTITION)
    	{
    		if (diskname[i][0])
    		{
    			__msg("SD : diskname[%d]=%s\n", i, diskname[i]);
                home_para->root_type += RAT_USB;
                cnt++;
    		}						
    		i++;
    	}
    }

    ret = rat_get_partition_name(RAT_SD_CARD, diskname);
    if(EPDK_OK == ret)
    {
    	i = 0;    
    	//while (i < RAT_MAX_PARTITION)
    	{
    		if (diskname[i][0])
    		{
    			__msg("SD : diskname[%d]=%s\n", i, diskname[i]);
                home_para->root_type += RAT_TF<<8;
                cnt++;
    		}						
    		i++;
    	}
    }
    
}

static __s32 app_home_proc(__gui_msg_t *msg)
{
    __msg("app_home_proc msg->id=%d, msg->h_deswin=%x, msg->dwAddData1=%d, msg->dwAddData2=%d\n"
        ,msg->id, msg->h_deswin, msg->dwAddData1, msg->dwAddData2);
	switch(msg->id)
	{
	case GUI_MSG_CREATE:
		{
			home_para_t *home_para;
			root_para_t *root_para;					

			root_para = (root_para_t *)GUI_WinGetAttr(msg->h_deswin);
			home_para = (home_para_t *)esMEMS_Balloc(sizeof(home_para_t));
            if(!home_para)
            {
                return EPDK_FAIL;
            }
			eLIBs_memset((void *)home_para, 0, sizeof(home_para_t));
			home_para->main_font = root_para->font;
			home_para->h_app_main = msg->h_deswin;
		
            //background
            gscene_bgd_set_state(BGD_STATUS_SHOW);           
            
			//主菜单
			//app_main_menu_create(home_para);

			//建立一个子菜单
			//check_disk(home_para);
            
			//GUI_LyrWinSetSta(home_para->lyr_mmenu, GUI_LYRWIN_STA_ON);

            gscene_hbar_set_state(HBAR_ST_SHOW);

            gscene_bgd_set_bottom();
            //GUI_WinSetFocusChild(home_para->h_mmenu);
			GUI_WinSetAddData(msg->h_deswin, (__u32)home_para);                                 
            __here__;         
		}
		return EPDK_OK;
	case GUI_MSG_CLOSE:
		GUI_ManWinDelete(msg->h_deswin);
		return EPDK_OK;
	case GUI_MSG_DESTROY:
		{
			home_para_t *home_para;

			home_para = (home_para_t *)GUI_WinGetAddData(msg->h_deswin);
			
			//GUI_LyrWinDelete(home_para->lyr_mmenu);

			//home_para->lyr_mmenu = 0;
            esMEMS_Bfree(home_para, sizeof(home_para_t));
		}
		return EPDK_OK;
	case GUI_MSG_KEY:
		__msg("******app_home_proc GUI_MSG_KEY*****\n");
		break;
	case GUI_MSG_TOUCH:
		break;
	case DSK_MSG_FS_PART_PLUGIN:
	case DSK_MSG_FS_PART_PLUGOUT:
		return EPDK_OK;
	case DSK_APP_RESTORE:
		{
			home_para_t *home_para;

			home_para = (home_para_t *)GUI_WinGetAddData(msg->h_deswin);

            __app_home_change_to_home_bg();
                       
			//GUI_LyrWinSetSta(home_para->lyr_mmenu, GUI_LYRWIN_STA_ON);
            gscene_hbar_set_state(HBAR_ST_SHOW);
            gscene_bgd_set_bottom();
            gscene_hbar_set_state(HBAR_ST_SHOW);
//            GUI_LyrWinSetSta(home_para->lyr_total_bg, GUI_LYRWIN_STA_ON);
//            GUI_LyrWinSetBottom(home_para->lyr_total_bg);

//            GUI_BMP_Draw(dsk_theme_hdl2buf(home_para->total_bg_hdl)
//               , main_uipara.lyr_total_bg.x, main_uipara.lyr_total_bg.y);

            GUI_WinSetFocusChild(home_para->h_mmenu);
		}
		return EPDK_OK;
	case GUI_MSG_COMMAND:
		return EPDK_OK;
	}
	
	return GUI_ManWinDefaultProc(msg);	
}

/*******************************************************************************************************
*********************************************************************************************************/
H_WIN app_home_create(root_para_t *para)
{
	__gui_manwincreate_para_t create_info;

	__inf("****************************************************************************************\n");
	__inf("********  enter home app  **************\n");
	__inf("****************************************************************************************\n");

	//__wait__;
	
	eLIBs_memset(&create_info, 0, sizeof(__gui_manwincreate_para_t));
	
	create_info.name            = APP_HOME;
	create_info.hParent         = para->h_parent;
	create_info.ManWindowProc   = (__pGUI_WIN_CB)esKRNL_GetCallBack((__pCBK_t)app_home_proc); 
	create_info.attr            = (void*)para;
	create_info.id				= APP_HOME_ID;
	create_info.hHosting        = NULL;

	return(GUI_ManWinCreate(&create_info));
}
