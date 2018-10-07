#include <apps.h>
#include "scrolling_i.h"//普通组按键id与状态，组号
#include "xml_array_demo.h"
static array_handle * xml_slaver_handle;
static int array_int;
static char * array_name;
static array_handle * XML_Array_create(void)
{//添加定时器数据
   array_handle * handle;
	handle = (array_handle *)esMEMS_Malloc(0, sizeof(array_handle)+1);
	if (!handle) {
		LogE("Malloc failed");
	}
	eLIBs_memset(handle, 0, sizeof(array_handle)+1);
   return handle;
}
static array_handle * XML_Array_HandleDate_Add(array_handle * handle,int groupid,char * toitemid)
{	
	handle->groupid = groupid;
	handle->toitemid = toitemid;
	//LogMI("handle->toitemid = %s",handle->toitemid);
	return handle;
}
static array_handle * XML_Array_Handle_Add(array_handle * handle,int groupid,char * toitemid,int slave_num)
{
	array_handle * SheBei_a;
	array_handle * handle_return;
	int i;
	for(i = 0;i < slave_num;i++){
		//LogMI("i = %d",i);
		handle = handle->next;
	}
		SheBei_a = XML_Array_create();
		SheBei_a = XML_Array_HandleDate_Add(SheBei_a, groupid,toitemid);
		handle->next = SheBei_a;
		handle_return = SheBei_a;
		array_int += 1;
	return handle_return;
}

static void XML_Array_handle_create(int groupid,char * toitemid)
{//添加定时器数据
   if(!xml_slaver_handle){
		xml_slaver_handle = (array_handle *)esMEMS_Malloc(0, sizeof(array_handle)+1);
		if (!xml_slaver_handle) {
			LogE("Malloc failed");
		}
		eLIBs_memset(xml_slaver_handle, 0, sizeof(array_handle)+1);
		xml_slaver_handle->groupid = groupid;
		xml_slaver_handle->toitemid = toitemid;
		array_int = 0;
	}
	XML_Array_Handle_Add(xml_slaver_handle, groupid, toitemid,array_int);
}
void XML_Array_Handle_Date(int groupid,char * toitemid)
{
	XML_Array_handle_create(groupid, toitemid);//通讯数据初始化
}

static void Array_Handle_search(array_handle * handle,int groupid)
{
	int buttonid;
	int buttonid2;
	char * buttonid3;
	char * buttonid4;
	buttonid = groupid;
	buttonid3 = array_name;
	buttonid2 = handle->groupid;
	buttonid4 = handle->toitemid;
	if(buttonid2 == buttonid){
		if(SLIB_strcmp(buttonid3,buttonid4) != 0){
		    Array_Return_DongZuo(handle->toitemid,handle->groupid);
		}else{
			Click_Return_DongZuo(handle->toitemid,0,handle->groupid);
		}
	}
	if(handle->next){
		Array_Handle_search(handle->next,groupid);
	}else{
		//LogMI("groupid = %d",groupid);
		//Onlyone_Return_DongZuo(NULL);
	}
		
}

void Find_Array_Handle(int groupid)
{
	if(xml_slaver_handle){
    	Array_Handle_search(xml_slaver_handle->next,groupid);
	}
}

static void Array_groupid_search(array_handle * handle,char * toitemid)
{
	char * buttonid;
	char * buttonid2;
	buttonid = toitemid;
	buttonid2 = handle->toitemid;
	if(SLIB_strcmp(buttonid2,buttonid) == 0){
		Find_Array_Handle(handle->groupid);
	}else{
		if(handle->next){
		   Array_groupid_search(handle->next,toitemid);
		}else{
		   //LogMI("toitemid = %s  NULL",toitemid);
		}
	}
}

void Find_Array_groupid(char * toitemid)
{    
    array_name = toitemid;
	if(xml_slaver_handle){
		Array_groupid_search(xml_slaver_handle,toitemid);
	}
}
static int Array_groupnum_search(array_handle * handle,char * toitemid)
{
	char * buttonid;
	char * buttonid2;
	int return_group_numl;
	buttonid = toitemid;
	buttonid2 = handle->toitemid;
	if(SLIB_strcmp(buttonid2,buttonid) == 0){
		return_group_numl = handle->groupid;
		return return_group_numl;
	}else{
		if(handle->next){
		   Array_groupnum_search(handle->next,toitemid);
		}else{
		   //LogMI("toitemid = %s  NULL",toitemid);
		   return -1;
		}
	}
}
int Find_Array_groupnum(char * toitemid)
{	 
    int return_group_numl;
	array_name = toitemid;
	if(xml_slaver_handle){
		return_group_numl = Array_groupnum_search(xml_slaver_handle,toitemid);
		return return_group_numl;
	}else{
		return -1;
	}
}

