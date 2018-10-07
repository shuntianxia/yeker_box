#include <apps.h>
#include "tongxin_demo.h"
#include "scrolling_i.h"
#include "cp_i.h"
#include "CRC_ALG.h"


extern int Button_Status;  
extern unsigned char Work_SheBei_id;
extern unsigned char ChuanKo_id;
extern unsigned char ChuanKo_id_send;
extern int	R_red_write;// 0 是发送数据 只判断对错。1 是读数据取值
extern H_WIN Gbutton_state_change;

#define SONG 
__u32  slider_ret = 0;
static Uart_inf uart_inf;
static unsigned char GPIO_PIN = 0;
static unsigned char GongNeng;
static unsigned char Date_1;			//计算数据1  
static unsigned char Date_2;			//计算数据2   与待比较数据

static __hdle Gpio[4];
static __s32 xml_Lamp_Sent(char * cha,int delay);
static __s32 Jie_Ko_Pin(int port,int num,int data) ;
static __s32 Uart_Send(unsigned char * packet,int len);

static __hdle gpio_reg(int port,int num)
{
	user_gpio_set_t gpio_set;
	__hdle hvolume_ctrl;
	
	// gpio_set.name = "ea ctrl";
	gpio_set.port = port;
	gpio_set.port_num = num;
	gpio_set.mul_sel = 1;
	gpio_set.drv_level = 1;
	gpio_set.pull = 1;
	gpio_set.data = 1;//收

	hvolume_ctrl = esPINS_PinGrpReq(&gpio_set, 1);
	if (!hvolume_ctrl)
	{
		__wrn("request Power Off pin failed\n");
		return NULL;
	}
	return hvolume_ctrl;
}

static __u32 Ini_gpio(void)
{
	Gpio[0]= gpio_reg(3,9);
	if (!Gpio[0])
	{
		__wrn("request Power Off pin failed\n");
		goto error;
	}
	Gpio[1]= gpio_reg(3,10);
	if (!Gpio[1])
	{
		__wrn("request Power Off pin failed\n");
		goto error;
	}
	Gpio[2]= gpio_reg(3,12);
	if (!Gpio[2])
	{
		__wrn("request Power Off pin failed\n");
		goto error;
	}
	Gpio[3]= gpio_reg(3,14);
	if (!Gpio[3])
	{
		__wrn("request Power Off pin failed\n");
		goto error;
	}
	__msg("gpio ok\n");
	return EPDK_TRUE;
error:
	if(Gpio[0]){
		esPINS_PinGrpRel(Gpio[0],0);
	}
	if(Gpio[1]){
		esPINS_PinGrpRel(Gpio[1],0);
	}
	if(Gpio[2]){
		esPINS_PinGrpRel(Gpio[2],0);
	}
	if(Gpio[3]){
		esPINS_PinGrpRel(Gpio[3],0);
	}
	return EPDK_FALSE;
}

static void Pin_write(int port,int state)
{
	__s32 value;
	
	value = esPINS_WritePinData(Gpio[port], state, NULL);
	if(value==EPDK_OK){
		__msg("set ok\n");
	}else{
		__msg("set fail\n");
	}
}

static unsigned char get_botelv_nb(int botelv)
{
    unsigned char cnt;
	
	switch(botelv){
		case 300:
			cnt = 0x01;
			break;
		case 1200:
			cnt = 0x02;
			break;
		case 2400:
			cnt = 0x03;
			break;
		case 4800:
			cnt = 0x04;
			break;
		case 9600:
			cnt = 0x05;
			break;
		case 19200:
			cnt = 0x06;
			break;
		case 38400:
			cnt = 0x07;
			break;
		case 57600:
			cnt = 0x08;
			break;
		case 115200:
			cnt = 0x09;
			break;
		case 230400:
			cnt = 0x0A;
			break;
		case 921600:
			cnt = 0x0B;
			break;
			break;
	}
	return cnt;
}

int Tong_Xin_ToInt(char cha)
{
	int cnt;

	if(cha>='0'&&cha<='9'){
		cnt = cha-'0';
		//return cnt;
	}else if(cha>='a'&&cha<='f'){
		cnt = cha - 'a'+10;
		//return cnt;		
	}else if(cha>='A'&&cha<='F'){
		cnt = cha -'A'+10;
		//return cnt;
	}else{
		cnt = -1;
		__msg("input char error\n");
	}
	return cnt;
}

