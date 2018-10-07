#include "epdk.h"

typedef  struct  tag_PARROT_DATA
{
    __s32       herb_obj;
    __s32       herb_lyr;
    __s32		sensor_index;
}__parrot_data_t;


static  __u8        mid_lotus = 0;
static  __mp        *mod_lotus = NULL;
static  ES_FILE     *parrot_disp = NULL;

#define 	CSIIN_SOURCE		0
#define 	TVIN_SOURCE			1
#define 	CSI_TVIN_SOURCE		2
#define 	VIDEO_SOURE			CSI_TVIN_SOURCE

#define 	LCD_TYPE		0
#define 	TV_TYPE			1
#define 	DISPLAY_TYPE			LCD_TYPE

#if VIDEO_SOURE==CSIIN_SOURCE
	#define ENCODE_PIXEL_SIZE_W		1920
	#define ENCODE_PIXEL_SIZE_H		1080
#elif VIDEO_SOURE==TVIN_SOURCE
	#define ENCODE_PIXEL_SIZE_W		720
	#define ENCODE_PIXEL_SIZE_H		480
#else
	#define ENCODE_PIXEL_SIZE_W		1280
	#define ENCODE_PIXEL_SIZE_H		720
#endif

#define	PIP_ENABLE		0

__s32   parrot_init(void)
{
#if VIDEO_SOURE==CSIIN_SOURCE
    esDEV_Plugin("\\drv\\csi.drv", 0, NULL, 0);
	__log( "video source csi in \n" );
#elif (VIDEO_SOURE==TVIN_SOURCE)
   	esDEV_Plugin("\\drv\\tvd.drv", 0, NULL, 0);
	__log( "video source tv in \n" );
#elif (VIDEO_SOURE==CSI_TVIN_SOURCE)
   	esDEV_Plugin("\\drv\\tvd.drv", 0, NULL, 0);
    esDEV_Plugin("\\drv\\csi.drv", 0, NULL, 0);
	__log( "video source csi and tv in \n" );
#endif
	__log( "parrot_init() \n" );

RETRY:
	mid_lotus = esMODS_MInstall( "d:\\mod\\lotus\\lotus.mod", 0 );
	if(mid_lotus == NULL)
	{
		__log( "install lotus fail\n" );
		goto RETRY;
	}	
	mod_lotus = esMODS_MOpen(mid_lotus, 0);
	if(mod_lotus == NULL)
	{
		__log( "open herb fail\n" );
		esMODS_MUninstall(mid_lotus);
		return EPDK_FAIL;
	}
	parrot_disp = eLIBs_fopen("b:\\DISP\\DISPLAY", "rb");
	if(parrot_disp == NULL)
	{
	    __log("open display drv fail\n");
    	esMODS_MClose(mod_lotus);
    	mod_lotus = NULL;
    	esMODS_MUninstall(mid_lotus);
    	mid_lotus = 0;
    	
    	return EPDK_FAIL;
	}
	
	return EPDK_OK;
}

__s32   parrot_uninit(void)
{
    if(parrot_disp)
    {
		eLIBs_fclose(parrot_disp);
		parrot_disp = 0;
    }
	if(mod_lotus)
	{
		esMODS_MClose(mod_lotus);
		mod_lotus = 0;
	}
	if(mid_lotus)
	{
		esMODS_MUninstall(mid_lotus);
		mid_lotus = 0;
	}
		
    esDEV_Plugout("\\drv\\usb_host0.drv", 0);
	
	return EPDK_OK;
}

__hdle   parrot_open(__s32 index)
{
    __parrot_data_t     *parrot_data;

    parrot_data = esMEMS_Malloc(0, sizeof(__parrot_data_t));
    if(parrot_data == NULL)
    {
    	__log("request memery for __parrot_dat_t fail\n");
    	return NULL;
    }
    eLIBs_memset(parrot_data, 0, sizeof(__parrot_data_t));
    
    parrot_data->herb_obj = esMODS_MIoctrl(mod_lotus, LOTUS_CMD_NEW_OBJ, 0, 0);
    if(parrot_data->herb_obj == 0)
    {
        __log("open herb.mod err!\n");
        esMEMS_Mfree(0, parrot_data);
        parrot_data = NULL;
        
        return NULL;
    }
    esMODS_MIoctrl(mod_lotus, LOTUS_CMD_SET_CSI, parrot_data->herb_obj, (void *)index);
    parrot_data->sensor_index = index;
    
    return (__hdle)parrot_data;
}

__s32   parrot_close(__hdle hdle)
{
    __parrot_data_t     *parrot_data;
    __lotus_status_t     parrot_status;
    
	if(hdle == NULL)
	{
		return EPDK_OK;
	}  
    parrot_data = (__parrot_data_t *)hdle;
    if(parrot_data->herb_obj)
    {
        parrot_status = (__lotus_status_t)esMODS_MIoctrl(mod_lotus, LOTUS_CMD_GET_STATUS, parrot_data->herb_obj, NULL);
    	if((parrot_status == LOTUS_STAT_RECORD) || (parrot_status == LOTUS_STAT_PAUSE))
    	{
    		esMODS_MIoctrl(mod_lotus, LOTUS_CMD_STOP_RECORD, 0, NULL);
    	}
		esMODS_MIoctrl(mod_lotus, LOTUS_CMD_STOP, parrot_data->herb_obj, NULL);
    	esMODS_MIoctrl(mod_lotus, LOTUS_CMD_DEL_OBJ, parrot_data->herb_obj, NULL);
    }
	
    esMEMS_Mfree(0, parrot_data);
    parrot_data = NULL;

    return EPDK_OK;  
}

