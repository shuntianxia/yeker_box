
#ifndef H_XML_HUADONGTIAO_DEMO_BENNIS_20131025
#define H_XML_HUADONGTIAO_DEMO_BENNIS_20131025

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

typedef struct Win_Huadongtiao_Handle{
	H_WIN	need_handle;	
    char *  toitemid;
	int    page;
	struct Win_Huadongtiao_Handle * next;
} win_huadongtiao_handle;

extern void XML_haudongtiao_Handle_Date(H_WIN need_handle,char *  toitemid);
extern win_huadongtiao_handle * XML_haudongtiao_Get_Handle(char *  toitemid);
extern win_huadongtiao_handle * XML_haudongtiao_Get_Handle_Next(win_huadongtiao_handle * handle,char *  toitemid);
extern unsigned char * XML_haudongtiao_GetChar_ToX(char * cha,unsigned char *xml_2);
extern void XML_haudongtiao_Get_PageHandle(int page);
#endif

