
#include "cp_i.h"

#ifdef THIS_DEBUG
#undef THIS_DEBUG
#endif
#ifdef MW_PROJ_DEBUG
#define THIS_DEBUG
#endif

typedef struct {
    EWin_InheritInfo_ForHeader ewii;
	HTHEME  on;
	void *ondata;
	HTHEME off;
	void *offdata;
    int user_state;
} NEWROLLER;

typedef struct {
    EWin_InheritInfo_ForHeader ewii;
    HTHEME bg;
    void * bgData;
} BottomFrameWndInfo;
#if 0
static H_LYR UpdateWndCreate(__s32 width, __s32 height,__u32 id)
{
	H_LYR hLyr;
	int rotation;
	FB frameBuf;
	HTHEME off;
	void * offData;
	__layerwincreate_para_t layerWinCreate;
	__disp_layer_para_t lyrPara;
	off = dsk_theme_open(id);//ID_MAIN_UPDATE_BMP
	offData = dsk_theme_hdl2buf(off);
	rotation = GUI_GetScnDir();
	MwLayerParaInit(&layerWinCreate, &frameBuf, &lyrPara, "Layer", rotation, 1, width, height);
	MwFillSize(&frameBuf.size, width , height);
	MwFillRect(&lyrPara.src_win, 0, 0, width, height);
	hLyr = GUI_LyrWinCreate(&layerWinCreate);
	if (NULL == hLyr) {
		__msg("Create layer (%s) failed", layerWinCreate.name);
		dsk_theme_close(off);
		return NULL;
	}
	GUI_LyrWinSel(hLyr); 
	GUI_BMP_Draw(offData, 0, 0);
	GUI_LyrWinSetSta(hLyr,GUI_LYRWIN_STA_ON);
	dsk_theme_close(off);
	return hLyr;
}

static __u32 system_update(void)
{
	ES_FILE *file = NULL;
	H_LYR h_lyr;
	
	file = eLIBs_fopen("f:\\ePDKv100.img","r+");
	if(file){
		eLIBs_fclose(file);
		h_lyr = UpdateWndCreate(720, 720,ID_CLOCK_UPDATE_BMP);
		//dsk_fw_update("f:\\ePDKv100.img",h_lyr);
	}else{
		h_lyr = UpdateWndCreate(720, 720,ID_CLOCK_SD_BMP);
		esKRNL_TimeDly(50);
	}
	GUI_LyrWinDelete(h_lyr);
	return EPDK_TRUE;
}
static __s32 settingupdateWndProc(__gui_msg_t * msg)
{
	NEWROLLER * wndDpt;
	//int i = 0;
	//int j = 0;
	H_LYR h_lyr;
	ES_FILE *file = NULL;
	
	wndDpt = (NEWROLLER *)GUI_WinGetAttr(msg->h_deswin);
	if ((!wndDpt) || (-1 == EWin_CheckInherit(&wndDpt->ewii))) {
		LogE("Null or wrong setting window private data");
		return -1;
	}
	
	switch (msg->id) {
	case GUI_MSG_CREATE:
		wndDpt->ewii.hWnd = msg->h_deswin;
		wndDpt->ewii.mask[0] = SCR_WNDINFOMASK_GESTURE;
		GG_FillRect(&wndDpt->ewii.wndRect,85, 140, 150, 57);
		wndDpt->ewii.gestureArgs.attr_x|= GG_GESTURE_ATTR_CLICK|GG_GESTURE_ATTR_MO;
		wndDpt->ewii.gestureArgs.hmsg_x[GG_GESTURE_MSG_DEST_CLICK] = msg->h_deswin;
		wndDpt->ewii.gestureArgs.hmsg_x[GG_GESTURE_MSG_DEST_MO] = msg->h_deswin;
		wndDpt->on = dsk_theme_open(ID_CLOCK_UPDATE_SYSC_BMP);
		wndDpt->ondata = dsk_theme_hdl2buf(wndDpt->on);
		wndDpt->off = dsk_theme_open(ID_CLOCK_UPDATE_SYSO_BMP);
		wndDpt->offdata= dsk_theme_hdl2buf(wndDpt->off);
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
	case GUI_MSG_USER_GESTURE_MO:
		wndDpt->user_state = wndDpt->user_state?0:1;
		GUI_WinUpdate(msg->h_deswin,ORANGE_TRUE);
		break;
	case GUI_MSG_USER_GESTURE_CLICK:
		wndDpt->user_state = 0;
		GUI_WinUpdate(msg->h_deswin,ORANGE_TRUE);
		system_update();
		break;
	default:
		break;
	}

	return GUI_FrmWinDefaultProc(msg);

}

