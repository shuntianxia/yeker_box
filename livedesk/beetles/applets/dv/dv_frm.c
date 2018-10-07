/*
*********************************************************************************************************
*											        ePDK
*						            the Easy Portable/Player Develop Kits
*									           home app sample
*
*						        	(c) Copyright 2006-2009, China
*											 All Rights Reserved
*
* File    : home.c
* By      : lyn
* Version : V1.00
*********************************************************************************************************
*/

#include "dv_i.h"
#include "dv_uipara.h"
#include "dv_frm.h"
#include "dv_common.h"

static void  __dv_get_time_mark_index(__date_t date, __time_t time, __u8 * mark, __u8 level)
{

	mark[0]  =   date.year / 1000;
	mark[1]  =  (date.year - mark[0] * 1000) / 100;
	mark[2]  =   (date.year - mark[0] * 1000 - mark[1] * 100) / 10;
	mark[3]  =   date.year - mark[0] * 1000 - mark[1] * 100 - mark[2] * 10;
	mark[4]  =   11;
	mark[5]  =   date.month / 10;
	mark[6]  =   date.month - mark[5] * 10;
	mark[7]  =   11;
	mark[8]  =   date.day / 10;
	mark[9]  =   date.day - mark[8] * 10;	
	mark[10] =   12;
	mark[11] =  time.hour / 10;
	mark[12] =   time.hour - mark[11] * 10;
	mark[13] =   10;	
	mark[14] =   time.minute / 10;
	mark[15] =   time.minute - mark[14] * 10;
	mark[16] =   10;
	mark[17] =   time.second / 10;
	mark[18] =   time.second - mark[17] * 10;	

	if(level == 1)
	{
		__u8 i = 0;
		for(i=0; i<=18; i++)
		{
			mark[i] += 20;
		}
	}
}


static __s32   __dv_get_first_file(char * dir, char * file_name, char * invalid_file)
{
    __s32           cnt = 0;
    char            name[1024];
    char*           tmp_sfx;
    ES_DIR*         tmp_dir;
    ES_DIRENT*      tmp_dirent;
    
	tmp_dir = eLIBs_opendir(dir);
    if(tmp_dir == NULL) 
    {
    	return EPDK_FAIL;
    }
    eLIBs_memset(&name, 0xFF, 1024);

    while(NULL != (tmp_dirent = eLIBs_readdir(tmp_dir))) 
    {
    	if(tmp_dirent->fatdirattr & FSYS_ATTR_DIRECTORY)
    	{   // 非法文件
    		if(tmp_dirent->d_name[0] == '.') 
    		{
    			continue;
    		}
		} 
		else 
		{
            tmp_sfx = eLIBs_strchrlast((char *)tmp_dirent->d_name, '.');
            if(NULL == tmp_sfx) 
            {
            	continue;
            }
            if(invalid_file != NULL)
            {   //　区别当前正在录制文件
                if(0 == eLIBs_strcmp(invalid_file, (char *)tmp_dirent->d_name))
                {
                    continue;
                }
            }
            if(eLIBs_stricmp(tmp_sfx, FILE_NAME_SUFFIX) == 0)
		    {
	            if(0 < eLIBs_strcmp(name, (char *)tmp_dirent->d_name))
	            {
	                cnt++;
	                eLIBs_strcpy(name, (char *)tmp_dirent->d_name);
	            }
		    }
		}
    }
    eLIBs_closedir(tmp_dir);
    
    if(cnt == 0)
    {   
        return EPDK_FAIL;
    }
    eLIBs_strcpy(file_name, dir);  
    eLIBs_strcat(file_name, "\\");   
    eLIBs_strcat(file_name, name);   
    
    return EPDK_OK;
    
}


static void dv_dialog_creat( dv_frmwin_para_t *dv_frm_ctrl, RECT* dialoag_rect, HTHEME bg_res, __u32 string_id)
{
	char str[32] ;
	GUI_RECT gui_rect ;
	dv_frm_ctrl->dialoag_lyr_hdl =  dv_layer_create( dialoag_rect,DISP_LAYER_WORK_MODE_NORMAL);
	GUI_LyrWinSetSta(dv_frm_ctrl->dialoag_lyr_hdl,GUI_LYRWIN_STA_ON);
	GUI_LyrWinSetTop(dv_frm_ctrl->dialoag_lyr_hdl);

	if (GUI_LyrWinGetSta(GUI_WinGetLyrWin(dv_frm_ctrl->frmwin_hdl)) == GUI_LYRWIN_STA_SUSPEND)
	{
		esKRNL_TimeDly(1); 
		if (GUI_LyrWinGetSta(GUI_WinGetLyrWin(dv_frm_ctrl->frmwin_hdl)) == GUI_LYRWIN_STA_SUSPEND)
			return ;
	}
	GUI_LyrWinSel(dv_frm_ctrl->dialoag_lyr_hdl);
	GUI_SetFont(dv_frm_ctrl->font);
	GUI_UC_SetEncodeUTF8();
	GUI_SetColor(dv_frm_ctrl->txt_color);

	GUI_BMP_Draw(dsk_theme_hdl2buf(dv_frm_ctrl->uipara->msg_box_bg), 0, 0);
	dsk_langres_get_menu_text(string_id,  str,sizeof(str));
	gui_rect.x0 = 0 ;
	gui_rect.y0 = 0 ;
	gui_rect.x1 = dialoag_rect->width - 1;
	gui_rect.y1 = dialoag_rect->height - 1;
	GUI_DispStringInRect(str, &gui_rect, GUI_TA_HCENTER|GUI_TA_VCENTER);

	// creat timer for timer out
    GUI_SetTimer(dv_frm_ctrl->frmwin_hdl, TIMER_DIALOAG_TIMEOUT_ID, 50, NULL);

}


