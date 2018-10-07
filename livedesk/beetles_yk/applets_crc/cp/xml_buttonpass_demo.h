
#ifndef H_XML_BUTTONPASS_DEMO_BENNIS_20140302
#define H_XML_BUTTONPASS_DEMO_BENNIS_20140302

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

typedef struct SaveButtonState{
	char * xml_id;
	int    buttonStatus;
	struct SaveButtonState * next;
} savebuttonstate;

void XML_SaveButtonState_Handle(char * xml_id,int buttonStatus);
void XML_ChangeButtonState_Handle(char * xml_id,int buttonStatus);
int XML_FindButtonState(char * xml_id);
#endif