static char Tong_Xin_ToChar(int cha)
{
	char cnt;

	if(cha>=0&&cha<=9){
		cnt = cha+'0';	
	}else if(cha>=10&&cha<=15){
		cnt = cha-10+'A';
	}else{
		cnt = -1;
		__msg("input char error\n");
	}
	return cnt;
}
static __hdle gpio_485_reg(void)
{
	__hdle hvolume_ctrl = NULL;
	user_gpio_set_t gpio_set;
	
	 // gpio_set.name = "ea ctrl";
	gpio_set.port = 1;
	gpio_set.port_num = 2;
	gpio_set.mul_sel = 1;
	gpio_set.drv_level = 1;
	gpio_set.pull = 1;
	gpio_set.data = 0;
	hvolume_ctrl = esPINS_PinGrpReq(&gpio_set, 1);
	if(hvolume_ctrl==0){
		__msg("gpio reg error\n");
		return  0;
	}
	return hvolume_ctrl;
}

static void gpio_485_del(void)
{
	esPINS_PinGrpRel(uart_inf.h_gpio, 0);
}

static __s32 gpio_485_write(int data) 
{
	esPINS_WritePinData(uart_inf.h_gpio, data, NULL);
}

static void receive_wrong(__s32 width, __s32 height,__u32 id)
{
	H_LYR hLyr;
	int rotation;
	FB frameBuf;
	HTHEME off;
	void * offData;
	__layerwincreate_para_t layerWinCreate;
	__disp_layer_para_t lyrPara;
	off = dsk_Systheme_open(id);//ID_MAIN_UPDATE_BMP
	offData = dsk_Systheme_hdl2buf(off);
	rotation = GUI_GetScnDir();
	MwLayerParaInit(&layerWinCreate, &frameBuf, &lyrPara, "Layer", rotation, 1, width, height);
	MwFillSize(&frameBuf.size, width , height);
	MwFillRect(&lyrPara.src_win, 0, 0, width, height);
	hLyr = GUI_LyrWinCreate(&layerWinCreate);
	if (NULL == hLyr) {
		__msg("Create layer (%s) failed", layerWinCreate.name);
		dsk_theme_close(off);
	}else{
		GUI_LyrWinSel(hLyr); 
		GUI_BMP_Draw(offData, 0, 0);
		GUI_LyrWinSetSta(hLyr,GUI_LYRWIN_STA_ON);
		dsk_theme_close(off);
		esKRNL_TimeDly(50);
		GUI_LyrWinDelete(hLyr);
	}
}

static __s32 Uart_Send(unsigned char * packet,int len)
{
	ES_FILE *f_uart;
	__u8 err;
	__u32 i = 0;
	
	f_uart = eLIBs_fopen("b:\\BUS\\UART2", "r+");
	if(f_uart == (ES_FILE *)0){
		__msg("UART OPEN ERROR\n");
		return EPDK_FAIL;
	}
    esKRNL_SemPend(uart_inf.uart_sem, 0, &err);
	gpio_485_write(1);
	eLIBs_fwrite(packet,1,len,f_uart);
	esKRNL_TimeDly(3);		//must be delay for uart FIFO
	gpio_485_write(0);
	esKRNL_SemPost(uart_inf.uart_sem);
	eLIBs_fclose(f_uart);
	return EPDK_OK;
}

static __s32 xml_Lamp_SentBTL(unsigned char cha)//波特率发送
{
	unsigned char * payload;

	__msg("set bote\n");
	payload = (unsigned char *)esMEMS_Malloc(0, 9);
	payload[0] = BST_HEAD1;
	payload[1] = BST_HEAD2;
	payload[2] = BST_HEAD3;
	payload[3] = Work_SheBei_id;	 //发送设备
	payload[4] = ChuanKo_id_send;    //接收设备
	payload[5] = 0x03; 				 //长度
	payload[6] = 0x46;;      	     //功能码
	payload[7] = cha;                //波特率 代码
	payload[8] = crc8_verify(&cha,1);
	Uart_Send(payload,9);
	esMEMS_Mfree(0, payload);
	esKRNL_TimeDly(10);
	return 1;
}

