
#include "cp_i.h"
#include "scrolling_i.h"
#include "setting.h"
#include "settime.h"
#include "tongxin_demo.h"
#include "..\..\shell\bmp_parser\bmp.h"
#ifdef THIS_DEBUG
#undef THIS_DEBUG
#endif

#ifdef MW_PROJ_DEBUG
#define THIS_DEBUG
#endif

static int homeSysTimeInit(void)
{
    __time_t tm;
    __date_t date;

    tm.hour = 23;
    tm.minute = 59;
    tm.second = 30;
    date.year = 2013;
    date.month = 1;
    date.day = 20;
    esTIME_SetTime(&tm);
    esTIME_SetDate(&date);
    return 0;
}

typedef struct {
    EWin_InheritInfo_ForHeader ewii;
    void * inertia;
    H_WIN hFocusWin; // Current child window
    H_WIN StandyWindow;
} HomeDescriptor;
static __bool standy_en = 0;
static HomeDescriptor * homeWndInit(__gui_msg_t * msg)
{
    HomeDescriptor * hdp;
    gg_handler domUIConfig;
    gg_handler node;
    char ** attr;
    int i;
	gg_handler toiteNode;
	gg_char * langid;
	int langid_int;

    hdp = esMEMS_Malloc(0, sizeof(HomeDescriptor));
    if (NULL == hdp) {
        LogE("Malloc failed");
        return NULL;
    }
    eLIBs_memset(hdp, 0, sizeof(HomeDescriptor));
    media_player_main("e:\\liu.mp3");
    /* UI config */
    domUIConfig = (gg_handler)GUI_WinGetAttr(msg->h_deswin);
    node = GG_DOMGetRoot(domUIConfig);
    if (0 == eLIBs_strcmp(GG_DOMGetNodeName(node), "config")) {
        LogE("UI config XML root node ok");
    } else {
        LogE("UI config XML root node error");
        return NULL;
    }
	toiteNode = GG_DOMGetRoot(domUIConfig);
	toiteNode = GG_DOMGetFirstChild(toiteNode);
	toiteNode = GG_DOMGetBrotherElementByTagName(toiteNode, "langid");
	langid = GG_DOMGetNodeValue(toiteNode);

    // Set the volume
    dsk_volume_set(100);
    homeSysTimeInit();
    GUI_SetScnDir(GUI_SCNDIR_ROTATE180);
    dsk_display_set_lcd_bright(LION_BRIGHT_LEVEL1);

    hdp->ewii.magic = EWIN_INFO_MAGIC;
    hdp->ewii.desktop = msg->h_deswin;
    hdp->ewii.hWnd = hdp->ewii.desktop;
    GUI_GetScreenSize((__s32 *)&hdp->ewii.desktopSize.width, (__s32 *)&hdp->ewii.desktopSize.height);
    GG_FillRect(&hdp->ewii.wndRect, 0, 0,
                    hdp->ewii.desktopSize.width, hdp->ewii.desktopSize.height);
    // Initiate the font module
    hdp->ewii.gdi.font = GUI_SFT_CreateFont(CP_SYS_FONT_SIZE, CP_SYS_FONT_PATH);
    hdp->ewii.gdi.fontSize = CP_SYS_FONT_SIZE;
    hdp->ewii.gestureArgs.htimer = hdp->ewii.desktop;
    hdp->ewii.gestureArgs.timerid = HOME_GESTURE_TIMER;
    hdp->inertia = GG_GestureOpen();
    /* UI config */
    node = GG_DOMGetFirstChild(node);
    //node = GG_DOMGetBrotherElementByTagName(node, "window");
    if (node) {
		hdp->ewii.langid = SLIB_atoi(langid);
		//LogMI("hdp->ewii.langid %d",hdp->ewii.langid);
        hdp->hFocusWin = SCR_ScrollingHostBuilder(&hdp->ewii, ScrollingCreate_V0001, node);
		esDEV_Plugin("\\drv\\touchpanel.drv", 0, 0, 1);
    } else {
        LogE("No window node in XML file");
        return NULL;
    }

	/* ======> Lihan 20160127
		485默认为接收状态
  	*/
	Uart_485_init();
	/* <=============
	*/
	
    return hdp;
}

