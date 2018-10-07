#include "cp_i.h"

#ifdef THIS_DEBUG
#undef THIS_DEBUG
#endif
#ifdef MW_PROJ_DEBUG
#define THIS_DEBUG
#endif

#define INVAL_TIME  1
#define INVAL_NULL  0
typedef struct {
	EWin_InheritInfo_ForHeader ewii;
	HTHEME	on;
	void *ondata;
	HTHEME off;
	void *offdata;
	HTHEME num;
	void *numdata;
	int user_state;
	int inval; 
} NEWROLLER;

typedef struct {
	EWin_InheritInfo_ForHeader ewii;
	HTHEME bg;
	void * bgData;
} BottomFrameWndInfo;

static int data[10] = {ID_CLOCK_0_BMP,ID_CLOCK_1_BMP,ID_CLOCK_2_BMP,ID_CLOCK_3_BMP,ID_CLOCK_4_BMP,
	ID_CLOCK_5_BMP,ID_CLOCK_6_BMP,ID_CLOCK_7_BMP,ID_CLOCK_8_BMP,ID_CLOCK_9_BMP};

static __time_t time;
static int hour;
static int min;

static __s32 TimeTopWndProc_1(__gui_msg_t * msg)
{
	NEWROLLER * wndDpt;
	   
	wndDpt = (NEWROLLER *)GUI_WinGetAttr(msg->h_deswin);
	if ((!wndDpt) || (-1 == EWin_CheckInherit(&wndDpt->ewii))) {
		LogE("Null or wrong setting window private data");
		return -1;
	}
	switch (msg->id) {
	case GUI_MSG_CREATE:
		wndDpt->ewii.hWnd = msg->h_deswin;
		wndDpt->ewii.mask[0] = SCR_WNDINFOMASK_GESTURE;
		GG_FillRect(&wndDpt->ewii.wndRect,50, 80, 61, 44);
		wndDpt->ewii.gestureArgs.attr_x|= GG_GESTURE_ATTR_CLICK;
		wndDpt->ewii.gestureArgs.hmsg_x[GG_GESTURE_MSG_DEST_CLICK] = msg->h_deswin;
		wndDpt->on = dsk_Systheme_open(ID_CLOCK_UP_O_BMP);
		wndDpt->ondata = dsk_Systheme_hdl2buf(wndDpt->on);
		wndDpt->off = dsk_Systheme_open(ID_CLOCK_UP_C_BMP);
		wndDpt->offdata= dsk_Systheme_hdl2buf(wndDpt->off);
		esTIME_GetTime(&time);
		hour = time.hour;
		GUI_SetBkColor(0);
		break;
	case GUI_MSG_DESTROY:
		dsk_theme_close(wndDpt->on);
		dsk_theme_close(wndDpt->off);
		esMEMS_Mfree(0, wndDpt);
		break;
	case GUI_MSG_PAINT:
		GUI_LyrWinSel(wndDpt->ewii.lyrHigher);
		if(wndDpt->user_state){
			GUI_BMP_Draw(wndDpt->ondata,wndDpt->ewii.wndRect.x, wndDpt->ewii.wndRect.y);
		}else{
			GUI_BMP_Draw(wndDpt->offdata,wndDpt->ewii.wndRect.x, wndDpt->ewii.wndRect.y);
		}
		wndDpt->num = dsk_Systheme_open(data[hour/10]);
		wndDpt->numdata = dsk_Systheme_hdl2buf(wndDpt->num);
		GUI_BMP_Draw(wndDpt->numdata,40,128);
		dsk_theme_close(wndDpt->num);
		wndDpt->num = dsk_Systheme_open(data[hour%10]);
		wndDpt->numdata = dsk_Systheme_hdl2buf(wndDpt->num);
		GUI_BMP_Draw(wndDpt->numdata,80,128);
		dsk_theme_close(wndDpt->num);
		break;
	case GUI_MSG_USER_GESTURE_CLICK:
		wndDpt->user_state = wndDpt->user_state?0:1;
		if(hour>22){
		   hour = 0;
		}else{
           hour++;
		}
		GUI_WinUpdate(msg->h_deswin,ORANGE_TRUE);
		dsk_keytone_on();
		break;
	default:
		break;
	}
	return GUI_FrmWinDefaultProc(msg);
}