static __s32 xml_Lamp_Sent(char * cha,int delay)
{
	unsigned char * payload;
	int len;
	int crc;
	int i,j,cha_len;
	
	if(R_red_write == 0){
		cha_len = eLIBs_strlen(cha);
		cha_len = (cha_len + 1) / 3;
		len = cha_len + 7;
		payload = esMEMS_Malloc(0, len);
		payload[0] = BST_HEAD1;
		payload[1] = BST_HEAD2;
		payload[2] = BST_HEAD3;
		payload[3] = Work_SheBei_id;	//发送设备
		payload[4] = ChuanKo_id_send;				//接收设备
		payload[5] = cha_len + 1;					//长度
		for(i = 6,j = 0;i < len - 1;i++,j++){
		  payload[i]= (Tong_Xin_ToInt(cha[j])<< 4) | Tong_Xin_ToInt(cha[j + 1]);
		  j = j + 2;	  
		}
		payload[6] = payload[6]<<1;		//功能码
		crc = crc8_verify(&payload[7],(cha_len - 1));
		payload[len - 1] = crc;				//CRC校验
		Uart_Send(payload,len);
		//__msg("%d\n",ChuanKo_id_send);
		esMEMS_Mfree(0, payload);
	} else {
		len = 9;
		payload = (__u8 *)esMEMS_Malloc(0, len);
		//payload_crc = esMEMS_Malloc(0, (cha_len - 1));
		payload[0] = BST_HEAD1;
		payload[1] = BST_HEAD2;
		payload[2] = BST_HEAD3;
		payload[3] = Work_SheBei_id + 1;	//发送设备
		payload[4] = ChuanKo_id_send;				//接收设备
		payload[6]= (Tong_Xin_ToInt(cha[0])<< 4) | Tong_Xin_ToInt(cha[1]);
		//payload[6] = GongNeng;
		if(payload[6] == 0x22){
			len = 7;
			payload[5] = 1;							//长度
			payload[6] = (payload[6]<<1) + 1;//功能码
		}else{
			payload[5] = 3;							//长度
			payload[6] = (payload[6]<<1) + 1;//功能码
			payload[7] = (Tong_Xin_ToInt(cha[3])<< 4) | Tong_Xin_ToInt(cha[4]);
			crc = crc8_verify(&payload[7],1);
			payload[8] = crc;//CRC校验
			Date_2 = (Tong_Xin_ToInt(cha[6])<< 4) | Tong_Xin_ToInt(cha[7]);
		}
		Uart_Send(payload,len);
		esMEMS_Mfree(0, payload);
	}
	if(delay<10){
		delay = 10;
	}
	esKRNL_TimeDly(delay/10);
}
static __s32 xml_Lamp_Sent_GPIO(int delay)
{
	unsigned char * payload;
	int len;

	if(R_red_write == 0){
		len = 10;
		payload = (unsigned char *)esMEMS_Malloc(0, len);
		payload[0] = BST_HEAD1;
		payload[1] = BST_HEAD2;
		payload[2] = BST_HEAD3;
		payload[3] = Work_SheBei_id;//发送设备
		payload[4] = ChuanKo_id_send;//接收设备
		payload[5] = 4;//长度
		payload[6] = GongNeng;//功能码
	    payload[7] = Date_1;
		payload[8] = Date_2;
		payload[9] = crc8_verify(&payload[7],2);
	} else if(R_red_write == 1){
		len = 9;
		payload = (unsigned char *)esMEMS_Malloc(0, len);
		payload[0] = BST_HEAD1;
		payload[1] = BST_HEAD2;
		payload[2] = BST_HEAD3;
		payload[3] = Work_SheBei_id + 1;//发送设备
		payload[4] = ChuanKo_id_send;//接收设备
		payload[6]= GongNeng;
		if(payload[6] == 0x44){
			len = 8;
			payload[5] = 1;//长度
			GongNeng = payload[6] = GongNeng + 1;//功能码
		}else{
			payload[5] = 3;//长度
			GongNeng = payload[6] = GongNeng + 1;//功能码
			payload[7] = Date_1;
			payload[8] =  crc8_verify(&payload[7],1);//CRC校验
		}
	}	
	Uart_Send(payload,len);
	esMEMS_Mfree(0,payload);
	if(delay<10){
		delay = 10;
	}
	esKRNL_TimeDly(delay/10);
	return 1;
}

