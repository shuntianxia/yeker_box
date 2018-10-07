
#ifndef H_XML_MATRIX_CAOZUO_BENNIS_20140603
#define H_XML_MATRIX_CAOZUO_BENNIS_20140603

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

typedef struct Matrix_CaoZuo_Handle{
	int	groupid;	
    int caozuo_static;
	struct Matrix_CaoZuo_Handle * next;
} matrix_caozuo_handle;

void Save_Matrix_Static(int	groupid,int caozuo_static);
int Find_Matrix_State(int groupid);
#endif