static void dv_dialog_destroy( dv_frmwin_para_t *dv_frm_ctrl)
{
    GUI_KillTimer(dv_frm_ctrl->frmwin_hdl, TIMER_DIALOAG_TIMEOUT_ID);
	dv_layer_destroy( dv_frm_ctrl->dialoag_lyr_hdl );
	dv_frm_ctrl->dialoag_lyr_hdl = NULL ;
}

static void  __dv_init(dv_frmwin_para_t* dv_frm_ctrl)
{
    __dv_config_t       tmp_config;
    __lotus_overlay_src_init_t  tmp_src;

	eLIBs_memset(&tmp_config, 0 , sizeof(tmp_config) );
    Cvr_DvOpen();
    
    if(dv_frm_ctrl->cur_state == DV_ON_REC)
    {
        tmp_config.work_mode    = WORK_MODE_REC;
		tmp_config.rec_quality = RECORD_QUALITY_640_480 ;
    }
    else if(dv_frm_ctrl->cur_state == DV_ON_CAM)
    {
        tmp_config.work_mode    = WORK_MODE_CAM;
    }
	
    Cvr_DvConfigPara(&tmp_config);	
    // 配置时间水印的资源

	//1280x720
	cvr_water_mark_get_overlay_data(&tmp_src, 1);
    Cvr_DvSetOverlaySrc(&tmp_src, 0);
    // 设置普通录像文件名称
    Cvr_DvChangeNamePrefix(FILE_NAME_PRIX);
    
}

static void  _dv_exit(dv_frmwin_para_t* dv_ctrl)
{
    Cvr_DvClose();
}

#if (CVR_TVOUT_ENABLE == 1)
static void __dv_tv_plugin(dv_frmwin_para_t* dv_frm_ctrl)
{
    RECT        rect;
    ES_FILE*    display;

	dsk_display_off();
	dsk_display_set_format(LION_DISP_TV_NTSC);
    
    display = eLIBs_fopen("b:\\DISP\\DISPLAY", "r+");
    if(display == NULL)
    {
        __wrn("can not open display driver\n");
    }
    switch(dv_frm_ctrl->cur_state)
    {   
        case DV_ON_REC:
        case DV_ON_CAM:
        case DV_ON_REC_SET:
        case DV_ON_CAM_SET:
        {
        	rect.x      = 30;
        	rect.y      = 20;
        	rect.width  = eLIBs_fioctrl(display, DISP_CMD_SCN_GET_WIDTH, 0, NULL) - 60;
        	rect.height = eLIBs_fioctrl(display, DISP_CMD_SCN_GET_HEIGHT, 0, NULL) - 40;
            Cvr_DvSetShowRect(rect, 0);
            Cvr_DvSetShowRect(rect, 1);
            break;
        }    
        default:
            break;
    }

	// need to do  handle UI when turn from lcd->tv 

	dsk_display_on(DISP_OUTPUT_TYPE_TV);
    eLIBs_fclose(display);
}

static void __dv_tv_plugout(dv_frmwin_para_t* dv_frm_ctrl)
{
    RECT        rect;
    ES_FILE*    display;
    
	dsk_display_off();
	dsk_display_set_format(LION_DISP_LCD);
    display = eLIBs_fopen("b:\\DISP\\DISPLAY", "r+");
    if(display == NULL)
    {
        __wrn("can not open display driver\n");
    }
    switch(dv_frm_ctrl->cur_state)
    {   
        case DV_ON_REC:
        case DV_ON_CAM:
        case DV_ON_REC_SET:
        case DV_ON_CAM_SET:
        {
        	rect.x      = 0;
        	rect.y      = 0;
        	rect.width  = eLIBs_fioctrl(display, DISP_CMD_SCN_GET_WIDTH, 0, NULL);
        	rect.height = eLIBs_fioctrl(display, DISP_CMD_SCN_GET_HEIGHT, 0, NULL);
            Cvr_DvSetShowRect(rect, 0);
            Cvr_DvSetShowRect(rect, 1);
            break;
        }    
        default:
            break;
    }

	// need to do  handle UI when turn from tv->lcd 

	
	dsk_display_on(DISP_OUTPUT_TYPE_LCD);
    eLIBs_fclose(display);
}
#endif

#if (CVR_HDMI_ENABLE == 1)
static void __dv_hdmi_plugin(dv_frmwin_para_t* dv_frm_ctrl)
{
    __u32       tmp_len;
    RECT        rect;
    ES_FILE*    display;
    
    dsk_display_off();
	dsk_display_set_format(LION_DISP_HDMI_720P_50HZ);
    
    display = eLIBs_fopen("b:\\DISP\\DISPLAY", "r+");
    if(display == NULL)
    {
        __wrn("can not open display driver\n");
    }
    switch(dv_frm_ctrl->cur_state)
    {   
        case DV_ON_REC:
        case DV_ON_CAM:
        case DV_ON_REC_SET:
        case DV_ON_CAM_SET:
        {
        	rect.x      = 30;
        	rect.y      = 20;
        	rect.width  = eLIBs_fioctrl(display, DISP_CMD_SCN_GET_WIDTH, 0, NULL) - 60;
        	rect.height = eLIBs_fioctrl(display, DISP_CMD_SCN_GET_HEIGHT, 0, NULL) - 40;
            Cvr_DvSetShowRect(rect, 0);
            Cvr_DvSetShowRect(rect, 1);
            break;
        }    
        default:
            break;
    }

	// need to do  handle UI when turn from tv->lcd 

	dsk_display_on(DISP_OUTPUT_TYPE_HDMI);
    eLIBs_fclose(display);
}