static void send_button_code_2_1(char * cha,int delay,char * array_in,char * array_out)
{
   int char_len,in_len,out_len;		//数据长度
   int i,k,code_number;
   int place;						//状态控制位 0 初始1判断延时2延时判断结束
   char * xml_Send_Date_Char;		//待发送数据临时储存cha 变int单位
   int ChuLi_ShuJu_Wei;				//字符串的数据位数
   
   char_len = eLIBs_strlen(cha); 
   in_len = eLIBs_strlen(array_in);  
   out_len = eLIBs_strlen(array_out);
   i = place = k =0;
   code_number = char_len + in_len + out_len;
   if(in_len == 0){
   	 code_number = code_number - 4;
	 //i++;
   }else if(in_len > 0){
	   code_number = code_number - 4;
	   code_number++;
   }
   if(out_len == 0){
	  code_number = code_number - 5;
	 //i++;
   }else if(out_len > 0){
	   code_number = code_number - 5;
	   code_number++;
   }
   xml_Send_Date_Char = (char *)esMEMS_Malloc(0,code_number);// 9 = [in] + [out]
   for(ChuLi_ShuJu_Wei = 0,k = 0;k < code_number;ChuLi_ShuJu_Wei++){
      if(array_in){
		  if(cha[ChuLi_ShuJu_Wei]=='[' && cha[ChuLi_ShuJu_Wei + 1]=='i'){
			 place = 0;
			 for(i = 0;i < in_len;i++){
				 xml_Send_Date_Char[k] = array_in[i];
				 k++;
			 }
		  }
	  }
	  if(array_out){
		  if(cha[ChuLi_ShuJu_Wei]=='[' && cha[ChuLi_ShuJu_Wei + 1]=='o'){
			 place = 0;
			 if(xml_Send_Date_Char[k - 1] != ','){
				xml_Send_Date_Char[k] = ',';
				k++;
			 }
			 for(i = 0;i < out_len;i++){
				 xml_Send_Date_Char[k] = array_out[i];
				 k++;
			 }
		  }
	  }
	  
	  if(place == 0){
		 xml_Send_Date_Char[k] = cha[ChuLi_ShuJu_Wei];
		 if(cha[ChuLi_ShuJu_Wei] == '0' || cha[ChuLi_ShuJu_Wei] == '1' || cha[ChuLi_ShuJu_Wei] == '2' || cha[ChuLi_ShuJu_Wei] == '3'
		 	 || cha[ChuLi_ShuJu_Wei] == '4' || cha[ChuLi_ShuJu_Wei] == '5' || cha[ChuLi_ShuJu_Wei] == '6' || cha[ChuLi_ShuJu_Wei] == '7'
		 	 || cha[ChuLi_ShuJu_Wei] == '8' || cha[ChuLi_ShuJu_Wei] == '9'){
			 k++;
		 }else if(cha[ChuLi_ShuJu_Wei] == 'a' || cha[ChuLi_ShuJu_Wei] == 'b' || cha[ChuLi_ShuJu_Wei] == 'c'
		  || cha[ChuLi_ShuJu_Wei] == 'd' || cha[ChuLi_ShuJu_Wei] == 'e' || cha[ChuLi_ShuJu_Wei] == 'f'){
			 k++;
		 }else if(cha[ChuLi_ShuJu_Wei] == 'A' || cha[ChuLi_ShuJu_Wei] == 'B' || cha[ChuLi_ShuJu_Wei] == 'C'
		  || cha[ChuLi_ShuJu_Wei] == 'D' || cha[ChuLi_ShuJu_Wei] == 'E' || cha[ChuLi_ShuJu_Wei] == 'F'){
			 k++;
		 }else if(cha[ChuLi_ShuJu_Wei] == ','){
			 if(xml_Send_Date_Char[k - 1] != ','){
				 k++;
			 }
		 }else if(cha[ChuLi_ShuJu_Wei] == NULL){
			 //LogMI("xml_Send_Date_Char[%d] = %c",k,xml_Send_Date_Char[k]);
		     break;
		 }
	  }
   } 
   //LogMI("xml_Send_Date_Char = %s",xml_Send_Date_Char);
   xml_Lamp_Sent(xml_Send_Date_Char,delay);
   esMEMS_Mfree(0, xml_Send_Date_Char);
}

