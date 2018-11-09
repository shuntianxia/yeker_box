
#include "cp_i.h"
#include "scrolling_i.h"
//#include "setting.h"
//#include "settime.h"
#include "tongxin_demo.h"
#include "..\..\shell\bmp_parser\bmp.h"
#include "beetles_app.h"
#ifdef THIS_DEBUG
#undef THIS_DEBUG
#endif

#ifdef MW_PROJ_DEBUG
#define THIS_DEBUG
#endif

typedef struct {
    EWin_InheritInfo_ForHeader ewii;
    void * inertia;
    H_WIN hFocusWin; // Current child window
    H_WIN StandyWindow;
} HomeDescriptor;

static __bool standy_en = 0;

static __u32 seedtbl[]=
    { 16381163, 902867507, 703006358, 825601098, 702943101, 600362569, 824180876, 928050656,
     500242512, 600364801, 928023969, 680398149, 672661988, 825601527, 825601527, 825613410,
     823090756, 526599359,   8342805, 631233585, 302865607, 926952182, 232272027, 824145274,
     926933440, 919200125, 631206441, 631245671, 726297021, 726289272,  85829891, 631270516,
     726269366, 727066702, 726269355, 750013275, 726212604, 672729451,  85809036, 825660596,
     417735750,  18473775, 631290206};
static __u8         index = 0;
static __u32        ramdomcnt = 15;          /* used by ramdom function              */
static __u32        ramdomseed = 16;

static __u32 my_random(__u32 max)
{
    __u32 ret;

    if(index > sizeof(seedtbl)/sizeof(seedtbl[0]))
    {
        index = 0;
    }
    ramdomcnt = ramdomcnt * 29 + 314;

    ramdomseed += ramdomcnt;

    ret = ramdomcnt;

    ret = ret + seedtbl[index++];

    ret = ret - (ret / max) * max;

    return ret;
}

static __s32 test_read_nor_open(__s32* fsize, ES_FILE**f, unsigned char** p)
{
    __s32 ret;
    __s32 i, j;
    __s32 cur_data;
    __s32 page_num;
    __s32 once_count;    
    __s32 start_pos;
    __s32 file_size;
    unsigned char * pbuf;
    char path[]=BEETLES_APP_ROOT"apps\\nor_reading_test.bin";
    ES_FILE* fp;
   
    //open file
    fp = eLIBs_fopen(path, "rb");
    if(!fp)
    {        
        eLIBs_printf("open file fail, path=%s\n", path);
        return EPDK_FAIL;
    }

    //get file size
    eLIBs_fseek(fp, 0, SEEK_END);
    file_size = eLIBs_ftell(fp);
    eLIBs_printf("file_size=%d\n", file_size);

    //cal page size
    page_num = (file_size+1023)>>10;
    eLIBs_printf("page_num=%d\n", page_num);

    //alloc page mem
    pbuf = esMEMS_Palloc(page_num, 0);
    if(!pbuf)
    {        
        eLIBs_printf("esMEMS_Palloc fail\n");
        eLIBs_fclose(fp);
        return EPDK_FAIL;
    }

    *fsize = file_size;
    *f = fp;
    *p = pbuf;
    return EPDK_OK;
}

static __s32 test_read_nor_read(__s32 fsize, ES_FILE*f, unsigned char* p)
{
    __s32 i,j;
    __s32 start_pos,once_count; 
    __s32 file_size;
    __s32 cur_data;
    __s32 result;
    ES_FILE* fp;
    __s32 ret;
    unsigned char* pbuf;

    file_size = fsize;
    fp = f;
    pbuf = p;

    result = EPDK_TRUE;
    
    //begin test
    for(i = 0 ; i < 1; i++)
    {
        //generate start reading pos
        start_pos = my_random(file_size-2);                       
        
        //generate reading count
        once_count = my_random(file_size-start_pos);
               
        if(0 == once_count)
        {
            once_count = 1;
        }

        //seek and read
        
        eLIBs_fseek(fp, start_pos, SEEK_SET);
        
        ret = eLIBs_fread(pbuf, 1,once_count, fp);
        
        if(ret != once_count)
        {
            result = EPDK_FALSE;
            eLIBs_printf("eLIBs_fread fail, once_count=%d, ret=%d\n", once_count, ret);
            break;
        }

        //veryfy data
        cur_data = start_pos%256;
        for(j = 0 ; j < once_count ; j++)
        {
            if(cur_data != pbuf[j])
            {
                break;
            }
            cur_data++;
            if(cur_data >= 256)
            {
                cur_data = 0;
            }
        }

        if(j < once_count)//has err data
        {
            result = EPDK_FALSE;
            
            eLIBs_printf("***************err encounter, ****************\n");
            eLIBs_printf("test bad, i=%d, start_pos=%d, once_count=%d, cur_data=%d, pbuf[%d]=%d, err_pos=%d, (start_pos+j)/1024=%f\n"
                , i, start_pos, once_count, cur_data, j, pbuf[j], start_pos+j, (start_pos+j)/1024.0);

            if(0){//dump data
                __s32 k;
                eLIBs_printf("***************err encounter, begin dump****************\n");

                for(k = 0 ; k < once_count ; k++)
                {
                    eLIBs_printf("pbuf[%.6d]=%X\n", k, pbuf[k]);
                }
                eLIBs_printf("***************end dump****************\n");
            }

            __wait__;
            //break;
        }
        else//test alright
        {
            eLIBs_printf("test alright, i=%d, start_pos=%d, once_count=%d\n"
                , i, start_pos, once_count);
        }
        
        esKRNL_TimeDly(10);
    }
        
    return EPDK_OK;
}

