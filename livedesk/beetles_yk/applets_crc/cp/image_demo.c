#include <apps.h>
#include "image_demo.h"
#include "cp_i.h"

static Image_handle* Image_date_add(Image_handle * handler,int image_num,int image_id)
{
	handler->image_id = image_id;
	handler->image_num = image_num;
	//LogMI("handler->image_id = %d",handler->image_id);	  
	//LogMI("handler->image_num = %d",handler->image_num);	  
	return handler;
}

Image_handle*  Image_handle_add(Image_handle * image_handle,int image_num,int image_id)
{
	Image_handle * create;
	Image_handle * handle_return;
	eLIBs_memcpy((Image_handle *)create, (Image_handle *)image_handle, sizeof(Image_handle));
	if(create->image_num != image_num ){
		create = create->next;
		if(create){
		   handle_return = Image_handle_add(create,image_num,image_id);
		}else{
			create = ImagehandleNew();
			create = Image_date_add(create,image_num,image_id);
			image_handle->next = create;
			handle_return = create;
			//LogMI("handler->image_id = %d",image_id);	  
		}
	}else if(create->image_num == image_num){
		handle_return = Image_date_add(image_handle,image_num,image_id);
		//LogMI("handler->image_id = %d",image_id);	  
	}
	return handle_return;

}

Image_handle * ImagehandleNew(void)
{
	Image_handle * create;
	create = (Image_handle *)esMEMS_Malloc(0, sizeof(Image_handle)+1);
	if (create) {
		eLIBs_memset(create, 0, sizeof(Image_handle)+1);
		return create;
	} else {
		return NULL;
	}
}

int  Image_id_search(Image_handle * image_handle,int image_num)
{	
    Image_handle * create;
	int handle_return;
	if(image_handle->image_num != image_num ){
		create = image_handle->next;
		if(create){
		   Image_id_search(image_handle->next,image_num);
		}else{
			return -1;
		}
	}else if(image_handle->image_num == image_num){
		handle_return = image_handle->image_id;
		//LogMI("handler->image_id = %d",image_handle->image_id);	  
		//LogMI("handler->image_num = %d",image_handle->image_num);
		return handle_return;
	}
}

