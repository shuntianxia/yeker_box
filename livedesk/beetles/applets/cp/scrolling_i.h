
#ifndef H_SCROLLING_I_BENNIS_20130131
#define H_SCROLLING_I_BENNIS_20130131
#include "tongxin_demo.h"
#include "image_demo.h"
#include "zu_code_demo.h"//矩阵组输入code保存的
#define INVALIDATE_NULL         0
#define INVALIDATE_ALL          ((unsigned int)-1)
#define INVALIDATE_BUTTON       0x00000001
#define INVALIDATE_SLIDER       0x00000002
#define INVALIDATE_BACKGROUND   0x00000004
#define INVALIDATE_FOREGROUND   0x00000008

typedef struct {
    EWin_InheritInfo_ForHeader ewii;
    RECT wndRect;           // Window location which is relative to the whole scrolling control
} SCR_ScrollingWndInfo_ForHeader;

typedef struct {
    SCR_ScrollingWndInfo_ForHeader scrolling;
} SCR_ScrollingMainWinDescriptor;

typedef struct {
    SCR_ScrollingWndInfo_ForHeader scrolling;
    gg_handler pageNode;
    HTHEME background;
	unsigned long color_code; 
    void * backgroundData;
} SCR_ScrollingPageDescriptor;

typedef struct {
    SCR_ScrollingWndInfo_ForHeader scrolling;
    gg_handler buttonNode;
	
	//GUI_FONT * button_font;
	
    //HTHEME buttonOn;
	int buttonOn_id;
    //void * buttonOnData;
	//void * hSlider;
    //HTHEME buttonOff;
	int buttonOff_id;
    //void * buttonOffData;
	HTHEME buttonMove;
	void * buttonMoveData;
	int    imageview_num;                  //图片框 显示的图片编号
	int    imageview_tal;                  //图片框图片总数
		
	char * xml_button_controltype;         //按键类型
	char * xml_id;                         //按键名字
	char * xml_click_id;						   //按键名字
	
	int carray_type;                       //0输出 1 输入
	gg_char * key_up_code;                 //矩阵组数据保存( in / out )
	
    int buttonStatus;                      //按键状态
	int buttonStatus_old;                  //按键上一次状态
	int buttonStatus_close;                // 1 不允许点击，0允许点击
	int invalidate;
    unsigned inva;	

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
	
	int word_x;                            //文字x坐标
	int word_y;                            //文字y坐标
	int word_size;                         //字体大小，目前用的是系统字体大小***************************
	int textview_chstr;                    //文本框文本id
	int word_id_on;                        //名字id 按下
	int word_id_off;                       //名字id 弹起
	
	int page;                              //跳转键跳转到哪页
	unsigned long color_code;              //颜色
	
	ButtonCode_handle * code_handle;  //按下数据保存
	Image_handle * image_date_handle;      //图片框图片编号

} SCR_ScrollingButtonDescriptor;

extern H_WIN ScrollingAppendManage(char * name, SCR_ScrollingWndInfo_ForHeader * inherit);
extern H_WIN ScrollingAppendPage(char * name, SCR_ScrollingWndInfo_ForHeader * inherit, int size, gg_handler pageNode, H_WIN hLayer, __pGUI_WIN_CB wndProc);
extern H_WIN ScrollingAppendButton(char * name, SCR_ScrollingWndInfo_ForHeader * inherit, gg_handler buttonNode, __pGUI_WIN_CB wndProc);

typedef H_WIN (*BuilderCreate)(SCR_ScrollingWndInfo_ForHeader * inherit, gg_handler pageNode);
extern H_WIN SCR_ScrollingHostBuilder(EWin_InheritInfo_ForHeader * inherit, BuilderCreate create, gg_handler windowNode);
extern H_WIN ScrollingCreate_V0001(SCR_ScrollingWndInfo_ForHeader * inherit, gg_handler pageNode);
extern H_WIN ScrollingBorderWndCreate_V0001(char * name, SCR_ScrollingWndInfo_ForHeader * inherit);
extern void Normal_Return_DongZuo(char * toitemid,char * toitemstatus,int imageid,int groupid,char * father_toitemid);
void Onlyone_Return_DongZuo(char * toitemid,int groupid);
void Click_Return_DongZuo(char * toitemid,int if_only,int groupid);
void Array_Return_DongZuo(char * toitemid,int groupid);
void Main_Button_Up(char * toitemid,int relation,int relation_slave);
#endif