static H_LYR StandyWndCreate(const char *file_path)
{
	H_LYR hLyr;
	int rotation;
	FB frameBuf;
	//HTHEME off;
	//void * offData;
	ES_FILE *file = NULL;
	void *file_data = NULL;
	bmp_file_head_t *f_head= NULL;
	__layerwincreate_para_t layerWinCreate;
	__disp_layer_para_t lyrPara;
	__u32 lenth;
	//off = dsk_theme_open(id);//ID_MAIN_UPDATE_BMP
	//offData = dsk_theme_hdl2buf(off);
	file = eLIBs_fopen(file_path,"rb");
	if(file == NULL){
		__msg("file open fail\n");
		//return NULL;
		goto end;
	}
	f_head = eLIBs_malloc(sizeof(bmp_file_head_t));
	if(f_head==NULL){
  		__msg("file malloc fail\n");
		goto end;
	}
	eLIBs_fread(f_head,1,sizeof(bmp_file_head_t),file);
	if(f_head->bfType[0]!='B'||f_head->bfType[1]!='M'){
		__msg("file isnot bmp\n");
		goto end;
	}
	file_data = eLIBs_malloc(f_head->bfSize);
	eLIBs_fseek(file,0,SEEK_SET);
	lenth = eLIBs_fread(file_data,1,f_head->bfSize,file);
	__msg("lenth==%d\n",lenth);
	rotation = GUI_GetScnDir();
	MwLayerParaInit(&layerWinCreate, &frameBuf, &lyrPara, "Layer", rotation, 1, 720, 720);
	MwFillSize(&frameBuf.size, 720 , 720);
	MwFillRect(&lyrPara.src_win, 0, 0, 720, 720);
	hLyr = GUI_LyrWinCreate(&layerWinCreate);
	if (NULL == hLyr) {
		__msg("Create layer (%s) failed", layerWinCreate.name);
		goto end;
	}
	GUI_LyrWinSel(hLyr); 
	GUI_BMP_Draw(file_data, 0, 0);
	GUI_LyrWinSetSta(hLyr,GUI_LYRWIN_STA_ON);
	eLIBs_fclose(file);
	eLIBs_free(f_head);
	eLIBs_free(file_data);
	return hLyr;
end:
	if(file){
       eLIBs_fclose(file);
	}
	if(f_head){
		eLIBs_free(f_head);
	}
	if(file_data){
		eLIBs_free(file_data);
	}
	return NULL;
}