static void __dv_hdmi_plugout(dv_frmwin_para_t* dv_frm_ctrl)
{
    RECT        rect;
    ES_FILE*    display;
    
    dsk_display_off();
	dsk_display_set_format(LION_DISP_LCD);
    
    display = eLIBs_fopen("b:\\DISP\\DISPLAY", "r+");
    if(display == NULL)
    {
        __wrn("can not open display driver\n");
    }
    switch(dv_frm_ctrl->cur_state)
    {   
        case DV_ON_REC:
        case DV_ON_CAM:
        case DV_ON_REC_SET:
        case DV_ON_CAM_SET:
        {
        	rect.x      = 0;
        	rect.y      = 0;
        	rect.width  = eLIBs_fioctrl(display, DISP_CMD_SCN_GET_WIDTH, 0, NULL);
        	rect.height = eLIBs_fioctrl(display, DISP_CMD_SCN_GET_HEIGHT, 0, NULL);
            Cvr_DvSetShowRect(rect, 0);
            Cvr_DvSetShowRect(rect, 1);
            break;
        }    
        default:
            break;
    }

    // need to do  handle UI when turn from tv->lcd 

	dsk_display_on(DISP_OUTPUT_TYPE_LCD);
    eLIBs_fclose(display);
}
#endif

static __s32 __dv_timer_record_proc(void * p_arg)
{
	dv_frmwin_para_t*        	dv_frm_ctrl;
	dv_frm_ctrl = (dv_frmwin_para_t *)p_arg;
    
	esTIME_GetTime(&dv_frm_ctrl->sys_time);
	esTIME_GetDate(&dv_frm_ctrl->sys_date);
	
    if(Cvr_DvGetRecState() != RECORD_STOP)
    {
        // 更新时间水印
        if(dv_frm_ctrl->user_data->water_mark_enable== 1)
        {
            dv_frm_ctrl->water_mark_para.total = 19;
			dv_frm_ctrl->water_mark_para.pos.x = 40; 
				dv_frm_ctrl->water_mark_para.pos.y = 40;
            	__dv_get_time_mark_index(dv_frm_ctrl->sys_date, dv_frm_ctrl->sys_time, dv_frm_ctrl->water_mark_para.IDlist, 1);
				Cvr_DvShowOverlay(&dv_frm_ctrl->water_mark_para, 0);	
        }
        else
        {
            Cvr_DvShowOverlay(NULL, 0);
        }
       
		if(dv_frm_ctrl->recording_flash_flag)
		{
			__gui_msg_t new_msg ;
			new_msg.id			= GUI_MSG_PAINT,
			new_msg.h_srcwin	= dv_frm_ctrl->frmwin_hdl;
			new_msg.h_deswin	= dv_frm_ctrl->frmwin_hdl;
			new_msg.dwReserved	= DV_PAINT_FLASH_ICON1;
			new_msg.dwAddData2	= 0;
			new_msg.p_arg		= NULL;
			
		    GUI_SendAsyncMessage (&new_msg);
			dv_frm_ctrl->recording_flash_flag = 0 ;
		}
		else
		{
			__gui_msg_t new_msg ;
			new_msg.id			= GUI_MSG_PAINT,
			new_msg.h_srcwin	= dv_frm_ctrl->frmwin_hdl;
			new_msg.h_deswin	= dv_frm_ctrl->frmwin_hdl;
			new_msg.dwReserved	= DV_PAINT_FLASH_ICON2;
			new_msg.dwAddData2	= 0;
			new_msg.p_arg		= NULL;
			
		    GUI_SendAsyncMessage (&new_msg);
			dv_frm_ctrl->recording_flash_flag = 1 ;

		}
       
    }
    
    return EPDK_OK;
}

static void __dv_delete_first_file_thread(void * p_arg)
{
	__s32               ret;
	__s64               tmp_size;
	char                cur_file[1024];
	char                file_path[1024];
	dv_frmwin_para_t*        	dv_frm_ctrl;
	dv_frm_ctrl = (dv_frmwin_para_t *)p_arg;
	
	while (1) 
    {
        if(esKRNL_TDelReq(EXEC_prioself) == OS_TASK_DEL_REQ)
       	{
       	    esKRNL_TDel(EXEC_prioself);
   			break;
       	}
        //　正在录像，且当前录像模式为循环覆盖（user_data->circle_rec_time_interval = 0 时录像不覆盖）
        if((dv_frm_ctrl->rec_status != 0)&&(dv_frm_ctrl->user_data->circle_rec_time_interval> 0))
        {
            tmp_size = eLIBs_GetVolFSpace(USER_DISK) - DISK_FULL_LEVEL;
            if(tmp_size <= (long long)0)    // 磁盘满
            {   
				// 获取当前正在录制文件名
           		Cvr_DvGetFileName(cur_file, 0);
                ret = __dv_get_first_file(FILE_PATH, file_path, cur_file);
                if(ret == EPDK_OK)
                {   // 删除文件失败
                    if(EPDK_FAIL == eLIBs_remove(file_path))
                    {
                        __log("remove file fail : %s, ret = %d \n", file_path, ret);
                    }
                }
			}				
            esKRNL_TimeDly(100);
        }
        else
        {
            esKRNL_TimeDly(100);
        }
    } 
}

