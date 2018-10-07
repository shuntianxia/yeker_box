
#include "cp_i.h"
#include "scrolling_i.h"
#include "transform.h"

#ifdef THIS_DEBUG
#undef THIS_DEBUG
#endif

#ifdef MW_PROJ_DEBUG
//#define THIS_DEBUG
#endif

#define WATCH_OFFSET_X      64
#define WATCH_OFFSET_Y      0

#define DIGIT_OFFSET_X      70
#define DIGIT_OFFSET_Y      180
#define DIGIT_OFFSET_SPACE  38
#define DIGIT_OFFSET_COLON  24
typedef struct {
    SCR_ScrollingWndInfo_ForHeader scrolling;
    int invalidate;
    __time_t tm;
    void * hhdl;
    void * mhdl;
    void * shdl;
    int bgicon;
    int bgblock;
    int forgblock;
    __u8 minute;
} ClockPrivate;

static __s32 clockWndProc(__gui_msg_t * msg)
{
    __u32 clockhands[CLOCKHANDS];
    FB fb;

    HTHEME hback;
    HBLOCK block;
    void * pic;
    int length;
    int start_x;
    int start_y;
#if 0
    ClockPrivate * draw;
    draw = (ClockPrivate *)GUI_WinGetAttr(msg->h_deswin);
    if ((!draw) || (-1 == EWin_CheckInherit(&draw->scrolling.ewii)) ||
        (!(draw->scrolling.ewii.mask[0] & SCR_WNDINFOMASK_SBUTTON))) {
        LogE("Null or wrong scrolling page window private data");
        return -1;
    }
    switch (msg->id) {
    case GUI_MSG_CREATE:
		draw->scrolling.ewii.mask[0] |= SCR_WNDINFOMASK_GESTURE;
        clockhands[0] = ID_CLOCK_PNG_DIG_HAND_H1_BMP;
        clockhands[1] = ID_CLOCK_PNG_DIG_HAND_H2_BMP;
        clockhands[2] = ID_CLOCK_PNG_DIG_HAND_H3_BMP;
        clockhands[3] = ID_CLOCK_PNG_DIG_HAND_H4_BMP;
        clockhands[4] = ID_CLOCK_PNG_DIG_HAND_H5_BMP;
        clockhands[5] = ID_CLOCK_PNG_DIG_HAND_H6_BMP;
        clockhands[6] = ID_CLOCK_PNG_DIG_HAND_H7_BMP;
        clockhands[7] = ID_CLOCK_PNG_DIG_HAND_H8_BMP;
        draw->hhdl = picswap_quadrant_begin(clockhands, 25, 80);
        clockhands[0] = ID_CLOCK_PNG_DIG_HAND_M1_BMP;
        clockhands[1] = ID_CLOCK_PNG_DIG_HAND_M2_BMP;
        clockhands[2] = ID_CLOCK_PNG_DIG_HAND_M3_BMP;
        clockhands[3] = ID_CLOCK_PNG_DIG_HAND_M4_BMP;
        clockhands[4] = ID_CLOCK_PNG_DIG_HAND_M5_BMP;
        clockhands[5] = ID_CLOCK_PNG_DIG_HAND_M6_BMP;
        clockhands[6] = ID_CLOCK_PNG_DIG_HAND_M7_BMP;
        clockhands[7] = ID_CLOCK_PNG_DIG_HAND_M8_BMP;
        draw->mhdl = picswap_quadrant_begin(clockhands, 25, 80);
        clockhands[0] = ID_CLOCK_PNG_DIG_HAND_S1_BMP;
        clockhands[1] = ID_CLOCK_PNG_DIG_HAND_S2_BMP;
        clockhands[2] = ID_CLOCK_PNG_DIG_HAND_S3_BMP;
        clockhands[3] = ID_CLOCK_PNG_DIG_HAND_S4_BMP;
        clockhands[4] = ID_CLOCK_PNG_DIG_HAND_S5_BMP;
        clockhands[5] = ID_CLOCK_PNG_DIG_HAND_S6_BMP;
        clockhands[6] = ID_CLOCK_PNG_DIG_HAND_S7_BMP;
        clockhands[7] = ID_CLOCK_PNG_DIG_HAND_S8_BMP;
        draw->shdl = picswap_quadrant_begin(clockhands, 25, 80);
        draw->bgicon = ID_CLOCK_BMP_DIGITALCLOCK_BG_BMP;
        GUI_LyrWinGetFB(draw->scrolling.ewii.lyrLow, &fb);
        draw->bgblock = GUI_BlockCreateFrom(NULL, fb.size.width, fb.size.height, fb.addr[0], 0);
        GUI_LyrWinGetFB(draw->scrolling.ewii.lyrHigher, &fb);
        draw->forgblock = GUI_BlockCreateFrom(NULL, fb.size.width, fb.size.height, fb.addr[0], 0);
        esTIME_GetTime(&draw->tm);
		draw->minute = draw->tm.minute;
        draw->invalidate |= INVALIDATE_BACKGROUND|INVALIDATE_FOREGROUND;
        break;
    case GUI_MSG_DESTROY:
        GUI_BlockDelete(draw->forgblock);
        GUI_BlockDelete(draw->bgblock);
        picswap_quadrant_end(draw->shdl);
        picswap_quadrant_end(draw->mhdl);
        picswap_quadrant_end(draw->hhdl);
        esMEMS_Mfree(0, draw);
        break;
    case GUI_MSG_PAINT:
        if (draw->invalidate & INVALIDATE_BACKGROUND) {
#ifdef THIS_DEBUG
            log_d("Invalidate background");
#endif
            GUI_LyrWinSel(draw->scrolling.ewii.lyrLow);
            hback = dsk_Systheme_open(draw->bgicon);
            pic = dsk_Systheme_hdl2buf(hback);
            GUI_BMP_Draw(pic, draw->scrolling.wndRect.x, draw->scrolling.wndRect.y);
            dsk_theme_close(hback);
            pic = picget_clock_pic(draw->hhdl, (draw->tm.hour % 12) * 5 + (draw->tm.minute / 12), &length, &start_x, &start_y);
            block = GUI_BlockCreateFrom(NULL, length, length, pic, 0);
            GUI_BlockBitBlit(draw->bgblock, draw->scrolling.wndRect.x + start_x + WATCH_OFFSET_X,
                draw->scrolling.wndRect.y + start_y + WATCH_OFFSET_Y,
                block, NULL, BLITFLAG_COLORALPHA, NULL);
            GUI_BlockDelete(block);
            pic = picget_clock_pic(draw->mhdl, draw->tm.minute, &length, &start_x, &start_y);
            block = GUI_BlockCreateFrom(NULL, length, length, pic, 0);
            GUI_BlockBitBlit(draw->bgblock, draw->scrolling.wndRect.x + start_x + WATCH_OFFSET_X,
                draw->scrolling.wndRect.y + start_y + WATCH_OFFSET_Y,
                block, NULL, BLITFLAG_COLORALPHA, NULL);
            GUI_BlockDelete(block);
        }
        
		if (draw->invalidate & INVALIDATE_FOREGROUND) {
			GUI_LyrWinSel(draw->scrolling.ewii.lyrHigher);
			GUI_SetBkColor(0);
            GUI_ClearRect(draw->scrolling.wndRect.x, draw->scrolling.wndRect.y,
                draw->scrolling.wndRect.x + draw->scrolling.wndRect.width, draw->scrolling.wndRect.y + draw->scrolling.wndRect.height);
        }
        pic = picget_clock_pic(draw->shdl, draw->tm.second, &length, &start_x, &start_y);
        block = GUI_BlockCreateFrom(NULL, length, length, pic, 0);
        GUI_BlockBitBlit(draw->forgblock, draw->scrolling.wndRect.x + start_x + WATCH_OFFSET_X,
            draw->scrolling.wndRect.y + start_y + WATCH_OFFSET_Y,
            block, NULL, BLITFLAG_COLORSET, NULL);
        GUI_BlockDelete(block);

        draw->invalidate = INVALIDATE_NULL;
        break;
    //case GUI_MSG_USER_REPAINT_NEIGHBOR:
        //draw->invalidate |= (INVALIDATE_BACKGROUND | INVALIDATE_FOREGROUND);
    case GUI_MSG_TIMER:
        esTIME_GetTime(&draw->tm);
        if (draw->minute != draw->tm.minute) {
            draw->minute = draw->tm.minute;
            draw->invalidate |= INVALIDATE_BACKGROUND;
        }
#ifdef THIS_DEBUG
        log_mi("time: %d,%d,%d", draw->tm.hour, draw->tm.minute, draw->tm.second);
        log_mi("Date: %d,%d,%d", draw->date.year, draw->date.month, draw->date.day);
#endif
        __here__;
        GUI_WinUpdate(msg->h_deswin, ORANGE_TRUE);
        break;
    case GUI_MSG_SET_FOCUS:
        draw->invalidate |= (INVALIDATE_BACKGROUND | INVALIDATE_FOREGROUND);
        GUI_SetTimer(msg->h_deswin, 1, 100, NULL);
		__msg("12");
        return 0;
    case GUI_MSG_SET_UNFOCUS:
        GUI_KillTimer(msg->h_deswin, 1);
        return 0;
    default:
        break;
    }
#endif
    return GUI_FrmWinDefaultProc(msg);
}

H_WIN DrawClockCreate(char * name, SCR_ScrollingWndInfo_ForHeader * inherit)
{
    SCR_ScrollingWndInfo_ForHeader * wndInfo;
    H_WIN hWnd;

    hWnd = ScrollingAppendManage(name, inherit);
    if (hWnd) {
        wndInfo = (SCR_ScrollingWndInfo_ForHeader *)GUI_WinGetAttr(hWnd);
        ScrollingAppendPage("clock_page", wndInfo, 0, GG_NULL, inherit->ewii.lyrLow,NULL);//clockWndProc
        ScrollingAppendButton("clock_button", wndInfo, NULL, clockWndProc);
    }
    return hWnd;
}