static __s32 homeWndProc(__gui_msg_t * msg)
{
	static unsigned char updata_idx = 0;
		
    HomeDescriptor * hdp;
    H_WIN wndGesture;
    EWin_InheritInfo_ForHeader * extWinIfno;
	H_WIN hwnd;
    char name[20];
    hdp = (HomeDescriptor *)GUI_WinGetAddData(msg->h_deswin);
    if (NULL == hdp) {
        if (GUI_MSG_CREATE != msg->id) {
            LogE("Failed to get home window private date, message ID (%d)", msg->id);
            return GUI_ManWinDefaultProc(msg);
        }
    }
   
    switch (msg->id) {
    case GUI_MSG_CREATE:
        hdp = homeWndInit(msg);
        if (!hdp) {
            GUI_PostQuitMessage(msg->h_deswin);
            LogE("Create Home window failed");
            return -1;
        }
		if(hdp->ewii.suspend){
           GUI_SetTimer(msg->h_deswin,SUSPEND_TIMER, 6000 * 10, NULL);  // 10 minutes
		}else{
           __msg("suspend is null");
		}
        GUI_WinSetAddData(msg->h_deswin, (__u32)hdp);
        GUI_WinSetFocusChild(hdp->hFocusWin);
        break;
    case GUI_MSG_DESTROY:
        GG_GestureClose(hdp->inertia);
        if (hdp->ewii.gdi.font) {
            GUI_SFT_ReleaseFont(hdp->ewii.gdi.font);
        }
        esMEMS_Mfree(0, hdp);
        break;
    case GUI_MSG_TOUCH:
#ifdef THIS_DEBUG
        //LogMI("Touch:%x,%d,%d", msg->dwAddData1, LOSWORD(msg->dwAddData2), HISWORD(msg->dwAddData2));
#endif  
         if(standy_en){
	        if(hdp->ewii.suspend == 1){
		           dsk_display_lcd_on();
	        }else if(hdp->ewii.suspend == 2){
	            
			}else if(hdp->ewii.suspend == 3){
				GUI_LyrWinDelete(hdp->StandyWindow);
			}
			standy_en = 0;
			break;
         }
		 if(LOSWORD(msg->dwAddData2) > 720 || HISWORD(msg->dwAddData2) > 720){
		 	break;
		 }
		GUI_ResetTimer(msg->h_deswin,SUSPEND_TIMER, 6000 * 10, NULL);
        if (GUI_MSG_TOUCH_DOWN == msg->dwAddData1) {
            wndGesture = EWin_GestureAttrCollect(GUI_WinGetFocusChild(msg->h_deswin), LOSWORD(msg->dwAddData2), HISWORD(msg->dwAddData2));
            if (wndGesture) {
                extWinIfno = GUI_WinGetAttr(wndGesture);
                GG_GestureOnDown(hdp->inertia, msg, LOSWORD(msg->dwAddData2), HISWORD(msg->dwAddData2), &extWinIfno->gestureArgs);
            }
        } else if (GUI_MSG_TOUCH_MOVE == msg->dwAddData1) {
            GG_GestureOnMove(hdp->inertia, msg, LOSWORD(msg->dwAddData2), HISWORD(msg->dwAddData2));
        } else if (GUI_MSG_TOUCH_UP == msg->dwAddData1) {
            GG_GestureOnUp(hdp->inertia, msg, LOSWORD(msg->dwAddData2), HISWORD(msg->dwAddData2));
		
		updata_idx = 0;
		GUI_KillTimer (msg->h_deswin, SYS_UPDATA_TIMER);
		
        }else if(GUI_MSG_TOUCH_LONGDOWN == msg->dwAddData1){
            GG_GestureOnLpress(hdp->inertia, msg, LOSWORD(msg->dwAddData2), HISWORD(msg->dwAddData2));
		}
        break;
    case GUI_MSG_TIMER:
        if (HOME_GESTURE_TIMER == msg->dwAddData1) {
            GG_GestureOnTimer(hdp->inertia, msg);
        }else if(SUSPEND_TIMER == msg->dwAddData1){
         	if(!standy_en){
	            if(hdp->ewii.suspend==1){
	                 dsk_display_lcd_off();
					 //standy_en = 1;
				}else if(hdp->ewii.suspend==2){
	                 msg->h_deswin = GUI_WinGetFocusChild(msg->h_deswin);
					 msg->id = GUI_MSG_USER_GESTURE_DONE;
					 msg->dwAddData2 = MAKELONG(720,0);
					 GUI_PostMessage(msg);
					 //standy_en = 1;
				}else if(hdp->ewii.suspend==3){
					 hdp->StandyWindow = StandyWndCreate("d:\\res\\boot_ui\\logo.bmp");
				}else{
	                
				}
				standy_en = 1;
         	}
	}
	else if(SYS_UPDATA_TIMER == msg->dwAddData1)
	{
		updata_idx = 0;
		GUI_KillTimer (msg->h_deswin, SYS_UPDATA_TIMER);

		GUI_LyrWinSetSta(hdp->ewii.lyrLow,GUI_LYRWIN_STA_SLEEP);
		GUI_LyrWinSetSta(hdp->ewii.lyrHigher,GUI_LYRWIN_STA_SLEEP);
		GUI_LyrWinSetSta(hdp->ewii.inilyrHigher,GUI_LYRWIN_STA_SLEEP);
            	hdp->hFocusWin = SettingWndCreate(&hdp->ewii);
		if(hdp->hFocusWin){
            		GUI_WinSetFocusChild(hdp->hFocusWin);
		}
	}
        break;
    case GUI_MSG_USER_WIN_CHANGE:
        if (HOME_SUBWIN_UPDATE == msg->dwAddData2) {
		if(0 == updata_idx)
			GUI_SetTimer(msg->h_deswin, SYS_UPDATA_TIMER, 100 * 10, NULL);
/*
			GUI_LyrWinSetSta(hdp->ewii.lyrLow,GUI_LYRWIN_STA_SLEEP);
			GUI_LyrWinSetSta(hdp->ewii.lyrHigher,GUI_LYRWIN_STA_SLEEP);
			GUI_LyrWinSetSta(hdp->ewii.inilyrHigher,GUI_LYRWIN_STA_SLEEP);
            		hdp->hFocusWin = SettingWndCreate(&hdp->ewii);
			if(hdp->hFocusWin){
            			GUI_WinSetFocusChild(hdp->hFocusWin);
			}
*/
        }else if(HOME_SUBWIN_SETTING_TIME== msg->dwAddData2){
			GUI_LyrWinSetSta(hdp->ewii.inilyrHigher,GUI_LYRWIN_STA_SLEEP);
			GUI_LyrWinSetSta(hdp->ewii.lyrHigher,GUI_LYRWIN_STA_SLEEP);
			GUI_LyrWinSetSta(hdp->ewii.lyrLow,GUI_LYRWIN_STA_SLEEP);
            hdp->hFocusWin = Setting_TimeWndCreate(&hdp->ewii);
			if(hdp->hFocusWin){
            	GUI_WinSetFocusChild(hdp->hFocusWin);
			}
		}else {
            if(GUI_ManWinDelete(hdp->hFocusWin)==ORANGE_OK){
            	GUI_WinSetFocusChild(GUI_WinGetFirstChild(msg->h_deswin));
			    GUI_LyrWinSetSta(hdp->ewii.lyrLow,GUI_LYRWIN_STA_ON);
				GUI_LyrWinSetSta(hdp->ewii.lyrHigher,GUI_LYRWIN_STA_ON);
				GUI_LyrWinSetSta(hdp->ewii.inilyrHigher,GUI_LYRWIN_STA_ON);
            }
        }
        break;
    default:
#ifdef THIS_DEBUG
//        LogMI("Unhandled home window message (%d)", msg->id);
#endif
		GUI_ManWinDefaultProc(msg);
        break;
    }
    return 0;
}

H_WIN HomeWndCreate(H_WIN parent, gg_handler domUIConfig)
{
    H_WIN hWin;
    __gui_manwincreate_para_t createInfo;

    eLIBs_memset(&createInfo, 0, sizeof(__gui_manwincreate_para_t));

    createInfo.name             = "HomeWindow";
    createInfo.hParent          = parent;
    createInfo.hHosting         = NULL;
    createInfo.ManWindowProc    = (__pGUI_WIN_CB)esKRNL_GetCallBack((__pCBK_t)homeWndProc);
    createInfo.attr             = domUIConfig;
    hWin                        = GUI_ManWinCreate(&createInfo);

    if (NULL == hWin) {
        LogE("Create main window (%s) failed", createInfo.name);
        return NULL;
    }
#ifdef THIS_DEBUG
    LogMI("Hello main window (%s)", createInfo.name);
#endif
    return hWin;
}


