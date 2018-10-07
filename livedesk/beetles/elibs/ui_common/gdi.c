
#include <apps.h>
#include <inertia.h>
#include <ui_common.h>

#include "gdi.h"

#ifdef THIS_DEBUG
#undef THIS_DEBUG
#endif
#ifdef UIC_DEBUG
#define THIS_DEBUG
#endif

void * MwGDICreate(void)
{
    mw_gdi_t * gdi;
    gdi = (mw_gdi_t *)esMEMS_Malloc(0, sizeof(mw_gdi_t));
    gdi->font = GUI_SFT_CreateFont(MW_SYS_FONT_SIZE, MW_SYS_FONT_PATH);
    return gdi;
}

int MwGDIDestroy(void * handler)
{
    if (handler) {
        esMEMS_Mfree(0, handler);
    } else {
        log_e("Wrong handler");
    }
    return 0;
}

int MwSysTextOut(void * gdi, __u32 text_id, int x, int y)
{
    mw_gdi_t * gdi_in;
    char GUI_UNI_PTR text[32];
    if (!gdi) {
        log_e("Null GDI handler");
        return -1;
    }
    gdi_in = (mw_gdi_t *)gdi;
    if (dsk_langres_get_menu_text(text_id, text, 32) > 0) {
        GUI_OpenAlphaBlend();
        GUI_CharSetToEncode(EPDK_CHARSET_ENM_UTF8);
        GUI_SetFont(gdi_in->font);
        GUI_SetDrawMode(GUI_DRAWMODE_TRANS);
        GUI_SetColor(GUI_BLACK);
        GUI_DispStringAt(text, x, y);
        GUI_CloseAlphaBlend();
    }
    return 0;
}


