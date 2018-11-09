#ifndef __PRESS_BUTTON_H__
#define __PRESS_BUTTON_H__

#include "beetles_app.h"

typedef struct press_button {
	//SCR_ScrollingWndInfo_ForHeader scrolling;
	__s32 item_id;
	__s32 page_id;
	__s32 jump_enable;
	__s32 jump_page_id;
	__s32 jump_delay_ms;
	__s32 item_type;
	__s32 relate_to_group;
	__s32 status;
	__s32 bmp_id[2];
	load_bmp_res_t bmp_res[2];
	__s32 text_id[2];
	__s32 text_size;
	__u32 text_color;
	__s32 text_x;
	__s32 text_y;
	ButtonCode_handle * code_handle;  //按下数据保存
}press_button_t;

typedef struct push_button {
	//SCR_ScrollingWndInfo_ForHeader scrolling;
	__s32 item_id;
	__s32 page_id;
	__s32 item_type;
	__s32 status;
	__s32 bmp_id[3];
	load_bmp_res_t bmp_res[3];
	__s32 text_id[3];
	__s32 text_size;
	__u32 text_color;
	__s32 text_x;
	__s32 text_y;
	ButtonCode_handle * code_handle;  //按下数据保存
}push_button_t;

typedef struct textview {
	//SCR_ScrollingWndInfo_ForHeader scrolling;
	__s32 item_id;
	__s32 page_id;
	__s32 item_type;
	__s32 text_id;
	__s32 text_size;
	__u32 text_color;
	__s32 x;
	__s32 y;
	__s32 width;
	__s32 height;
	//__gui_rect_t rect;
}textview_t;

typedef struct imageview {
	//SCR_ScrollingWndInfo_ForHeader scrolling;
	__s32 item_id;
	__s32 page_id;
	__s32 item_type;
	int    imageview_num;                  //图片框 显示的图片编号
	int    imageview_tal;                  //图片框图片总数
	Image_handle * image_date_handle;      //图片框图片编号
}imageview_t;

typedef struct slider {
	//SCR_ScrollingWndInfo_ForHeader scrolling;
	__s32 item_id;
	__s32 page_id;
	__s32 item_type;
	int lampLimit;                         //滑动条 高度/ 宽度
	int lampLimit_max;                     //滑动条最大值
	int lampLimit_min;                     //滑动条最小值
	int lampLimit_height;                  //滑动条的滑块 高度/ 宽度
	int lampValue;                         //滑动条像素位置
	int old_lampValue;                     //滑动条上一次像素位置
	int lampSlide;
	int lampStep;
	int value;                             //滑动条值
	int old_value;						   //滑动条上一次的值
	int click_move;						   //受控移动
}slider_t;

__s32 press_button_proc(__gui_msg_t * msg);

#endif