__s32 test_file_exist(char *filename)
{
    ES_FILE* fp;
		
	fp = eLIBs_fopen(filename, "rb");
	if(fp == NULL)
	{
		return 0;
	} else {
		return 1;
	}
}

static HomeDescriptor * homeWndInit(__gui_msg_t * msg)
{
    HomeDescriptor * hdp;
    gg_handler domUIConfig;
    gg_handler node, properties;
    char ** attr;
    int i;
	gg_handler toiteNode;
	gg_char * langid;
	int langid_int;

    hdp = esMEMS_Malloc(0, sizeof(HomeDescriptor));
    if (NULL == hdp) {
        LogE("Malloc failed");
        return NULL;
    }
    eLIBs_memset(hdp, 0, sizeof(HomeDescriptor));
    media_player_main("e:\\liu.mp3");
    /* UI config */
    domUIConfig = (gg_handler)GUI_WinGetAttr(msg->h_deswin);
    node = GG_DOMGetRoot(domUIConfig);
    if (0 == eLIBs_strcmp(GG_DOMGetNodeName(node), "config")) {
        LogE("UI config XML root node ok");
    } else {
        LogE("UI config XML root node error");
        return NULL;
    }
	toiteNode = GG_DOMGetRoot(domUIConfig);
	toiteNode = GG_DOMGetFirstChild(toiteNode);
	toiteNode = GG_DOMGetBrotherElementByTagName(toiteNode, "langid");
	langid = GG_DOMGetNodeValue(toiteNode);

    // Set the volume
    dsk_volume_set(100);

    hdp->ewii.magic = EWIN_INFO_MAGIC;
    hdp->ewii.desktop = msg->h_deswin;
    hdp->ewii.hWnd = hdp->ewii.desktop;
    GUI_GetScreenSize((__s32 *)&hdp->ewii.desktopSize.width, (__s32 *)&hdp->ewii.desktopSize.height);
    GG_FillRect(&hdp->ewii.wndRect, 0, 0,
                    hdp->ewii.desktopSize.width, hdp->ewii.desktopSize.height);
    // Initiate the font module
    hdp->ewii.gdi.font = GUI_SFT_CreateFont(CP_SYS_FONT_SIZE, CP_SYS_FONT_PATH);
    hdp->ewii.gdi.fontSize = CP_SYS_FONT_SIZE;
    hdp->ewii.gestureArgs.htimer = hdp->ewii.desktop;
    hdp->ewii.gestureArgs.timerid = HOME_GESTURE_TIMER;
    hdp->inertia = GG_GestureOpen();
    /* UI config */
    properties = GG_DOMGetFirstChild(node);
    //node = GG_DOMGetBrotherElementByTagName(node, "window");
    if (properties) {
		hdp->ewii.langid = SLIB_atoi(langid);
		//LogMI("hdp->ewii.langid %d",hdp->ewii.langid);
        hdp->hFocusWin = SCR_ScrollingHostBuilder(&hdp->ewii, ScrollingCreate_V0001, properties);
    } else {
        LogE("No window node in XML file");
        return NULL;
    }

	/* ======> Lihan 20160127
		485默认为接收状态
  	*/
	Uart_485_init();
	/* <=============
	*/
	
    return hdp;
}

