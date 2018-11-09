#include <apps.h>
#include "xml_demo.h"
//#include "scrolling_common.h"

static win_handle * xml_win_handle;
__s32 Lamp_Sent(unsigned char * packet, int len)
{
	ES_FILE * uart;

    uart = eLIBs_fopen("b:\\BUS\\UART1", "r+");
    if (!uart) {
        //LogE("Open UART 1 failed");
    }
    eLIBs_fwrite(packet, 1, len, uart);
	eLIBs_fclose(uart);
	//LogMI("send date");
	return EPDK_OK;
}
int char_to_int(char cha)
{
	int cnt;
	if(cha > 90){
		cha = cha - 32;
	}
	cnt = 0;
	switch(cha){
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			cnt = SLIB_atoi(&cha);
			break;
		case 'A':
			cnt = 10;
			break;
		case 'B':
			cnt = 11;
			break;
		case 'C':
			cnt = 12;
			break;
		case 'D':
			cnt = 13;
			break;
		case 'E':
			cnt = 14;
			break;
		case 'F':
			cnt = 15;
			break;
	}
	return cnt;
}
static win_handle * XML_WinHandle_Create(void)
{
	win_handle * handle;
	handle = (win_handle *)esMEMS_Malloc(0, sizeof(win_handle));
	if (!handle) {
		//LogE("SHRBEI_A failed");
		return NULL;
	}
	eLIBs_memset(handle, 0, sizeof(win_handle));
	return handle;
}

static win_handle * XML_HandleDate_Add(win_handle * handle,H_WIN need_handle,int page,int button,int group)
{
	handle->need_handle = need_handle;
	//log_mi("handle->ch_count = %d",handle->ch_count);
	handle->page = page;
	handle->button = button;
	handle->group = group;
	//LogMI("handle->group = %d",handle->group);
	return handle;
}

static win_handle * XML_Handle_Add(win_handle * handle,H_WIN need_handle,int page,int button,int group)
{
	win_handle * SheBei_a;
	win_handle * handle_return;
	if((handle->page != page)||(handle->button != button)){
		SheBei_a = handle->next;
		if(SheBei_a){
			handle_return = XML_Handle_Add(SheBei_a, need_handle, page,button, group);
		}else{
		    SheBei_a = XML_WinHandle_Create();
		    SheBei_a = XML_HandleDate_Add(SheBei_a, need_handle, page,button, group);
		    handle->next = SheBei_a;
		    handle_return = SheBei_a;
		}
	}else if((handle->page == page)&&(handle->button == button)){
	    XML_HandleDate_Add(handle, need_handle, page,button, group);
		handle_return = handle;
	}else{
		return NULL;
	}
	return handle_return;
}
static win_handle * XML_WinHandle_Search(win_handle * handle,int page,int button,int group)
{
	win_handle * SheBei_a;
	win_handle * handle_return;
	if(handle){
		if(handle->group != group){
			SheBei_a = handle->next;
			if(SheBei_a){
				handle_return = XML_WinHandle_Search(SheBei_a,page,button,group);
			}else{
			   return NULL;
			}
		}else if(handle->group == group){
			if((handle->page != page)||(handle->button != button)){
				handle_return = handle;
			}else if((handle->page == page)&&(handle->button == button)){
				SheBei_a = handle->next;
				if(SheBei_a){
					handle_return = XML_WinHandle_Search(SheBei_a,page,button,group);
				}else{
					return NULL;
				}
			}
		}else{
			return NULL;
		}
	}
	return handle_return;
}

static void XML_handle_create(H_WIN need_handle,int page,int button,int group)
{//添加定时器数据
   if(!xml_win_handle){
		xml_win_handle = (win_handle *)esMEMS_Malloc(0, sizeof(win_handle));
		if (!xml_win_handle) {
			//LogE("Malloc failed");
			;
		}
		eLIBs_memset(xml_win_handle, 0, sizeof(win_handle));
	}
	XML_Handle_Add(xml_win_handle, need_handle,page,button, group);
}
win_handle * XML_Get_Handle(int page,int button,int group)
{
	win_handle * SheBei_a;
	SheBei_a = XML_WinHandle_Search(xml_win_handle,page,button,group);
	return SheBei_a;
}

