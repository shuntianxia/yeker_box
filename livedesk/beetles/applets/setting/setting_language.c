#include "setting_language.h"
#include "beetles_app.h"
#include "button_ex.h"
#include "app_setting.h"

typedef enum {
	SETTING_LANGUAGE_ITEM_ID_RETURN,
	SETTING_LANGUAGE_ITEM_ID_CHINESE,
	SETTING_LANGUAGE_ITEM_ID_ENGLISH,
	SETTING_LANGUAGE_ITEM_ID_ALL,
}setting_language_item_id_t;

typedef struct {
	RECT rect_bg;
	RECT rect_pic;
	__s32 pic_id;
	load_bmp_res_t pic_res;
}setting_language_ui_t;

typedef struct {
	H_LYR layer;
	H_WIN button[SETTING_LANGUAGE_ITEM_ID_ALL];
}setting_language_para_t;

static setting_language_ui_t setting_language_ui =
{
	{0, 0, 720, 720},
	{261, 68, 202, 38},
	ID_SETTING_LANG_LANGUAGE_CHOICE_TITLE_BMP,
};

static button_ex_ui_t setting_language_bt_ui[] =
{
	{
		SETTING_LANGUAGE_ITEM_ID_RETURN,
		BUTTON_TYPE_PRESS,
		{0, 0, 170, 170},
		{
			ID_SETTING_RETURN_NORMAL_BMP,
			ID_SETTING_RETURN_FOCUS_BMP,
		},
	},
	{
		SETTING_LANGUAGE_ITEM_ID_CHINESE,
		BUTTON_TYPE_ONOFF,
		{155, 299, 408, 100},
		{
			ID_SETTING_LANG_CHINESE_NORMAL_BMP,
			ID_SETTING_LANG_CHINESE_FOCUS_BMP,
			ID_SETTING_LANG_CHINESE_DISABLE_BMP,
		},
	},
	{
		SETTING_LANGUAGE_ITEM_ID_ENGLISH,
		BUTTON_TYPE_ONOFF,
		{155, 449, 408, 100},
		{
			ID_SETTING_LANG_ENGLISH_NORMAL_BMP,
			ID_SETTING_LANG_ENGLISH_FOCUS_BMP,
			ID_SETTING_LANG_ENGLISH_DISABLE_BMP,
		},
	},
};

static void setting_language_res_init()
{
	int i;

	create_bmp_res(setting_language_ui.pic_id, setting_language_ui.pic_res);
	for(i = 0; i < (sizeof(setting_language_bt_ui)/sizeof(setting_language_bt_ui[0])); i++) {
		create_bmp_res(setting_language_bt_ui[i].bmp_id[0], setting_language_bt_ui[i].bmp_res[0]);
		create_bmp_res(setting_language_bt_ui[i].bmp_id[1], setting_language_bt_ui[i].bmp_res[1]);
		create_bmp_res(setting_language_bt_ui[i].bmp_id[2], setting_language_bt_ui[i].bmp_res[2]);
	}
}

static void setting_language_res_deinit()
{
	int i;

	destroy_bmp_res(setting_language_ui.pic_res);
	for(i = 0; i < (sizeof(setting_language_bt_ui)/sizeof(setting_language_bt_ui[0])); i++) {
		destroy_bmp_res(setting_language_bt_ui[i].bmp_res[0]);
		destroy_bmp_res(setting_language_bt_ui[i].bmp_res[1]);
		destroy_bmp_res(setting_language_bt_ui[i].bmp_res[2]);
	}
}

static H_LYR setting_language_layer_create()
{
	RECT rect;
	SIZE screen_size;
		
	rect.x = 0;
	rect.y = 0;
	dsk_display_get_size(&screen_size.width,&screen_size.height);
	rect.width = screen_size.width;
	rect.height = screen_size.height;
	
	return app_32bpp_layer_create(rect, 1);
}

static __s32 setting_language_paint(setting_language_para_t *setting_language_para)
{
	GUI_LyrWinSel(setting_language_para->layer);
	GUI_BMP_RES_Draw(setting_language_ui.pic_res, setting_language_ui.rect_pic.x, setting_language_ui.rect_pic.y);
	return EPDK_OK;
}