static void send_button_code_1(char * cha,int delay,int slider_value)
{
   int char_len,in_len;					//数据长度
   int i,k;
   int place;							//状态控制位 0 初始1判断延时2延时判断结束
   char * xml_Send_Date_Char;			//待发送数据临时储存cha 变int单位
   unsigned char * xml_Send_Date_value;
   int ChuLi_ShuJu_Wei;					//字符串的数据位数
  
   char_len = eLIBs_strlen(cha) + 1; 
   i = place = k =0;
   if(slider_value > 255){
	   xml_Send_Date_value = (unsigned char *)esMEMS_Malloc(0,5);
	   xml_Send_Date_value[0] = Tong_Xin_ToChar(slider_value/256/16);
	   xml_Send_Date_value[1] = Tong_Xin_ToChar(slider_value/256%16);
	   xml_Send_Date_value[2] = ',';
	   xml_Send_Date_value[3] = Tong_Xin_ToChar(slider_value%256/16);
	   xml_Send_Date_value[4] = Tong_Xin_ToChar(slider_value%256%16);
	   in_len = 5;
   }else{
	   xml_Send_Date_value = (unsigned char *)esMEMS_Malloc(0,2);
	   xml_Send_Date_value[0] = Tong_Xin_ToChar(slider_value%256/16);
	   xml_Send_Date_value[1] = Tong_Xin_ToChar(slider_value%256%16);
	   in_len = 2;
   }
   xml_Send_Date_Char = (char *)esMEMS_Malloc(0,char_len + in_len);
   for(ChuLi_ShuJu_Wei = 0,k = 0;ChuLi_ShuJu_Wei < char_len ;ChuLi_ShuJu_Wei++){
	  if(cha[ChuLi_ShuJu_Wei]=='[' && cha[ChuLi_ShuJu_Wei + 1]=='v'){
		 place = 1;
		 for(i = 0;i < in_len;i++){
			 xml_Send_Date_Char[k] = xml_Send_Date_value[i];
			 //LogMI("xml_Send_Date_Char[%d] = %c",k,xml_Send_Date_Char[k]);
			 k++;
		 }
	  }else if(cha[ChuLi_ShuJu_Wei]==']'){
		 place = 0;
		 ChuLi_ShuJu_Wei++;
	  }
	  if(place == 0){
		 xml_Send_Date_Char[k] = cha[ChuLi_ShuJu_Wei];
		 k++;
	  }
   } 
   //LogMI("xml_Send_Date_Char = %s",xml_Send_Date_Char);
   xml_Lamp_Sent(xml_Send_Date_Char,delay);
   esMEMS_Mfree(0, xml_Send_Date_Char);
   esMEMS_Mfree(0, xml_Send_Date_value);
}

