#include <apps.h>
#include "xml_haudongtiao_demo.h"//按键句柄与id
#include "scrolling_i.h"

static win_huadongtiao_handle * xml_huadongtiao_win_handle;
static int button_page_j = 1;
static int button_page_x = 1;
static int huadongtiao_char_to_int(char cha)
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
		case 'a':
			cnt = 10;
			break;
		case 'B':
		case 'b':
			cnt = 11;
			break;
		case 'C':
		case 'c':
			cnt = 12;
			break;
		case 'D':
		case 'd':
			cnt = 13;
			break;
		case 'E':
		case 'e':
			cnt = 14;
			break;
		case 'F':
		case 'f':
			cnt = 15;
			break;
	}
	return cnt;
}
static win_huadongtiao_handle * XML_haudongtiao_WinHandle_Create(void)
{
	win_huadongtiao_handle * handle;
	handle = (win_huadongtiao_handle *)esMEMS_Malloc(0, sizeof(win_huadongtiao_handle)+1);
	if (!handle) {
		LogE("SHRBEI_A failed");
		return NULL;
	}
	eLIBs_memset(handle, 0, sizeof(win_huadongtiao_handle)+1);
	return handle;
}

static win_huadongtiao_handle * XML_haudongtiao_HandleDate_Add(win_huadongtiao_handle * handle,H_WIN need_handle,char *  toitemid)
{
	unsigned char *xml_1;
	int char_len;
	int cnt;
	int page;
	char_len = eLIBs_strlen(toitemid);
	xml_1 = (unsigned char *)esMEMS_Malloc(0, char_len);
	for(cnt = 0;cnt < char_len - 2;cnt++){
		xml_1[cnt] = char_to_int(toitemid[cnt + 2]);
	} 
	page = xml_1[0] << 4 | xml_1[0 + 1];
	handle->need_handle = need_handle;
	handle->toitemid = toitemid;
	if(button_page_x != page){
		button_page_j = button_page_j + 1;
		button_page_x = page;
	}
	handle->page = button_page_j;
	//LogMI("handle->page = %d",handle->page);
	esMEMS_Mfree(0, xml_1);
	return handle;
}

static win_huadongtiao_handle * XML_haudongtiao_Handle_Add(win_huadongtiao_handle * handle,H_WIN need_handle,char *  toitemid)
{
	win_huadongtiao_handle * SheBei_a;
	win_huadongtiao_handle * handle_return;
	char * chat_test;
	char * chat_test2;
	chat_test = toitemid;
	chat_test2 = handle->toitemid;
	//LogMI("toitemid %s",toitemid);
	if((SLIB_strcmp(chat_test,chat_test2) != 0)){
		SheBei_a = handle->next;
		if(SheBei_a){
			handle_return = XML_haudongtiao_Handle_Add(SheBei_a, need_handle, toitemid);
		}else{
		    SheBei_a = XML_haudongtiao_WinHandle_Create();
		    SheBei_a = XML_haudongtiao_HandleDate_Add(SheBei_a, need_handle, toitemid);
		    handle->next = SheBei_a;
		    handle_return = SheBei_a;
		}
	}else if((SLIB_strcmp(chat_test,chat_test2) == 0)){
	    XML_haudongtiao_HandleDate_Add(handle, need_handle, toitemid);
		handle_return = handle;
	}else{
		return NULL;
	}
	return handle_return;
}
static win_huadongtiao_handle * XML_haudongtiao_WinHandle_Search(win_huadongtiao_handle * handle,char *  toitemid)
{
	win_huadongtiao_handle * SheBei_a;
	win_huadongtiao_handle * handle_return;
	char * chat_test;
	char * chat_test2;
	chat_test = toitemid;
	chat_test2 = handle->toitemid;
	
	if(handle){
		if((SLIB_strcmp(chat_test,chat_test2) != 0)){
			SheBei_a = handle->next;
			if(SheBei_a){
				handle_return = XML_haudongtiao_WinHandle_Search(SheBei_a,toitemid);
			}else{
				return NULL;
			}
		}else if((SLIB_strcmp(chat_test,chat_test2) == 0)){
			handle_return = handle;
		}else{
			return NULL;
		}
	}else{
		return NULL;
	}
	return handle_return;
}

static void XML_haudongtiao_handle_create(H_WIN need_handle,char *  toitemid)
{//添加定时器数据
   if(!xml_huadongtiao_win_handle){
		xml_huadongtiao_win_handle = (win_huadongtiao_handle *)esMEMS_Malloc(0, sizeof(win_huadongtiao_handle)+1);
		if (!xml_huadongtiao_win_handle) {
			LogE("Malloc failed");
		}
		eLIBs_memset(xml_huadongtiao_win_handle, 0, sizeof(win_huadongtiao_handle)+1);
	}
	XML_haudongtiao_Handle_Add(xml_huadongtiao_win_handle, need_handle, toitemid);
}
win_huadongtiao_handle * XML_haudongtiao_Get_Handle(char *  toitemid)
{
	win_huadongtiao_handle * SheBei_a;
	SheBei_a = XML_haudongtiao_WinHandle_Search(xml_huadongtiao_win_handle,toitemid);
	return SheBei_a;
}

win_huadongtiao_handle * XML_haudongtiao_Get_Handle_Next(win_huadongtiao_handle * handle,char *  toitemid)
{
	win_huadongtiao_handle * SheBei_a;
	SheBei_a = XML_haudongtiao_WinHandle_Search(handle,toitemid);
	return SheBei_a;
}
void XML_haudongtiao_Handle_Date(H_WIN need_handle,char *  toitemid)
{
    XML_haudongtiao_handle_create(need_handle, toitemid);//通讯数据初始化
}
unsigned char * XML_haudongtiao_GetChar_ToX(char * cha,unsigned char *xml_2)
{//将字符串转换为16进制，允许abcdef，待发送的16进制字符串
	unsigned char *xml_1;
	int char_len;
	int cnt,cnt_2;
	char_len = eLIBs_strlen(cha);
	xml_1 = (unsigned char *)esMEMS_Malloc(0, char_len);
	for(cnt = 0;cnt < char_len - 2;cnt++){
		xml_1[cnt] = huadongtiao_char_to_int(cha[cnt + 2]);
	} 
	for(cnt = 0,cnt_2 = 0;cnt < char_len - 2;cnt_2 ++ ,cnt += 2){
		xml_2[cnt_2] = xml_1[cnt] << 4 | xml_1[cnt + 1];
	}
	esMEMS_Mfree(0, xml_1);
	return xml_2;
}
static void XML_haudongtiao_PageHandle_Search(win_huadongtiao_handle * handle,int page)
{
	win_huadongtiao_handle * SheBei_a;
	//char name[128];
	
	if(handle){
		if(page != handle->page){
			SheBei_a = handle->next;
			if(SheBei_a){
				XML_haudongtiao_PageHandle_Search(SheBei_a,page);
			}
		}else{
			//GUI_WinGetName(handle->need_handle,name);
			//__msg("name %s\n",name);
			Return_Button_State(handle->need_handle,handle->page);
			esKRNL_TimeDly(4);
			SheBei_a = handle->next;
			if(SheBei_a){
				XML_haudongtiao_PageHandle_Search(SheBei_a,page);
			}
		}
	}
}

void XML_haudongtiao_Get_PageHandle(int	page)
{
	if(xml_huadongtiao_win_handle){
		XML_haudongtiao_PageHandle_Search(xml_huadongtiao_win_handle,page);
	}
}