static __s32 __dv_on_start_record( __gui_msg_t * msg)
{
    __s64               tmp_size;
    char                file_path[1024];
	dv_frmwin_para_t*        	dv_frm_ctrl;

	dv_frm_ctrl = (dv_frmwin_para_t *)GUI_WinGetAttr(msg->h_deswin);
    // 循环录像打开
    if(dv_frm_ctrl->user_data->circle_rec_time_interval > 0)
    {     
        tmp_size = eLIBs_GetVolFSpace(USER_DISK) - DISK_FULL_LEVEL;
        if(tmp_size <= (long long)0)    // 磁盘满
        {   
			// 获取最早录制的文件（非当前录制文件）
			__dv_get_first_file(FILE_PATH, file_path, NULL);
			eLIBs_remove(file_path);
        }
        // 开始录像
        Cvr_DvStartRecord();
        dv_frm_ctrl->rec_status = 1;
        
    }
    else	// 未打开循环录像
    {
        // 磁盘满检测
        tmp_size = eLIBs_GetVolFSpace(USER_DISK) - DISK_FULL_LEVEL;
        if(tmp_size <= (long long)0)
        {
            // 磁盘满对话框,need to do
        	//__home_on_disk_full(msg);
        }
        else
        {
            // 开始录像
            Cvr_DvStartRecord();
            dv_frm_ctrl->rec_status = 1;
        }
    }
	return EPDK_OK;

}

static __s32 __dv_on_stop_record(__gui_msg_t * msg)
{
	dv_frmwin_para_t*        	dv_frm_ctrl;

	dv_frm_ctrl = (dv_frmwin_para_t *)GUI_WinGetAttr(msg->h_deswin);
    // 清除时间水印
    if(dv_frm_ctrl->user_data->water_mark_enable == 1)
    {
        dv_frm_ctrl->water_mark_para.total = 0;
		dv_frm_ctrl->water_mark_para.pos.x = 40; 
		dv_frm_ctrl->water_mark_para.pos.y = 40;
		Cvr_DvShowOverlay(&dv_frm_ctrl->water_mark_para, 0);	
    }    
    dv_frm_ctrl->rec_status = 0;                    
	Cvr_DvStopRecord();
   	eLIBs_memset((void*)(&dv_frm_ctrl->cur_rec_time),0,sizeof((dv_frm_ctrl->cur_rec_time)));

    // need to do : draw recording time to zero	
    
    return EPDK_OK;
}

static __s32 __dv_on_recording(__gui_msg_t * msg)
{
    __s64               tmp_size;
	dv_frmwin_para_t*        	dv_frm_ctrl;

	dv_frm_ctrl = (dv_frmwin_para_t *)GUI_WinGetAttr(msg->h_deswin);
    // 循环录像打开
    if(dv_frm_ctrl->user_data->circle_rec_time_interval > 0)
    {
        // 循环录制
        if(Cvr_DvGetCurRecTime() >= dv_frm_ctrl->user_data->circle_rec_time_interval)
        {
            dv_frm_ctrl->rec_status = 0;
            Cvr_DvHaltRecord();  
			eLIBs_memset((void*)(&dv_frm_ctrl->cur_rec_time),0,sizeof((dv_frm_ctrl->cur_rec_time)));
            // 开始录像
            Cvr_DvStartRecord();
        }
    }
    // 未打开循环录像，则采用10分钟循环录像，磁盘满即停止录像。
    else
    {        
        tmp_size = eLIBs_GetVolFSpace(USER_DISK) - DISK_FULL_LEVEL;
        if(tmp_size <= (long long)0)    // 磁盘满
        {
            dv_frm_ctrl->rec_status = 0;
            Cvr_DvStopRecord();
			eLIBs_memset((void*)(&dv_frm_ctrl->cur_rec_time),0,sizeof((dv_frm_ctrl->cur_rec_time)));
            
            // 更新场景显示图标
            // need to do :  disk full dialoag ; 
        }
        else
        {
            if(Cvr_DvGetCurRecTime() >= 10 * 60)  // 循环录制
            {
                dv_frm_ctrl->rec_status = 0;
                Cvr_DvHaltRecord();  
				eLIBs_memset((void*)(&dv_frm_ctrl->cur_rec_time),0,sizeof((dv_frm_ctrl->cur_rec_time)));
                // 开始录像
                Cvr_DvStartRecord();
            }
        }
    }
	return EPDK_OK;

}

static __s32 __dv_frm_on_create(__gui_msg_t * msg)
{
	dv_frmwin_para_t*        	dv_frm_ctrl;
	dv_frm_ctrl = (dv_frmwin_para_t *)GUI_WinGetAttr(msg->h_deswin);
	
    // 初始化水印图标资源
    cvr_water_mark_res_init();
      
    dv_frm_ctrl->cur_state    = DV_ON_REC;
    dv_frm_ctrl->rec_status   = 0;
    dv_frm_ctrl->frmwin_hdl = msg->h_deswin ;
	dv_frm_ctrl->user_data = dsk_reg_get_para_by_app(REG_APP_DV);

    // 打开DV模块
    __dv_init(dv_frm_ctrl);

    // 创建时间显示TIMER
    GUI_SetTimer(msg->h_deswin, TIMER_STATUS_ID, 50, NULL);
    dv_frm_ctrl->os_timer_hdl = esKRNL_TmrCreate(500, OS_TMR_OPT_PRIO_HIGH | OS_TMR_OPT_PERIODIC, (OS_TMR_CALLBACK)__dv_timer_record_proc, (void *)dv_frm_ctrl);
    if(dv_frm_ctrl->os_timer_hdl)
    {
        esKRNL_TmrStart(dv_frm_ctrl->os_timer_hdl);
    }
    dv_frm_ctrl->delete_file_tsk = esKRNL_TCreate(__dv_delete_first_file_thread, dv_frm_ctrl, 0x2000, KRNL_priolevel3);   

    return EPDK_OK;
}

