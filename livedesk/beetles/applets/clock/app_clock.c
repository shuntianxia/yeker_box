/*********************************************************************
*********************************************************************/
#include "app_clock_i.h"

#define LCD_WIDTH 		720
#define LCD_HEIGHT 		720

/************************************************************
*************************************************************/
static H_LYR clock_32bpp_layer_create(RECT LayerRect, __u8 pipe)
{
	H_LYR layer = NULL;
	
	FB  fb =
	{
	    {0, 0},                                   		/* size      */
	    {0, 0, 0},                                      /* buffer    */
	    {FB_TYPE_RGB, {PIXEL_COLOR_ARGB8888, 0, (__rgb_seq_t)0}},    /* fmt       */
	    
	};

	__disp_layer_para_t lstlyr =
	{
	    DISP_LAYER_WORK_MODE_NORMAL,
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
        "APP_CLOCK",                       
      	NULL,                                 
      	GUI_LYRWIN_STA_SUSPEND,               
      	GUI_LYRWIN_NORMAL                     
    };	
	
	
    fb.size.width		= LayerRect.width;            
    fb.size.height		= LayerRect.height;	        
    fb.fmt.fmt.rgb.pixelfmt = PIXEL_COLOR_ARGB8888; //PIXEL_MONO_8BPP; //PIXEL_COLOR_ARGB8888;
	
	lstlyr.src_win.x  		= 0;
	lstlyr.src_win.y  		= 0;

	if (LayerRect.width > LCD_WIDTH)
		lstlyr.src_win.width  	= LCD_WIDTH;
	else 
		lstlyr.src_win.width 	= LayerRect.width;

	lstlyr.src_win.height 	= LayerRect.height;
	
	lstlyr.scn_win.x		= LayerRect.x;
	lstlyr.scn_win.y		= LayerRect.y;
	
	if (LayerRect.width > LCD_WIDTH)
		lstlyr.scn_win.width  	= LCD_WIDTH;
	else 
		lstlyr.scn_win.width 	= LayerRect.width;

	lstlyr.scn_win.height 	= LayerRect.height;
	
	lstlyr.pipe = pipe;		//1
	lstlyr.fb = &fb;
	lyrcreate_info.lyrpara = &lstlyr;
	
	layer = GUI_LyrWinCreate(&lyrcreate_info);
	
	if( !layer )
	{
		__err("app clock layer create error !\n");
	}
	return layer;	
}
/*************************************************************
***************************************************************/
static __s32 __app_clock_init(__gui_msg_t* msg)
{
	app_clock_t *clock_para;
	RECT LayerRect;
	root_para_t *para;
	reg_system_para_t* sys_para;

	para = (root_para_t *)GUI_WinGetAttr(msg->h_deswin);
	clock_para = (app_clock_t *)esMEMS_Balloc(sizeof(app_clock_t));
	if (clock_para == NULL)
		return EPDK_FAIL;
	eLIBs_memset(clock_para, 0, sizeof(app_clock_t));
	GUI_WinSetAddData(msg->h_deswin, (__s32)clock_para);
	
	clock_para->font = para->font;
	clock_para->h_parent = msg->h_deswin;
	LayerRect.x = CLOCK_LAYER_X;
	LayerRect.y = CLOCK_LAYER_Y;
	LayerRect.width = CLOCK_LAYER_W;
	LayerRect.height = CLOCK_LAYER_H;
	clock_para->lyr = clock_32bpp_layer_create(LayerRect, 1);
	clock_para->frm_win = app_clock_frm_create(clock_para);
	GUI_LyrWinSetSta(clock_para->lyr, GUI_LYRWIN_STA_ON);
	GUI_LyrWinSetTop(clock_para->lyr);
	GUI_WinSetActiveChild(clock_para->frm_win);	

    sys_para = (reg_system_para_t*)dsk_reg_get_para_by_app(REG_APP_SYSTEM);
    g_set_close_scn_time(sys_para->backlightoff);
	g_enable_close_scn();
	return EPDK_OK;
}
/*************************************************************
***************************************************************/
static __s32 __app_clock_uninit(__gui_msg_t* msg)
{
	app_clock_t *clock_para;
	
	clock_para = (app_clock_t *)GUI_WinGetAddData(msg->h_deswin);
	
	g_disable_close_scn();
	GUI_LyrWinDelete(clock_para->lyr);
	esMEMS_Bfree(clock_para, sizeof(app_clock_t));
	clock_para = NULL;
	return EPDK_OK;
}
/************************************************************
*************************************************************/
static __s32 __app_clock_proc(__gui_msg_t* msg)
{
	switch(msg->id)
	{
		case GUI_MSG_CREATE:
		{
			gscene_bgd_set_state(BGD_STATUS_SHOW);
			font_res_init();
			__app_clock_init(msg);
			break;
		}
		case GUI_MSG_KEY:
		{
			break;
		}
		case GUI_MSG_DESTROY:
		{
			font_res_deinit();
			__app_clock_uninit(msg);
			gscene_bgd_set_state(BGD_STATUS_HIDE);
			break;
		}
		case GUI_MSG_CLOSE:
		{
			GUI_ManWinDelete(msg->h_deswin);
			break;
		}
		case GUI_MSG_TOUCH:
		{
			break;
		}
		case GUI_MSG_COMMAND:
		{
			return EPDK_OK;
		}
		default:
			break;
	}	
	return GUI_ManWinDefaultProc(msg); 
}

H_WIN app_clock_create(root_para_t *para)
{
	__gui_manwincreate_para_t create_info;
    H_WIN hManWin;

	__inf("****************************************************************************************\n");
	__inf("********  enter clock app  **************\n");
	__inf("****************************************************************************************\n");
	
	eLIBs_memset(&create_info, 0, sizeof(__gui_manwincreate_para_t));
	
	create_info.name            = APP_CLOCK;
	create_info.hParent         = para->h_parent;
	create_info.ManWindowProc   = (__pGUI_WIN_CB)esKRNL_GetCallBack((__pCBK_t)__app_clock_proc); 
	create_info.attr            = (void*)para;
	create_info.id				= APP_CLOCK_ID;
	create_info.hHosting        = NULL;

    hManWin = GUI_ManWinCreate(&create_info);
    __msg("app_clock_create, hManWin=%x\n", hManWin);

    return hManWin;
}
