#include <apps.h>
#include "scrolling_i.h"//普通组按键id与状态，组号
#include "xml_zu_demo.h"
#include "xml_buttonpass_demo.h"

static primary_handle * xml_primary_handle;
static slaver_handle * xml_slaver_handle;
static int slave_int;
static char * click_xml_id_name;	//点击主键名字					   
static primary_handle * XML_Primary_create(void)
{//添加定时器数据
   primary_handle * handle;
	handle = (primary_handle *)esMEMS_Malloc(0, sizeof(primary_handle));
	if (!handle) {
		LogE("Malloc failed");
	}
	eLIBs_memset(handle, 0, sizeof(primary_handle));
   return handle;
}

static primary_handle * XML_Primary_HandleDate_Add(primary_handle * handle,int groupid,char * relation,char * toitemid,char * toitemstatus)
{
	char * relation_test;
	relation_test = relation;
	
	handle->groupid = groupid;
	if(SLIB_strcmp(relation_test,"primary") == 0){
		relation = "primary";
		handle->groupid = groupid;
		handle->relation = relation;
		handle->toitemid = toitemid;
	}else{
		//LogE("grouptype wrong");
	}
	return handle;
}
static primary_handle * XML_Primary_Handle_Add(primary_handle * handle,int groupid,char * relation,char * toitemid,char * toitemstatus)
{
	primary_handle * SheBei_a;
	primary_handle * handle_return;
	handle_return = NULL;
	if(handle->groupid != groupid){
		SheBei_a = handle->next;
		if(SheBei_a){
			handle_return = XML_Primary_Handle_Add(SheBei_a, groupid, relation,toitemid, toitemstatus);
		}else{
			if(SLIB_strcmp(relation,"primary") == 0){
				relation = "primary";
			    SheBei_a = XML_Primary_create();
				SheBei_a = XML_Primary_HandleDate_Add(SheBei_a, groupid, relation,toitemid,toitemstatus);
				handle->next = SheBei_a;
				handle_return = SheBei_a;
			}
		}
	}else if(handle->groupid == groupid){
		handle_return = handle;
	}
	return handle_return;
}

static void XML_Primary_handle_create(int groupid,char * relation,char * toitemid,char * toitemstatus)
{//添加定时器数据
   if(!xml_primary_handle){
		xml_primary_handle = (primary_handle *)esMEMS_Malloc(0, sizeof(primary_handle));
		if (!xml_primary_handle) {
			LogE("Malloc failed");
		}
		eLIBs_memset(xml_primary_handle, 0, sizeof(primary_handle));
		
		xml_primary_handle->groupid = groupid;
		xml_primary_handle->relation = relation;
		xml_primary_handle->toitemid = toitemid;
	}
	XML_Primary_Handle_Add(xml_primary_handle, groupid, relation, toitemid, toitemstatus);
}
void XML_Primary_Handle_Date(int groupid,char * relation,char * toitemid,char * toitemstatus)
{
	XML_Primary_handle_create(groupid, relation, toitemid, toitemstatus);//通讯数据初始化
}

static slaver_handle * XML_Slave_create(void)
{//添加定时器数据
   slaver_handle * handle;
	handle = (slaver_handle *)esMEMS_Malloc(0, sizeof(slaver_handle));
	if (!handle) {
		LogE("Malloc failed");
	}
	eLIBs_memset(handle, 0, sizeof(slaver_handle));
   return handle;
}

static slaver_handle * XML_Slave_HandleDate_Add(slaver_handle * handle,int groupid,char * relation,
	        char * toitemid,char * toitemstatus,int imageid)
{
	char * relation_test;
	relation_test = relation;
	
	if(SLIB_strcmp(relation_test,"slaver") == 0){
		relation = "slaver";
		handle->groupid = groupid;
		handle->relation = relation;
		handle->toitemid = toitemid;
		handle->imageid = imageid;
		handle->toitemstatus = toitemstatus;
	}else{
		//LogE("grouptype wrong");
	}
	return handle;
}
static slaver_handle * XML_Slave_Handle_Add(slaver_handle * handle,int groupid,char * relation,char * toitemid,
	       char * toitemstatus,int slave_num,int imageid)
{
	slaver_handle * SheBei_a;
	slaver_handle * handle_return;
	int i;
	for(i = 0;i < slave_num;i++){
		handle = handle->next;
	}
		SheBei_a = XML_Slave_create();
		SheBei_a = XML_Slave_HandleDate_Add(SheBei_a, groupid, relation,toitemid,toitemstatus,imageid);
		handle->next = SheBei_a;
		handle_return = SheBei_a;
		slave_int += 1;
	return handle_return;
}