static __s32 __dv_frm_on_destroy(__gui_msg_t * msg)
{
	dv_frmwin_para_t*        	dv_frm_ctrl;
	dv_frm_ctrl = (dv_frmwin_para_t *)GUI_WinGetAttr(msg->h_deswin);
    // 判断是否处于录像中，若是则停止．
    if(dv_frm_ctrl->rec_status != 0) 
    {   
        dv_frm_ctrl->rec_status = 0;
        // 停止录制
        Cvr_DvStopRecord();
    }
    if(dv_frm_ctrl->delete_file_tsk)
    {
        while(esKRNL_TDelReq(dv_frm_ctrl->delete_file_tsk) != OS_TASK_NOT_EXIST)
        {
            esKRNL_TimeDly(1);
        }
        dv_frm_ctrl->delete_file_tsk = NULL;
    }
    // 删除线程
    if(dv_frm_ctrl->os_timer_hdl!= NULL)
    {
        esKRNL_TmrStop(dv_frm_ctrl->os_timer_hdl, OS_TMR_OPT_PRIO_HIGH | OS_TMR_OPT_PERIODIC, dv_frm_ctrl);
        esKRNL_TmrDel(dv_frm_ctrl->os_timer_hdl);
        dv_frm_ctrl->os_timer_hdl = NULL;
    }
    
    GUI_KillTimer(msg->h_deswin, TIMER_STATUS_ID);
    if( GUI_IsTimerInstalled(msg->h_deswin,TIMER_DIALOAG_TIMEOUT_ID) )
    	GUI_KillTimer(msg->h_deswin, TIMER_DIALOAG_TIMEOUT_ID);

    _dv_exit(dv_frm_ctrl);

    cvr_water_mark_res_uninit();

	esMEMS_Mfree(0,dv_frm_ctrl);
	
    return EPDK_OK;
}


static __s32 __dv_frm_on_paint(__gui_msg_t * msg)
{
	dv_frmwin_para_t*        	dv_frm_ctrl;
	dv_frm_ctrl = (dv_frmwin_para_t *)GUI_WinGetAttr(msg->h_deswin);

	if (GUI_LyrWinGetSta(GUI_WinGetLyrWin(msg->h_deswin)) == GUI_LYRWIN_STA_SUSPEND)
		return EPDK_FAIL;

	GUI_LyrWinSel(dv_frm_ctrl->frm_lyr_hdl);
	GUI_SetFont(dv_frm_ctrl->font);
	GUI_UC_SetEncodeUTF8();
	GUI_SetColor(TEXT_COLOR);
	
	if( DV_PAINT_FLASH_ICON1 == msg->dwReserved )
	{
		GUI_BMP_Draw(dsk_theme_hdl2buf(dv_frm_ctrl->uipara->rec_flash1_handle), dv_frm_ctrl->uipara->work_mode_icon_pos.x, dv_frm_ctrl->uipara->work_mode_icon_pos.y);
	}
	else if( DV_PAINT_FLASH_ICON2 == msg->dwReserved )
	{
		GUI_BMP_Draw(dsk_theme_hdl2buf(dv_frm_ctrl->uipara->rec_flash2_handle), dv_frm_ctrl->uipara->work_mode_icon_pos.x, dv_frm_ctrl->uipara->work_mode_icon_pos.y);
	}
	else if( DV_PAINT_MENUBAR_BG == msg->dwReserved )
	{
		GUI_BMP_Draw(dsk_theme_hdl2buf(dv_frm_ctrl->uipara->menu_bar_bg_handle), dv_frm_ctrl->uipara->menu_bar_bg_rect.x, dv_frm_ctrl->uipara->menu_bar_bg_rect.y);
		GUI_BMP_Draw(dsk_theme_hdl2buf(dv_frm_ctrl->uipara->mp_fcs_handle), dv_frm_ctrl->uipara->menu_bar_bg_rect.x, dv_frm_ctrl->uipara->menu_bar_bg_rect.y);
	}	
	else if( DV_CLEAR_MENUBAR_BG == msg->dwReserved )
	{
		GUI_ClearRect(dv_frm_ctrl->uipara->menu_bar_bg_rect.x, dv_frm_ctrl->uipara->menu_bar_bg_rect.y, \
			dv_frm_ctrl->uipara->menu_bar_bg_rect.x + dv_frm_ctrl->uipara->menu_bar_bg_rect.width - 1 , \
			dv_frm_ctrl->uipara->menu_bar_bg_rect.y + dv_frm_ctrl->uipara->menu_bar_bg_rect.height - 1 );
	}			
	else	// paint all
	{
		char str[32] ;
		GUI_BMP_Draw(dsk_theme_hdl2buf(dv_frm_ctrl->uipara->hbar_back_handle), dv_frm_ctrl->uipara->hbar_bg_rect.x, dv_frm_ctrl->uipara->hbar_bg_rect.y);
		if( DV_ON_REC == dv_frm_ctrl->cur_state )
		{
			GUI_BMP_Draw(dsk_theme_hdl2buf(dv_frm_ctrl->uipara->work_rec_handle), dv_frm_ctrl->uipara->work_mode_icon_pos.x, dv_frm_ctrl->uipara->work_mode_icon_pos.y);
			dsk_langres_get_menu_text(dv_frm_ctrl->uipara->rec_quantity_strid[dv_frm_ctrl->user_data->rec_quality], str, sizeof(str));
			GUI_DispStringAt(str,dv_frm_ctrl->uipara->quantity_text_pos.x, dv_frm_ctrl->uipara->quantity_text_pos.y );
		}
		else
		{
			GUI_BMP_Draw(dsk_theme_hdl2buf(dv_frm_ctrl->uipara->work_cam_handle), dv_frm_ctrl->uipara->work_mode_icon_pos.x, dv_frm_ctrl->uipara->work_mode_icon_pos.y);
			dsk_langres_get_menu_text(dv_frm_ctrl->uipara->image_quantity_strid [dv_frm_ctrl->user_data->cam_quality], str, sizeof(str));
			GUI_DispStringAt(str,dv_frm_ctrl->uipara->quantity_text_pos.x, dv_frm_ctrl->uipara->quantity_text_pos.y );
		}
	}
    return EPDK_OK;
}

