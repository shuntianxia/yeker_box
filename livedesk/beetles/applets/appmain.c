
#include <apps.h>
#include "beetles_app.h"
//#include <inertia.h>

//#include <ui_common.h>
//#include <uicfg.h>
//#include <scrolling.h>

#include <gadget.h>
#include "./cp/home.h"
#include "./cp/cp_i.h"

static H_WIN hDesktop;

static __s32 msg_hook_cb(__gui_msg_t * msg)
{
    __gui_msg_t msgex;
    msgex.id        = GUI_MSG_TOUCH;
    msgex.h_srcwin  = NULL;
    msgex.h_deswin  = hDesktop;
    msgex.dwAddData1= msg->dwAddData1;
    msgex.dwAddData2= msg->dwAddData2;
    msgex.p_arg     = NULL;
    GUI_PostMessage(&msgex);
    return EPDK_OK;
}
#if 0
__s32 app_main(void * p_arg)
{
    __memit_hook_t emit_hook;
    __gui_msg_t msg;
    __s32 ret;
    gg_handler domUIConfig;
    LogI("===== Application Startup =====\n");
    LogI("\n Memory: totalSize = %x, freeSize = %x\n", esMEMS_TotalMemSize(), esMEMS_FreeMemSize());
    //register_uicfg(MW_DEFAULT_UI_CONFIG);
    domUIConfig = GG_DOMCreate(CP_DEFAULT_UI_CONFIG);
    if (domUIConfig) {
        hDesktop = HomeWndCreate(NULL, domUIConfig);
        //GG_DOMDestroy(domUIConfig);
        GUI_SetActiveManWin(hDesktop);
    } else {
        // Create the first window, that is desktop.
        //hDesktop = MwScrollingWndCreate(NULL);
	    // GUI_SetActiveManWin(hDesktop);
    }
    LogI("\n After UI Parser Memory: totalSize = %x, freeSize = %x\n", esMEMS_TotalMemSize(), esMEMS_FreeMemSize());

    // Create the message forward feature.
    msg_emit_init();
    emit_hook.msg_hook = msg_hook_cb;
    msg_emit_register_hook(&emit_hook);

    // Begin looping UI message.
    while (GUI_GetMessageEx(&msg, hDesktop)) {
        ret = GUI_DispatchMessage(&msg);
        if (msg.p_arg) {
            GUI_SetSyncMsgRetVal(&msg, ret);
            GUI_PostSyncSem(&msg);
        }
    }

	GUI_WinThreadCleanup(hDesktop);	

    LogI("\n===== Application End =====\n");
	return EPDK_OK;	
}
#else
H_WIN app_main_create(root_para_t *para)
{
	gg_handler domUIConfig;
	H_WIN hwin = NULL;

	LogI("============= Application Startup ==============");
    LogI("Memory %x,%x", esMEMS_TotalMemSize(), esMEMS_FreeMemSize());
    //register_uicfg(MW_DEFAULT_UI_CONFIG);
    __here__;
	user_theme_init("d:\\res\\theme.bin");
    domUIConfig = GG_DOMCreate(CP_DEFAULT_UI_CONFIG);
    if (domUIConfig) {
    	__here__;
        hwin = HomeWndCreate(para->h_parent, domUIConfig);
        __here__;
        GG_DOMDestroy(domUIConfig);
        GUI_SetActiveManWin(hwin);
		GUI_WinSetTouchFocus(hwin);
    } else {
    	__here__;
        // Create the first window, that is desktop.
        //hDesktop = MwScrollingWndCreate(NULL);
	    // GUI_SetActiveManWin(hDesktop);
    }
    __here__;
    LogI("Memory %x,%x", esMEMS_TotalMemSize(), esMEMS_FreeMemSize());
    return hwin;
}
#endif

