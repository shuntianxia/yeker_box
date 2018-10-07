
#include "cp_i.h"

#ifdef THIS_DEBUG
#undef THIS_DEBUG
#endif
#ifdef MW_PROJ_DEBUG
//#define THIS_DEBUG
#endif

int EWin_CheckInherit(EWin_InheritInfo_ForHeader * inherit)
{
    if (!inherit) {
        LogE("Null handler");
        return -1;
    }
    if (EWIN_INFO_MAGIC != inherit->magic) {
        LogE("Wrong inherit magic");
        return -1;
    }
    return 0;
}

static int DrawLayerSetDefault(__layerwincreate_para_t * layerWinCreate, FB * frameBuf, __disp_layer_para_t * lyrPara,
                char * lyrName, SIZE * lyrSize, __u8 pipe)
{
    eLIBs_memset(frameBuf, 0, sizeof(FB));
    GG_CopySize(&frameBuf->size, lyrSize);
    frameBuf->fmt.type = FB_TYPE_RGB;
    frameBuf->fmt.fmt.rgb.pixelfmt = PIXEL_COLOR_ARGB8888;
    frameBuf->fmt.fmt.rgb.br_swap = 0;
    frameBuf->fmt.fmt.rgb.pixseq = RGB_SEQ_ARGB;

    eLIBs_memset(lyrPara, 0, sizeof(__disp_layer_para_t));
    lyrPara->mode = DISP_LAYER_WORK_MODE_NORMAL;
    lyrPara->alpha_val = 190;
    lyrPara->pipe = pipe;
    lyrPara->prio = 0xFF;
    lyrPara->channel = DISP_LAYER_OUTPUT_CHN_DE_CH1;
	lyrPara->fb = frameBuf;

    eLIBs_memset((void *)layerWinCreate, 0, sizeof(__layerwincreate_para_t));
	layerWinCreate->name = lyrName;
	layerWinCreate->lyrpara = lyrPara;
	layerWinCreate->initsta = GUI_LYRWIN_STA_SLEEP;
    return 0;
}

H_LYR EWin_LayerCreate(char * lyrName, SIZE * layer, RECT * screen, RECT * source, __u8 higherLayer)
{
    H_LYR hLyr;
    __layerwincreate_para_t layerWinCreate;
    FB frameBuf;
    __disp_layer_para_t lyrPara;
    SIZE lyrSize;

    if ((GUI_GetScnDir() == GUI_SCNDIR_ROTATE90) || (GUI_GetScnDir() == GUI_SCNDIR_ROTATE270)) {
        GG_FillSize(&lyrSize, layer->height, layer->width);
    } else {
        GG_FillSize(&lyrSize, layer->width, layer->height);
    }
    DrawLayerSetDefault(&layerWinCreate, &frameBuf, &lyrPara, lyrName, &lyrSize, higherLayer);
    GG_CopyRect(&lyrPara.scn_win, screen);
    GG_CopyRect(&lyrPara.src_win, source);
    hLyr = GUI_LyrWinCreate(&layerWinCreate);
    if (NULL == hLyr) {
        LogE("Create draw layer (%s) failed", layerWinCreate.name);
        return NULL;
    }
#ifdef THIS_DEBUG
    LogMI("Hellow (%s) draw layer (%s)", higherLayer ? "Top" : "Bottom", layerWinCreate.name);
#endif
    return hLyr;
}