static __s32 __dv_frm_on_key_proc(__gui_msg_t * msg)
{
	dv_frmwin_para_t*        	dv_frm_ctrl;
	dv_frm_ctrl = (dv_frmwin_para_t *)GUI_WinGetAttr(msg->h_deswin);

    if(dv_frm_ctrl->cur_state == DV_ON_REC)
    {
        if(msg->dwAddData1 == GUI_MSG_KEY_ENTER)        // 录像的开始、停止
        {   
            if(msg->dwAddData2 == KEY_UP_ACTION)
            {
        		ES_DIR*			p_file;
    
			    // 磁盘空间检测
			    p_file = eLIBs_opendir(USER_DISK);
				if(!p_file)
				{
					RECT rect ;
					rect.x = ( dv_frm_ctrl->uipara->scn_w - dv_frm_ctrl->uipara->msg_box_size.width )>>1 ;
					rect.y = ( dv_frm_ctrl->uipara->scn_h - dv_frm_ctrl->uipara->msg_box_size.height )>>1 ;
					rect.width = dv_frm_ctrl->uipara->msg_box_size.width ;
					rect.height = dv_frm_ctrl->uipara->msg_box_size.height ;
					dv_dialog_creat(dv_frm_ctrl, &rect, dv_frm_ctrl->uipara->msg_box_bg , dv_frm_ctrl->uipara->no_sd_id);
					return EPDK_OK;
				}
                // 开始录像
                if(RECORD_STOP == Cvr_DvGetRecState())
                {   
                    __dv_on_start_record(msg);
                }
                // 停止录像
                else if(RECORD_START == Cvr_DvGetRecState())
                {
                    __dv_on_stop_record(msg);
					__dv_frm_on_paint(msg);
                }
            }
        }
        else if(msg->dwAddData1 == GUI_MSG_KEY_VADD)        // 切换模式(进入拍照模式)
        {   
            if(msg->dwAddData2 == KEY_UP_ACTION)
            {
                if(RECORD_STOP == Cvr_DvGetRecState())
                {
                    // 切换至拍照状态
					Cvr_DvSetWorkMode(WORK_MODE_CAM);
					dv_frm_ctrl->cur_state = DV_ON_CAM ;
					//  UI 部分更新到从录像模式切换到拍照状态
					__dv_frm_on_paint( msg);
                }
            }
        }
        else if(msg->dwAddData1 == GUI_MSG_KEY_MENU)       // 弹出设置菜单
        {   
            if(msg->dwAddData2 == KEY_UP_ACTION)
            {
                if(RECORD_STOP == Cvr_DvGetRecState())
                {
                	dv_frm_ctrl->cur_state = DV_ON_REC_SET;
					if(dv_frm_ctrl->dialoag_lyr_hdl)
					{
						dv_layer_destroy(dv_frm_ctrl->dialoag_lyr_hdl);
						dv_frm_ctrl->dialoag_lyr_hdl = NULL ;
					}
					msg->dwReserved = DV_PAINT_MENUBAR_BG ;
					__dv_frm_on_paint( msg);
					Dv_cmd2parent(msg->h_deswin, CMD_DV_FRM_POP_REC_SETTING_MENU, 0, 0);
                }
            }
        }
        return EPDK_OK;
    }
    else if(dv_frm_ctrl->cur_state == DV_ON_CAM)
    {
        if(msg->dwAddData1 == GUI_MSG_KEY_ENTER)        // 开始拍照
        {   
            if(msg->dwAddData2 == KEY_UP_ACTION)
            {
        		ES_DIR*         p_file;
				__u64			tmp_size ;

			    // 磁盘空间检测
			    p_file = eLIBs_opendir(USER_DISK);
				if(!p_file)
				{
					RECT rect ;
					//  弹出没有tf 卡对话框
					rect.x = ( dv_frm_ctrl->uipara->scn_w - dv_frm_ctrl->uipara->msg_box_size.width )>>1 ;
					rect.y = ( dv_frm_ctrl->uipara->scn_h - dv_frm_ctrl->uipara->msg_box_size.height )>>1 ;
					rect.width = dv_frm_ctrl->uipara->msg_box_size.width ;
					rect.height = dv_frm_ctrl->uipara->msg_box_size.height ;
					dv_dialog_creat(dv_frm_ctrl, &rect, dv_frm_ctrl->uipara->msg_box_bg , dv_frm_ctrl->uipara->no_sd_id);
					return EPDK_OK;
				}
                // 磁盘空间已满，无法进行拍照
        	    tmp_size = eLIBs_GetVolFSpace(USER_DISK) - DISK_FULL_LEVEL;
        	    if(tmp_size <= (long long)0)    // 停止当前录制，提示磁盘空间满
        	    {
        		   	// need to do :弹出磁盘满对话框
        		   	RECT rect ;
					rect.x = ( dv_frm_ctrl->uipara->scn_w - dv_frm_ctrl->uipara->msg_box_size.width )>>1 ;
					rect.y = ( dv_frm_ctrl->uipara->scn_h - dv_frm_ctrl->uipara->msg_box_size.height )>>1 ;
					rect.width = dv_frm_ctrl->uipara->msg_box_size.width ;
					rect.height = dv_frm_ctrl->uipara->msg_box_size.height ;
					dv_dialog_creat(dv_frm_ctrl, &rect, dv_frm_ctrl->uipara->msg_box_bg , dv_frm_ctrl->uipara->disk_full_id);
					return EPDK_OK;
        		}

        	    // 设置时间水印
                if(dv_frm_ctrl->user_data->water_mark_enable == 1)
                {
                    dv_frm_ctrl->water_mark_para.total = 19;
        			dv_frm_ctrl->water_mark_para.pos.x = 20; 
        			dv_frm_ctrl->water_mark_para.pos.y = 20;	
        			__dv_get_time_mark_index(dv_frm_ctrl->sys_date, dv_frm_ctrl->sys_time, dv_frm_ctrl->water_mark_para.IDlist, 1);
        			Cvr_DvShowOverlay(&dv_frm_ctrl->water_mark_para, 0);
                }
                // 拍照声音, 后面视需求添加
                #if 0
                if(EPDK_OK == keytone_set_path(VOICE_TAKE_IMG))
                {
                    keytone_play();
                }
				#endif

                if( DISP_OUTPUT_TYPE_LCD == dsk_display_get_output_type())
                {
                    // 开始拍照
                    Cvr_DvTakeImage(1); 
                }
                else
                {   
                	//A13 只有一个scaler 图层
                	__wrn("only can take picture in lcd output mode ");
                }
                // 清除时间水印
                if(dv_frm_ctrl->user_data->water_mark_enable == 1)
                {
                    dv_frm_ctrl->water_mark_para.total = 0;
        			dv_frm_ctrl->water_mark_para.pos.x = 20; 
        			dv_frm_ctrl->water_mark_para.pos.y = 20;	

        			Cvr_DvShowOverlay(&dv_frm_ctrl->water_mark_para, 0);
        			Cvr_DvShowOverlay(&dv_frm_ctrl->water_mark_para, 1);
                }
            }
        }
        else if(msg->dwAddData1 == GUI_MSG_KEY_VADD)        // 切换模式(进入录像预览模式)
        {   
            if(msg->dwAddData2 == KEY_UP_ACTION)
            {
				// 切换至录像预览状态
				Cvr_DvSetWorkMode(WORK_MODE_REC);
				dv_frm_ctrl->cur_state = DV_ON_REC;
				// UI 部分更新到从拍照状态切换到录像模式
				__dv_frm_on_paint( msg);
         	}
        }
        else if(msg->dwAddData1 == GUI_MSG_KEY_MENU)       // 弹出设置菜单
        {   
            if(msg->dwAddData2 == KEY_UP_ACTION)
            {	
                dv_frm_ctrl->cur_state = DV_ON_CAM_SET;
				if(dv_frm_ctrl->dialoag_lyr_hdl)
				{
					dv_layer_destroy(dv_frm_ctrl->dialoag_lyr_hdl);
					dv_frm_ctrl->dialoag_lyr_hdl = NULL ;
				}
				msg->dwReserved = DV_PAINT_MENUBAR_BG ;
				__dv_frm_on_paint( msg);
				Dv_cmd2parent(msg->h_deswin, CMD_DV_FRM_POP_IMAGE_SETTING_MENU, 0, 0);
            }
        }
        return EPDK_OK;
    }	    

	return EPDK_OK;
}

