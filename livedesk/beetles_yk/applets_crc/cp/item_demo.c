#include <apps.h>
#include "item_demo.h"
#include "cp_i.h"
static Item_handle * Item_handle_code;

static Item_handle * ItemhandleNew(void)
{
	Item_handle * create;
	create = (Item_handle *)esMEMS_Malloc(0, sizeof(Item_handle));
	if (create) {
		eLIBs_memset(create, 0, sizeof(Item_handle));
		return create;
	} else {
		return NULL;
	}
}
static Item_handle* Item_date_add(Item_handle * handler,int group_id,char * input_code)
{
	handler->group_id = group_id;
	handler->input_code = input_code;
	return handler;
}

static Item_handle*  Item_handle_add(Item_handle * image_handle,int group_id,char * input_code)
{
	Item_handle * create;
	Item_handle * handle_return;
	if(create->group_id != group_id ){
		create = create->next;
		if(create){
		   handle_return = Item_handle_add(create,group_id,input_code);
		}else{
			create = ItemhandleNew();
			create = Item_date_add(create,group_id,input_code);
			image_handle->next = create;
			handle_return = create;
			//LogMI("handler->group_id = %d",group_id);	  
		}
	}else if(create->group_id == group_id){
		handle_return = Item_date_add(image_handle,group_id,input_code);
		//LogMI("handler->group_id = %d",group_id);	  
	}
	return handle_return;

}
static void Item_handle_create(int group_id,char * input_code)
{//添加定时器数据
   if(!Item_handle_code){
		Item_handle_code = (Item_handle *)esMEMS_Malloc(0, sizeof(Item_handle));
		if (!Item_handle_code) {
			LogE("Malloc failed");
		}
		eLIBs_memset(Item_handle_code, 0, sizeof(Item_handle));
		Item_handle_code->group_id = group_id;
		Item_handle_code->input_code = input_code;
	}
	Item_handle_add(Item_handle_code, group_id, input_code);
}
void Item_Handle_Date(int group_id,char * input_code)
{
	Item_handle_create(group_id, input_code);//通讯数据初始化
}

static char * Item_code_search(Item_handle * image_handle,int group_id)
{	
    Item_handle * create;
	char * handle_return;
	if(image_handle->group_id != group_id ){
		create = image_handle->next;
		//LogMI("handler->group_id = %d",image_handle->group_id);	  
		if(create){
		    Item_code_search(image_handle->next,group_id);
		}else{
			return NULL;
		}
	}else if(image_handle->group_id == group_id){
		handle_return = image_handle->input_code;
		return handle_return;
	}
}

char * Item_code_return(int group_id)
{   
	char * return_group_code;
	if(Item_handle_code){
		return_group_code = Item_code_search(Item_handle_code,group_id);
		return return_group_code;
	}else{
		return NULL;
	}
}