static __s32 app_scroll_proc(__gui_msg_t * msg)
{
	//static unsigned char updata_idx = 0;
	gg_handler domUIConfig;
    HomeDescriptor * hdp;
    H_WIN wndGesture;
    EWin_InheritInfo_ForHeader * extWinIfno;
	H_WIN hwnd;
    char name[20];
    hdp = (HomeDescriptor *)GUI_WinGetAddData(msg->h_deswin);
    if (NULL == hdp) {
        if (GUI_MSG_CREATE != msg->id) {
            LogE("Failed to get home window private date, message ID (%d)", msg->id);
            return GUI_ManWinDefaultProc(msg);
        }
    }
   
    switch (msg->id) {
    case GUI_MSG_CREATE:
		if(test_file_exist(CP_USER_UI_CONFIG)) {
			user_langres_init("e:\\res\\lang.bin");
	    	user_theme_init("e:\\res\\theme.bin");
	    	domUIConfig = GG_DOMCreate(CP_USER_UI_CONFIG);
		} else {
			user_langres_init("d:\\res\\lang.bin");
	    	user_theme_init("d:\\res\\theme.bin");
	    	domUIConfig = GG_DOMCreate(CP_DEFAULT_UI_CONFIG);
	    }
	    GUI_WinSetAttr(msg->h_deswin, domUIConfig);
        hdp = homeWndInit(msg);
        GG_DOMDestroy(domUIConfig);
        if (!hdp) {
            GUI_PostQuitMessage(msg->h_deswin);
            LogE("Create Home window failed");
            return -1;
        }
        GUI_WinSetAddData(msg->h_deswin, (__u32)hdp);
        GUI_WinSetFocusChild(hdp->hFocusWin);
        break;
    case GUI_MSG_DESTROY:
    	user_langres_exit();
    	user_theme_exit();
        GG_GestureClose(hdp->inertia);
        if (hdp->ewii.gdi.font) {
            GUI_SFT_ReleaseFont(hdp->ewii.gdi.font);
        }
        esMEMS_Mfree(0, hdp);
        break;
    case GUI_MSG_TOUCH:
#ifdef THIS_DEBUG
        //LogMI("Touch:%x,%d,%d", msg->dwAddData1, LOSWORD(msg->dwAddData2), HISWORD(msg->dwAddData2));
#endif  
		 if(LOSWORD(msg->dwAddData2) > 720 || HISWORD(msg->dwAddData2) > 720){
		 	break;
		 }
        if (GUI_MSG_TOUCH_DOWN == msg->dwAddData1) {
            wndGesture = EWin_GestureAttrCollect(GUI_WinGetFocusChild(msg->h_deswin), LOSWORD(msg->dwAddData2), HISWORD(msg->dwAddData2));
            if (wndGesture) {
                extWinIfno = GUI_WinGetAttr(wndGesture);
                GG_GestureOnDown(hdp->inertia, msg, LOSWORD(msg->dwAddData2), HISWORD(msg->dwAddData2), &extWinIfno->gestureArgs);
            }
        } else if (GUI_MSG_TOUCH_MOVE == msg->dwAddData1) {
            GG_GestureOnMove(hdp->inertia, msg, LOSWORD(msg->dwAddData2), HISWORD(msg->dwAddData2));
        } else if (GUI_MSG_TOUCH_UP == msg->dwAddData1) {
            GG_GestureOnUp(hdp->inertia, msg, LOSWORD(msg->dwAddData2), HISWORD(msg->dwAddData2));
		
		//updata_idx = 0;
		
        }else if(GUI_MSG_TOUCH_LONGDOWN == msg->dwAddData1){
            GG_GestureOnLpress(hdp->inertia, msg, LOSWORD(msg->dwAddData2), HISWORD(msg->dwAddData2));
		}
        break;
    case GUI_MSG_TIMER:
        if (HOME_GESTURE_TIMER == msg->dwAddData1) {
            GG_GestureOnTimer(hdp->inertia, msg);
        }
        break;

    default:
#ifdef THIS_DEBUG
//        LogMI("Unhandled home window message (%d)", msg->id);
#endif
		GUI_ManWinDefaultProc(msg);
        break;
    }
    return 0;
}

H_WIN app_scroll_create(root_para_t *para)
{
    H_WIN hWin;
    __gui_manwincreate_para_t createInfo;

    eLIBs_memset(&createInfo, 0, sizeof(__gui_manwincreate_para_t));

    createInfo.name             = APP_SCROLL;
    createInfo.hParent          = para->h_parent;
    createInfo.id				= APP_SCROLL_ID;
    createInfo.hHosting         = NULL;
    createInfo.ManWindowProc    = (__pGUI_WIN_CB)esKRNL_GetCallBack((__pCBK_t)app_scroll_proc);
    createInfo.attr             = para;
    hWin                        = GUI_ManWinCreate(&createInfo);

    if (NULL == hWin) {
        LogE("Create main window (%s) failed", createInfo.name);
        return NULL;
    }
#ifdef THIS_DEBUG
    LogMI("Hello main window (%s)", createInfo.name);
#endif
    return hWin;
}