static __s32 __dv_frm_on_timer_proc(__gui_msg_t * msg)
{
    dv_frmwin_para_t*        	dv_frm_ctrl;

	dv_frm_ctrl = (dv_frmwin_para_t *)GUI_WinGetAttr(msg->h_deswin);
   
    if(msg->dwAddData1 == TIMER_STATUS_ID)    // 录像时，更新当前已录制时间;回放时，更新当前播放时间和进度
    {   
        // 录像状态检测
        if(dv_frm_ctrl->rec_status != 0)
        {
            __dv_on_recording(msg);
    	}
    }
	else if(msg->dwAddData1 == TIMER_DIALOAG_TIMEOUT_ID)
	{
		dv_dialog_destroy(dv_frm_ctrl);
	}
    return EPDK_OK;
}

static __s32 __dv_frm_set_record_quality_proc(__gui_msg_t * msg)
{
    dv_frmwin_para_t*        	dv_frm_ctrl;

	dv_frm_ctrl = (dv_frmwin_para_t *)GUI_WinGetAttr(msg->h_deswin);

   	if( dv_frm_ctrl->cur_state == DV_ON_REC_SET)
   	{
   		switch(msg->dwAddData1)
		{
			case 0:
	        dv_frm_ctrl->user_data->rec_quality = 0;
	        Cvr_DvSetRecQuality(RECORD_QUALITY_640_480);
			break ;
			case 1:
	        dv_frm_ctrl->user_data->rec_quality = 1;
	        Cvr_DvSetRecQuality(RECORD_QUALITY_1280_720);
			break ;
   		}
   	}
	else if( dv_frm_ctrl->cur_state == DV_ON_CAM_SET)
   	{
   		switch(msg->dwAddData1)
		{
			case 0:
	        dv_frm_ctrl->user_data->cam_quality = 0;
	        Cvr_DvSetRecQuality(CAMERA_QUALITY_100);
			break ;
			case 1:
	        dv_frm_ctrl->user_data->cam_quality = 1;
	        Cvr_DvSetRecQuality(CAMERA_QUALITY_200);
			break ;
			case 2:
	        dv_frm_ctrl->user_data->cam_quality = 2;
	        Cvr_DvSetRecQuality(CAMERA_QUALITY_300);
			break ;
   		}
   	}
	// updata quality setting
	__dv_frm_on_paint(msg);
    return EPDK_OK;
}
static __s32 __dv_frm_main_proc(__gui_msg_t * msg)
{
    switch(msg->id)
    {
		case GUI_MSG_CREATE:
		{
			__dv_frm_on_create(msg);
		}
			return EPDK_OK;		
		case GUI_MSG_DESTROY:
		{ 
            __dv_frm_on_destroy(msg);
		}
			return EPDK_OK;
		case GUI_MSG_PAINT:
		{
			__dv_frm_on_paint(msg);
		}
		case GUI_MSG_KEY:
	    {
            __dv_frm_on_key_proc(msg);
	    }	
	        return EPDK_OK;
		case GUI_MSG_COMMAND:
		{
		}
		    return EPDK_OK; 
		case GUI_MSG_TIMER:
		{
		    __dv_frm_on_timer_proc(msg);
		}
		    return EPDK_OK;

    	case DSK_MSG_TVDAC_PLUGIN:		// tvdac plug in
    	{
			dv_frmwin_para_t*        	dv_frm_ctrl;
			dv_frm_ctrl = (dv_frmwin_para_t *)GUI_WinGetAttr(msg->h_deswin);
    	    __msg("TVDAC plug in \n");
#if (CVR_TVOUT_ENABLE == 1)
    	    __dv_tv_plugin(dv_frm_ctrl);
#endif
    	}
    	    break;
    	case DSK_MSG_TVDAC_PLUGOUT:		// tvdac plug out
    	{
			dv_frmwin_para_t*        	dv_frm_ctrl;
			dv_frm_ctrl = (dv_frmwin_para_t *)GUI_WinGetAttr(msg->h_deswin);

    	    __msg("TVDAC plug out \n");
#if (CVR_TVOUT_ENABLE == 1)
    	    __dv_tv_plugout(dv_frm_ctrl);
#endif
    	}
    	    break;
    	case DSK_MSG_HDMI_PLUGIN:		// hdmi plug in
    	{
			dv_frmwin_para_t*        	dv_frm_ctrl;
			dv_frm_ctrl = (dv_frmwin_para_t *)GUI_WinGetAttr(msg->h_deswin);

    	    __msg("HDMI plug in \n");
#if (CVR_HDMI_ENABLE == 1)
    	    __dv_hdmi_plugin(dv_frm_ctrl);
#endif
    	}
    	    break;
    	case DSK_MSG_HDMI_PLUGOUT:		// hdmi plug out
    	{
			dv_frmwin_para_t*        	dv_frm_ctrl;
			dv_frm_ctrl = (dv_frmwin_para_t *)GUI_WinGetAttr(msg->h_deswin);
    	    __msg("HDMI plug out \n");
#if (CVR_HDMI_ENABLE == 1)
    	    __dv_hdmi_plugout(dv_frm_ctrl);
#endif
    	}
    	    break;
    	case DSK_MSG_FS_PART_PLUGIN:	// 文件系统分区插入
            __msg("File system plug in \n");
    	    break;
    	case DSK_MSG_FS_PART_PLUGOUT:	// 文件系统分区拔出 
    	{
            __msg("File system plug out \n");
			// 停止当前录制
			__dv_on_stop_record(msg);
    	}
    	    break;
		case CMD_DV_FRM_SET_RECORD_RESOLUTION:
		{
			__dv_frm_set_record_quality_proc(msg);
		}
			break ;
		case CMD_DV_FRM_SETTING_MENU_EXIT:
		{
			__dv_frm_set_record_quality_proc(msg);
		}
			break ;

			
    	default:
    	    break;
    }
	return GUI_FrmWinDefaultProc(msg);
}