H_WIN EWin_MainWindowCreate(char * name, EWin_InheritInfo_ForHeader * wndDpt, __pGUI_WIN_CB wndProc)
{
    H_WIN hWin;
    __gui_manwincreate_para_t createInfo;

    eLIBs_memset(&createInfo, 0, sizeof(__gui_manwincreate_para_t));
    createInfo.name             = name;
    createInfo.hParent          = wndDpt->hWnd;
    createInfo.hHosting         = NULL;
    createInfo.ManWindowProc    = (__pGUI_WIN_CB)esKRNL_GetCallBack((__pCBK_t)wndProc);
    createInfo.attr             = wndDpt;
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

H_WIN EWin_FrameWindowCreate(char * name, EWin_InheritInfo_ForHeader * wndDpt, H_WIN hLayer, RECT * wndRect, __pGUI_WIN_CB wndProc)
{
    H_WIN hWin;
	__gui_framewincreate_para_t frmWndPara;

	eLIBs_memset(&frmWndPara, 0, sizeof(__gui_framewincreate_para_t));
    frmWndPara.name         = name;
    frmWndPara.dwStyle      = WS_VISIBLE;
	frmWndPara.dwExStyle    = WS_EX_NONE;
	frmWndPara.spCaption    = NULL;
	frmWndPara.hOwner       = NULL;
	frmWndPara.hHosting     = wndDpt->hWnd;
    frmWndPara.hLayer       = hLayer;
	frmWndPara.FrameWinProc = (__pGUI_WIN_CB)esKRNL_GetCallBack((__pCBK_t)wndProc);
    // frmWndPara.id            = 0;
    GG_CopyRect(&frmWndPara.rect, wndRect);
	frmWndPara.BkColor.alpha= 0;
	frmWndPara.BkColor.red  = 0;
	frmWndPara.BkColor.green= 0;
	frmWndPara.BkColor.blue = 0;
	frmWndPara.attr         = wndDpt;
	frmWndPara.dwReserved   = 0;
    hWin = GUI_FrmWinCreate(&frmWndPara);
    if (NULL == hWin) {
        LogE("Failed to create framework window (%s)", frmWndPara.name);
        return 0;
    }
    //GUI_WinSetStyle(hWin, WS_VISIBLE);
    GUI_WinUpdate(hWin, ORANGE_TRUE);
#ifdef THIS_DEBUG
    LogMI("Hello framework window (%s)", frmWndPara.name);
#endif
    return hWin;
}

H_WIN EWin_GestureAttrCollect(H_WIN start, __s32 x, __s32 y)
{
    H_WIN temp;
    H_WIN latest;
    EWin_InheritInfo_ForHeader * winInfo;
#ifdef THIS_DEBUG
    char name[GUI_NAME_MAX];
#endif

    latest = NULL;
    while (start) {
#ifdef THIS_DEBUG
        if (0 == GUI_WinGetName(start, name)) {
            LogMI("Win name=%s,%x\n", name, GUI_WinGetType(start));
        }
#endif
        if (TYPE_MANWIN == GUI_WinGetType(start)) {
            temp = GUI_WinGetFocusChild(start);
            if (!temp) {
                start = GUI_WinGetFirstChild(start);
            } else {
                start = temp;
            }
			continue;
        } else {
            temp = GUI_WinGetNextBro(start);
			if(temp){
               start = temp;
			}else{
               return GUI_WinGetFirstChild(GUI_WinGetParent(start));
			}
        }
        winInfo = GUI_WinGetAttr(start);
        if (winInfo) {
            if ((0 == EWin_CheckInherit(winInfo))&& (SCR_WNDINFOMASK_GESTURE & winInfo->mask[0]) &&
                (winInfo->gestureArgs.attr_x || winInfo->gestureArgs.attr_y)){
               
                if (TYPE_FRAMEWIN == GUI_WinGetType(start)) {
#ifdef THIS_DEBUG
                    LogMI("Win rectangle = %d,%d,%d,%d,%d,%d",
                        winInfo->wndRect.x, winInfo->wndRect.y,
                        winInfo->wndRect.width, winInfo->wndRect.height, x, y);
#endif
                    if (0 == GG_PointInRect(&winInfo->wndRect, x, y)) {
                        latest = start;
						break;
                    }
                } else if (TYPE_MANWIN == GUI_WinGetType(start)) {
                    latest = start;
                }
            }
        }
    }
    return latest;
}