void Send_ButtonCode_Normal(char * port,char * baurate,char * datas,char * delay,H_WIN handle)
{					
	int pin_state;
	
	GPIO_PIN = 0;
	if(datas){
		if(SLIB_strcmp(port,"GPIO_1") == 0){
			pin_state = SLIB_atoi(baurate);
			if(pin_state == 1){
				Date_2 = 1;//bit0 = 1
			}else{
				Date_2 = 0;//bit0 = 0
			}
			Pin_write(1,Date_2);
			return;
		}else if(SLIB_strcmp(port,"GPIO_2") == 0){
			pin_state = SLIB_atoi(baurate);
			if(pin_state == 1){
				Date_2 = 1;//bit0 = 1
			}else{
				Date_2 = 0;//bit0 = 0
			}
			Pin_write(2,Date_2);
			return;
		}else if(SLIB_strcmp(port,"GPIO_3") == 0){
			pin_state = SLIB_atoi(baurate);
			if(pin_state == 1){
				Date_2 = 1;//bit0 = 1
			}else{
				Date_2 = 0;//bit0 = 0
			}
			Pin_write(3,Date_2);
			return;
		}else if(SLIB_strcmp(port,"GPIO_4") == 0){
			pin_state = SLIB_atoi(baurate);
			if(pin_state == 1){
				Date_2 = 1;//bit0 = 1
			}else{
				Date_2 = 0;//bit0 = 0
			}
			Pin_write(4,Date_2);
			return;
		}else if(SLIB_strcmp(port,"GPIO_7") == 0){
			ChuanKo_id_send = ChuanKo_id << 4;//com0
			GPIO_PIN = 1;
			{
				pin_state = SLIB_atoi(baurate);
				GongNeng = 0x54;
				Date_1 = 0x07;
				if(pin_state == 1){
					Date_2 = 0x01;//bit0 = 1
				}else{
					Date_2 = 0x0;//bit0 = 0
				}
			}
		}else if(SLIB_strcmp(port,"GPIO_8") == 0){
			ChuanKo_id_send = ChuanKo_id << 4;//com0
			GPIO_PIN = 1;
			{
				pin_state = SLIB_atoi(baurate);
				GongNeng = 0x54;
				Date_1 = 0x08;
				if(pin_state == 1){
					Date_2 = 0x01;//bit0 = 1
				}else{
					Date_2 = 0x0;//bit0 = 0
				}
			}
		}else{
			//bote = SLIB_atoi(baurate);
			//bote = get_botelv_nb(bote);
			if(SLIB_strcmp(port,"COM_0") == 0){
				ChuanKo_id_send = ChuanKo_id << 4;
			}else if(SLIB_strcmp(port,"COM_1,0") == 0){
				ChuanKo_id_send = (ChuanKo_id << 4) + 1;
				if(R_red_write==1){
					return;
				}
				/*if(bote_com_1!=bote){
					xml_Lamp_SentBTL(bote);
				}*/
			}else if(SLIB_strcmp(port,"COM_2,0") == 0){
				ChuanKo_id_send = (ChuanKo_id << 4) + 2;
				if(R_red_write==1){
					return;
				}
				/*if(bote_com_2!=bote){
					xml_Lamp_SentBTL(bote);
				}*/
			}else if(SLIB_strcmp(port,"COM_3,0") == 0){
				ChuanKo_id_send = (ChuanKo_id << 4) + 3;
				if(R_red_write==1){
					return;
				}
				/*if(bote_com_3!=bote){
					xml_Lamp_SentBTL(bote);
				}*/
			}else if(SLIB_strcmp(port,"COM_4,0") == 0){
				ChuanKo_id_send = (ChuanKo_id << 4) + 4;
				if(R_red_write==1){
					return;
				}
				/*if(bote_com_4!=bote){
					xml_Lamp_SentBTL(bote);
				}*/
			}
		}
		if(GPIO_PIN==1){
			xml_Lamp_Sent_GPIO(SLIB_atoi(delay));//延时与数据
		}else {
			xml_Lamp_Sent(datas,SLIB_atoi(delay));
		}
	}
}

void Send_ButtonCode_Matrix(char * port,char * baurate,char * datas,char * delay,char * array_in,char * array_out,H_WIN handle)
{
	//端口， 波特，率 数据， 延时  com
	
	if(datas){
	
		if(SLIB_strcmp(port,"COM_0") == 0){
			ChuanKo_id_send = ChuanKo_id << 4;
		}else if(SLIB_strcmp(port,"COM_1,1") == 0){
			ChuanKo_id_send = (ChuanKo_id << 4) + 1;	
			
			R_red_write =1;//读串口
		}else if(SLIB_strcmp(port,"COM_1,0") == 0){
			ChuanKo_id_send = (ChuanKo_id << 4) + 1;
			
			R_red_write =0;//不读串口
		}else if(SLIB_strcmp(port,"COM_2,0") == 0){
			ChuanKo_id_send = (ChuanKo_id << 4) + 2;
			
			R_red_write =0;//不读串口
		}else if(SLIB_strcmp(port,"COM_3,0") == 0){
			ChuanKo_id_send = (ChuanKo_id << 4) + 3;
		
			R_red_write =0;//不读串口
		}else if(SLIB_strcmp(port,"COM_4,0") == 0){
			ChuanKo_id_send = (ChuanKo_id << 4) + 4;
			R_red_write =0;//不读串口
		}
		if(array_in){
			send_button_code_2_1(datas, SLIB_atoi(delay), array_in, array_out);//有输入发码
		}else{
			send_button_code_2_1(datas, SLIB_atoi(delay), array_in, array_out);//无输入发码
		}
		
	}
}