static __s32 TimeTopWndProc_2(__gui_msg_t * msg)
{
	NEWROLLER * wndDpt;
	   
	wndDpt = (NEWROLLER *)GUI_WinGetAttr(msg->h_deswin);
	if ((!wndDpt) || (-1 == EWin_CheckInherit(&wndDpt->ewii))) {
		LogE("Null or wrong setting window private data");
		return -1;
	}
	switch (msg->id) {
	case GUI_MSG_CREATE:
		wndDpt->ewii.hWnd = msg->h_deswin;
		wndDpt->ewii.mask[0] = SCR_WNDINFOMASK_GESTURE;
		GG_FillRect(&wndDpt->ewii.wndRect,210, 80, 61, 44);
		wndDpt->ewii.gestureArgs.attr_x|= GG_GESTURE_ATTR_CLICK;
		wndDpt->ewii.gestureArgs.hmsg_x[GG_GESTURE_MSG_DEST_CLICK] = msg->h_deswin;
		wndDpt->on = dsk_Systheme_open(ID_CLOCK_UP_O_BMP);
		wndDpt->ondata = dsk_Systheme_hdl2buf(wndDpt->on);
		wndDpt->off = dsk_Systheme_open(ID_CLOCK_UP_C_BMP);
		wndDpt->offdata= dsk_Systheme_hdl2buf(wndDpt->off);
		esTIME_GetTime(&time);
		min = time.minute;
		GUI_SetBkColor(0);
		break;
	case GUI_MSG_DESTROY:
		dsk_theme_close(wndDpt->on);
		dsk_theme_close(wndDpt->off);
		esMEMS_Mfree(0, wndDpt);
		break;
	case GUI_MSG_PAINT:
		GUI_LyrWinSel(wndDpt->ewii.lyrHigher);
		if(wndDpt->user_state){
			GUI_BMP_Draw(wndDpt->ondata,wndDpt->ewii.wndRect.x, wndDpt->ewii.wndRect.y);
		}else{
			GUI_BMP_Draw(wndDpt->offdata,wndDpt->ewii.wndRect.x, wndDpt->ewii.wndRect.y);
		}
		wndDpt->num = dsk_Systheme_open(data[min/10]);
		wndDpt->numdata = dsk_Systheme_hdl2buf(wndDpt->num);
		GUI_BMP_Draw(wndDpt->numdata,200,128);
		dsk_theme_close(wndDpt->num);
		wndDpt->num = dsk_Systheme_open(data[min%10]);
		wndDpt->numdata = dsk_Systheme_hdl2buf(wndDpt->num);
		GUI_BMP_Draw(wndDpt->numdata,240,128);
		dsk_theme_close(wndDpt->num);
		break;
	case GUI_MSG_USER_GESTURE_CLICK:
		wndDpt->user_state = wndDpt->user_state?0:1;
		if(min>58){
		   min = 0;
		}else{
           min++;
		}
		GUI_WinUpdate(msg->h_deswin,ORANGE_TRUE);
		dsk_keytone_on();
		break;
	default:
		break;
	}
	return GUI_FrmWinDefaultProc(msg);
}

