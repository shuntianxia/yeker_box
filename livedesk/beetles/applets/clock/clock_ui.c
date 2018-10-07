/***************************************************************
******************************************************************/
#include "app_clock_i.h"

#define TIME_X		78
#define TIME_Y		38
#define TIME_W		562
#define TIME_H		562

#define POINTER_OFFSET  0

#define POINTER_W		360
#define POINTER_H		360

#define DATE_X		408
#define DATE_Y		677

#define WEEK_X		544
#define WEEK_Y		675


#define CLOCK_TIME_OUT_ID 0x1000

static __s32 hour_id_tab[] = 
{
	ID_CLOCK_H1_BMP,
	ID_CLOCK_H2_BMP,
	ID_CLOCK_H3_BMP,
	ID_CLOCK_H4_BMP,
	ID_CLOCK_H5_BMP,
	ID_CLOCK_H6_BMP,
	ID_CLOCK_H7_BMP,
	ID_CLOCK_H8_BMP,
	ID_CLOCK_H9_BMP,
	ID_CLOCK_H10_BMP,
	ID_CLOCK_H11_BMP,
	ID_CLOCK_H12_BMP,
	ID_CLOCK_H13_BMP,
	ID_CLOCK_H14_BMP,
	ID_CLOCK_H15_BMP,
	ID_CLOCK_H16_BMP,
};

static __s32 minute_id_tab[] = 
{
	ID_CLOCK_M1_BMP,
	ID_CLOCK_M2_BMP,
	ID_CLOCK_M3_BMP,
	ID_CLOCK_M4_BMP,
	ID_CLOCK_M5_BMP,
	ID_CLOCK_M6_BMP,
	ID_CLOCK_M7_BMP,
	ID_CLOCK_M8_BMP,
	ID_CLOCK_M9_BMP,
	ID_CLOCK_M10_BMP,
	ID_CLOCK_M11_BMP,
	ID_CLOCK_M12_BMP,
	ID_CLOCK_M13_BMP,
	ID_CLOCK_M14_BMP,
	ID_CLOCK_M15_BMP,
	ID_CLOCK_M16_BMP,
};

static __s32 second_id_tab[] = 
{
	ID_CLOCK_S1_BMP,
	ID_CLOCK_S2_BMP,
	ID_CLOCK_S3_BMP,
	ID_CLOCK_S4_BMP,
	ID_CLOCK_S5_BMP,
	ID_CLOCK_S6_BMP,
	ID_CLOCK_S7_BMP,
	ID_CLOCK_S8_BMP,
	ID_CLOCK_S9_BMP,
	ID_CLOCK_S10_BMP,
	ID_CLOCK_S11_BMP,
	ID_CLOCK_S12_BMP,
	ID_CLOCK_S13_BMP,
	ID_CLOCK_S14_BMP,
	ID_CLOCK_S15_BMP,
	ID_CLOCK_S16_BMP,
};

static __s32 time_num_tab[] =
{
	ID_CLOCK_TIME_T_0_BMP,
	ID_CLOCK_TIME_T_1_BMP,
	ID_CLOCK_TIME_T_2_BMP,
	ID_CLOCK_TIME_T_3_BMP,
	ID_CLOCK_TIME_T_4_BMP,
	ID_CLOCK_TIME_T_5_BMP,
	ID_CLOCK_TIME_T_6_BMP,
	ID_CLOCK_TIME_T_7_BMP,
	ID_CLOCK_TIME_T_8_BMP,
	ID_CLOCK_TIME_T_9_BMP,
	ID_CLOCK_TIME_COLON_BMP,
};

static __s32 week_tab[7] =
{
	STRING_CLOCK_SUNDAY,
	STRING_CLOCK_MONDAY,
	STRING_CLOCK_TUESDAY,
	STRING_CLOCK_WEDNESDAY,
	STRING_CLOCK_THURSDAY,
	STRING_CLOCK_FRIDAY,
	STRING_CLOCK_SATURDAY,
};

