
#ifndef H_ZU_CODE_DEMO_BENNIS_20140105
#define H_ZU_CODE_DEMO_BENNIS_20140105
#include "cp_i.h"

#if 0
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

typedef struct ButtonCode_Handle{
	char * port;
	char * baurate;
	char * datas;
	char * delay;
	char * up_down;
	struct ButtonCode_Handle * next;
} ButtonCode_handle;
ButtonCode_handle * ButtonCodehandleNew(void);

void Buttonhandle_add(ButtonCode_handle*handle,gg_handler codeNode,int num,int num2);
void Buttonhandle_Search_Up(ButtonCode_handle*handle);
void Buttonhandle_Search_Down(ButtonCode_handle*handle);
void Buttonhandle_Search_2(ButtonCode_handle*handle,char * array_in,char * array_out);
#endif

