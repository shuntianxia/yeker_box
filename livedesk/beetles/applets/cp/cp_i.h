
#ifndef H_CP_I_BENNIS_20130125
#define H_CP_I_BENNIS_20130125

#include <apps.h>
#include <gadget.h>
#include <user_msgs.h>
#include "ext_window.h"
#include "user_theme.h"

#define HOME_GESTURE_TIMER      5
#define SUSPEND_TIMER           6

#define SYS_UPDATA_TIMER		7

enum {
    HOME_SUBWIN_SCROLLING = 0,
	HOME_SUBWIN_SETTING_TIME ,
    HOME_SUBWIN_UPDATE
};

#define inline                  __inline
#define CP_SYS_FONT_SIZE        20
#define CP_SYS_FONT_PATH        "d:\\res\\fonts\\font16.sft"
#define CP_DEFAULT_UI_CONFIG    "d:\\res\\ui_utf_8.xml"

/* For Test........Lhan */
//#define MW_PROJ_DEBUG

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

