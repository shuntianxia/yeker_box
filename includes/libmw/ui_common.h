
#ifndef H_UI_COMMON_BENNIS_20121031
#define H_UI_COMMON_BENNIS_20121031

#define MW_MULTI_TOUCH_NR   1

typedef enum {
    GUI_MSG_USER_GESTURE_MOVE   = (GUI_MSG_USER_DEF + 1),   // For scrolling event.
    GUI_MSG_USER_GESTURE_BOUND  = (GUI_MSG_USER_DEF + 2),   // For scrolling event.
    GUI_MSG_USER_GESTURE_DONE   = (GUI_MSG_USER_DEF + 3),   // For scrolling event. Inertia will send this message out if moving is done.
    GUI_MSG_USER_GESTURE_CLICK  = (GUI_MSG_USER_DEF + 4),   // For click event
    GUI_MSG_USER_GESTURE_DCLICK = (GUI_MSG_USER_DEF + 5),   // For double click event
    GUI_MSG_USER_GESTURE_LPRESS = (GUI_MSG_USER_DEF + 6),   // For long-press event
    GUI_MSG_USER_GESTURE_ERROR  = (GUI_MSG_USER_DEF + 7),   // Error handle
    GUI_MSG_USER_GESTURE_NEIGHBOR = (GUI_MSG_USER_DEF + 8), // Sends to desktop window
    GUI_MSG_USER_GESTURE_PAINT_NEIGHBOR = (GUI_MSG_USER_DEF + 9), // External used message
	GUI_MSG_USER_GESTURE_CLICK_MSG = (GUI_MSG_USER_DEF + 10), // External used message
	GUI_MSG_USER_GESTURE_CONTRAL = (GUI_MSG_USER_DEF + 11), // External used message
    GUI_MSG_USER_WIN_SWITCH = (GUI_MSG_USER_DEF + 12)
} __gui_msgid_user_t;

typedef enum {
    MW_MOTDIR_LEFT_RIGHT = 0,
    MW_MOTDIR_RIGHT_LEFT,
    MW_MOTDIR_TOP_DOWN,
    MW_MOTDIR_BOTTOM_UP,
    MW_MOTDIR_NONE
} MwMotionDirect;

#define inline                  __inline
#define MW_DEV_DISPLAY          "b:\\DISP\\DISPLAY"
#define MW_DEFAULT_FONT_SIZE    20
#define MW_DEFAULT_FONT_PATH    "d:\\res\\fonts\\fangsong_20.sft"
#define MW_SYS_FONT_SIZE        20
#define MW_SYS_FONT_PATH        "d:\\res\\fonts\\fangsong_20.sft"
#define MW_DEFAULT_UI_CONFIG    "d:\\res\\profile\\ui_utf_8.xml"

#ifdef MW_PROJ_DEBUG

#define log_d(...)      (eLIBs_printf("<Debug||%s||L%d> ", __FILE__, __LINE__), \
                                eLIBs_printf(__VA_ARGS__), eLIBs_printf("\n"))
#define log_i(...)      (eLIBs_printf(__VA_ARGS__), eLIBs_printf("\n"))
#define log_mi(...)     (eLIBs_printf("<%s||L%d> ", __FILE__, __LINE__), \
                            eLIBs_printf(__VA_ARGS__), eLIBs_printf("\n"))
#define log_w(...)      (eLIBs_printf("<Warning||%s||L%d> ", __FILE__, __LINE__), \
                                eLIBs_printf(__VA_ARGS__), eLIBs_printf("\n"))
#define log_e(...)      (eLIBs_printf("<Error||%s||L%d> ", __FILE__, __LINE__), \
                            eLIBs_printf(__VA_ARGS__), eLIBs_printf("\n"))
#define log_fatal(...)  (eLIBs_printf("<Fatal||%s||L%d> ", __FILE__, __LINE__), \
                            eLIBs_printf(__VA_ARGS__), eLIBs_printf("\n"), esKSRV_Reset())

#else

#define log_d(...)
#define log_i(...)
#define log_mi(...)
#define log_w(...)
#define log_e(...)
#define log_fatal(...)

#endif

#define MW_MATH_SWAP_BYTE(x)        ((x) << 24 | (((x) << 8) & 0x00FF0000) | (((x) >> 8) & 0x0000FF00) | (((x) >> 24) & 0x000000FF))
#define MW_MATH_ABS(x)              (((x) < 0) ? -(x) : (x))
#define MW_MATH_MAX(x, y)           ((x) < (y) ? (y) : (x))
#define MW_MATH_MIN(x, y)           ((x) > (y) ? (y) : (x))
#define MW_MATH_HALF(x)             ((x) >> 1)
#define MW_MATH_DOUBLE(x)           ((x) << 1)
#define MW_MATH_IS_ODD(x)           ((x) & 0x1)
#define MW_MATH_CYCLE_INC_0(x, max)  \
    {(x) += 1; (x) = ((x) >= (max)) ? 0 : (x);}
#define MW_MATH_CYCLE_DEC_0(x, max)  \
    {(x) -= 1; (x) = ((x) < 0) ? ((max) - 1) : (x);}

int MwFillSize(SIZE * sz, __u32 width, __u32 height);
int MwFillRect(RECT * rect, __s32 x, __s32 y, __u32 width, __u32 height);
int MwFillOffset(OFFSET * oset, __s32 x, __s32 y);
int MwCopyRect(RECT * dest, RECT * src);
int MwPointInRect(RECT * rect, __s32 x, __s32 y);
int MwFillFbInit(FB * fb);
int MwFillFbTypeRgb(FB * fb, __fb_type_t type, __pixel_rgbfmt_t pixelfmt, __rgb_seq_t pixelseq, __bool br_swap);
int MwFillLayerParaInit(__disp_layer_para_t * lyrpara);
int MwLayerParaInit(__layerwincreate_para_t * layerWinCreate, FB * frameBuf,
                __disp_layer_para_t * lyrPara, char * lyrName, int rotation,
                __u32 pipe, __u32 width, __u32 height);
int MwLayerParaInitEx(__layerwincreate_para_t * layerWinCreate, FB * frameBuf,
                __disp_layer_para_t * lyrPara, char * lyrName, int rotation,
                __u32 pipe, RECT * rect);

void * MwGDICreate(void);
int MwGDIDestroy(void * handler);
int MwSysTextOut(void * gdi, __u32 text_id, int x, int y);
H_WIN MwGetToppestWin(H_WIN start);
#define MW_BROTHER_WIN_ITERATE_BEGIN(wnd)   while (wnd) {
#define MW_BROTHER_WIN_ITERATE_END(wnd)     (wnd) = GUI_WinGetNextBro(wnd); }

/* Window Information Data Type */
#define WIDT_INERTIA        0x1

typedef struct {
    long widtMask;
    iner_start_t iner;
    RECT wndRect;
    H_LYR lyrBg;        // Background Layer
    RECT lyrRectBg;
    H_LYR lyrFore;      // Foreground Layer
    RECT lyrRectFore;
} WndInheritInfo;

#define WNDINFO_STRUCT_HEADER_DECLARE()     WndInheritInfo common

#endif

