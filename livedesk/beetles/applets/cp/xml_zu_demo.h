
#ifndef H_XML_ZU_DEMO_BENNIS_20131025
#define H_XML_ZU_DEMO_BENNIS_20131025

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

typedef struct Slaver_Handle{
	char * toitemid;//´Ó
	char * relation;
	char * toitemstatus;
	int    groupid;
	int    imageid;
	struct Slaver_Handle * next;
} slaver_handle;

typedef struct Zu_Handle{
	char * toitemid;//Ö÷
	char * relation;
	int    groupid;
	struct Zu_Handle * next;
} primary_handle;
void XML_Primary_Handle_Date(int groupid,char * relation,char * toitemid,char * toitemstatus);
void XML_Slave_Handle_Date(int groupid,char * relation,char * toitemid,char * toitemstatus,int imageid);
void Find_Primary_Handle(char * itemid);
void Find_Slave_Handle(int groupid,char * father_toitemid);
void CFind_Primary_Toitemid(char * Slave_Toitemid);
//int IfC_SameGroup_Normal(char * click,char * toitemid);
int Imageview_Id_Return(char * primary_toitemid,char * slave_toitemid);

#endif

