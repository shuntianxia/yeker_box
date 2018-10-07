
#ifndef H_GADGET_BENNIS_20130124
#define H_GADGET_BENNIS_20130124

#ifdef __cplugplug
extern "C" {
#endif

/**
 * Common definitions
 */
typedef char                    gg_int8;
typedef unsigned char           gg_uint8;
typedef short                   gg_int16;
typedef unsigned short          gg_uint16;
typedef int                     gg_int32;
typedef unsigned int            gg_uint32;
typedef char                    gg_bool;
typedef char                    gg_char;
typedef char                    gg_wchar;
typedef void *                  gg_handler;
typedef unsigned long           gg_size;
#define GG_TRUE                 1
#define GG_FALSE                0
#define GG_NULL                 ((void *)0)
#define GG_LF                   '\r'
#define GG_TAB                  '\t'
#define GG_NL                   '\n'

/**
 * DOM module
 */
extern gg_handler GG_DOMCreate(gg_wchar * document);
extern gg_int32 GG_DOMDestroy(gg_handler handler);
extern gg_handler GG_DOMGetRoot(gg_handler handler);
extern gg_int32 GG_DOMGetElementsByTagName(gg_handler node, gg_wchar * tag);
extern gg_handler GG_DOMGetNextElementByTagName(gg_handler node, gg_wchar * tag);
extern gg_handler GG_DOMGetBrotherElementByTagName(gg_handler node, gg_wchar * tag);
extern gg_handler GG_DOMGetpreBrotherElementByTagName(gg_handler node, gg_wchar * tag);

extern gg_int32 GG_DOMHasChildNodes(gg_handler node);
extern gg_handler GG_DOMGetAncestor(gg_handler node);
extern gg_handler GG_DOMGetFirstChild(gg_handler node);
extern gg_handler GG_DOMGetNextBrother(gg_handler node);
extern gg_wchar * GG_DOMGetNodeName(gg_handler node);
extern gg_wchar ** GG_DOMGetNodeAttributes(gg_handler node);
extern gg_wchar * GG_DOMGetNodeValue(gg_handler node);

/**
 * Utilities
 */
#define GG_MATH_SWAP_BYTE(x)        ((x) << 24 | (((x) << 8) & 0x00FF0000) | (((x) >> 8) & 0x0000FF00) | (((x) >> 24) & 0x000000FF))
#define GG_MATH_ABS(x)              (((x) < 0) ? -(x) : (x))
#define GG_MATH_MAX(x, y)           ((x) < (y) ? (y) : (x))
#define GG_MATH_MIN(x, y)           ((x) > (y) ? (y) : (x))
#define GG_MATH_HALF(x)             ((x) >> 1)
#define GG_MATH_DOUBLE(x)           ((x) << 1)
#define GG_MATH_IS_ODD(x)           ((x) & 0x1)
#define GG_MATH_CYCLE_INC_0(x, max)  \
        {(x) += 1; (x) = ((x) >= (max)) ? 0 : (x);}
#define GG_MATH_CYCLE_DEC_0(x, max)  \
        {(x) -= 1; (x) = ((x) < 0) ? ((max) - 1) : (x);}

extern int GG_FillSize(SIZE * sz, __u32 width, __u32 height);
extern int GG_FillRect(RECT * rect, __s32 x, __s32 y, __u32 width, __u32 height);
extern int GG_FillOffset(OFFSET * oset, __s32 x, __s32 y);
extern int GG_CopySize(SIZE * dest, SIZE * src);
extern int GG_CopyRect(RECT * dest, RECT * src);
// Include the border
extern int GG_PointInRect(RECT * rect, __s32 x, __s32 y);
extern gg_char * GG_DupStr(gg_char * src);
// Return the library version number
extern int GG_GetVersion(void);

/**
 * GDI module
 */
typedef struct {
    GUI_FONT * font;
    unsigned int fontSize;
} GG_GDI;

/**
 * Gesture module
 *
 * 1. Enhanced gesture feature launching arguments.
 *    For the meanings of the parameters, see the structure definition of 'GG_GESTUREATTR'.
 *    Especially, in this structure, the gesture operation type corresponds to the Window handle,
 *    that is, this gesture operation result message will be sent to the corresponding Window handle.
 * 2. Window System should be has inheritable capabilities.
 *    In detail, the child window is able to offer the aggregated parent windows and
 *    myself window gesture attributes to Gesture module.
 * 3. Regarding Window System, each window which has its own gesture attribute.
 *    And its gesture attribute will be inherited or overwritten by the child window.
 *    So, be careful to handle window gesture attribute.
 * 4. Gesture process procedure.
 *   a. When the touch event happen, Window System collect all of the windows gesture attributes from
 *      bottom to top, that is, from parent window to child window.
 *      Please see the function of 'EWin_GestureAttrCollect'.
 *   b. The final gesture attribute for Gesture module has this priority,
 *      the last main window --> the last frame window with touch point captured.
 */

#define GUI_MSG_USER_GESTURE_START  (GUI_MSG_USER_DEF + 0x20)

typedef enum {
    GUI_MSG_USER_GESTURE_MOVE   = (GUI_MSG_USER_GESTURE_START + 0),     // For scrolling event.
    GUI_MSG_USER_GESTURE_DONE   = (GUI_MSG_USER_GESTURE_START + 1),     // For scrolling event. Inertia will send this message out if moving is done.
    GUI_MSG_USER_GESTURE_CLICK  = (GUI_MSG_USER_GESTURE_START + 2),     // For click event
    GUI_MSG_USER_GESTURE_MO     = (GUI_MSG_USER_GESTURE_START + 3),     // For double click event
    GUI_MSG_USER_GESTURE_LPRESS = (GUI_MSG_USER_GESTURE_START + 4),     // For long-press event
    GUI_MSG_USER_GESTURE_ERROR  = (GUI_MSG_USER_GESTURE_START + 5),     // Error handle
    GUI_MSG_USER_GESTURE_PAINT_NEIGHBOR = (GUI_MSG_USER_GESTURE_START + 6),   // Sends to desktop window
	GUI_MSG_USER_GESTURE_CLICK_MSG = (GUI_MSG_USER_DEF + 7), // External used message
	GUI_MSG_USER_GESTURE_CONTRAL = (GUI_MSG_USER_DEF + 8), // External used message
	GUI_MSG_USER_GESTURE_SHANG_MSG = (GUI_MSG_USER_DEF + 9), //Ö÷¼ü±ä»¯
	GUI_MSG_USER_GESTURE_XIA_MSG = (GUI_MSG_USER_DEF + 10), 
	GUI_MSG_USER_GESTURE_RED_ZHUANGTAI = (GUI_MSG_USER_DEF + 11), 
	GUI_MSG_USER_GESTURE_CHANGE_ZHUANGTAI = (GUI_MSG_USER_DEF + 12), 
    GUI_MSG_USER_GESTURE_END
} GG_GESTUREMESSAGE;

typedef enum {
    GG_GESTURE_DIR_LEFT_RIGHT = 0,
    GG_GESTURE_DIR_RIGHT_LEFT,
    GG_GESTURE_DIR_TOP_DOWN,
    GG_GESTURE_DIR_BOTTOM_UP
} GG_GESTUREDIRECT;

typedef enum {
    GG_GESTURE_MSG_DEST_CLICK = 0,
    GG_GESTURE_MSG_DEST_MO,
    GG_GESTURE_MSG_DEST_LPRESS,
    GG_GESTURE_MSG_DEST_SLIDE,
    GG_GESTURE_MSG_DEST_INER,
    GG_GESTURE_MSG_DEST_INER_SBLOCK,
    GG_GESTURE_MSG_DEST_INER_BLOCKS,
    GG_GESTURE_MSG_DEST_MAX
} GG_GESTUREMSGDEST;

// Gesture Attribute
#define GG_GESTURE_ATTR_POSITIVE    0x0001  // Default is negative motion
#define GG_GESTURE_ATTR_CLICK       0x0080  // Enable click feature
#define GG_GESTURE_ATTR_MO          0x0010  // Enable touch
#define GG_GESTURE_ATTR_LPRESS      0x0020  // Enable long-press feature
#define GG_GESTURE_ATTR_SLIDE       0x0040  // Sliding without inertia
#define GG_GESTURE_ATTR_INER        0x0100  // Absolutely free sliding
#define GG_GESTURE_ATTR_INER_SBLOCK 0x0200  // Enable free sliding across only one block
#define GG_GESTURE_ATTR_INER_BLOCKS 0x0400  // Enable free sliding across multi blocks
#define GG_GESTURE_ATTR_COMB_MOVE   (GG_GESTURE_ATTR_SLIDE | GG_GESTURE_ATTR_INER | GG_GESTURE_ATTR_INER_SBLOCK | GG_GESTURE_ATTR_INER_BLOCKS)

typedef struct {
    // MW_GESTURE_ATTR_DCLICK, MW_GESTURE_ATTR_LPRESS only for attr_x member using
    int attr_x;
    int attr_y;

    // For the message handle of the recipient, corresponding to attr_x / attr_y members.
    H_WIN hmsg_x[GG_GESTURE_MSG_DEST_MAX];
    H_WIN hmsg_y[GG_GESTURE_MSG_DEST_MAX];

    // Block size
    int steps_x;
    int steps_y;

    // Start point position
    int start_x;
    int start_y;

    // They are only for MW_GESTURE_ATTR_INER_SBLOCK and MW_GESTURE_ATTR_INER_BLOCKS
    int headboundary;   // 0 -- false; 1 -- true
    int tailboundary;

    // For timer
    H_WIN htimer;
    __u16 timerid;
} GG_GESTUREATTR;

extern void * GG_GestureOpen(void);
extern int GG_GestureClose(void * hdl);
extern int GG_GestureOnDown(void * hdl, __gui_msg_t * msg, int x, int y, GG_GESTUREATTR * cls);
extern int GG_GestureOnMove(void * hdl, __gui_msg_t * msg, int x, int y);
extern int GG_GestureOnUp(void * hdl, __gui_msg_t * msg, int x, int y);
extern int GG_GestureOnLpress(void * hdl, __gui_msg_t * msg, int x, int y);
extern void GG_GestureOnTimer(void * hdl, __gui_msg_t * msg);


#ifdef __cplugplug
}
#endif

#endif