static H_WIN settingupdateFrameWndCreate(EWin_InheritInfo_ForHeader * inherit)
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

	return EWin_FrameWindowCreate("Setting_update", &wndDpt->ewii, wndDpt->ewii.lyrHigher, &wndDpt->ewii.wndRect, settingupdateWndProc);
}

static __s32 settingTopWndProc(__gui_msg_t * msg)
{
	NEWROLLER * wndDpt;
	//int i = 0;
	//int j = 0;
	H_LYR h_lyr;
	ES_FILE *file = NULL;
	
	wndDpt = (NEWROLLER *)GUI_WinGetAttr(msg->h_deswin);
	if ((!wndDpt) || (-1 == EWin_CheckInherit(&wndDpt->ewii))) {
		LogE("Null or wrong setting window private data");
		return -1;
	}
	
	switch (msg->id) {
	case GUI_MSG_CREATE:
		wndDpt->ewii.hWnd = msg->h_deswin;
		wndDpt->ewii.mask[0] = SCR_WNDINFOMASK_GESTURE;
		GG_FillRect(&wndDpt->ewii.wndRect,85, 80, 150, 57);
		wndDpt->ewii.gestureArgs.attr_x|= GG_GESTURE_ATTR_CLICK|GG_GESTURE_ATTR_MO;
		wndDpt->ewii.gestureArgs.hmsg_x[GG_GESTURE_MSG_DEST_CLICK] = msg->h_deswin;
		wndDpt->ewii.gestureArgs.hmsg_x[GG_GESTURE_MSG_DEST_MO] = msg->h_deswin;
		wndDpt->on = dsk_theme_open(ID_CLOCK_RES_O_BMP);
		wndDpt->ondata = dsk_theme_hdl2buf(wndDpt->on);
		wndDpt->off = dsk_theme_open(ID_CLOCK_RES_C_BMP);
		wndDpt->offdata= dsk_theme_hdl2buf(wndDpt->off);
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
	case GUI_MSG_USER_GESTURE_MO:
		wndDpt->user_state = wndDpt->user_state?0:1;
		GUI_WinUpdate(msg->h_deswin,ORANGE_TRUE);
		break;
	case GUI_MSG_USER_GESTURE_CLICK:
		wndDpt->user_state = 0;
		GUI_WinUpdate(msg->h_deswin,ORANGE_TRUE);
		if(dir_has_existed("f:\\res")){
			h_lyr = UpdateWndCreate(720, 720,ID_CLOCK_UPDATE_BMP);
			folder_copy("f:\\res","e:\\res");
			dsk_keytone_on();
			file = eLIBs_fopen("b:\\STORAGE\\SPINOR","rb");
			if(file)
			{
				eLIBs_fioctrl(file, DEV_IOC_USR_FLUSH_CACHE, 0, 0);
				eLIBs_fclose(file);
			}
			esKSRV_Reset();
		}else{
		    h_lyr = UpdateWndCreate(720, 720,ID_CLOCK_SD_BMP);
			esKRNL_TimeDly(50);
		}
		GUI_LyrWinDelete(h_lyr);
		break;
	default:
		break;
	}

	return GUI_FrmWinDefaultProc(msg);

}

static H_WIN settingTopFrameWndCreate(EWin_InheritInfo_ForHeader * inherit)
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

    return EWin_FrameWindowCreate("SettingWinFore", &wndDpt->ewii, wndDpt->ewii.lyrHigher, &wndDpt->ewii.wndRect, settingTopWndProc);
}