win_handle * XML_Get_Handle_Next(win_handle * handle,int page,int button,int group)
{
	win_handle * SheBei_a;
	SheBei_a = XML_WinHandle_Search(handle,page,button,group);
	return SheBei_a;
}
void XML_Handle_Date(H_WIN need_handle,int page,int button,int group)
{
    XML_handle_create(need_handle, page,button, group);//通讯数据初始化
}
unsigned char * XML_GetChar_ToX(char * cha,unsigned char *xml_2)
{//将字符串转换为16进制，允许abcdef，待发送的16进制字符串
	unsigned char *xml_1;
	int char_len;
	int cnt,cnt_2;
	char_len = eLIBs_strlen(cha);
	xml_1 = (unsigned char *)esMEMS_Malloc(0, char_len);
	for(cnt = 0;cnt < char_len - 2;cnt++){
		xml_1[cnt] = char_to_int(cha[cnt + 2]);
	} 
	for(cnt = 0,cnt_2 = 0;cnt < char_len - 2;cnt_2 ++ ,cnt += 2){
		xml_2[cnt_2] = xml_1[cnt] << 4 | xml_1[cnt + 1];
	}
	esMEMS_Mfree(0, xml_1);
	return xml_2;
}

/* 返回ch字符在sign数组中的序号 */
int getIndexOfSigns(char ch)
{
    if(ch >= '0' && ch <= '9')
    {
        return ch - '0';
    }
    if(ch >= 'A' && ch <='F') 
    {
        return ch - 'A' + 10;
    }
    if(ch >= 'a' && ch <= 'f')
    {
        return ch - 'a' + 10;
    }
    return -1;
}

/* 十六进制数转换为十进制数 */
long hexToDec(char *source)
{
    long sum = 0;
    long t = 1;
    int i, len;
 
    len = strlen(source);
    for(i=len-1; i>=0; i--)
    {
        sum += t * getIndexOfSigns(*(source + i));
        t *= 16;
    }  
 
    return sum;
}