__s32   parrot_get_csi_mode(__hdle hdle, __csi_mode_all_t * mode)
{
    __parrot_data_t     *parrot_data;
    
	if(hdle == NULL)
	{
		return EPDK_FAIL;
	}
	parrot_data = (__parrot_data_t *)hdle;
	
	return (esMODS_MIoctrl(mod_lotus, LOTUS_CMD_GET_OUT_MODE_ALL, parrot_data->herb_obj, (void *)mode));
}

__s32   parrot_set_mode(__hdle hdle, __lotus_record_mode_t mode)
{
    __parrot_data_t     *parrot_data;
    __lotus_status_t     parrot_status;
    
	if(hdle == NULL)
	{
		return EPDK_FAIL;
	}
    parrot_data = (__parrot_data_t *)hdle;
    
    parrot_status = (__lotus_status_t)esMODS_MIoctrl(mod_lotus, LOTUS_CMD_GET_STATUS, parrot_data->herb_obj, NULL);
	if((parrot_status == LOTUS_STAT_RECORD) || (parrot_status == LOTUS_STAT_PAUSE))
	{   // 非录像停止状态设置模式无效
		return EPDK_FAIL;
	}
    esMODS_MIoctrl(mod_lotus, LOTUS_CMD_STOP, parrot_data->herb_obj, NULL);
	return esMODS_MIoctrl(mod_lotus, LOTUS_CMD_SET_MODE, parrot_data->herb_obj, (void *)mode);
}

__s32   parrot_set_media_file(__hdle hdle, __lotus_media_file_inf * info)
{
    __parrot_data_t     *parrot_data;
    __lotus_status_t     parrot_status;
    
	if(hdle == NULL)
	{
		return EPDK_FAIL;
	}
    parrot_data = (__parrot_data_t *)hdle;
    
    parrot_status = (__lotus_status_t)esMODS_MIoctrl(mod_lotus, LOTUS_CMD_GET_STATUS, parrot_data->herb_obj, NULL);
	if((parrot_status == LOTUS_STAT_IDLE) || (parrot_status == LOTUS_STAT_STANDBY))
	{
		return esMODS_MIoctrl(mod_lotus, LOTUS_CMD_SET_MEDIAFILE, parrot_data->herb_obj, info);	
	}
	else
	{
		return EPDK_FAIL;
    }
}

__s32   parrot_set_record_thumb(__hdle hdle, FB * fb)
{    
    __parrot_data_t     *parrot_data;
    
	if(hdle == NULL)
	{
		return EPDK_FAIL;
	}
    parrot_data = (__parrot_data_t *)hdle;
    
    return esMODS_MIoctrl(mod_lotus, LOTUS_CMD_NEED_PREVIEW_PIC_FOR_RECORD, parrot_data->herb_obj, fb);
}

__s32   parrot_set_preview_mode(__hdle hdle, __csi_mode_t * csi_mode)
{
    __parrot_data_t     *parrot_data;
    __lotus_status_t     parrot_status;
    
	if(hdle == NULL)
	{
		return EPDK_FAIL;
	}
    parrot_data = (__parrot_data_t *)hdle;
    
    parrot_status = (__lotus_status_t)esMODS_MIoctrl(mod_lotus, LOTUS_CMD_GET_STATUS, parrot_data->herb_obj, NULL);
	if((parrot_status == LOTUS_STAT_IDLE) || (parrot_status == LOTUS_STAT_STANDBY))
	{
	    esMODS_MIoctrl(mod_lotus, LOTUS_CMD_SET_PREVIEW_OUT_MODE, parrot_data->herb_obj, csi_mode);
	    
		esMODS_MIoctrl(mod_lotus, LOTUS_CMD_START, parrot_data->herb_obj, NULL);

        return EPDK_OK;
	}
	else
	{
		return EPDK_FAIL;
    }
}

__s32   parrot_start_preview(__hdle hdle)
{
    __parrot_data_t     *parrot_data;
    
	if(hdle == NULL)
	{
		return EPDK_FAIL;
	}
    parrot_data = (__parrot_data_t *)hdle;
    
    return esMODS_MIoctrl(mod_lotus, LOTUS_CMD_ENABLE_PLAYBACK, parrot_data->herb_obj, (void *)1);
}

__s32   parrot_stop_preview(__hdle hdle)
{
    __parrot_data_t     *parrot_data;

	if(hdle == NULL)
	{
		return EPDK_FAIL;
	}
    parrot_data = (__parrot_data_t *)hdle;

    esMODS_MIoctrl(mod_lotus, LOTUS_CMD_ENABLE_PLAYBACK, parrot_data->herb_obj, (void *)0);
	return EPDK_OK;
}

