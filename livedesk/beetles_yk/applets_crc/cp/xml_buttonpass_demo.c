#include <apps.h>
#include "scrolling_i.h"//普通组按键id与状态，组号
#include "xml_buttonpass_demo.h"
#include "xml_demo.h"

static savebuttonstate * save_button_state;
////////////////////////////////////////////////////////////////////////////////
static savebuttonstate * XML_SaveButtonStateHandle_create(void)
{//添加定时器数据
   savebuttonstate * handle;
	handle = (savebuttonstate *)esMEMS_Malloc(0, sizeof(savebuttonstate));
	if (!handle) {
		LogE("Malloc failed");
	}
	eLIBs_memset(handle, 0, sizeof(savebuttonstate));
   return handle;
}
static savebuttonstate * XML_SaveButtonStateDate_Add(savebuttonstate * handle, char * xml_id, int buttonStatus)
{	
	handle->xml_id = xml_id;
	handle->buttonStatus = buttonStatus;
	return handle;
}

static savebuttonstate * XML_SaveButtonState_Add(savebuttonstate * handle, char * xml_id, int buttonStatus)
{
	savebuttonstate * SheBei_a;
	savebuttonstate * handle_return;
	char * chat_test;
	char * chat_test2;
	if(handle){
		chat_test = xml_id;
		chat_test2 = handle->xml_id;
		if(SLIB_strcmp(chat_test,chat_test2) == 0){
			XML_SaveButtonStateDate_Add(handle, xml_id, buttonStatus);
		}else if(SLIB_strcmp(chat_test,chat_test2) != 0){
			SheBei_a = handle->next;
			if(SheBei_a){
				XML_SaveButtonState_Add(SheBei_a, xml_id, buttonStatus);
			}else{
				SheBei_a = XML_SaveButtonStateHandle_create();
				SheBei_a = XML_SaveButtonStateDate_Add(SheBei_a, xml_id, buttonStatus);
				handle->next = SheBei_a;
				handle_return = SheBei_a;
				return handle_return;
			}
		}
	}
	return NULL;
}

static void XML_SaveButtonState_create(char * xml_id, int buttonStatus)
{
   if(!save_button_state){
		save_button_state = (savebuttonstate *)esMEMS_Malloc(0, sizeof(savebuttonstate));
		if (!save_button_state) {
			LogE("Malloc failed");
		}
		eLIBs_memset(save_button_state, 0, sizeof(savebuttonstate));
	}
	XML_SaveButtonState_Add(save_button_state, xml_id, buttonStatus);
}

void XML_SaveButtonState_Handle(char * xml_id,int buttonStatus)
{
	XML_SaveButtonState_create(xml_id, buttonStatus);//按键数据初始化
}

void XML_ChangeButtonState_Handle(char * xml_id,int buttonStatus)
{
	XML_SaveButtonState_Add(save_button_state, xml_id, buttonStatus);//按键数据初始化
}

static int XML_SaveButtonState_search(savebuttonstate * handle,char * xml_id)
{
	char * buttonid;
	char * buttonid2;
	buttonid = xml_id;
	buttonid2 = handle->xml_id;
	if(SLIB_strcmp(buttonid2,buttonid) == 0){
		//LogMI("save_button_state->buttonStatus = %d",handle->buttonStatus);
		return handle->buttonStatus;
	}else{
		if(handle->next){
		   XML_SaveButtonState_search(handle->next,xml_id);
		}else{
		   //LogMI("xml_id = %s  NULL",xml_id);
		   return -1;
		}
	}
}

int XML_FindButtonState(char * xml_id)
{	
	int i;
	
	if(save_button_state){
		i = XML_SaveButtonState_search(save_button_state,xml_id);
		return i;
	}else{
		return -1;
	}
}

