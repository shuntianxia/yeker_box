
#include <apps.h>
#include <inertia.h>
#include <ui_common.h>

#ifdef THIS_DEBUG
#undef THIS_DEBUG
#endif

#ifdef MW_PROJ_DEBUG
#define THIS_DEBUG
#endif

int MwFillSize(SIZE * sz, __u32 width, __u32 height)
{
    sz->width = width;
    sz->height = height;
    return 0;
}

int MwFillRect(RECT * rect, __s32 x, __s32 y, __u32 width, __u32 height)
{
    rect->x = x;
    rect->y = y;
    rect->width = width;
    rect->height = height;
    return 0;
}

int MwFillOffset(OFFSET * oset, __s32 x, __s32 y)
{
    oset->x = x;
    oset->y = y;
    return 0;
}

int MwCopyRect(RECT * dest, RECT * src)
{
    dest->x = src->x;
    dest->y = src->y;
    dest->width = src->width;
    dest->height = src->height;
    return 0;
}

// Include the border
int MwPointInRect(RECT * rect, __s32 x, __s32 y)
{
    if ((x >= rect->x && x <= (rect->x + rect->width)) &&
        (y >= rect->y && y <= (rect->y + rect->height))) {
        return 0;
    }
    return -1;
}

int MwFillFbInit(FB * fb)
{
    eLIBs_memset((void *)fb, 0, sizeof(FB));
    fb->fmt.type = FB_TYPE_RGB;
    fb->fmt.fmt.rgb.pixelfmt = PIXEL_COLOR_ARGB8888;
    fb->fmt.fmt.rgb.br_swap = 0;
    fb->fmt.fmt.rgb.pixseq = RGB_SEQ_ARGB;
    return 0;
}

int MwFillFbTypeRgb(FB * fb, __fb_type_t type, __pixel_rgbfmt_t pixelfmt, __rgb_seq_t pixelseq, __bool br_swap)
{
    fb->fmt.type = type;
    fb->fmt.fmt.rgb.pixelfmt = pixelfmt;
    fb->fmt.fmt.rgb.br_swap = br_swap;
    fb->fmt.fmt.rgb.pixseq = pixelseq;
    return 0;
}

int MwFillLayerParaInit(__disp_layer_para_t * lyrPara)
{
    eLIBs_memset((void *)lyrPara, 0, sizeof(__disp_layer_para_t));
    lyrPara->mode = DISP_LAYER_WORK_MODE_NORMAL;
    lyrPara->alpha_val = 190;
    lyrPara->pipe = 1;
    lyrPara->prio = 0xFF;
    lyrPara->channel = DISP_LAYER_OUTPUT_CHN_DE_CH1;
    return 0;
}

int MwLayerParaInit(__layerwincreate_para_t * layerWinCreate, FB * frameBuf,
                __disp_layer_para_t * lyrPara, char * lyrName, int rotation,
                __u32 pipe, __u32 width, __u32 height)
{
    eLIBs_memset((void *)layerWinCreate, 0, sizeof(__layerwincreate_para_t));

    MwFillFbInit(frameBuf);
    MwFillLayerParaInit(lyrPara);
    lyrPara->pipe = (__u8)pipe;

	if ((rotation == GUI_SCNDIR_ROTATE90) || (rotation == GUI_SCNDIR_ROTATE270)) {
        MwFillSize(&frameBuf->size, height, width);
        MwFillRect(&lyrPara->scn_win, 0, 0, height, width);
        MwFillRect(&lyrPara->src_win, 0, 0, height, width);
    } else {
        MwFillSize(&frameBuf->size, width, height);
        MwFillRect(&lyrPara->scn_win, 0, 0, width, height);
        MwFillRect(&lyrPara->src_win, 0, 0, width, height);
    }
	lyrPara->fb = frameBuf;
	layerWinCreate->name = lyrName;
	layerWinCreate->lyrpara = lyrPara;
	layerWinCreate->initsta = GUI_LYRWIN_STA_SLEEP;
    return 0;
}

int MwLayerParaInitEx(__layerwincreate_para_t * layerWinCreate, FB * frameBuf,
                __disp_layer_para_t * lyrPara, char * lyrName, int rotation,
                __u32 pipe, RECT * rect)
{
    eLIBs_memset((void *)layerWinCreate, 0, sizeof(__layerwincreate_para_t));

    MwFillFbInit(frameBuf);
    MwFillLayerParaInit(lyrPara);
    lyrPara->pipe = (__u8)pipe;

	if ((rotation == GUI_SCNDIR_ROTATE90) || (rotation == GUI_SCNDIR_ROTATE270)) {
        MwFillSize(&frameBuf->size, rect->height, rect->width);
        MwFillRect(&lyrPara->scn_win, rect->y, rect->x, rect->height, rect->width);
        MwFillRect(&lyrPara->src_win, 0, 0, rect->height, rect->width);
    } else {
        MwFillSize(&frameBuf->size, rect->width, rect->height);
        MwFillRect(&lyrPara->scn_win, rect->x, rect->y, rect->width, rect->height);
        MwFillRect(&lyrPara->src_win, 0, 0, rect->width, rect->height);
    }
	lyrPara->fb = frameBuf;
	layerWinCreate->name = lyrName;
	layerWinCreate->lyrpara = lyrPara;
	layerWinCreate->initsta = GUI_LYRWIN_STA_SLEEP;
    return 0;
}

int MwEnableKeycolor(void)
{
    __u32 arg[3];
    ES_FILE * p_disp;
    __disp_colorkey_t colorkey;

    // open display driver handle
    p_disp = eLIBs_fopen(MW_DEV_DISPLAY, "r+");
    if (!p_disp) {
        log_e("Open display device fail!\n");
        return -1;
    }
    colorkey.ck_min.alpha = 0;
    colorkey.ck_min.red   = 0;
    colorkey.ck_min.green = 0;
    colorkey.ck_min.blue  = 0;
    colorkey.ck_max.alpha = 1;
    colorkey.ck_max.red   = 1;
    colorkey.ck_max.green = 1;
    colorkey.ck_max.blue  = 1;
    colorkey.red_match_rule= 2;
    colorkey.green_match_rule= 2;
    colorkey.blue_match_rule= 2;
    eLIBs_fioctrl(p_disp, DISP_CMD_LCD_ON, 0, 0);
    arg[0] = (__u32)&colorkey;
    arg[1] = 0;
    arg[2] = 0;
    eLIBs_fioctrl(p_disp, DISP_CMD_SET_COLORKEY, 0, (void *)arg);
    eLIBs_fclose(p_disp);
    return 0;
}

H_WIN MwGetToppestWin(H_WIN start)
{
    H_WIN next;
#ifdef THIS_DEBUG
    char name[GUI_NAME_MAX];
#endif
    next = start;
    while (next) {
#ifdef THIS_DEBUG
        if (ORANGE_OK == GUI_WinGetName(next, name)) {
            log_mi("Win name=%s", name);
        }
#endif
        start = next;
        next = GUI_WinGetFirstChild(next);
    }
    return start;
}