__s32   parrot_start_record(__hdle hdle)
{
    __parrot_data_t     *parrot_data;
    __lotus_status_t     parrot_status;
    
	if(hdle == NULL)
	{
		return EPDK_FAIL;
	}
    parrot_data = (__parrot_data_t *)hdle;
    
    parrot_status = (__lotus_status_t)esMODS_MIoctrl(mod_lotus, LOTUS_CMD_GET_STATUS, parrot_data->herb_obj, NULL);
	if(parrot_status == LOTUS_STAT_STANDBY)
	{
		return (esMODS_MIoctrl(mod_lotus, LOTUS_CMD_START_RECORD, parrot_data->herb_obj, NULL));
	}
	else
	{	
		return EPDK_FAIL;
	}
}

__s32   parrot_halt_record(__hdle hdle)
{
    __parrot_data_t     *parrot_data;
    __lotus_status_t     parrot_status;
    
	if(hdle == NULL)
	{
		return EPDK_FAIL;
	}
    parrot_data = (__parrot_data_t *)hdle;
    
    parrot_status = (__lotus_status_t)esMODS_MIoctrl(mod_lotus, LOTUS_CMD_GET_STATUS, parrot_data->herb_obj, NULL);
	if(parrot_status == LOTUS_STAT_RECORD)
	{
		return (esMODS_MIoctrl(mod_lotus, LOTUS_CMD_HOLD_ON, parrot_data->herb_obj, NULL));
	}
	else
	{	
		return EPDK_FAIL;
	} 
}

__s32   parrot_stop_record(__hdle hdle)
{
    __parrot_data_t     *parrot_data;
    __lotus_status_t     parrot_status;
    
	if(hdle == NULL)
	{
		return EPDK_FAIL;
	}
    parrot_data = (__parrot_data_t *)hdle;
    
    parrot_status = (__lotus_status_t)esMODS_MIoctrl(mod_lotus, LOTUS_CMD_GET_STATUS, parrot_data->herb_obj, NULL);
	if(parrot_status == LOTUS_STAT_RECORD)
	{
		return (esMODS_MIoctrl(mod_lotus, LOTUS_CMD_STOP_RECORD, parrot_data->herb_obj, NULL));
	}
	else
	{	
		return EPDK_FAIL;
	} 
}

__s32   parrot_get_record_time(__hdle hdle)
{    
    __parrot_data_t     *parrot_data;
    __lotus_status_t     parrot_status;
    
	if(hdle == NULL)
	{
		return 0;
	}
    parrot_data = (__parrot_data_t *)hdle;
    
    parrot_status = (__lotus_status_t)esMODS_MIoctrl(mod_lotus, LOTUS_CMD_GET_STATUS, parrot_data->herb_obj, NULL);
	if(parrot_status == LOTUS_STAT_RECORD)
	{
	    return (esMODS_MIoctrl(mod_lotus, LOTUS_CMD_GET_CUR_TIME, parrot_data->herb_obj, NULL));
	}
	else
	{	
		return 0;
	} 
}

__s32   parrot_take_picture(__hdle hdle, __u32 cnt, FB * fb)
{
    __parrot_data_t     *parrot_data;
    __lotus_status_t     parrot_status;
    
	if(hdle == NULL)
	{
		return EPDK_FAIL;
	}
    parrot_data = (__parrot_data_t *)hdle;
    
    parrot_status = (__lotus_status_t)esMODS_MIoctrl(mod_lotus, LOTUS_CMD_GET_STATUS, parrot_data->herb_obj, NULL);
	if(parrot_status == LOTUS_STAT_STANDBY)
	{	
		return (esMODS_MIoctrl(mod_lotus, LOTUS_CMD_TAKE_IMAGE, parrot_data->herb_obj, fb));
	}
	else
	{	
		return EPDK_FAIL;
	}
}

__s32   parrot_set_md_gate(__hdle hdle, __s32 enable)
{
    __parrot_data_t     *parrot_data;
    
	if(hdle == NULL)
	{
		return EPDK_FAIL;
	}
    parrot_data = (__parrot_data_t *)hdle;
      
	if(enable)
	{
		return (esMODS_MIoctrl(mod_lotus, LOTUS_CMD_MD_ONOFF, parrot_data->herb_obj, (void *)1));
	}
	
	else
	{
		return (esMODS_MIoctrl(mod_lotus, LOTUS_CMD_MD_ONOFF, parrot_data->herb_obj, (void *)0));
	}
}

__s32   parrot_set_md_para(__hdle hdle, __lotus_motion_dectect_para_t * para)
{
    __parrot_data_t     *parrot_data;
    
	if(hdle == NULL)
	{
		return EPDK_FAIL;
	}
    parrot_data = (__parrot_data_t *)hdle;
    
	return esMODS_MIoctrl(mod_lotus, LOTUS_CMD_SET_MD_PARA, parrot_data->herb_obj, para);
}