__s32 dv_frm_create(dv_frmwin_para_t *para)
{
	RECT rect ;
	__gui_framewincreate_para_t framewin_para;
	dv_frmwin_para_t * dv_frmwin_ctrl ;
	
	dv_frmwin_ctrl = (dv_frmwin_para_t *)esMEMS_Malloc(0, sizeof(dv_frmwin_para_t));
	if(dv_frmwin_ctrl == NULL)
	{
		__wrn("malloc error \n");
		return EPDK_FAIL;
	}
	eLIBs_memcpy(dv_frmwin_ctrl,para,sizeof(dv_frmwin_para_t));

	dv_frmwin_ctrl->uipara = Dv_GetUipara( );
	rect.x = 0 ;
	rect.y = 0 ;
	rect.width = dv_frmwin_ctrl->uipara->scn_w ;
	rect.height = dv_frmwin_ctrl->uipara->scn_h ;
	
	dv_frmwin_ctrl->frm_lyr_hdl = dv_layer_create(&rect, DISP_LAYER_WORK_MODE_NORMAL);
	GUI_LyrWinSetSta(dv_frmwin_ctrl->frm_lyr_hdl,GUI_LYRWIN_STA_ON);
	eLIBs_memset(&framewin_para, 0, sizeof(__gui_framewincreate_para_t));
	framewin_para.name =	"dv frame win",
	framewin_para.dwExStyle = WS_EX_NONE;
	framewin_para.dwStyle = WS_NONE|WS_VISIBLE;
	framewin_para.spCaption =  NULL;
	framewin_para.hOwner	= NULL;
	framewin_para.id         = DV_FRM_WIN_ID;
	framewin_para.hHosting = para->h_parent;
	framewin_para.FrameWinProc = (__pGUI_WIN_CB)esKRNL_GetCallBack((__pCBK_t)__dv_frm_main_proc);
	framewin_para.rect.x =0;
	framewin_para.rect.y = 0;
	framewin_para.rect.width= rect.width;
	framewin_para.rect.height = rect.height;
	framewin_para.BkColor.alpha =  0;
	framewin_para.BkColor.red = 0;
	framewin_para.BkColor.green = 0;
	framewin_para.BkColor.blue = 0;
	framewin_para.attr =  (void *)dv_frmwin_ctrl;
	framewin_para.hLayer = dv_frmwin_ctrl->frm_lyr_hdl;
	return(GUI_FrmWinCreate(&framewin_para));

}

__s32 dv_frm_destroy( H_WIN h_win )
{
	dv_frmwin_para_t*        	dv_frm_ctrl;

	dv_frm_ctrl = (dv_frmwin_para_t *)GUI_WinGetAttr(h_win);
	// for frame win ,there is no need to call  GUI_FrmWinDelete to destroy frame win ,because the function of  GUI_LyrWinDelete will do that 
	GUI_LyrWinDelete(dv_frm_ctrl->frm_lyr_hdl);	
}