static __s32 TimeTopWndProc_3(__gui_msg_t * msg)
{
	NEWROLLER * wndDpt;
	   
	wndDpt = (NEWROLLER *)GUI_WinGetAttr(msg->h_deswin);
	if ((!wndDpt) || (-1 == EWin_CheckInherit(&wndDpt->ewii))) {
		LogE("Null or wrong setting window private data");
		return -1;
	}
	switch (msg->id) {
	case GUI_MSG_CREATE:
		wndDpt->ewii.hWnd = msg->h_deswin;
		wndDpt->ewii.mask[0] = SCR_WNDINFOMASK_GESTURE;
		GG_FillRect(&wndDpt->ewii.wndRect,50, 192, 61, 44);
		wndDpt->ewii.gestureArgs.attr_x|= GG_GESTURE_ATTR_CLICK;
		wndDpt->ewii.gestureArgs.hmsg_x[GG_GESTURE_MSG_DEST_CLICK] = msg->h_deswin;
		wndDpt->on = dsk_Systheme_open(ID_CLOCK_DOWN_O_BMP);
		wndDpt->ondata = dsk_Systheme_hdl2buf(wndDpt->on);
		wndDpt->off = dsk_Systheme_open(ID_CLOCK_DOWN_C_BMP);
		wndDpt->offdata= dsk_Systheme_hdl2buf(wndDpt->off);
		esTIME_GetTime(&time);
		hour = time.hour;
		GUI_SetBkColor(0);
		break;
	case GUI_MSG_DESTROY:
		dsk_theme_close(wndDpt->on);
		dsk_theme_close(wndDpt->off);
		esMEMS_Mfree(0, wndDpt);
		break;
	case GUI_MSG_PAINT:
		GUI_LyrWinSel(wndDpt->ewii.lyrHigher);
		if(wndDpt->user_state){
			GUI_BMP_Draw(wndDpt->ondata,wndDpt->ewii.wndRect.x, wndDpt->ewii.wndRect.y);
		}else{
			GUI_BMP_Draw(wndDpt->offdata,wndDpt->ewii.wndRect.x, wndDpt->ewii.wndRect.y);
		}
		wndDpt->num = dsk_Systheme_open(data[hour/10]);
		wndDpt->numdata = dsk_Systheme_hdl2buf(wndDpt->num);
		GUI_BMP_Draw(wndDpt->numdata,40,128);
		dsk_theme_close(wndDpt->num);
		wndDpt->num = dsk_Systheme_open(data[hour%10]);
		wndDpt->numdata = dsk_Systheme_hdl2buf(wndDpt->num);
		GUI_BMP_Draw(wndDpt->numdata,80,128);
		dsk_theme_close(wndDpt->num);
		break;
	case GUI_MSG_USER_GESTURE_CLICK:
		wndDpt->user_state = wndDpt->user_state?0:1;
		if(hour < 1){
		   hour = 23;
		}else{
           hour--;
		}
		GUI_WinUpdate(msg->h_deswin,ORANGE_TRUE);
		dsk_keytone_on();
		break;
	default:
		break;
	}
	return GUI_FrmWinDefaultProc(msg);
}

static __s32 TimeTopWndProc_4(__gui_msg_t * msg)
{
	NEWROLLER * wndDpt;
	   
	wndDpt = (NEWROLLER *)GUI_WinGetAttr(msg->h_deswin);
	if ((!wndDpt) || (-1 == EWin_CheckInherit(&wndDpt->ewii))) {
		LogE("Null or wrong setting window private data");
		return -1;
	}
	switch (msg->id) {
	case GUI_MSG_CREATE:
		wndDpt->ewii.hWnd = msg->h_deswin;
		wndDpt->ewii.mask[0] = SCR_WNDINFOMASK_GESTURE;
		GG_FillRect(&wndDpt->ewii.wndRect,210, 192, 61, 44);
		wndDpt->ewii.gestureArgs.attr_x|= GG_GESTURE_ATTR_CLICK;
		wndDpt->ewii.gestureArgs.hmsg_x[GG_GESTURE_MSG_DEST_CLICK] = msg->h_deswin;
		wndDpt->on = dsk_Systheme_open(ID_CLOCK_DOWN_O_BMP);
		wndDpt->ondata = dsk_Systheme_hdl2buf(wndDpt->on);
		wndDpt->off = dsk_Systheme_open(ID_CLOCK_DOWN_C_BMP);
		wndDpt->offdata= dsk_Systheme_hdl2buf(wndDpt->off);
		esTIME_GetTime(&time);
		min = time.minute;
		GUI_SetBkColor(0);
		break;
	case GUI_MSG_DESTROY:
		dsk_theme_close(wndDpt->on);
		dsk_theme_close(wndDpt->off);
		esMEMS_Mfree(0, wndDpt);
		break;
	case GUI_MSG_PAINT:
		GUI_LyrWinSel(wndDpt->ewii.lyrHigher);
		if(wndDpt->user_state){
			GUI_BMP_Draw(wndDpt->ondata,wndDpt->ewii.wndRect.x, wndDpt->ewii.wndRect.y);
		}else{
			GUI_BMP_Draw(wndDpt->offdata,wndDpt->ewii.wndRect.x, wndDpt->ewii.wndRect.y);
		}
		wndDpt->num = dsk_Systheme_open(data[min/10]);
		wndDpt->numdata = dsk_Systheme_hdl2buf(wndDpt->num);
		GUI_BMP_Draw(wndDpt->numdata,200,128);
		dsk_theme_close(wndDpt->num);
		wndDpt->num = dsk_Systheme_open(data[min%10]);
		wndDpt->numdata = dsk_Systheme_hdl2buf(wndDpt->num);
		GUI_BMP_Draw(wndDpt->numdata,240,128);
		dsk_theme_close(wndDpt->num);
		break;
	case GUI_MSG_USER_GESTURE_CLICK:
		wndDpt->user_state = wndDpt->user_state?0:1;
		if(min<1){
		   min = 59;
		}else{
           min--;
		}
		GUI_WinUpdate(msg->h_deswin,ORANGE_TRUE);
		dsk_keytone_on();
		break;
	default:
		break;
	}
	return GUI_FrmWinDefaultProc(msg);
}