static __s32 setting_language_proc(__gui_msg_t* msg)
{
	setting_ctrl_t *setting_ctrl;
	
	setting_ctrl = (setting_ctrl_t *)GUI_WinGetAttr(msg->h_deswin);

	switch(msg->id)
	{
		case GUI_MSG_CREATE:
		{
			setting_language_para_t *setting_language_para;

			setting_language_para = (setting_language_para_t *)esMEMS_Balloc(sizeof(setting_language_para_t));
			eLIBs_memset((void*)setting_language_para, 0, sizeof(setting_language_para_t));
			GUI_WinSetAddData(msg->h_deswin, (__u32)setting_language_para);

			setting_language_para->layer = setting_language_layer_create();
        	setting_language_res_init();
        	
        	GUI_LyrWinSetSta(setting_language_para->layer, GUI_LYRWIN_STA_SLEEP);
        	setting_language_paint(setting_language_para);
        	button_group_create(msg->h_deswin, \
				setting_language_para->layer, \
				setting_language_bt_ui, \
				setting_language_para->button, \
				sizeof(setting_language_bt_ui)/sizeof(setting_language_bt_ui[0]));
        	GUI_LyrWinSetSta(setting_language_para->layer, GUI_LYRWIN_STA_ON);
			GUI_LyrWinSetTop(setting_language_para->layer);
			break;
		}
			
		case GUI_MSG_DESTROY:
		{
			setting_language_para_t *setting_language_para;

			setting_language_para = (setting_language_para_t *)GUI_WinGetAddData(msg->h_deswin);
			
	        button_group_destroy(setting_language_para->button, \
				sizeof(setting_language_bt_ui)/sizeof(setting_language_bt_ui[0]));
        	setting_language_res_deinit();

        	if(setting_language_para->layer != NULL) {
				GUI_LyrWinSetSta(setting_language_para->layer, GUI_LYRWIN_STA_OFF);
				GUI_LyrWinDelete(setting_language_para->layer);
				setting_language_para->layer = NULL;
			}

			eLIBs_memset(setting_language_para, 0, sizeof(setting_language_para_t));
			esMEMS_Bfree(setting_language_para, sizeof(setting_language_para_t));
			break;
		}
		
		case GUI_MSG_CLOSE:
		{
			GUI_ManWinDelete(msg->h_deswin);
			break;
		}
			
		case GUI_MSG_COMMAND:
		{
			switch(LOSWORD(msg->dwAddData1))
			{
				case SETTING_LANGUAGE_ITEM_ID_RETURN:
					if(HISWORD(msg->dwAddData1) == BN_CLICKED) {
						__gui_notify_msg_t		notify_msg;
						
						notify_msg.hWnd 	= msg->h_deswin;
						notify_msg.id		= GUI_WinGetItemId(msg->h_deswin);
						notify_msg.msgcode	= ITEM_MSG_CODE_RETURN;
						notify_msg.dwAddData = 0;
						notify_msg.dwReserved = 0;
						GUI_NotifyParent(&notify_msg);
					}
					break;
					
				case SETTING_LANGUAGE_ITEM_ID_CHINESE:
					if(HISWORD(msg->dwAddData1) == BN_PUSHED) {
						;
					}
					break;

				case SETTING_LANGUAGE_ITEM_ID_ENGLISH:
					if(HISWORD(msg->dwAddData1) == BN_PUSHED) {
						;
					}
					break;
					
				default:
					break;
			}
			return EPDK_OK;
		}

		default:
			break;
	}	
	return GUI_ManWinDefaultProc(msg); 
}	

H_WIN setting_language_create(H_WIN parent, __u16 id)
{
	__gui_manwincreate_para_t create_info;
	setting_ctrl_t *setting_ctrl;
	
	setting_ctrl = (setting_ctrl_t *)GUI_WinGetAttr(parent);
	
	eLIBs_memset(&create_info, 0, sizeof(__gui_manwincreate_para_t));
	
	create_info.name            = "SETTING_LANGUAGE_WIN";
	create_info.hParent         = parent;
	create_info.ManWindowProc   = (__pGUI_WIN_CB)esKRNL_GetCallBack((__pCBK_t)setting_language_proc); 
	create_info.attr            = (void*)setting_ctrl;
	create_info.id				= id;
	create_info.hHosting        = NULL;

    return GUI_ManWinCreate(&create_info);
}
