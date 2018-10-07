#include <apps.h>
#include "scrolling_i.h"//普通组按键id与状态，组号
#include "xml_onlyone_demo.h"
static onlyone_handle * xml_slaver_handle;
static int onlyone_int;
static char * onlyone_name;
static onlyone_handle * XML_Onlyone_create(void)
{//添加定时器数据
   onlyone_handle * handle;
	handle = (onlyone_handle *)esMEMS_Malloc(0, sizeof(onlyone_handle));
	if (!handle) {
		LogE("Malloc failed");
	}
	eLIBs_memset(handle, 0, sizeof(onlyone_handle));
   return handle;
}
static onlyone_handle * XML_Onlyone_HandleDate_Add(onlyone_handle * handle,int groupid,char * toitemid)
{	
	handle->groupid = groupid;
	handle->toitemid = toitemid;
	return handle;
}
static onlyone_handle * XML_Onlyone_Handle_Add(onlyone_handle * handle,int groupid,char * toitemid,int slave_num)
{
	onlyone_handle * SheBei_a;
	onlyone_handle * handle_return;
	int i;
	for(i = 0;i < slave_num;i++){
		//LogMI("onlyone_int = %d",onlyone_int);
		//LogMI("i = %d",i);
		handle = handle->next;
	}
		SheBei_a = XML_Onlyone_create();
		SheBei_a = XML_Onlyone_HandleDate_Add(SheBei_a, groupid,toitemid);
		handle->next = SheBei_a;
		handle_return = SheBei_a;
		onlyone_int += 1;
	return handle_return;
}

static void XML_Onlyone_handle_create(int groupid,char * toitemid)
{//添加定时器数据
   if(!xml_slaver_handle){
		xml_slaver_handle = (onlyone_handle *)esMEMS_Malloc(0, sizeof(onlyone_handle));
		if (!xml_slaver_handle) {
			LogE("Malloc failed");
		}
		eLIBs_memset(xml_slaver_handle, 0, sizeof(onlyone_handle));
		xml_slaver_handle->groupid = groupid;
		xml_slaver_handle->toitemid = toitemid;
		onlyone_int = 0;
	}
	XML_Onlyone_Handle_Add(xml_slaver_handle, groupid, toitemid,onlyone_int);
}
void XML_Onlyone_Handle_Date(int groupid,char * toitemid)
{
	XML_Onlyone_handle_create(groupid, toitemid);//通讯数据初始化
}

static void Onlyone_Handle_search(onlyone_handle * handle,int groupid)
{
	int buttonid;
	int buttonid2;
	char * buttonid3;
	char * buttonid4;
	buttonid = groupid;
	buttonid3 = onlyone_name;
    buttonid2 = handle->groupid;
    buttonid4 = handle->toitemid;
	if(buttonid2 == buttonid){
		if(SLIB_strcmp(buttonid3,buttonid4) != 0){
		    Onlyone_Return_DongZuo(handle->toitemid,handle->groupid);
		}else{
		    //LogMI("handle->groupid = %d  ",handle->groupid);
			Click_Return_DongZuo(handle->toitemid,1,handle->groupid);
		}
	}
	if(handle->next){
		//LogMI("groupid = %d  NULL",groupid);
		Onlyone_Handle_search(handle->next,groupid);
	}else{
		//LogMI("groupid = %d  NULL",groupid);
	}
		
}

void Find_Onlyone_Handle(int groupid)
{
	if(xml_slaver_handle){
    	Onlyone_Handle_search(xml_slaver_handle->next,groupid);
	}
}

static void Onlyone_groupid_search(onlyone_handle * handle,char * toitemid)
{
	char * buttonid;
	char * buttonid2;
	buttonid = toitemid;
	buttonid2 = handle->toitemid;
	if(SLIB_strcmp(buttonid2,buttonid) == 0){
		Find_Onlyone_Handle(handle->groupid);
	}else{
		if(handle->next){
		   Onlyone_groupid_search(handle->next,toitemid);
		}else{
		   //LogMI("toitemid = %s  NULL",toitemid);
		}
	}
}

void Find_Onlyone_groupid(char * toitemid)
{    
    onlyone_name = toitemid;
	if(xml_slaver_handle){
		Onlyone_groupid_search(xml_slaver_handle,toitemid);
	}

}
static int Onlyone_Ifgroupid_search(onlyone_handle * handle,char * toitemid)
{
	char * buttonid;
	char * buttonid2;
	buttonid = toitemid;
	buttonid2 = handle->toitemid;
	if(SLIB_strcmp(buttonid2,buttonid) == 0){
		//LogMI("xml_Onlyone_handle->groupid = %d",handle->groupid);
		return handle->groupid;
	}else{
		if(handle->next){
		   Onlyone_Ifgroupid_search(handle->next,toitemid);
		}else{
		   //LogMI("toitemid = %s  NULL",toitemid);
		   return -1;
		}
	}
}

int Find_IfOnlyone(char * toitemid)
{	
    int i;
	
	if(xml_slaver_handle){
		i = Onlyone_Ifgroupid_search(xml_slaver_handle,toitemid);
		return i;
	}else{
		return -1;
	}
}