static __s32 TimeTopWndProc_5(__gui_msg_t * msg)
{
	NEWROLLER * wndDpt;
	   
	wndDpt = (NEWROLLER *)GUI_WinGetAttr(msg->h_deswin);
	if ((!wndDpt) || (-1 == EWin_CheckInherit(&wndDpt->ewii))) {
		LogE("Null or wrong setting window private data");
		return -1;
	}
	switch (msg->id) {
	case GUI_MSG_CREATE:
		wndDpt->ewii.hWnd = msg->h_deswin;
		wndDpt->ewii.mask[0] = SCR_WNDINFOMASK_GESTURE;
		GG_FillRect(&wndDpt->ewii.wndRect,280, 0, 61, 44);
		wndDpt->ewii.gestureArgs.attr_x|= GG_GESTURE_ATTR_CLICK;
		wndDpt->ewii.gestureArgs.hmsg_x[GG_GESTURE_MSG_DEST_CLICK] = msg->h_deswin;
		wndDpt->on = dsk_Systheme_open(ID_CLOCK_DOWN_O_BMP);
		wndDpt->ondata = dsk_Systheme_hdl2buf(wndDpt->on);
		wndDpt->off = dsk_Systheme_open(ID_CLOCK_DOWN_C_BMP);
		wndDpt->offdata= dsk_Systheme_hdl2buf(wndDpt->off);
		GUI_SetBkColor(0);
		break;
	case GUI_MSG_DESTROY:
		dsk_theme_close(wndDpt->on);
		dsk_theme_close(wndDpt->off);
		esMEMS_Mfree(0, wndDpt);
		break;
	case GUI_MSG_PAINT:
		GUI_LyrWinSel(wndDpt->ewii.lyrHigher);
		if(wndDpt->user_state){
			GUI_BMP_Draw(wndDpt->ondata,wndDpt->ewii.wndRect.x, wndDpt->ewii.wndRect.y);
		}else{
			GUI_BMP_Draw(wndDpt->offdata,wndDpt->ewii.wndRect.x, wndDpt->ewii.wndRect.y);
		}
		break;
	case GUI_MSG_USER_GESTURE_CLICK:
		esTIME_SetTime(&time);
		time.hour = hour;
		time.minute = min; 
		esTIME_SetTime(&time);
		msg->h_deswin = wndDpt->ewii.desktop;
		msg->id = GUI_MSG_USER_WIN_CHANGE;
		msg->dwAddData2 = HOME_SUBWIN_SCROLLING;
		GUI_PostMessage(msg);
		dsk_keytone_on();
		break;
	default:
		break;
	}
	return GUI_FrmWinDefaultProc(msg);
}

static H_WIN TimeTopFrameWndCreate_1(EWin_InheritInfo_ForHeader * inherit)
{
	NEWROLLER * wndDpt;

	if (-1 == EWin_CheckInherit(inherit)) return NULL;

	wndDpt = (NEWROLLER *)esMEMS_Malloc(0, sizeof(NEWROLLER));
	if (NULL == wndDpt) {
		LogE("Malloc failed");
		return NULL;
	}
	eLIBs_memset(wndDpt, 0, sizeof(NEWROLLER));
	eLIBs_memcpy(wndDpt, inherit, sizeof(EWin_InheritInfo_ForHeader));

	return EWin_FrameWindowCreate("SettingWinFore", &wndDpt->ewii, wndDpt->ewii.lyrHigher, &wndDpt->ewii.wndRect, TimeTopWndProc_1);
}

static H_WIN TimeTopFrameWndCreate_2(EWin_InheritInfo_ForHeader * inherit)
{
	NEWROLLER * wndDpt;

	if (-1 == EWin_CheckInherit(inherit)) return NULL;

	wndDpt = (NEWROLLER *)esMEMS_Malloc(0, sizeof(NEWROLLER));
	if (NULL == wndDpt) {
		LogE("Malloc failed");
		return NULL;
	}
	eLIBs_memset(wndDpt, 0, sizeof(NEWROLLER));
	eLIBs_memcpy(wndDpt, inherit, sizeof(EWin_InheritInfo_ForHeader));

	return EWin_FrameWindowCreate("SettingWinFore", &wndDpt->ewii, wndDpt->ewii.lyrHigher, &wndDpt->ewii.wndRect, TimeTopWndProc_2);
}