typedef struct tag_app_clock_ui
{
	H_LYR lyr;
	GUI_FONT *font;
	GUI_FONT *font1;

	load_bmp_res_t res_time_bg;
	load_bmp_res_t res_centor_point;
	load_bmp_res_t res_time_num[11];
	load_bmp_res_t res_date_hor;
	//load_bmp_res_t res_week[7];
	load_bmp_res_t res_hour[16];
	load_bmp_res_t res_minute[16];
	load_bmp_res_t res_second[16];
}app_clock_ui_t;
/*********************************************************
**********************************************************/
static __s32 app_clock_frm_init(__gui_msg_t *msg)
{
	app_clock_ui_t *clock_para;
	app_clock_t *para;
	__gui_msg_t exmsg;
	int i;

	para = (app_clock_t *)GUI_WinGetAttr(msg->h_deswin);
	clock_para = (app_clock_ui_t *)esMEMS_Balloc(sizeof(app_clock_ui_t));
	if (clock_para == NULL)
	{
		return EPDK_FAIL;
	}
	eLIBs_memset(clock_para, 0, sizeof(app_clock_ui_t));
	GUI_WinSetAddData(msg->h_deswin, (__s32)clock_para);

	clock_para->font = para->font;
	clock_para->lyr = para->lyr;

	create_bmp_res(ID_CLOCK_DIAL_PLATE_BMP, clock_para->res_time_bg);
	create_bmp_res(ID_CLOCK_DOT_BMP, clock_para->res_centor_point);
	for(i=0; i<16; i++)
	{
		create_bmp_res(hour_id_tab[i], clock_para->res_hour[i]);
		create_bmp_res(minute_id_tab[i], clock_para->res_minute[i]);
		create_bmp_res(second_id_tab[i], clock_para->res_second[i]);
	}

	for(i=0; i<11; i++)
	{
		create_bmp_res(time_num_tab[i], clock_para->res_time_num[i]);
	}
	create_bmp_res(ID_CLOCK_HLINE_BMP, clock_para->res_date_hor);

	GUI_SetTimer(msg->h_deswin, CLOCK_TIME_OUT_ID, 50, 0);
	return EPDK_OK;
}
/*********************************************************
***********************************************************/
static __s32 app_clock_frm_uninit(__gui_msg_t *msg)
{
	app_clock_ui_t *clock_para;
	__s32 i;

	clock_para = (app_clock_ui_t *)GUI_WinGetAddData(msg->h_deswin);
	
	GUI_KillTimer(msg->h_deswin, CLOCK_TIME_OUT_ID);

	destroy_bmp_res( clock_para->res_time_bg);
	destroy_bmp_res( clock_para->res_centor_point);
	for(i=0; i<16; i++)
	{
		destroy_bmp_res( clock_para->res_hour[i]);
		destroy_bmp_res( clock_para->res_minute[i]);
		destroy_bmp_res( clock_para->res_second[i]);
	}
	for(i=0; i<11; i++)
	{
		destroy_bmp_res(clock_para->res_time_num[i]);
	}
	destroy_bmp_res(clock_para->res_date_hor);
	return EPDK_OK;
}
/*****************************************************************
*****************************************************************/
static __s32 time_paint(__gui_msg_t *msg)
{
	app_clock_ui_t *para;
	__time_t t_time;
	__s32 i, j, x, y;
	__u8 *data;
	__u8 *hour_data;
	__u8 *minute_data;
	__u8 *second_data;
	__u8 *point;
	FB fb;

	para = (app_clock_ui_t *)GUI_WinGetAddData(msg->h_deswin);
	data = (__u8 *)esMEMS_Balloc(TIME_W*TIME_H*4);
	esTIME_GetTime(&t_time);

	hour_data = (__u8 *)esMEMS_Balloc(POINTER_W*POINTER_H*4);
	minute_data = (__u8 *)esMEMS_Balloc(POINTER_W*POINTER_H*4);
	second_data = (__u8 *)esMEMS_Balloc(POINTER_W*POINTER_H*4);
	point = (__u8 *)esMEMS_Balloc(84*84*4);

	GUI_LyrWinGetFB(para->lyr, &fb);
	esTIME_GetTime(&t_time);
	eLIBs_memcpy(data, ((__u8 *)(para->res_time_bg.data)+54), TIME_W*TIME_H*4);
	eLIBs_memcpy(point, ((__u8 *)(para->res_centor_point.data)+54), 84*84*4);

	if ((t_time.hour%12/3) == 0)
	{
		x = TIME_W-POINTER_W-POINTER_OFFSET;
		y = POINTER_OFFSET;
		eLIBs_memcpy(hour_data, ((__u8 *)(para->res_hour[t_time.hour%12%3*5+t_time.minute*5/60].data)+54), POINTER_W*POINTER_H*4);
	}
	else if ((t_time.hour%12/3) == 1)
	{
		x = (TIME_W)-POINTER_W-POINTER_OFFSET;
		y = (TIME_H)-POINTER_H-POINTER_OFFSET;
		for(i=0;i<POINTER_H; i++)
		{
			eLIBs_memcpy(hour_data+i*POINTER_W*4, ((__u8 *)(para->res_hour[15-(t_time.hour%12%3*5+t_time.minute*5/60)].data)+54+(POINTER_H-i-1)*POINTER_W*4), POINTER_W*4);
		}
	}
	else if ((t_time.hour%12/3) == 2)
	{
		x = POINTER_OFFSET;
		y = TIME_H-POINTER_H-POINTER_OFFSET;
		for(i=0;i<POINTER_H*POINTER_W; i++)
		{
			eLIBs_memcpy(hour_data+i*4, ((__u8 *)(para->res_hour[t_time.hour%12%3*5+t_time.minute*5/60].data)+54+(POINTER_H*POINTER_W-1-i)*4), 4);
		}
	}
	else 
	{
		x = POINTER_OFFSET;
		y = POINTER_OFFSET;
		for(i=0;i<POINTER_H; i++)
		{
			for(j=0; j<POINTER_W; j++)
			{
				eLIBs_memcpy(hour_data+i*POINTER_W*4+j*4, ((__u8 *)(para->res_hour[15-(t_time.hour%12%3*5+t_time.minute*5/60)].data)+54+((i+1)*POINTER_W-1-j)*4), 4);
			}
		}
	}
	
	for(i=0; i<POINTER_H; i++)
	{
		for(j=0; j<POINTER_W; j++)
		{
			if (*((__u8 *)(hour_data+(i*POINTER_W+j)*4)) != 0x00)
			{
				*((__u8 *)(data+((i+y)*TIME_W+x+j)*4+0)) = *((__u8 *)(hour_data+(i*POINTER_W+j)*4+0));
				*((__u8 *)(data+((i+y)*TIME_W+x+j)*4+1)) = *((__u8 *)(hour_data+(i*POINTER_W+j)*4+1));
				*((__u8 *)(data+((i+y)*TIME_W+x+j)*4+2)) = *((__u8 *)(hour_data+(i*POINTER_W+j)*4+2));
				*((__u8 *)(data+((i+y)*TIME_W+x+j)*4+3)) = *((__u8 *)(hour_data+(i*POINTER_W+j)*4+3));
			}
		}
	}
	if ((t_time.minute/15) == 0)
	{
		x = TIME_W-POINTER_W-POINTER_OFFSET;
		y = POINTER_OFFSET;
		eLIBs_memcpy(minute_data, ((__u8 *)(para->res_minute[t_time.minute%15].data)+54), POINTER_W*POINTER_H*4);
	}
	else if ((t_time.minute/15) == 1)
	{
		x = (TIME_W)-POINTER_W-POINTER_OFFSET;
		y = (TIME_H)-POINTER_H-POINTER_OFFSET;
		for(i=0;i<POINTER_H; i++)
		{
			eLIBs_memcpy(minute_data+i*POINTER_W*4, ((__u8 *)(para->res_minute[15-t_time.minute%15].data)+54+(POINTER_H-i-1)*POINTER_W*4), POINTER_W*4);
		}
	}
	else if ((t_time.minute/15) == 2)
	{
		x = POINTER_OFFSET;
		y = TIME_H-POINTER_H-POINTER_OFFSET;
		for(i=0;i<POINTER_H*POINTER_W; i++)
		{
			eLIBs_memcpy(minute_data+i*4, ((__u8 *)(para->res_minute[t_time.minute%15].data)+54+(POINTER_H*POINTER_W-1-i)*4), 4);
		}
	}
	else 
	{
		x = POINTER_OFFSET;
		y = POINTER_OFFSET;
		for(i=0;i<POINTER_H; i++)
		{
			for(j=0; j<POINTER_W; j++)
			{
				eLIBs_memcpy(minute_data+i*POINTER_W*4+j*4, ((__u8 *)(para->res_minute[15-t_time.minute%15].data)+54+((i+1)*POINTER_W-1-j)*4), 4);
			}
		}
	}

	for(i=0; i<POINTER_H; i++)
	{
		for(j=0; j<POINTER_W; j++)
		{
			if (*((__u8 *)(minute_data+(i*POINTER_W+j)*4)) != 0x00)
			{
				*((__u8 *)(data+((i+y)*TIME_W+x+j)*4+0)) = *((__u8 *)(minute_data+(i*POINTER_W+j)*4+0));
				*((__u8 *)(data+((i+y)*TIME_W+x+j)*4+1)) = *((__u8 *)(minute_data+(i*POINTER_W+j)*4+1));
				*((__u8 *)(data+((i+y)*TIME_W+x+j)*4+2)) = *((__u8 *)(minute_data+(i*POINTER_W+j)*4+2));
				*((__u8 *)(data+((i+y)*TIME_W+x+j)*4+3)) = *((__u8 *)(minute_data+(i*POINTER_W+j)*4+3));
			}
		}
	}

	if ((t_time.second/15) == 0)
	{
		x = TIME_W-POINTER_W-POINTER_OFFSET;
		y = POINTER_OFFSET;
		eLIBs_memcpy(second_data, ((__u8 *)(para->res_second[t_time.second%15].data)+54), POINTER_W*POINTER_H*4);
	}
	else if ((t_time.second/15) == 1)
	{
		x = (TIME_W)-POINTER_W-POINTER_OFFSET;
		y = (TIME_H)-POINTER_H-POINTER_OFFSET;
		for(i=0;i<POINTER_H; i++)
		{
			eLIBs_memcpy(second_data+i*POINTER_W*4, ((__u8 *)(para->res_second[15-t_time.second%15].data)+54+(POINTER_H-i-1)*POINTER_W*4), POINTER_W*4);
		}
	}
	else if ((t_time.second/15) == 2)
	{
		x = POINTER_OFFSET;
		y = TIME_H-POINTER_H-POINTER_OFFSET;
		for(i=0;i<POINTER_H*POINTER_W; i++)
		{
			eLIBs_memcpy(second_data+i*4, ((__u8 *)(para->res_second[t_time.second%15].data)+54+(POINTER_H*POINTER_W-1-i)*4), 4);
		}
	}
	else 
	{
		x = POINTER_OFFSET;
		y = POINTER_OFFSET;
		for(i=0;i<POINTER_H; i++)
		{
			for(j=0; j<POINTER_W; j++)
			{
				eLIBs_memcpy(second_data+i*POINTER_W*4+j*4, ((__u8 *)(para->res_second[15-t_time.second%15].data)+54+((i+1)*POINTER_W-1-j)*4), 4);
			}
		}
	}

	for(i=0; i<POINTER_H; i++)
	{
		for(j=0; j<POINTER_W; j++)
		{
			if (*((__u8 *)(second_data+(i*POINTER_W+j)*4)) != 0x00)
			{
				*((__u8 *)(data+((i+y)*TIME_W+x+j)*4+0)) = *((__u8 *)(second_data+(i*POINTER_W+j)*4+0));
				*((__u8 *)(data+((i+y)*TIME_W+x+j)*4+1)) = *((__u8 *)(second_data+(i*POINTER_W+j)*4+1));
				*((__u8 *)(data+((i+y)*TIME_W+x+j)*4+2)) = *((__u8 *)(second_data+(i*POINTER_W+j)*4+2));
				*((__u8 *)(data+((i+y)*TIME_W+x+j)*4+3)) = *((__u8 *)(second_data+(i*POINTER_W+j)*4+3));
			}
		}
	}

	x = (TIME_W-84)/2;
	y = (TIME_H-84)/2;
	for(i=0; i<84; i++)
	{
		for(j=0; j<84; j++)
		{
			if (*((__u8 *)(point+(i*84+j)*4+3)) == 0xff)
			{
				*((__u8 *)(data+((i+y)*TIME_W+x+j)*4+0)) = *((__u8 *)(point+(i*84+j)*4+0));
				*((__u8 *)(data+((i+y)*TIME_W+x+j)*4+1)) = *((__u8 *)(point+(i*84+j)*4+1));
				*((__u8 *)(data+((i+y)*TIME_W+x+j)*4+2)) = *((__u8 *)(point+(i*84+j)*4+2));
				*((__u8 *)(data+((i+y)*TIME_W+x+j)*4+3)) = *((__u8 *)(point+(i*84+j)*4+3));
			}
		}
	}
	
	for(i=0; i<TIME_H; i++)
	{
		eLIBs_memcpy((__u8 *)(fb.addr[0])+(i+TIME_Y)*fb.size.width*4+TIME_X*4, data+i*TIME_W*4, TIME_W*4);
	}
	
	esMEMS_Bfree(data, TIME_W*TIME_H*4);
	esMEMS_Bfree(point, 84*84*4);
	esMEMS_Bfree(hour_data, POINTER_W*POINTER_H*4);
	esMEMS_Bfree(minute_data, POINTER_W*POINTER_H*4);
	esMEMS_Bfree(second_data, POINTER_W*POINTER_H*4);

	GUI_BMP_RES_Draw(para->res_time_num[t_time.hour/10], 497, 600);
	GUI_BMP_RES_Draw(para->res_time_num[t_time.hour%10], 537, 600);
	GUI_BMP_RES_Draw(para->res_time_num[10], 577, 600);
	GUI_BMP_RES_Draw(para->res_time_num[t_time.minute/10], 600, 600);
	GUI_BMP_RES_Draw(para->res_time_num[t_time.minute%10], 640, 600);
	return EPDK_OK;
}
/*****************************************************************
*****************************************************************/
static __s32 date_paint(__gui_msg_t *msg)
{
	app_clock_ui_t *para;
	__date_t t_date;
	__s32 i, week, month, year;
	char str_date[20];

	para = (app_clock_ui_t *)GUI_WinGetAddData(msg->h_deswin);
	esTIME_GetDate(&t_date);

	if (t_date.month < 3)
	{
		month = t_date.month + 12;
		year = t_date.year-1;
	}	
	else
	{
		month = t_date.month; 
		year = t_date.year;
	}
		
	week = (t_date.day+1)+2*month+3*(month+1)/5+year+year/4-year/100+year/400;
	week %= 7;

	eLIBs_sprintf(str_date, "%d-%02d-%02d", t_date.year, t_date.month, t_date.day);

	//GUI_LyrWinSel(headbar_para->front_layer);
	//GUI_SetBkColor(0x00000000);
	//GUI_ClearRectEx(&headbar_ui.date);
	
	GUI_SetFont(app_get_font(18));
	GUI_SetColor(GUI_WHITE);
	GUI_UC_SetEncodeUTF8();
	GUI_SetDrawMode(GUI_DRAWMODE_TRANS);
	GUI_DispStringAt(str_date, DATE_X, DATE_Y);

	get_lang_res(week_tab[week], str_date);
	GUI_DispStringAt(str_date, 544, 675);
}
/*******************************************************************
*******************************************************************/
static __s32 app_clock_frm_paint(__gui_msg_t *msg, __s32 mode)
{
	app_clock_ui_t *clock_para;
	reg_system_para_t *system_para;
	H_LYR lyr;
	GUI_RECT rcrect;
	int i;
	clock_para = (app_clock_ui_t *)GUI_WinGetAddData(msg->h_deswin);
	system_para = (reg_system_para_t *)dsk_reg_get_para_by_app(REG_APP_SYSTEM);
	
	lyr = GUI_WinGetLyrWin(msg->h_deswin);
	if (GUI_LyrWinGetSta(lyr) == GUI_LYRWIN_STA_SUSPEND)
		return EPDK_FAIL;

	GUI_LyrWinSel(lyr);  
	GUI_SetFont(clock_para->font);
	GUI_SetDrawMode(GUI_DRAWMODE_TRANS);
	GUI_SetColor(GUI_WHITE);	
	GUI_UC_SetEncodeUTF8();	

	if (mode == CLOCK_PAINT_ALL)
	{
		time_paint(msg);
		date_paint(msg);
	}
	else if (mode == CLOCK_PAINT_DATE)
	{
		time_paint(msg);
		date_paint(msg);
	}
	
	return EPDK_OK;
}
/*************************************************************
****************************************************************/
static __s32 app_clock_frm_tp_deal(__gui_msg_t *msg)
{
	app_clock_ui_t *clock_para;
	reg_system_para_t *system_para;
	__s16 x,y,i;

	system_para = (reg_system_para_t *)dsk_reg_get_para_by_app(REG_APP_SYSTEM);
	clock_para = (app_clock_ui_t *)GUI_WinGetAddData(msg->h_deswin);
	x = LOSWORD(msg->dwAddData2);
	y = HISWORD(msg->dwAddData2);

	switch(msg->dwAddData1)
 	{
 		case GUI_MSG_TOUCH_DOWN:
 		case GUI_MSG_TOUCH_OVERDOWN:
 		{
 			break;
 		}
 		case GUI_MSG_TOUCH_UP:
 		case GUI_MSG_TOUCH_OVERUP:
 		{
 			break;
 		}
 		default:
 			break;
 	}

	return EPDK_OK;
} 
/*****************************************************************

		_remote_menu_win_cb

*****************************************************************/
static __s32 _clock_frm_cb(__gui_msg_t *msg)
{
    __s32 ret = EPDK_OK;                               
	
    switch(msg->id)
    {
        case GUI_MSG_CREATE: 
    	{
    		app_clock_frm_init(msg);
    		return EPDK_OK;
    	}
		case GUI_MSG_PAINT:
			app_clock_frm_paint(msg, CLOCK_PAINT_ALL);
			return EPDK_OK;
        case GUI_MSG_CLOSE:
			GUI_ManWinDelete(msg->h_deswin);
          	return EPDK_OK;          
        case GUI_MSG_DESTROY:	
			{
				app_clock_frm_uninit(msg);
        	}
          	return EPDK_OK;
        case GUI_MSG_TIMER:
        {
        	app_clock_frm_paint(msg, CLOCK_PAINT_DATE);
        	return EPDK_OK;
        }
        case GUI_MSG_COMMAND:
			
			break;
        case GUI_MSG_TOUCH:
			{
				app_clock_frm_tp_deal(msg);
			}
            return EPDK_OK;
		case GUI_MSG_KEY:
			{
				break;
   		 	}
        default:
            ret = GUI_FrmWinDefaultProc(msg);
            return ret;			
    }    
    return GUI_FrmWinDefaultProc(msg);	
}


