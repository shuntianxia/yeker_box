
#ifndef H_TONGXIN_DEMO_BENNIS_20131025
#define H_TONGXIN_DEMO_BENNIS_20131025
#include "cp_i.h"

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

typedef struct Test_Handle{
	char * key_code1;
	char * key_delay1;
	char * key_bus1;
	char * key_baurate1;
	int bianhao;
	struct Test_Handle * next;
} Test_handle;

typedef struct UART_INF{
	__u8 h_thread;
	__hdle h_gpio;
	__krnl_event_t *uart_sem;
}Uart_inf;

#define UART_BUFF  24

extern Test_handle * tongxinhandleNew(void);
//extern void send_button_code_2_1(char * cha,int delay,char * array_in,char * array_out);
//extern void send_button_code_1(char * cha,int delay,int slider_value);
extern void Send_ButtonCode_Normal(char * port,char * baurate,char * datas,char * delay,H_WIN handle);
extern void Send_ButtonCode_Matrix(char * port,char * baurate,char * datas,char * delay,char * array_in,char * array_out,H_WIN handle);
extern void Send_ButtonCode_Slider(char * port,char * baurate,char * datas,char * delay,int slider_value,H_WIN handle);

int Tong_Xin_ToInt(char cha);
int Chang_Button_State(unsigned char *	get_date,H_WIN	Gbutton_handle);
void Uart_inf_ini(void);


#endif