static H_WIN TimeTopFrameWndCreate_3(EWin_InheritInfo_ForHeader * inherit)
{
	NEWROLLER * wndDpt;

	if (-1 == EWin_CheckInherit(inherit)) return NULL;

	wndDpt = (NEWROLLER *)esMEMS_Malloc(0, sizeof(NEWROLLER));
	if (NULL == wndDpt) {
		LogE("Malloc failed");
		return NULL;
	}
	eLIBs_memset(wndDpt, 0, sizeof(NEWROLLER));
	eLIBs_memcpy(wndDpt, inherit, sizeof(EWin_InheritInfo_ForHeader));

	return EWin_FrameWindowCreate("SettingWinFore", &wndDpt->ewii, wndDpt->ewii.lyrHigher, &wndDpt->ewii.wndRect, TimeTopWndProc_3);
}

static H_WIN TimeTopFrameWndCreate_4(EWin_InheritInfo_ForHeader * inherit)
{
	NEWROLLER * wndDpt;

	if (-1 == EWin_CheckInherit(inherit)) return NULL;

	wndDpt = (NEWROLLER *)esMEMS_Malloc(0, sizeof(NEWROLLER));
	if (NULL == wndDpt) {
		LogE("Malloc failed");
		return NULL;
	}
	eLIBs_memset(wndDpt, 0, sizeof(NEWROLLER));
	eLIBs_memcpy(wndDpt, inherit, sizeof(EWin_InheritInfo_ForHeader));

	return EWin_FrameWindowCreate("SettingWinFore", &wndDpt->ewii, wndDpt->ewii.lyrHigher, &wndDpt->ewii.wndRect, TimeTopWndProc_4);
}

static H_WIN TimeTopFrameWndCreate_5(EWin_InheritInfo_ForHeader * inherit)
{
	NEWROLLER * wndDpt;

	if (-1 == EWin_CheckInherit(inherit)) return NULL;

	wndDpt = (NEWROLLER *)esMEMS_Malloc(0, sizeof(NEWROLLER));
	if (NULL == wndDpt) {
		LogE("Malloc failed");
		return NULL;
	}
	eLIBs_memset(wndDpt, 0, sizeof(NEWROLLER));
	eLIBs_memcpy(wndDpt, inherit, sizeof(EWin_InheritInfo_ForHeader));

	return EWin_FrameWindowCreate("SettingWinFore", &wndDpt->ewii, wndDpt->ewii.lyrHigher, &wndDpt->ewii.wndRect, TimeTopWndProc_5);
}

static __s32 TimeBottomWndProc(__gui_msg_t * msg)
{
	BottomFrameWndInfo * wndDpt;

	wndDpt = (BottomFrameWndInfo *)GUI_WinGetAttr(msg->h_deswin);
	if ((!wndDpt) || (-1 == EWin_CheckInherit(&wndDpt->ewii))) {
		LogE("Null or wrong setting window private data");
		return -1;
	}

	switch (msg->id) {
	case GUI_MSG_CREATE:
		wndDpt->ewii.mask[0] = 0;
		wndDpt->ewii.hWnd = msg->h_deswin;
		wndDpt->bg = dsk_Systheme_open(ID_CLOCK_SETTIME_BG_BMP);
		wndDpt->bgData = dsk_Systheme_hdl2buf(wndDpt->bg);
		break;
	case GUI_MSG_DESTROY:
		dsk_theme_close(wndDpt->bg);
		esMEMS_Mfree(0, wndDpt);
		break;
	case GUI_MSG_PAINT:
		GUI_LyrWinSel(wndDpt->ewii.lyrLow);
		GUI_BMP_Draw(wndDpt->bgData, wndDpt->ewii.wndRect.x, wndDpt->ewii.wndRect.y);
		break;
/*	case GUI_MSG_USER_GESTURE_LPRESS:
		LogMI("No handled main window message (%d)", msg->id);
		msg->h_deswin = wndDpt->ewii.desktop;
		msg->id = GUI_MSG_USER_WIN_CHANGE;
		msg->dwAddData2 = HOME_SUBWIN_SCROLLING;
		GUI_PostMessage(msg);
		break;*/
	default:
		LogMI("No handled main window message (%d)", msg->id);
		break;
	}
	return GUI_FrmWinDefaultProc(msg);
}

