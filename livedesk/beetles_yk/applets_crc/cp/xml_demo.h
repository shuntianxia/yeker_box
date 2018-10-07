
#ifndef H_XML_DEMO_BENNIS_20131025
#define H_XML_DEMO_BENNIS_20131025

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
typedef struct Win_Handle{
	H_WIN		need_handle;
	int         page;
	int         button;
	int         group;
	struct Win_Handle * next;
} win_handle;

extern void XML_Handle_Date(H_WIN need_handle,int page,int button,int group);
extern win_handle * XML_Get_Handle(int page,int button,int group);
extern win_handle * XML_Get_Handle_Next(win_handle * handle,int page,int button,int group);
extern unsigned char * XML_GetChar_ToX(char * cha,unsigned char *xml_2);
//void send_button_code(char * cha);
extern __s32 dsk_ea_pin(int port,int num,int data) ;
extern int char_to_int(char cha);


#endif

