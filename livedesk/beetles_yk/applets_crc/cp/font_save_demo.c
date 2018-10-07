#include <apps.h>
#include "font_save_demo.h"
#include "cp_i.h"

static Font_handle * font_handle;


static Font_handle * XML_Font_handle_WinHandle_Create(void)
{
	Font_handle * handle;
	handle = (Font_handle *)esMEMS_Malloc(0, sizeof(Font_handle));
	if (!handle) {
		LogE("SHRBEI_A failed");
		return NULL;
	}
	eLIBs_memset(handle, 0, sizeof(Font_handle));
	return handle;
}

static Font_handle * XML_Font_HandleDate_Add(Font_handle * handle , int word_size , const char *  font_file)
{
	handle->word_size = word_size;
	handle->font_file = font_file;
	handle->font = GUI_SFT_CreateFont(word_size, font_file);
	return handle;
}

static void XML_Font_Handle_Add(Font_handle * handle , int word_size , const char *  font_file)
{
	Font_handle * SheBei_a;
	if(handle){
		if(SLIB_strcmp(handle->font_file,font_file) == 0){
			//LogMI("font_file = %s",font_file);
			if(word_size != handle->word_size){
				SheBei_a = handle->next;
				if(SheBei_a){
					//LogMI("word_size = %d",word_size);
					XML_Font_Handle_Add(SheBei_a, word_size, font_file);
				}else{
					//LogMI("word_size = %d",word_size);
					SheBei_a = XML_Font_handle_WinHandle_Create();
					SheBei_a = XML_Font_HandleDate_Add(SheBei_a, word_size, font_file);
					handle->next = SheBei_a;
				}
			}
		}else{
		        SheBei_a = handle->next;
				if(SheBei_a){
					//LogMI("word_size = %d",word_size);
					XML_Font_Handle_Add(SheBei_a, word_size, font_file);
				}else{
					//LogMI("word_size = %d",word_size);
					SheBei_a = XML_Font_handle_WinHandle_Create();
					SheBei_a = XML_Font_HandleDate_Add(SheBei_a, word_size, font_file);
					handle->next = SheBei_a;
				}
		}
	}
}
static Font_handle * XML_Font_handle_Search(Font_handle * handle , int word_size , const char *  font_file)
{
	Font_handle * SheBei_a;
	if(handle){
		if(SLIB_strcmp(handle->font_file,font_file) == 0){
			//LogMI("handle->word_size = %d",handle->word_size);
			if(handle->word_size != word_size){
				SheBei_a = handle->next;
				if(SheBei_a){
					//LogMI("word_size = %d",word_size);
					XML_Font_handle_Search(SheBei_a,word_size,font_file);
				}else{
					//LogMI("word_size = %d",word_size);
					return NULL;
				}
			}else if(handle->word_size == word_size){
				//LogMI("word_size = %d",word_size);
				return handle;
			}
		}else{
	        SheBei_a = handle->next;
			if(SheBei_a){
				//LogMI("word_size = %d",word_size);
				XML_Font_handle_Search(SheBei_a,word_size,font_file);
			}else{
				return NULL;
			}
		}
	}else{
		return NULL;
	}
}

static void XML_Font_handle_create(int word_size , const char * font_file)
{//添加定时器数据
   if(!font_handle){
		font_handle = (Font_handle *)esMEMS_Malloc(0, sizeof(Font_handle));
		if (!font_handle) {
			LogE("Malloc failed");
		}
		eLIBs_memset(font_handle, 0, sizeof(Font_handle));
		font_handle->word_size = word_size;
		font_handle->font_file = font_file;
		font_handle->font = GUI_SFT_CreateFont(word_size, font_file);
	}
	XML_Font_Handle_Add(font_handle, word_size, font_file);
}
GUI_FONT * XML_Font_handle_Get_Font(int word_size , const char * font_file)
{
	Font_handle * SheBei_a;
    SheBei_a = XML_Font_handle_Search(font_handle , word_size,font_file);
    if(SheBei_a){
	    //LogMI("word_size = %d",word_size);
        return SheBei_a->font;
    }else{
	    //LogMI("word_size = %d",word_size);
	    return NULL;
    }
}

void XML_Font_handle_Date(int word_size ,const char * font_file)
{
	XML_Font_handle_create(word_size, font_file);//通讯数据初始化
}


