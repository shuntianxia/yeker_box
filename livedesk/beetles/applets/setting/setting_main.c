#include "setting_main.h"
#include "beetles_app.h"
#include "button_ex.h"
#include "app_setting.h"

typedef enum {
	SETTING_MAIN_ITEM_ID_RETURN,
	SETTING_MAIN_ITEM_ID_LOCK,
	SETTING_MAIN_ITEM_ID_RESET_PASSWD,
	SETTING_MAIN_ITEM_ID_TIMER,
	SETTING_MAIN_ITEM_ID_SCREEN_SAVER,
	SETTING_MAIN_ITEM_ID_SEASON,
	SETTING_MAIN_ITEM_ID_BASIC,
	SETTING_MAIN_ITEM_ID_NETWORK,
	SETTING_MAIN_ITEM_ID_UPGRADE,
	SETTING_MAIN_ITEM_ID_ALL,
}setting_main_item_id_t;

typedef struct {
	RECT rect_bg;
	RECT rect_pic;
	__s32 pic_id;
	load_bmp_res_t pic_res;
}setting_main_ui_t;

typedef struct {
	H_LYR layer;
	H_WIN button[SETTING_MAIN_ITEM_ID_ALL];
}setting_main_para_t;

static setting_main_ui_t setting_main_ui =
{
	{0, 0, 720, 720},
	{294, 23, 130, 130},
	ID_SETTING_ICON_LOCK_BMP,
};

static button_ex_ui_t setting_main_bt_ui[] =
{
	{
		SETTING_MAIN_ITEM_ID_RETURN,
		BUTTON_TYPE_PRESS,
		{0, 0, 170, 170},
		{
			ID_SETTING_RETURN_NORMAL_BMP,
			ID_SETTING_RETURN_FOCUS_BMP,
		},
	},
	{
		SETTING_MAIN_ITEM_ID_LOCK,
		BUTTON_TYPE_PRESS,
		{69, 181, 271, 100},
		{
			ID_SETTING_PERSONALITY_LOCK_NORMAL_BMP,
			ID_SETTING_PERSONALITY_LOCK_FOCUS_BMP,
			ID_SETTING_PERSONALITY_LOCK_DISABLE_BMP,
		},
	},
	{
		SETTING_MAIN_ITEM_ID_RESET_PASSWD,
		BUTTON_TYPE_PRESS,
		{379, 181, 271, 100},
		{
			ID_SETTING_RESET_PASSWORD_NORMAL_BMP,
			ID_SETTING_RESET_PASSWORD_FOCUS_BMP,
			ID_SETTING_RESET_PASSWORD_DISABLE_BMP,
		},
	},
	{
		SETTING_MAIN_ITEM_ID_TIMER,
		BUTTON_TYPE_PRESS,
		{69, 311, 271, 100},
		{
			ID_SETTING_TIMER_NORMAL_BMP,
			ID_SETTING_TIMER_FOCUS_BMP,
			ID_SETTING_TIMER_DISABLE_BMP,
		},
	},
	{
		SETTING_MAIN_ITEM_ID_SCREEN_SAVER,
		BUTTON_TYPE_PRESS,
		{379, 311, 271, 100},
		{
			ID_SETTING_SCREENSAVER_NORMAL_BMP,
			ID_SETTING_SCREENSAVER_FOCUS_BMP,
			ID_SETTING_SCREENSAVER_DISABLE_BMP,
		},
	},
	{
		SETTING_MAIN_ITEM_ID_SEASON,
		BUTTON_TYPE_PRESS,
		{69, 441, 271, 100},
		{
			ID_SETTING_SEASON_SUMMER_NORMAL_BMP,
			ID_SETTING_SEASON_SUMMER_FOCUS_BMP,
			ID_SETTING_SEASON_SUMMER_DISABLE_BMP,
		},
	},
	{
		SETTING_MAIN_ITEM_ID_BASIC,
		BUTTON_TYPE_PRESS,
		{379, 441, 271, 100},
		{
			ID_SETTING_BASIC_SETUP_NORMAL_BMP,
			ID_SETTING_BASIC_SETUP_FOCUS_BMP,
			ID_SETTING_BASIC_SETUP_DISABLE_BMP,
		},
	},
	{
		SETTING_MAIN_ITEM_ID_NETWORK,
		BUTTON_TYPE_PRESS,
		{69, 571, 271, 100},
		{
			ID_SETTING_NETWORK_SETUP_NORMAL_BMP,
			ID_SETTING_NETWORK_SETUP_FOCUS_BMP,
			ID_SETTING_NETWORK_SETUP_DISABLE_BMP,
		},
	},
	{
		SETTING_MAIN_ITEM_ID_UPGRADE,
		BUTTON_TYPE_PRESS,
		{379, 571, 271, 100},
		{
			ID_SETTING_DEV_UPGRADE_NORMAL_BMP,
			ID_SETTING_DEV_UPGRADE_FOCUS_BMP,
			ID_SETTING_DEV_UPGRADE_DISABLE_BMP,
		},
	},
};

static void setting_main_res_init()
{
	int i;

	create_bmp_res(setting_main_ui.pic_id, setting_main_ui.pic_res);
	for(i = 0; i < (sizeof(setting_main_bt_ui)/sizeof(setting_main_bt_ui[0])); i++) {
		create_bmp_res(setting_main_bt_ui[i].bmp_id[0], setting_main_bt_ui[i].bmp_res[0]);
		create_bmp_res(setting_main_bt_ui[i].bmp_id[1], setting_main_bt_ui[i].bmp_res[1]);
		create_bmp_res(setting_main_bt_ui[i].bmp_id[2], setting_main_bt_ui[i].bmp_res[2]);
	}
}

