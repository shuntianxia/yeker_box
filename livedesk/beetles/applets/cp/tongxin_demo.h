
#ifndef H_TONGXIN_DEMO_BENNIS_20131025
#define H_TONGXIN_DEMO_BENNIS_20131025
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

typedef struct Test_Handle{
	char * key_code1;
	char * key_delay1;
	char * key_bus1;
	char * key_baurate1;
	int bianhao;
	struct Test_Handle * next;
} Test_handle;
extern Test_handle * tongxinhandleNew(void);
extern void XML_send_code(Test_handle * handler);
extern __s32 Button_Id_Bug(char * cha);
extern void send_button_code_2(char * cha,int delay,char * array_in,char * array_out);
extern void send_button_code_2_1(char * cha,int delay,char * array_in,char * array_out);
extern void send_button_code_1(char * cha,int delay,int slider_value);
extern void Send_ButtonCode_Normal(char * port,char * baurate,char * datas,char * delay);
extern void Send_ButtonCode_Matrix(char * port,char * baurate,char * datas,char * delay,char * array_in,char * array_out);
extern void Send_ButtonCode_Slider(char * port,char * baurate,char * datas,char * delay,int slider_value);

/* ======> Lihan 20160127
	485默认为接收状态
  */
extern void Uart_485_init(void);
/* <=============
*/

#endif

