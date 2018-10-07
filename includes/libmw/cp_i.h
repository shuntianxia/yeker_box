
#ifndef H_CP_I_BENNIS_20130125
#define H_CP_I_BENNIS_20130125

#include <apps.h>
#include <gadget.h>
#include <user_msgs.h>
#include "ext_window.h"

#define HOME_GESTURE_TIMER      5
enum {
    HOME_SUBWIN_SCROLLING = 0,
    HOME_SUBWIN_SETTING
};

#define inline                  __inline
#define CP_SYS_FONT_SIZE        20
#define CP_SYS_FONT_PATH        "d:\\res\\fonts\\fangsong_20.sft"
#define CP_DEFAULT_UI_CONFIG    "d:\\res\\profile\\ui_utf_8.xml"

#ifdef MW_PROJ_DEBUG

#define LogD(...)       (eLIBs_printf("<Debug||%s||L%d> ", __FILE__, __LINE__), \
                                eLIBs_printf(__VA_ARGS__), eLIBs_printf("\n"))
#define LogI(...)       (eLIBs_printf(__VA_ARGS__), eLIBs_printf("\n"))
#define LogMI(...)      (eLIBs_printf("<%s||L%d> ", __FILE__, __LINE__), \
                            eLIBs_printf(__VA_ARGS__), eLIBs_printf("\n"))
#define LogW(...)       (eLIBs_printf("<Warning||%s||L%d> ", __FILE__, __LINE__), \
                                eLIBs_printf(__VA_ARGS__), eLIBs_printf("\n"))
#define LogE(...)       (eLIBs_printf("<Error||%s||L%d> ", __FILE__, __LINE__), \
                            eLIBs_printf(__VA_ARGS__), eLIBs_printf("\n"))
#define LogFatal(...)   (eLIBs_printf("<Fatal||%s||L%d> ", __FILE__, __LINE__), \
                            eLIBs_printf(__VA_ARGS__), eLIBs_printf("\n"), esKSRV_Reset())

#else

#define LogD(...)
#define LogI(...)
#define LogMI(...)
#define LogW(...)
#define LogE(...)
#define LogFatal(...)

#endif

#endif