void Send_ButtonCode_Slider(char * port,char * baurate,char * datas,char * delay,int slider_value,H_WIN handle)
{
	//端口， 波特，率 数据， 延时  com
	
	if(datas){

		if(SLIB_strcmp(port,"COM_0") == 0){
			ChuanKo_id_send = ChuanKo_id << 4;
		}else if(SLIB_strcmp(port,"COM_1,0") == 0){
			ChuanKo_id_send = (ChuanKo_id << 4) + 1;
			
		}else if(SLIB_strcmp(port,"COM_2,0") == 0){
			ChuanKo_id_send = (ChuanKo_id << 4) + 2;
			
		}else if(SLIB_strcmp(port,"COM_3,0") == 0){
			ChuanKo_id_send = (ChuanKo_id << 4) + 3;
			
		}else if(SLIB_strcmp(port,"COM_4,0") == 0){
			ChuanKo_id_send = (ChuanKo_id << 4) + 4;
		}
		send_button_code_1(datas,SLIB_atoi(delay),slider_value);//延时与数据
	}
}
//判断是否与相反状态一致

static int Chang_Button_State(unsigned char *  get_data,H_WIN  Gbutton_handle)
{
	__gui_msg_t  msgsend;
	
	if(GPIO_PIN == 0){
	    if(get_data[1] == 0x55){				//io 口控制高低电平与视频  只判断一个io
	    	//__msg("%d,%d\n",Date_2,get_data[3]);
			if(Date_2 == get_data[3]){
				msgsend.dwAddData2 = Button_Status;//same
				msgsend.id = GUI_MSG_USER_GESTURE_CHANGE_ZHUANGTAI;
				msgsend.h_deswin = Gbutton_handle;
				GUI_PostMessage(&msgsend);
			}else{
				__msg("recive %d\n",Date_2);
				return EPDK_FAIL;
			}
	    }else if(get_data[1] == 0x75){
		    if(get_data[2] == 0x03){//判断通道
				if(Date_2 == get_data[3]){
					if(Button_Status==1){
						msgsend.dwAddData2 = 0;
						msgsend.id = GUI_MSG_USER_GESTURE_CHANGE_ZHUANGTAI;
						msgsend.h_deswin = Gbutton_handle;
						GUI_PostMessage(&msgsend);
					}
				}else{
					if(Button_Status==0){
						msgsend.dwAddData2 = 1;
						msgsend.id = GUI_MSG_USER_GESTURE_CHANGE_ZHUANGTAI;
						msgsend.h_deswin = Gbutton_handle;
						GUI_PostMessage(&msgsend);
					}
				}
			}else if(get_data[2] == 0x04){// 音频音量
				msgsend.dwAddData2 = get_data[4];
				msgsend.id = GUI_MSG_USER_GESTURE_CHANGE_ZHUANGTAI;
				msgsend.h_deswin = Gbutton_handle;
				GUI_PostMessage(&msgsend);
			}
	    }
	}else{
		if(Date_2 == get_data[3]){
			if(Button_Status==1){
				msgsend.dwAddData2 = 0;
				msgsend.id = GUI_MSG_USER_GESTURE_CHANGE_ZHUANGTAI;
				msgsend.h_deswin = Gbutton_handle;
				GUI_PostMessage(&msgsend);
			}
		}else{
			if(Button_Status==0){
				msgsend.dwAddData2 = 1;
				msgsend.id = GUI_MSG_USER_GESTURE_CHANGE_ZHUANGTAI;
				msgsend.h_deswin = Gbutton_handle;
				GUI_PostMessage(&msgsend);
			}
		}
	}
	return EPDK_OK;
}

