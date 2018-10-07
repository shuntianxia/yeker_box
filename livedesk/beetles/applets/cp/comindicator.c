
#include <apps.h>
//#include "scrolling_common.h"

#include "comindicator.h"

typedef struct {
    MwIndicatorWndArgs cls;
    //H_LYR lyrTop;
    HTHEME on;
    void * onData;
    HTHEME off;
    void * offData;
    SIZE picture;
    SIZE block;
	int start;
} IndicatorWndDesc;

static __s32 IndicatorWndProc(__gui_msg_t * msg)
{
	#if 0
    IndicatorWndDesc * desc;
    int i;
    switch (msg->id) {
    case GUI_MSG_CREATE:
        desc = (IndicatorWndDesc *)GUI_WinGetAttr(msg->h_deswin);
        if (NULL == desc) {
            return -1;
        }
        desc->on = dsk_theme_open(ID_CLOCK_COM_PAGE_S_BMP);
        desc->onData = dsk_theme_hdl2buf(desc->on);
        desc->off = dsk_theme_open(ID_CLOCK_COM_PAGE_BMP);
        desc->offData = dsk_theme_hdl2buf(desc->off);
        MwFillSize(&desc->picture, 12, 12);
        MwFillSize(&desc->block, 26, 12);
		desc->start = ((desc->cls.wndRect.width - (desc->block.width * desc->cls.winNr)) >> 1) + 7;
        if (desc->start) {
            desc->start;
        }
        break;
    case GUI_MSG_DESTROY:
        desc = (IndicatorWndDesc *)GUI_WinGetAttr(msg->h_deswin);
        if (desc) {
            dsk_theme_close(desc->off);
            dsk_theme_close(desc->on);
            esMEMS_Mfree(0, desc);
        }
        break;
    case GUI_MSG_PAINT:
        desc = (IndicatorWndDesc *)GUI_WinGetAttr(msg->h_deswin);
        if (desc) {
            GUI_LyrWinSel(desc->cls.lyrTop);
            for (i = 0; i <desc->cls.winNr; i++) {
                if (i == msg->dwAddData1) {
                   GUI_BMP_Draw(desc->onData, desc->cls.wndRect.x  + desc->start + (i * desc->block.width), desc->cls.wndRect.y+5);
                } else {
                   GUI_BMP_Draw(desc->offData, desc->cls.wndRect.x + desc->start + (i * desc->block.width), desc->cls.wndRect.y+5);
                }
            }
        }
        break;
    default:
        break;
    }
    #endif
    return GUI_FrmWinDefaultProc(msg);
}

H_WIN MwIndicatorWndCreate(char * wndName, H_WIN parent, MwIndicatorWndArgs * args)
{
	__gui_framewincreate_para_t frmWndPara;
    IndicatorWndDesc          * desc;
    H_WIN                       hWin;

    desc = (IndicatorWndDesc *)esMEMS_Malloc(0, sizeof(IndicatorWndDesc));
    if (NULL == desc) {
        __msg("Malloc error");
        return 0;
    }

    eLIBs_memset(desc, 0, sizeof(IndicatorWndDesc));
    eLIBs_memcpy(&desc->cls, args, sizeof(MwIndicatorWndArgs));

	eLIBs_memset(&frmWndPara, 0, sizeof(__gui_framewincreate_para_t));
    frmWndPara.name         = wndName,
    frmWndPara.dwStyle      = WS_VISIBLE;
	frmWndPara.dwExStyle    = WS_EX_NONE;
	frmWndPara.spCaption    = NULL;
	frmWndPara.hOwner       = NULL;
	frmWndPara.hHosting     = parent;
    frmWndPara.hLayer       = desc->cls.lyrTop;
	frmWndPara.FrameWinProc = (__pGUI_WIN_CB)esKRNL_GetCallBack((__pCBK_t)IndicatorWndProc);
    // frmWndPara.id            = 0;
    MwCopyRect(&frmWndPara.rect, &args->wndRect);
	frmWndPara.BkColor.alpha= 0;
	frmWndPara.BkColor.red  = 0;
	frmWndPara.BkColor.green= 0;
	frmWndPara.BkColor.blue = 0;
	frmWndPara.attr         = desc;
	frmWndPara.dwReserved   = 0;

    hWin = GUI_FrmWinCreate(&frmWndPara);

    if (NULL == hWin) {
        __msg("Failed to create framework window (%s)", frmWndPara.name);
        return 0;
    }
    GUI_WinUpdate(hWin,ORANGE_TRUE);
    __msg("Hello framework window (%s)", frmWndPara.name);
    return hWin;
}

int MwIndicatorWndUpdate(H_WIN hWnd, int index)
{
    __gui_msg_t new_msg;
    new_msg.id			= GUI_MSG_PAINT,
	new_msg.h_srcwin	= NULL;
	new_msg.h_deswin	= hWnd;
	new_msg.dwAddData1	= index;
	new_msg.dwAddData2	= 0;
	new_msg.p_arg		= NULL;
	
    //GUI_PostMessage (&new_msg);
    GUI_SendAsyncMessage (&new_msg);
    return 0;
}