static void setting_main_res_deinit()
{
	int i;

	destroy_bmp_res(setting_main_ui.pic_res);
	for(i = 0; i < (sizeof(setting_main_bt_ui)/sizeof(setting_main_bt_ui[0])); i++) {
		destroy_bmp_res(setting_main_bt_ui[i].bmp_res[0]);
		destroy_bmp_res(setting_main_bt_ui[i].bmp_res[1]);
		destroy_bmp_res(setting_main_bt_ui[i].bmp_res[2]);
	}
}

static H_LYR setting_main_layer_create()
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

static __s32 setting_main_paint(setting_main_para_t *setting_main_para)
{
	GUI_LyrWinSel(setting_main_para->layer);
	GUI_BMP_RES_Draw(setting_main_ui.pic_res, setting_main_ui.rect_pic.x, setting_main_ui.rect_pic.y);
	return EPDK_OK;
}

static __s32 setting_main_proc(__gui_msg_t* msg)
{
	setting_ctrl_t *setting_ctrl;
	
	setting_ctrl = (setting_ctrl_t *)GUI_WinGetAttr(msg->h_deswin);

	switch(msg->id)
	{
		case GUI_MSG_CREATE:
		{
			setting_main_para_t *setting_main_para;

			setting_main_para = (setting_main_para_t *)esMEMS_Balloc(sizeof(setting_main_para_t));
			eLIBs_memset((void*)setting_main_para, 0, sizeof(setting_main_para_t));
			GUI_WinSetAddData(msg->h_deswin, (__u32)setting_main_para);

			setting_main_para->layer = setting_main_layer_create();
        	setting_main_res_init();
        	
        	GUI_LyrWinSetSta(setting_main_para->layer, GUI_LYRWIN_STA_SLEEP);
        	setting_main_paint(setting_main_para);
        	button_group_create(msg->h_deswin, \
				setting_main_para->layer, \
				setting_main_bt_ui, \
				setting_main_para->button, \
				sizeof(setting_main_bt_ui)/sizeof(setting_main_bt_ui[0]));
        	GUI_LyrWinSetSta(setting_main_para->layer, GUI_LYRWIN_STA_ON);
			GUI_LyrWinSetTop(setting_main_para->layer);
			break;
		}
			
		case GUI_MSG_DESTROY:
		{
			setting_main_para_t *setting_main_para;

			setting_main_para = (setting_main_para_t *)GUI_WinGetAddData(msg->h_deswin);
			
	        button_group_destroy(setting_main_para->button, \
				sizeof(setting_main_bt_ui)/sizeof(setting_main_bt_ui[0]));
        	setting_main_res_deinit();

        	if(setting_main_para->layer != NULL) {
				GUI_LyrWinSetSta(setting_main_para->layer, GUI_LYRWIN_STA_OFF);
				GUI_LyrWinDelete(setting_main_para->layer);
				setting_main_para->layer = NULL;
			}

			eLIBs_memset(setting_main_para, 0, sizeof(setting_main_para_t));
			esMEMS_Bfree(setting_main_para, sizeof(setting_main_para_t));
			break;
		}
		
		case GUI_MSG_CLOSE:
		{
			GUI_ManWinDelete(msg->h_deswin);
			break;
		}
			
		case GUI_MSG_COMMAND:
		{
			//eLIBs_printf("@@@@ setting_main item_id is %d, msgcode is %d\n", LOSWORD(msg->dwAddData1), HISWORD(msg->dwAddData1));
			switch(LOSWORD(msg->dwAddData1))
			{
	#if 1					
				case SETTING_MAIN_ITEM_ID_RETURN:
					if(HISWORD(msg->dwAddData1) == BN_CLICKED) {
						#if 0
						__gui_notify_msg_t		notify_msg;
						
						notify_msg.hWnd 	= msg->h_deswin;
						notify_msg.id		= GUI_WinGetItemId(msg->h_deswin);
						notify_msg.msgcode	= ITEM_MSG_CODE_RETURN;
						notify_msg.dwAddData = (__u32)&setting_main_para->date;
						notify_msg.dwReserved = (__u32)&setting_main_para->time;
						GUI_NotifyParent(&notify_msg);
						#endif
					}
					break;
					
				case SETTING_MAIN_ITEM_ID_BASIC:
					if(HISWORD(msg->dwAddData1) == BN_CLICKED) {
						__gui_notify_msg_t		notify_msg;
						
						notify_msg.hWnd 	= msg->h_deswin;
						notify_msg.id		= GUI_WinGetItemId(msg->h_deswin);
						notify_msg.msgcode	= ITEM_MSG_CLICKED;
						notify_msg.dwAddData = SETTING_WIN_ID_BASIC;
						notify_msg.dwReserved = 0;
						GUI_NotifyParent(&notify_msg);
					}
					break;
	#endif
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

H_WIN setting_main_create(H_WIN parent, __u16 id)
{
	__gui_manwincreate_para_t create_info;
	setting_ctrl_t *setting_ctrl;
	
	setting_ctrl = (setting_ctrl_t *)GUI_WinGetAttr(parent);
	
	eLIBs_memset(&create_info, 0, sizeof(__gui_manwincreate_para_t));
	
	create_info.name            = "SETTING_MAIN_WIN";
	create_info.hParent         = parent;
	create_info.ManWindowProc   = (__pGUI_WIN_CB)esKRNL_GetCallBack((__pCBK_t)setting_main_proc); 
	create_info.attr            = (void*)setting_ctrl;
	create_info.id				= id;
	create_info.hHosting        = NULL;

    return GUI_ManWinCreate(&create_info);
}