H_WIN app_clock_frm_create(app_clock_t *para)
{	
	__gui_framewincreate_para_t framewin_para;
	FB fb;

	GUI_LyrWinGetFB(para->lyr, &fb);
	
	eLIBs_memset(&framewin_para, 0, sizeof(__gui_framewincreate_para_t));
	framewin_para.name =	"clock frm window",
	framewin_para.dwExStyle = WS_EX_NONE;
	framewin_para.dwStyle = WS_NONE|WS_VISIBLE;
	framewin_para.spCaption =  NULL;
	framewin_para.hOwner	= NULL;
	framewin_para.id        = APP_CLOCK_ID;	
	framewin_para.hHosting = para->h_parent;
	framewin_para.FrameWinProc = (__pGUI_WIN_CB)esKRNL_GetCallBack((__pCBK_t)_clock_frm_cb);
	framewin_para.rect.x =0;
	framewin_para.rect.y = 0;
	framewin_para.rect.width= fb.size.width;
	framewin_para.rect.height = fb.size.height;
	framewin_para.BkColor.alpha =  0;
	framewin_para.BkColor.red = 0;
	framewin_para.BkColor.green = 0;
	framewin_para.BkColor.blue = 0;
	framewin_para.attr =  (void *)para;
	framewin_para.hLayer = para->lyr;			//会被listbar调用
	return (GUI_FrmWinCreate(&framewin_para));						//在framewin 的create消息中将handle,传送给music_player_ctrl->h_framewin
}