__s32   parrot_get_md_result(__hdle hdle)
{
	__s32				ret, result;
    __parrot_data_t     *parrot_data;
    
	if(hdle == NULL)
	{
		return EPDK_FAIL;
	}
    parrot_data = (__parrot_data_t *)hdle;
    ret = esMODS_MIoctrl(mod_lotus, LOTUS_CMD_GET_MD_DETECT, parrot_data->herb_obj, (void *)&result);
    if(ret == EPDK_OK)
    {
        return result;
    }    
    return -1;
}


__s32   parrot_request_preview_layer(RECT rect, __s32 pipe, __s32 prio)
{    
    __s32               hlayer; 

    __u32                   arg[3];
    __disp_fb_t             image_fb_para = {0};
	__disp_layer_info_t     image_layer_info = {0};
	
	image_fb_para.size.height      	= 0;                    // DONT'T CARE
	image_fb_para.size.width       	= 0;                    // DONT'T CARE
	image_fb_para.addr[0]          	= NULL;
	image_fb_para.format         	= DISP_FORMAT_RGB565;   // DONT'T CARE 
	image_fb_para.seq     			= DISP_SEQ_ARGB;  	    // DONT'T CARE
	image_fb_para.mode      		= 0;                    // DONT'T CARE
	image_fb_para.br_swap       	= 0;                    // DONT'T CARE
	image_fb_para.cs_mode 			= NULL;	

	image_layer_info.mode			= DISP_LAYER_WORK_MODE_NORMAL;      
    image_layer_info.pipe			= pipe;       
    image_layer_info.prio			= prio;       
    image_layer_info.alpha_en		= 0;   
    image_layer_info.alpha_val		= 255;  
    image_layer_info.ck_enable		= 0;  
    image_layer_info.src_win.x      = 0;
	image_layer_info.src_win.y      = 0;
	image_layer_info.src_win.width  = rect.width ;
	image_layer_info.src_win.height = rect.height;
    image_layer_info.scn_win.x      = rect.x     ;
	image_layer_info.scn_win.y      = rect.y     ;
	image_layer_info.scn_win.width  = rect.width ;
	image_layer_info.scn_win.height = rect.height;   
    image_layer_info.fb				= image_fb_para;

	arg[0] = DISP_LAYER_WORK_MODE_NORMAL;
	arg[1] = 0;
	arg[2] = 0;
	hlayer = eLIBs_fioctrl(parrot_disp, DISP_CMD_LAYER_REQUEST, 0, (void *)arg);
	if(hlayer == NULL)
	{
		__log("Error in applying for the preview layer.\n");
		return NULL;
	}
	
	arg[0] = hlayer;
	arg[1] = (__u32)&image_layer_info;
	arg[2] = 0;
	eLIBs_fioctrl(parrot_disp, DISP_CMD_LAYER_SET_PARA, 0, (void *)arg);
	
	arg[0] = hlayer;
	arg[1] = 0;
	arg[2] = 0;
	eLIBs_fioctrl(parrot_disp, DISP_CMD_LAYER_TOP, 0, (void *)arg);


	return hlayer;
}

__s32   parrot_release_preview_layer(__s32  hlayer)
{
    __u32       arg[3];

	if(hlayer)
	{
		arg[0] = hlayer;
		arg[1] = 0;
		arg[2] = 0;
		eLIBs_fioctrl(parrot_disp, DISP_CMD_LAYER_RELEASE, 0, (void *)arg);
		hlayer = NULL;

	}
	
	return EPDK_OK;
}

__s32   parrot_set_show_layer(__hdle hdle, __s32 hlayer)
{
    __parrot_data_t     *parrot_data;
    
	if(hdle == NULL)
	{
		return EPDK_FAIL;
	}
    parrot_data = (__parrot_data_t *)hdle;
    parrot_data->herb_lyr = hlayer;
    
    if(EPDK_FAIL == esMODS_MIoctrl(mod_lotus, LOTUS_CMD_SET_VID_LAYERHDL, parrot_data->herb_obj, (void *)hlayer))
    {
    	return EPDK_FAIL;
   	}
    esMODS_MIoctrl(mod_lotus, LOTUS_CMD_SWITCH_VID_SHOW, parrot_data->herb_obj, (void *)1);
    
	return EPDK_OK;
}

__s32   parrot_set_show_switch(__hdle hdle, __s32 enable)
{
    __parrot_data_t     *parrot_data;
    
	if(hdle == NULL)
	{
		return EPDK_FAIL;
	}
	parrot_data = (__parrot_data_t *)hdle;
	
	if(enable == 0)
	{
	    esMODS_MIoctrl(mod_lotus, LOTUS_CMD_SWITCH_VID_SHOW, parrot_data->herb_obj, (void *)0);
	}
	else
	{
	    esMODS_MIoctrl(mod_lotus, LOTUS_CMD_SWITCH_VID_SHOW, parrot_data->herb_obj, (void *)1);
    }
    
    return EPDK_OK;
}