static __s32 settingBottomWndProc(__gui_msg_t * msg)
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
        wndDpt->bg = dsk_theme_open(ID_CLOCK_UPDATE_BG_BMP);
        wndDpt->bgData = dsk_theme_hdl2buf(wndDpt->bg);
        break;
    case GUI_MSG_DESTROY:
        dsk_theme_close(wndDpt->bg);
        esMEMS_Mfree(0, wndDpt);
        break;
    case GUI_MSG_PAINT:
        GUI_LyrWinSel(wndDpt->ewii.lyrLow);
        GUI_BMP_Draw(wndDpt->bgData, wndDpt->ewii.wndRect.x, wndDpt->ewii.wndRect.y);
		GUI_SetColor(GUI_WHITE);
		GUI_DispStringAt("WP-V1.4",(80+150),210);
        break;
    case GUI_MSG_USER_GESTURE_LPRESS:
        msg->h_deswin = wndDpt->ewii.desktop;
        msg->id = GUI_MSG_USER_WIN_CHANGE;
        msg->dwAddData2 = HOME_SUBWIN_SCROLLING;
        GUI_PostMessage(msg);
        break;
    default:
        LogMI("No handled main window message (%d)", msg->id);
        break;
    }
    return GUI_FrmWinDefaultProc(msg);
}

static H_WIN settingFrameWndCreate(EWin_InheritInfo_ForHeader * inherit)
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

    return EWin_FrameWindowCreate("SettingWinBg", &wndDpt->ewii, wndDpt->ewii.lyrLow, &wndDpt->ewii.wndRect, settingBottomWndProc);
}
#endif

static __s32 settingMainWndProc(__gui_msg_t * msg)
{
	#if 0
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
        wndDpt->gestureArgs.attr_x = GG_GESTURE_ATTR_LPRESS;
        wndDpt->gestureArgs.hmsg_x[GG_GESTURE_MSG_DEST_LPRESS] = msg->h_deswin;
        wndDpt->hWnd = msg->h_deswin;
        // wndDpt->wndRect, don't set, inherit from parent window
        GG_FillRect(&source, 0, 0, wndDpt->wndRect.width, wndDpt->wndRect.height);
        GG_FillSize(&wndDpt->lyrLowSize, wndDpt->wndRect.width, wndDpt->wndRect.height);
        wndDpt->lyrLow = EWin_LayerCreate("SettingLayerLow", &wndDpt->lyrLowSize, &source, &source, 0);
        GG_CopySize(&wndDpt->lyrHigherSize, &wndDpt->lyrLowSize);
        wndDpt->lyrHigher = EWin_LayerCreate("SettingLayerHigher", &wndDpt->lyrHigherSize, &source, &source, 1);

        settingFrameWndCreate(wndDpt);
        settingTopFrameWndCreate(wndDpt);
     	settingupdateFrameWndCreate(wndDpt);
        GUI_LyrWinSetSta(wndDpt->lyrLow, GUI_LYRWIN_STA_ON);
        GUI_LyrWinSetSta(wndDpt->lyrHigher, GUI_LYRWIN_STA_ON);
        break;
    case GUI_MSG_DESTROY:
        GUI_LyrWinDelete(wndDpt->lyrHigher);
        GUI_LyrWinDelete(wndDpt->lyrLow);
        esMEMS_Mfree(0, wndDpt);
        break;
    case GUI_MSG_USER_GESTURE_LPRESS:
        msg->h_deswin = wndDpt->desktop;
        msg->id = GUI_MSG_USER_WIN_CHANGE;
        msg->dwAddData2 = HOME_SUBWIN_SCROLLING;
        GUI_PostMessage(msg);
        break;
    default:
        break;
    }
    #endif
    return GUI_ManWinDefaultProc(msg);
}

H_WIN SettingWndCreate(EWin_InheritInfo_ForHeader * inherit)
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
    return EWin_MainWindowCreate("SettingWindow", wndDpt, settingMainWndProc);
}