static void XML_Slave_handle_create(int groupid,char * relation,char * toitemid,char * toitemstatus,int imageid)
{//添加定时器数据
   if(!xml_slaver_handle){
		xml_slaver_handle = (slaver_handle *)esMEMS_Malloc(0, sizeof(slaver_handle));
		if (!xml_slaver_handle) {
			LogE("Malloc failed");
		}
		eLIBs_memset(xml_slaver_handle, 0, sizeof(slaver_handle));
		xml_slaver_handle->groupid = groupid;
		xml_slaver_handle->relation = relation;
		xml_slaver_handle->toitemid = toitemid;
		xml_slaver_handle->imageid = imageid;
		xml_slaver_handle->toitemstatus = toitemstatus;
		slave_int = 0;
	}
	XML_Slave_Handle_Add(xml_slaver_handle, groupid, relation, toitemid, toitemstatus,slave_int,imageid);
}

static void Slave_Handle_search(slaver_handle * handle,int groupid,char * father_toitemid)
{
	int buttonid;
	int buttonid2;
	buttonid = groupid;
	if(handle){
	   buttonid2 = handle->groupid;
	}else{
	   buttonid2 = -1;
	}
	if(buttonid2 == buttonid){
		Normal_Return_DongZuo(handle->toitemid,handle->toitemstatus,handle->imageid,handle->groupid,father_toitemid);
	}
	if(handle->next){
		Slave_Handle_search(handle->next,groupid,father_toitemid);
	}else{
		//LogMI("groupid = %d  NULL",groupid);
	}
		
}

void Find_Slave_Handle(int groupid,char * father_toitemid)
{
	if(xml_slaver_handle){
    	Slave_Handle_search(xml_slaver_handle->next,groupid,father_toitemid);
	}
}

void XML_Slave_Handle_Date(int groupid,char * relation,char * toitemid,char * toitemstatus,int imageid)
{
	XML_Slave_handle_create(groupid, relation, toitemid, toitemstatus,imageid);//通讯数据初始化
}
static void Primary_Handle_search(primary_handle * handle,char * toitemid)
{
	char * buttonid;
	char * buttonid2;
	buttonid = toitemid;
	buttonid2 = handle->toitemid;
	if(SLIB_strcmp(buttonid2,buttonid) == 0){
		Click_Return_DongZuo(handle->toitemid,0,handle->groupid);
		Find_Slave_Handle(handle->groupid,handle->toitemid);
	}else{
		if(handle->next){
		    Primary_Handle_search(handle->next,toitemid);
		}else{
		    //LogMI("toitemid = %s  NULL",toitemid);
		}
	}
}

void Find_Primary_Handle(char * toitemid)
{
    if(xml_primary_handle){
	    Primary_Handle_search(xml_primary_handle,toitemid);
	}
}
static void Primary_Name_search(primary_handle * handle,int groupid,char * toitemstatus,int relation_slave)
{
	int buttonid;
	int buttonid2;
	buttonid = groupid;
	buttonid2 = handle->groupid;
	if(buttonid2 == buttonid){
		if(SLIB_strcmp(click_xml_id_name,handle->toitemid) != 0){
			if(SLIB_strcmp(toitemstatus,"inc") == 0){
			   Main_Button_Up(handle->toitemid,3,relation_slave);
			}else if(SLIB_strcmp(toitemstatus,"dec") == 0){
			   Main_Button_Up(handle->toitemid,2,relation_slave);
			}else if(SLIB_strcmp(toitemstatus,"same") == 0){
			   Main_Button_Up(handle->toitemid,1,relation_slave);
			}else if(SLIB_strcmp(toitemstatus,"invert") == 0){
			   Main_Button_Up(handle->toitemid,0,relation_slave);
			}else if(SLIB_strcmp(toitemstatus,"lock") == 0){
			   Main_Button_Up(handle->toitemid,4,relation_slave);
			}else if(SLIB_strcmp(toitemstatus,"sync") == 0){
			   Main_Button_Up(handle->toitemid,5,relation_slave);//图片框
			}
		}
	}else{
		if(handle->next){
			Primary_Name_search(handle->next,groupid,toitemstatus,relation_slave);
		}else{
			//LogMI("toitemid = %s  NULL",groupid);
		}
	}
}