__s32   parrot_set_show_mode(__hdle hdle, __lotus_vid_win_ratio_mode_t mode)
{
    __parrot_data_t     *parrot_data;
    
	if(hdle == NULL)
	{
		return EPDK_FAIL;
	}
    parrot_data = (__parrot_data_t *)hdle;
    
	return esMODS_MIoctrl(mod_lotus, LOTUS_CMD_SET_VID_SHOW_MODE, parrot_data->herb_obj, (void *)mode);
}

__s32   parrot_set_show_rect(__hdle hdle, RECT rect)
{
    __parrot_data_t     *parrot_data;
    
	if(hdle == NULL)
	{
		return EPDK_FAIL;
	}
    parrot_data = (__parrot_data_t *)hdle;
    
	return esMODS_MIoctrl(mod_lotus, LOTUS_CMD_SET_VID_WINDOW, parrot_data->herb_obj, (void *)&(rect));   
}

__s32   parrot_set_ce(__hdle hdle, __csi_color_effect_t ce)
{
    __parrot_data_t     *parrot_data;
    
	if(hdle == NULL)
	{
		return EPDK_FAIL;
	}
    parrot_data = (__parrot_data_t *)hdle;
    
    return esMODS_MIoctrl(mod_lotus, LOTUS_CMD_SET_COLOR_EFFECT, parrot_data->herb_obj, (void *)ce);
}

__s32   parrot_set_awb(__hdle hdle, __csi_awb_t awb)
{
    __parrot_data_t     *parrot_data;
    
	if(hdle == NULL)
	{
		return EPDK_FAIL;
	}
    parrot_data = (__parrot_data_t *)hdle;
    
    return esMODS_MIoctrl(mod_lotus, LOTUS_CMD_SET_AWB, parrot_data->herb_obj, (void *)awb);
}

__s32   parrot_set_ae(__hdle hdle, __csi_ae_t ae)
{
    __parrot_data_t     *parrot_data;
    
	if(hdle == NULL)
	{
		return EPDK_FAIL;
	}
    parrot_data = (__parrot_data_t *)hdle;
    
    return esMODS_MIoctrl(mod_lotus, LOTUS_CMD_SET_AE, parrot_data->herb_obj, (void *)ae);
}

__s32   parrot_set_band(__hdle hdle, __csi_band_t band)
{
    __parrot_data_t     *parrot_data;
    
	if(hdle == NULL)
	{
		return EPDK_FAIL;
	}
    parrot_data = (__parrot_data_t *)hdle;
    
    return esMODS_MIoctrl(mod_lotus, LOTUS_CMD_SET_BAND, parrot_data->herb_obj, (void *)band);
}

__csi_color_effect_t   parrot_get_ce(__hdle hdle)
{
    __parrot_data_t     *parrot_data;
    
	if(hdle == NULL)
	{
		return (__csi_color_effect_t)0;
	}
    parrot_data = (__parrot_data_t *)hdle;
    
    return (__csi_color_effect_t)esMODS_MIoctrl(mod_lotus, LOTUS_CMD_GET_COLOR_EFFECT, parrot_data->herb_obj, NULL);
}

__csi_awb_t   parrot_get_awb(__hdle hdle)
{
    __parrot_data_t     *parrot_data;
    
	if(hdle == NULL)
	{
		return (__csi_awb_t)0;
	}
    parrot_data = (__parrot_data_t *)hdle;
    
    return (__csi_awb_t)esMODS_MIoctrl(mod_lotus, LOTUS_CMD_GET_AWB, parrot_data->herb_obj, NULL);
}

__csi_ae_t   parrot_get_ae(__hdle hdle)
{
    __parrot_data_t     *parrot_data;
    
	if(hdle == NULL)
	{
		return (__csi_ae_t)0;
	}
    parrot_data = (__parrot_data_t *)hdle;
    
    return (__csi_ae_t)esMODS_MIoctrl(mod_lotus, LOTUS_CMD_GET_AE, parrot_data->herb_obj, NULL);
}

__csi_band_t   parrot_get_band(__hdle hdle)
{
    __parrot_data_t     *parrot_data;
    
	if(hdle == NULL)
	{
		return (__csi_band_t)0;
	}
    parrot_data = (__parrot_data_t *)hdle;
    
    return (__csi_band_t)esMODS_MIoctrl(mod_lotus, LOTUS_CMD_GET_BAND, parrot_data->herb_obj, NULL);
}

__s32   parrot_add_overlay_src(__hdle hdle, __lotus_overlay_src_init_t * src)
{
    __parrot_data_t     *parrot_data;
    
	if(hdle == NULL)
	{
		return EPDK_FAIL;
	}
    parrot_data = (__parrot_data_t *)hdle;
    
    return (__csi_ae_t)esMODS_MIoctrl(mod_lotus, LOTUS_CMD_ADD_OVERLAY_SRC, parrot_data->herb_obj, (void *)src);
}