static int return_pingfang_zhi(int a,int b)
{
   int i;
   for(i = 0;i < b - 1; i++){
   	a = 10 * a;
   }
   return a;
}
/*
void send_button_code(char * cha)
{
   int Code_i,Time_j,Clear_k,Change_c,Change_b;
   //Code_i 待发送数据位Time_j 延时数据位Clear_k清灵数据位Change_c格式转换位
   int place;//状态控制位 0 初始1判断延时2延时判断结束
   unsigned char *xml_Send_Date_Char;//待发送数据临时储存cha 变int单位
   unsigned char *xml_Send_Date;//待发送数据临时储存 xml_Send_Date_Char  2个int 2和1
   unsigned char *xml_Delay_Time_Date;//待发送数据延时储存
   int Time_delay_cnt;//延时时间
   int char_len;//数据长度
   int ChuLi_ShuJu_Wei;//字符串的数据位数
   char_len = eLIBs_strlen(cha);
   xml_Send_Date_Char = (unsigned char *)esMEMS_Malloc(0, char_len/2);
   xml_Send_Date = (unsigned char *)esMEMS_Malloc(0, char_len/4);
   xml_Delay_Time_Date = (unsigned char *)esMEMS_Malloc(0, char_len);
   Time_delay_cnt = place = 0;
   for(ChuLi_ShuJu_Wei = 0,Code_i = 0,Time_j = 0;ChuLi_ShuJu_Wei < char_len;ChuLi_ShuJu_Wei++){
   	 if(cha[ChuLi_ShuJu_Wei]=='['){
		 place = 1;
		 for(Clear_k = 0;Clear_k < char_len;Clear_k ++){
			 xml_Delay_Time_Date[Clear_k] = 0;
		 }
		 //LogMI("cha[%d] = %c",ChuLi_ShuJu_Wei,cha[ChuLi_ShuJu_Wei]);
	 }else if(cha[ChuLi_ShuJu_Wei]==']'){
		 place = 2;
		 for(Change_b = 0;Change_b < Time_j;Change_b ++){
			 Time_delay_cnt = return_pingfang_zhi(xml_Delay_Time_Date[Change_b],(Time_j - Change_b)) + Time_delay_cnt;
			 //LogMI("Time_delay_cnt = %d",Time_delay_cnt);
		 }
		 Time_j = 0;
		 if(Time_delay_cnt){
			 for(Change_b = 0,Change_c = 0;Change_c < Code_i/2;Change_c ++ ,Change_b += 2){
				 xml_Send_Date[Change_c] = xml_Send_Date_Char[Change_b] << 4 | xml_Send_Date_Char[Change_b + 1];
				 //LogMI("xml_Send_Date[%d] = %X",Change_c,xml_Send_Date[Change_c]);
			 }
			 Lamp_Sent(xml_Send_Date,Code_i/2);
			 //LogMI("Time_delay_cnt = %d",Time_delay_cnt);
		     esKRNL_TimeDly(Time_delay_cnt);
			 for(Clear_k = 0;Clear_k < Code_i;Clear_k ++){
				xml_Send_Date_Char[Clear_k] = 0;
			 }
		     Code_i = 0;
			 Time_j = 0;
			 Time_delay_cnt = 0;
		 }
		 //LogMI("cha[%d] = %c",ChuLi_ShuJu_Wei,cha[ChuLi_ShuJu_Wei]);
	 }else{
	     if(place == 1){
			 xml_Delay_Time_Date[Time_j] = char_to_int(cha[ChuLi_ShuJu_Wei]);
			 //LogMI("xml_Delay_Time_Date[%d] = %x",Time_j,xml_Delay_Time_Date[Time_j]);
			 Time_j++;
		 }else{
			place = 0;
			xml_Send_Date_Char[Code_i] = char_to_int(cha[ChuLi_ShuJu_Wei]);
			//LogMI("xml_Send_Date_Char[%d] = %x",Code_i,xml_Send_Date_Char[Code_i]);
			Code_i++;
		 }
	 }
   } 
   //LogMI("Code_i %d",Code_i);
   for(ChuLi_ShuJu_Wei = 0,Change_c = 0;Change_c < Code_i/2;Change_c ++ ,ChuLi_ShuJu_Wei += 2){
	   xml_Send_Date[Change_c] = xml_Send_Date_Char[ChuLi_ShuJu_Wei] << 4 | xml_Send_Date_Char[ChuLi_ShuJu_Wei + 1];
	   //LogMI("xml_Send_Date[%d] = %X",Change_c,xml_Send_Date[Change_c]);
   }
   Lamp_Sent(xml_Send_Date,Code_i/2);
   esMEMS_Mfree(0, xml_Send_Date_Char);
   esMEMS_Mfree(0, xml_Send_Date);
   esMEMS_Mfree(0, xml_Delay_Time_Date);
}
*/
__s32 dsk_ea_pin(int port,int num,int data) 
{
	user_gpio_set_t gpio_set;
	 __hdle hvolume_ctrl = NULL;

	// gpio_set.name = "ea ctrl";
	 gpio_set.port = port;
	 gpio_set.port_num = num;
	 gpio_set.mul_sel = 1;
	 gpio_set.drv_level = 1;
	 gpio_set.pull = 1;
	 gpio_set.data = data;

	 hvolume_ctrl = esPINS_PinGrpReq(&gpio_set, 1);
	if (!hvolume_ctrl)
	{
		__wrn("request Power Off pin failed\n");
		return EPDK_FAIL;
	}

	esPINS_WritePinData(hvolume_ctrl, data, NULL);
	esPINS_PinGrpRel(hvolume_ctrl, 2);
	return EPDK_OK;
}


