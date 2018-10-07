#include <apps.h>
#include "xml_matrix_caozuo.h"//矩阵建输入操作保存

static matrix_caozuo_handle * Matrix_caozuo_handle;
//保存
static matrix_caozuo_handle * Matrix_caozuo_handle_Create(void)
{
	matrix_caozuo_handle * handle;
	handle = (matrix_caozuo_handle *)esMEMS_Malloc(0, sizeof(matrix_caozuo_handle)+1);
	if (!handle) {
		LogE("SHRBEI_A failed");
		return NULL;
	}
	eLIBs_memset(handle, 0, sizeof(matrix_caozuo_handle)+1);
	return handle;
}

static matrix_caozuo_handle * XML_Matrix_HandleDate_Add(matrix_caozuo_handle * handle,int groupid,int caozuo_static)
{
	handle->groupid = groupid;
	handle->caozuo_static = caozuo_static;
	//LogMI("handle->caozuo_static  %d",handle->caozuo_static );
	return handle;
}

static matrix_caozuo_handle * Matrix_Handle_Add(matrix_caozuo_handle * handle,int groupid,int caozuo_static)
{
	matrix_caozuo_handle * SheBei_a;
	matrix_caozuo_handle * handle_return;
	int chat_groupid;
	int chat_static;
	chat_groupid = groupid;
	chat_static = caozuo_static;
	//LogMI("toitemid %s",toitemid);
	if(chat_groupid != handle->groupid){
		SheBei_a = handle->next;
		if(SheBei_a){
			handle_return = Matrix_Handle_Add(SheBei_a, groupid, caozuo_static);
		}else{
			SheBei_a = Matrix_caozuo_handle_Create();
			SheBei_a = XML_Matrix_HandleDate_Add(SheBei_a, groupid, caozuo_static);
			handle->next = SheBei_a;
			handle_return = SheBei_a;
		}
	}else if(chat_groupid == handle->groupid){
		XML_Matrix_HandleDate_Add(handle, groupid, caozuo_static);
		handle_return = handle;
	}else{
		return NULL;
	}
	return handle_return;
}
void Save_Matrix_Static(int groupid,int caozuo_static)//caozuo_static 为0 无操作 1有操作
{
	if(!Matrix_caozuo_handle){
		 Matrix_caozuo_handle = (matrix_caozuo_handle *)esMEMS_Malloc(0, sizeof(matrix_caozuo_handle)+1);
		 if (!Matrix_caozuo_handle) {
			 LogE("Malloc failed");
		 }
		 eLIBs_memset(Matrix_caozuo_handle, 0, sizeof(matrix_caozuo_handle)+1);
	 }
	 Matrix_Handle_Add(Matrix_caozuo_handle, groupid, caozuo_static);
}
//读

static int Matrix_State_search(matrix_caozuo_handle * handle,int groupid)
{
	int buttonid;
	int buttonid2;
	buttonid = groupid;
	buttonid2 = handle->groupid;
	if(buttonid2 == buttonid){
		return handle->caozuo_static;
	}else{
		if(handle->next){
		   Matrix_State_search(handle->next,groupid);
		}else{
		   //LogMI("xml_id = %s  NULL",xml_id);
		   return -1;
		}
	}
}
int Find_Matrix_State(int groupid)
{	
	int i;
	
	if(Matrix_caozuo_handle){
		i = Matrix_State_search(Matrix_caozuo_handle,groupid);
		return i;
	}else{
		return -1;
	}
}