__s32   parrot_show_overlay_blk(__hdle hdle, __lotus_overlay_dis_par_t * para)
{
    __parrot_data_t     *parrot_data;
    
	if(hdle == NULL)
	{
		return EPDK_FAIL;
	}
    parrot_data = (__parrot_data_t *)hdle;
    
    return (__csi_ae_t)esMODS_MIoctrl(mod_lotus, LOTUS_CMD_SHOW_OVERLAY_BLK, parrot_data->herb_obj, (void *)para);
}

__s32   parrot_set_hit_holding_time(__hdle hdle, __s32 time)
{
    __parrot_data_t     *parrot_data;
    
	if(hdle == NULL)
	{
		return EPDK_FAIL;
	}
    parrot_data = (__parrot_data_t *)hdle;
    
    return esMODS_MIoctrl(mod_lotus, LOTUS_CMD_SET_HOLDING_TIME, parrot_data->herb_obj, (void *)time);
}

__s32   parrot_seek_hit_seek_time(__hdle hdle, __s32 time)
{
    __parrot_data_t     *parrot_data;
    
	if(hdle == NULL)
	{
		return EPDK_FAIL;
	}
    parrot_data = (__parrot_data_t *)hdle;
    
    return esMODS_MIoctrl(mod_lotus, LOTUS_CMD_SEEK_TIME, parrot_data->herb_obj, (void *)time);
}

__s32	parrot_set_record_mute(__hdle hdle, __s32 state)
{
    __parrot_data_t     *parrot_data;
    
	if(hdle == NULL)
	{
		return EPDK_FAIL;
	}
    parrot_data = (__parrot_data_t *)hdle;
    
    return esMODS_MIoctrl(mod_lotus, LOTUS_CMD_MUTE, parrot_data->herb_obj, (void *)state);
}

__s32   parrot_set_output_mode(__hdle hdle, __lotus_muxer_type_t type)
{
    __parrot_data_t     *parrot_data;
    
	if(hdle == NULL)
	{
		return EPDK_FAIL;
	}
    parrot_data = (__parrot_data_t *)hdle;
    
    return esMODS_MIoctrl(mod_lotus, LOTUS_CMD_SET_MUXER_OUT, parrot_data->herb_obj, (void *)type);
}

__s32   parrot_set_sync_callback(__hdle hdle, __lotus_sync_callback_t  *sync_callback)
{
    __parrot_data_t     *parrot_data;
    
	if(hdle == NULL)
	{
		return EPDK_FAIL;
	}
    parrot_data = (__parrot_data_t *)hdle;
    
    return esMODS_MIoctrl(mod_lotus, LOTUS_CMD_REG_SYNC_CB, parrot_data->herb_obj, (void *)sync_callback);
}



#define TEST_REC_NAME		"g:\\abc_"
#define TEST_REC_NAME_EXT	".avi"

#define TEST_REC_NUM_MAX	20
#define TEST_REC_TIME		60
static __u32	g_record_cnt = 0;
static __hdle   dv_h[2] = 0;
static __s32    dv_hlayer[2] = 0;

typedef struct
{
   __s32 bPreview;
   __csi_mode_all_t  csi_mode_all;
} cvr_ctrl_t;


__s32 dtv_cvr_set_record(__csi_mode_t*      csi_mode,__u32 sensor_id)
{
	char 							str[10] = {0};
    __lotus_media_file_inf			media_info = {0};
	__u32							delete_file_index;
	ES_FILE* pfile ;
	__u32					quality = 40 ;
	
    eLIBs_strcpy(media_info.file_path,TEST_REC_NAME);
    eLIBs_sprintf(str, "%04d", ((quality*100)|g_record_cnt));
    eLIBs_strcat(media_info.file_path,str);
    eLIBs_strcat(media_info.file_path,TEST_REC_NAME_EXT);

	g_record_cnt = (g_record_cnt+1)%TEST_REC_NUM_MAX;

    __log("new file is : %s g_record_cnt=%d\n", media_info.file_path,g_record_cnt);

	media_info.uFileFmt                	= SW_VIDEO_FORMAT_AVI;
    media_info.audio_inf.uAbsFmt   		= SW_CODEC_TYPE_NONE;
    media_info.audio_inf.uVolume        = MAX_VOLUME_VAL;//
    media_info.audio_inf.uChn           = LOTUS_AUDIO_CHN_MONO;//
    media_info.audio_inf.uSampWidth     = LOTUS_AUDIO_SAMP_WIDTH_16;
    media_info.audio_inf.uAudioBps      = 80000;
    media_info.audio_inf.uAudioSampRate = 22050;// 8000 48k 24k(22.05k在录像和音乐同时工作时，声音无变形)
    media_info.audio_inf.uAudioSource   = LOTUS_AUDIO_SRC_MIC;
    media_info.audio_inf.frame_style    = 1;
	__msg("csi_mode->size.width = %d %d %d\n",csi_mode->size.width, csi_mode->size.height,csi_mode->frame_rate);
    media_info.video_inf.uVbsFmt        = SW_CODEC_TYPE_MJPEG;
    media_info.video_inf.nWidth         = ENCODE_PIXEL_SIZE_W;//csi_mode->size.width;
    media_info.video_inf.nHeight        = ENCODE_PIXEL_SIZE_H;//csi_mode->size.height;
	media_info.video_inf.srcWidth = csi_mode->size.width;
    media_info.video_inf.srcHeight = csi_mode->size.height;

    media_info.video_inf.uVideoFrmRate  = csi_mode->frame_rate;
    media_info.video_inf.nVideoBps      = 8000000;

    media_info.video_inf.color_format   = csi_mode->color_format;
    media_info.video_inf.color_space    = csi_mode->color_space;
    media_info.video_inf.qp_max         = 35; // 20
    media_info.video_inf.qp_min         = 20;  // 1
    media_info.video_inf.maxKeyInterval = 29; // 10  29
    media_info.video_inf.jpgenc_quality = quality; 
	media_info.video_inf.nVideoBps	= 8000000; 
	media_info.video_inf.videoTimes	= TEST_REC_TIME ;
	delete_file_index = (g_record_cnt+TEST_REC_NUM_MAX/2) % TEST_REC_NUM_MAX;

	
    parrot_set_media_file(dv_h[sensor_id], &media_info);
	#if 0
	eLIBs_memset(str,0,sizeof(str));
	eLIBs_memset(media_info.file_path,0,sizeof(media_info.file_path));
	eLIBs_strcpy(media_info.file_path,TEST_REC_NAME);
    eLIBs_sprintf(str, "%04d", delete_file_index);
    eLIBs_strcat(media_info.file_path,str);
    eLIBs_strcat(media_info.file_path,TEST_REC_NAME_EXT);
	pfile = eLIBs_fopen(media_info.file_path,"rb");
	if(pfile)
	{
		__log("start to delete file %s\n",media_info.file_path);
		eLIBs_fclose(pfile);
		eLIBs_remove(media_info.file_path);
	}
	#endif
	return 0 ;

}