static void Primary_slaverId_search(slaver_handle * handle,char * Slave_Toitemid)
{
	char * buttonid;
	char * buttonid2;
	buttonid = Slave_Toitemid;
	buttonid2 = handle->toitemid;
	if(SLIB_strcmp(buttonid2,buttonid) == 0){
		Primary_Name_search(xml_primary_handle,handle->groupid,handle->toitemstatus,XML_FindButtonState(Slave_Toitemid));
		if(handle->next){
			Primary_slaverId_search(handle->next,Slave_Toitemid);
		}else{
			//LogMI("toitemid = %s  NULL",Slave_Toitemid);
		}
	}else{
		if(handle->next){
		    Primary_slaverId_search(handle->next,Slave_Toitemid);
		}else{
		    //LogMI("toitemid = %s  NULL",Slave_Toitemid);
		}
	}
}
void CFind_Primary_Toitemid(char * Slave_Toitemid)
{
	if(xml_slaver_handle){
		click_xml_id_name = Slave_Toitemid;
		Primary_slaverId_search(xml_slaver_handle,Slave_Toitemid);
	}
}
//按键点击返回
/*
static int If_SameGroupN(slaver_handle * handle,int zu_num,char * toitemid)
{
	char * buttonid;
	char * buttonid2;
	buttonid = toitemid;
	buttonid2 = handle->toitemid;
	if(SLIB_strcmp(buttonid2,buttonid) == 0){
		if(handle->groupid == zu_num){
			//LogMI("toitemid = %s",toitemid);
			//LogMI("handle->toitemid = %s",handle->toitemid);
			//LogMI("handle->groupid = %d",handle->groupid);
			return handle->groupid;
		}else{
			if(handle->next){
			   If_SameGroupN(handle->next,zu_num,toitemid);
			}else{
			   return -1;
			}
		}
	}else{
		if(handle->next){
			If_SameGroupN(handle->next,zu_num,toitemid);
		}else{
			return -1;
		}
	}
}
static int IfT_SameGroup_Normal(int zu_num,char * toitemid)
{	
	int i;
	if(xml_slaver_handle){
		i = If_SameGroupN(xml_slaver_handle,zu_num,toitemid);
		return i;
	}
	return -1;
}
static int Normal_Ifgroupid_search(primary_handle * handle,char * click,char * toitemid)
{
	int i;
	char * buttonid;
	char * buttonid2;
	buttonid = click;
	buttonid2 = handle->toitemid;
	if(SLIB_strcmp(buttonid2,buttonid) == 0){
		//LogMI("xml_Onlyone_handle->groupid = %d",handle->groupid);
		i = IfT_SameGroup_Normal(handle->groupid,toitemid);
		if(i < 0){
			if(handle->next){
			   Normal_Ifgroupid_search(handle->next,click,toitemid);
			}else{
			   return -1;
			}
		}else{
			return i;
		}
	}else{
		if(handle->next){
		   Normal_Ifgroupid_search(handle->next,click,toitemid);
		}else{
		   return -1;
		}
	}
}

int IfC_SameGroup_Normal(char * click,char * toitemid)
{	
	int i;
	//LogMI("click = %s",click);
	//LogMI("toitemid = %s",toitemid);
	if(xml_primary_handle){
		i = Normal_Ifgroupid_search(xml_primary_handle,click,toitemid);
		return i;
	}
	return -1;
}*/
//图片框id返回
static int If_Imageview(slaver_handle * handle,int zu_num,char * slave_toitemid)
{
	char * buttonid;
	char * buttonid2;
	buttonid = slave_toitemid;
	buttonid2 = handle->toitemid;
	if(SLIB_strcmp(buttonid2,buttonid) == 0){
		if(handle->groupid == zu_num){
			return handle->imageid;
		}else{
			if(handle->next){
			   If_Imageview(handle->next,zu_num,slave_toitemid);
			}else{
			   return -1;
			}
		}
	}else{
		if(handle->next){
			If_Imageview(handle->next,zu_num,slave_toitemid);
		}else{
			return -1;
		}
	}
}
static int IfT_SameGroup_Imageview(int zu_num,char * slave_toitemid)
{	
	int i;
	if(xml_slaver_handle){
		i = If_Imageview(xml_slaver_handle,zu_num,slave_toitemid);
		return i;
	}
	return -1;
}
static int Imageview_Id_search(primary_handle * handle,char * primary_toitemid,char * slave_toitemid)
{
	int i;
	char * buttonid;
	char * buttonid2;
	buttonid = primary_toitemid;
	buttonid2 = handle->toitemid;
	if(SLIB_strcmp(buttonid2,buttonid) == 0){
		i = IfT_SameGroup_Imageview(handle->groupid,slave_toitemid);
		if(i < 0){
			if(handle->next){
			   Imageview_Id_search(handle->next,primary_toitemid,slave_toitemid);
			}else{
			   return -1;
			}
		}else{
			return i;
		}
	}else{
		if(handle->next){
		   Imageview_Id_search(handle->next,primary_toitemid,slave_toitemid);
		}else{
		   return -1;
		}
	}
}
int Imageview_Id_Return(char * primary_toitemid,char * slave_toitemid)
{
	int i;
	if(xml_primary_handle){
		i = Imageview_Id_search(xml_primary_handle,primary_toitemid,slave_toitemid);
		return i;
	}
	return -1;
}