static H_WIN TimeFrameWndCreate(EWin_InheritInfo_ForHeader * inherit)
{
	BottomFrameWndInfo * wndDpt;

	if (-1 == EWin_CheckInherit(inherit)) return NULL;

	wndDpt = (BottomFrameWndInfo *)esMEMS_Malloc(0, sizeof(BottomFrameWndInfo));
	if (NULL == wndDpt) {
		LogE("Malloc failed");
		return NULL;
	}
	eLIBs_memset(wndDpt, 0, sizeof(BottomFrameWndInfo));
	eLIBs_memcpy(wndDpt, inherit, sizeof(EWin_InheritInfo_ForHeader));

	return EWin_FrameWindowCreate("SettingWinBg", &wndDpt->ewii, wndDpt->ewii.lyrLow, &wndDpt->ewii.wndRect, TimeBottomWndProc);
}

static __s32 TimeMainWndProc(__gui_msg_t * msg)
{
	EWin_InheritInfo_ForHeader * wndDpt;
	RECT source;
	wndDpt = (EWin_InheritInfo_ForHeader *)GUI_WinGetAttr(msg->h_deswin);
	if ((!wndDpt) || (-1 == EWin_CheckInherit(wndDpt))) {
		LogE("Null or wrong setting window private data");
		return -1;
	}
	switch (msg->id) {
	case GUI_MSG_CREATE:
		wndDpt->mask[0] |= SCR_WNDINFOMASK_GESTURE;
		//wndDpt->gestureArgs.attr_x = GG_GESTURE_ATTR_LPRESS;
		//wndDpt->gestureArgs.hmsg_x[GG_GESTURE_MSG_DEST_LPRESS] = msg->h_deswin;
		wndDpt->hWnd = msg->h_deswin;
		// wndDpt->wndRect, don't set, inherit from parent window
		GG_FillRect(&source, 0, 0, wndDpt->wndRect.width, wndDpt->wndRect.height);
		GG_FillSize(&wndDpt->lyrLowSize, wndDpt->wndRect.width, wndDpt->wndRect.height);
		wndDpt->lyrLow = EWin_LayerCreate("TimeLayerLow", &wndDpt->lyrLowSize, &source, &source, 0);
		GG_CopySize(&wndDpt->lyrHigherSize, &wndDpt->lyrLowSize);
		wndDpt->lyrHigher = EWin_LayerCreate("TimeLayerHigher", &wndDpt->lyrHigherSize, &source, &source, 1);

		TimeFrameWndCreate(wndDpt);
		TimeTopFrameWndCreate_1(wndDpt);
		TimeTopFrameWndCreate_2(wndDpt);
		TimeTopFrameWndCreate_3(wndDpt);
		TimeTopFrameWndCreate_4(wndDpt);
		TimeTopFrameWndCreate_5(wndDpt);
		GUI_LyrWinSetSta(wndDpt->lyrLow, GUI_LYRWIN_STA_ON);
		GUI_LyrWinSetSta(wndDpt->lyrHigher, GUI_LYRWIN_STA_ON);
		break;
	case GUI_MSG_DESTROY:
		GUI_LyrWinDelete(wndDpt->lyrHigher);
		GUI_LyrWinDelete(wndDpt->lyrLow);
		esMEMS_Mfree(0, wndDpt);
		break;
	default:
		break;
	}
	return GUI_ManWinDefaultProc(msg);
}

H_WIN Setting_TimeWndCreate(EWin_InheritInfo_ForHeader * inherit)
{
	EWin_InheritInfo_ForHeader * wndDpt;
	if (-1 == EWin_CheckInherit(inherit)) {
		return NULL;
	}
	wndDpt = (EWin_InheritInfo_ForHeader *)esMEMS_Malloc(0, sizeof(EWin_InheritInfo_ForHeader));
	if (NULL == wndDpt) {
		LogE("Malloc failed");
		return NULL;
	}
	eLIBs_memcpy(wndDpt, inherit, sizeof(EWin_InheritInfo_ForHeader));
	return EWin_MainWindowCreate("TimeWindow", wndDpt, TimeMainWndProc);
}