__s32 mjpeg_test_start(void)
{
	cvr_ctrl_t APPcvr[2];
	RECT      rect;
	ES_FILE*  h_pdisp;
	__u32 i ;
	__u32	rec_time ;
	__s32	output_type ;
	__u32	arg[3] ;
	
	__log("=======================================================================\n");
	__log("=========================| mjpeg_test_start |==========================\n");
	__log("=======================================================================\n");

	parrot_init();

	dv_h[0] = parrot_open(0);
	if(dv_h[0] == NULL)
	{
		__log("init parrot0 fail\n");        
		return EPDK_FAIL;
	}
	dv_h[1] = parrot_open(1);
	if(dv_h[1] == NULL)
	{
		__log("init parrot1 fail\n");        
		return EPDK_FAIL;
	}
    // 创建预览图层
    h_pdisp = eLIBs_fopen("b:\\DISP\\DISPLAY", "r+");
    if(h_pdisp == NULL)
    {
        __log("can not open display driver\n");
        parrot_uninit();        
        return EPDK_FAIL;
    }

#if DISPLAY_TYPE==TV_TYPE
    output_type = eLIBs_fioctrl(h_pdisp, DISP_CMD_GET_OUTPUT_TYPE, 0, NULL);
	__log("output type is %d , switch to tv mode\n",output_type);
	if( DISP_OUTPUT_TYPE_TV!= output_type )
	{
		eLIBs_fioctrl(h_pdisp, DISP_CMD_LCD_OFF, 0, 0);

		// 设置DAC0 的source 为0 ,表示 cvbs (具体跟电路图相关)
		arg[0] = 0;
		arg[1] = 0;
		arg[2] = 0;
	    eLIBs_fioctrl(h_pdisp, DISP_CMD_TV_SET_DAC_SOURCE, 0, (void *)arg);

		arg[0] = DISP_TV_MOD_NTSC;
		arg[1] = 0;
		arg[2] = 0;
		eLIBs_fioctrl(h_pdisp, DISP_CMD_TV_SET_MODE, 0, (void *)arg);
		eLIBs_fioctrl(h_pdisp, DISP_CMD_TV_ON, 0, 0);
	}
	__log( "display output type is tv \n" );
#else if DISPLAY_TYPE==LCD_TYPE
	__log("output type is %d , switch to lcd mode\n",output_type);
    output_type = eLIBs_fioctrl(h_pdisp, DISP_CMD_GET_OUTPUT_TYPE, 0, NULL);
	if( DISP_OUTPUT_TYPE_LCD != output_type )
	{
		eLIBs_fioctrl(h_pdisp, DISP_CMD_TV_OFF, 0, 0);
		eLIBs_fioctrl(h_pdisp, DISP_CMD_LCD_ON, 0, 0);
	}
	__log( "display output type is lcd \n" );
#endif

    rect.x      = 0;
    rect.y      = 0;
    rect.width  = eLIBs_fioctrl(h_pdisp, DISP_CMD_SCN_GET_WIDTH, 0, NULL);
    rect.height = eLIBs_fioctrl(h_pdisp, DISP_CMD_SCN_GET_HEIGHT , 0, NULL);
    dv_hlayer[0] = parrot_request_preview_layer(rect, 0, 0xff);
	__log("dv_hlayer[0]=%d\n",dv_hlayer[0]);
	parrot_set_show_layer(dv_h[0], dv_hlayer[0]);
    parrot_set_show_mode(dv_h[0], LOTUS_VID_WIN_FULLSCN);
	parrot_set_mode(dv_h[0], LOTUS_RECORD_MODE_CAMCORDER);
    parrot_set_show_rect(dv_h[0], rect);    
#if PIP_ENABLE
    rect.width  = 240;
    rect.height = 160;
	rect.x      = eLIBs_fioctrl(h_pdisp, DISP_CMD_SCN_GET_WIDTH, 0, NULL) - rect.width;
    rect.y      = 0;
    eLIBs_fclose(h_pdisp);
    dv_hlayer[1] = parrot_request_preview_layer(rect, 1, 0xff);
	parrot_set_show_layer(dv_h[1], dv_hlayer[1]);
	__log("dv_hlayer[1]=%d\n",dv_hlayer[1]);
	
    parrot_set_show_mode(dv_h[1], LOTUS_VID_WIN_FULLSCN);
	parrot_set_mode(dv_h[1], LOTUS_RECORD_MODE_CAMCORDER);
    parrot_set_show_rect(dv_h[1], rect);   
#endif

	parrot_get_csi_mode(dv_h[0], &APPcvr[0].csi_mode_all);
	for( i=0; i<APPcvr[0].csi_mode_all.number; i++ )
	{
		__log("pAPPcvr.csi_mode_all.csi_mode[%d].color_format is %d %d %d %d\n",i,APPcvr[0].csi_mode_all.csi_mode[i].color_format,
	                                                                     APPcvr[0].csi_mode_all.csi_mode[i].size.width,
	                                                                    APPcvr[0].csi_mode_all.csi_mode[i].size.height,
	                                                                    APPcvr[0].csi_mode_all.csi_mode[i].csi_buf_scrab_mode);
	}
	//__log("please input select csi mode:%d", (i=__getc() - '0'));
#if VIDEO_SOURE==CSIIN_SOURCE
	i = 2 ;
#else VIDEO_SOURE==TVIN_SOURCE
	i = 0 ;
#endif

	dtv_cvr_set_record(&APPcvr[0].csi_mode_all.csi_mode[i],0);
	parrot_set_preview_mode(dv_h[0], &APPcvr[0].csi_mode_all.csi_mode[i]);
	parrot_start_preview(dv_h[0]);

	APPcvr[0].bPreview = EPDK_TRUE;

	parrot_get_csi_mode(dv_h[1], &APPcvr[1].csi_mode_all);
	for( i=0; i<APPcvr[1].csi_mode_all.number; i++ )
	{
		__log("pAPPcvr.csi_mode_all.csi_mode[%d].color_format is %d %d %d %d\n",i,APPcvr[1].csi_mode_all.csi_mode[i].color_format,
	                                                                     APPcvr[1].csi_mode_all.csi_mode[i].size.width,
	                                                                    APPcvr[1].csi_mode_all.csi_mode[i].size.height,
	                                                                    APPcvr[1].csi_mode_all.csi_mode[i].csi_buf_scrab_mode);
	}
	//__log("please input select csi mode:%d", (i=__getc() - '0'));
#if VIDEO_SOURE==CSIIN_SOURCE
	i = 2 ;
#else VIDEO_SOURE==TVIN_SOURCE
	i = 0 ;
#endif

	dtv_cvr_set_record(&APPcvr[1].csi_mode_all.csi_mode[i],1);
	parrot_set_preview_mode(dv_h[1], &APPcvr[1].csi_mode_all.csi_mode[i]);
	parrot_start_preview(dv_h[1]);

	APPcvr[1].bPreview = EPDK_TRUE;

	parrot_start_record(dv_h[0]);
	parrot_start_record(dv_h[1]);
	rec_time = 0;
	while(1)
	{
		rec_time = parrot_get_record_time(dv_h[0]);
		__log("object 0 record time = %d ...............\n",rec_time);
		rec_time = parrot_get_record_time(dv_h[1]);
		__log("object 1 record time = %d ...............\n",rec_time);
		if(rec_time>TEST_REC_TIME*1000)
		{
			__log("halt record ...............\n");
			parrot_halt_record(dv_h[0]);
			dtv_cvr_set_record(&APPcvr[0].csi_mode_all.csi_mode[i],0);
			parrot_start_record(dv_h[0]);

			parrot_halt_record(dv_h[1]);
			dtv_cvr_set_record(&APPcvr[0].csi_mode_all.csi_mode[i],1);
			parrot_start_record(dv_h[1]);

		}
		__log("record time = %d ...............\n",rec_time);
		esKRNL_TimeDly(1000);
	}
	 eLIBs_fclose(h_pdisp);

	return EPDK_OK;
}


__s32 mjpeg_test_exit(void)
{
	parrot_stop_preview(dv_h[0]);
	parrot_release_preview_layer(dv_hlayer[0]);
	parrot_close(dv_h[0]);

    parrot_uninit();
    
	return EPDK_OK;
}