static void  recive_data(__u8 *buff,ES_FILE * file)
{
	__u32 i = 0;
	__u8 *packet2 = buff;
	__u32 cnt = 0;
	__u8 crc_check;
	
	while(1){
		if(0 != eLIBs_fread(&packet2[i],1, 1, file)){
			 if(i == 1){
				 if(packet2[1] != 0x53){
					if(packet2[0] != 0x42){
						packet2[0] = 0x00;
					}else{
						packet2[0] = packet2[1];
						i = 0;
					}
				 }
				 #ifdef SONG
				 	__msg("1=%02x\n",packet2[1]);
				 #endif
			 }
			 if(i == 2){
				 if(packet2[2] != 0x54){
					packet2[0] = 0x00;
				 }
				 #ifdef SONG
				 	__msg("2=%02x\n",packet2[2]);
				 #endif
			 }
			 if(i == 5){
				if(R_red_write == 0){
					if(packet2[5] != 0x03){
					   packet2[0] = 0x00;
					}
				}
				#ifdef SONG
				 	__msg("%02x\n",packet2[5]);
				#endif
			 }
			 if(i > 5){
				 if(i == (5 + packet2[5])){
					 crc_check = crc8_verify(&packet2[7],(packet2[5] - 2));
					 __msg("%d,%d\n",crc_check,packet2[i]);
					 if(packet2[i] != crc_check){
					 		__msg("%d,%d\n",crc_check,packet2[i]);
							receive_wrong(720,720,ID_CLOCK_RECEIVE_WRONG_BMP);
					 }else{
					 	if(packet2[3]!=0){
							__u8 id;
							id = packet2[3]&(~0x03);
							if(id!=Work_SheBei_id){
								__msg("id not this\n");
								continue;	
							}
						}
						if(packet2[3] % 2 == 1){
							__msg("receive right\n");
							Chang_Button_State(&packet2[5],Gbutton_state_change);
						}else{
							 if(packet2[7] == 0x00){
								__msg("receive right\n");
								//slider_sem_post();
								slider_ret = 1;
								//fuction_ctl(packet2);
							 }else if(packet2[7] == 0xFF){
								receive_wrong(720,720,ID_CLOCK_RECEIVE_WRONG_BMP);
								__msg("receive wrong\n"); 		 
							 }else{
								__msg("up power\n");
							 }
						 }
					 }
					 break;
				 }
			 }
			 if(packet2[0] == 0x42){
				 i++ ;	
			 }else{
			 	__msg("0=%02x\n",packet2[0]);
				 i = 0;
			 }
		 }else{
			esKRNL_TimeDly(4);//wait data for a moment
			cnt++;
			if(cnt==2){
				break;
			}
		 }
	}
}

static void RECEIVE_DATE_103(void)
{
   ES_FILE * uart;
   unsigned char * packet;
   __u8 err;
   
   uart = eLIBs_fopen("b:\\BUS\\UART2", "r+");
   if (!uart) {
	   LogE("Open UART 2 failed");
	   return;
   }   
   packet = (unsigned char *)esMEMS_Malloc(0, UART_BUFF);
   if(packet==NULL){
		__msg("maollc fail\n");
		goto end;
   }
   while(1){ 
	 if (esKRNL_TDelReq(OS_PRIO_SELF) == OS_TASK_DEL_REQ) {
			goto end;
	 }
	 esKRNL_SemPend(uart_inf.uart_sem, 0, &err);
	 recive_data(packet,uart);
	 esKRNL_SemPost(uart_inf.uart_sem);
	 esKRNL_TimeDly(3);
   } 
end:
   eLIBs_fclose(uart);
   esMEMS_Mfree(0, packet);
   esKRNL_TDel(OS_PRIO_SELF);
}

void Uart_inf_ini(void)
{
	__u8 err;

	uart_inf.h_gpio = gpio_485_reg();
	if(uart_inf.h_gpio==0){
		__msg("485 gpio error\n");
	}
	uart_inf.h_thread = esKRNL_TCreate(RECEIVE_DATE_103, NULL, 0x1000, KRNL_priolevel4);
	if(uart_inf.h_thread==0){
		__msg("thread create error\n");
		goto END;
	}
	uart_inf.uart_sem = esKRNL_SemCreate(1);
	if(uart_inf.uart_sem==(__krnl_event_t *)0){
		__msg("sem create error\n");
		goto END;
	}
	return;
END:
	if(uart_inf.h_thread){
		__msg("uart recive thread del\n");
		esKRNL_TDel(uart_inf.h_thread);
	}
	if(uart_inf.uart_sem){
		__msg("uart sem del\n");
		esKRNL_SemDel(uart_inf.uart_sem, OS_DEL_ALWAYS, &err);
	}
}
