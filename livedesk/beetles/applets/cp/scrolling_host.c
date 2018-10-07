#include "cp_i.h"
#include "scrolling_i.h"
#include "clock.h"
#include "comindicator.h"
#include "xml_demo.h"//
#include "xml_haudongtiao_demo.h"// button	 h_deswin
#include "xml_onlyone_demo.h"//多选一
#include "xml_zu_demo.h"//普通组
#include "xml_array_demo.h"//矩阵组
#include "tongxin_demo.h"//通信
//#include <Esh_shell.h>

#ifdef THIS_DEBUG
#undef THIS_DEBUG
#endif
#ifdef MW_PROJ_DEBUG
#define THIS_DEBUG
#endif

#define ADDITIONAL_WINS     2

typedef struct {
    SCR_ScrollingWndInfo_ForHeader scrolling;
	EWin_InheritInfo_ForHeader *page;
    BuilderCreate create;
    gg_handler root;
	gg_handler window_root;
    int totalWins;
    int current;
	int geatrue_direction;
	int clock_en;
	int fore_current;
	int indicate_en;
	H_WIN indicatorWnd;
    H_WIN hWins[ADDITIONAL_WINS];// This member must be the last one
} ScrollingHostWinDescriptor;
//extern __bool shell_flag;
//extern __bool shell_flag;

static void Save_Zu_Date(gg_handler window_root)
{
	gg_handler window;
	int groupWins,i;
	int groupNum,j;
	int imageid;
	gg_handler toiteNode;
	gg_handler toiteNode1;
	gg_handler toiterelation;
	gg_handler toitecode;
	gg_char * toitemid;
	gg_char * relation;
	gg_char * toitemstatus;
	gg_char * grouptype;
	char * relation_test;
	char * xml_button_controltype;
	
	window = GG_DOMGetBrotherElementByTagName(window_root,"window");

	groupWins = GG_DOMGetElementsByTagName(window, "group");
	LogMI("groupWins = %d",groupWins);
	if(groupWins > 0){
		toiteNode = GG_DOMGetFirstChild(window);
		for(i = 0;i <= groupWins;i++){
			if(i == 0){
				toiteNode = GG_DOMGetBrotherElementByTagName(toiteNode, "group");
				toiterelation = GG_DOMGetFirstChild(toiteNode);
				grouptype = GG_DOMGetNodeValue(GG_DOMGetBrotherElementByTagName(toiterelation, "grouptype"));
				//LogMI("grouptype = %s",grouptype);
				toiterelation = GG_DOMGetBrotherElementByTagName(toiterelation, "groupid");
				groupNum = GG_DOMGetElementsByTagName(toiteNode, "relationship");
				//LogMI("groupNum = %d",groupNum);
				if(SLIB_strcmp(grouptype,"normal") == 0){
					for(j = 0;j < groupNum;j++){
						if(j == 0){
							toiterelation = GG_DOMGetFirstChild(toiteNode);
							toiterelation = GG_DOMGetBrotherElementByTagName(toiterelation, "relationship"); 
							toiteNode1 = GG_DOMGetFirstChild(toiterelation);
							toitemid = GG_DOMGetNodeValue(GG_DOMGetBrotherElementByTagName(toiteNode1, "toitemid"));
							relation_test = relation = GG_DOMGetNodeValue(GG_DOMGetBrotherElementByTagName(toiteNode1, "relation"));
							toitemstatus = GG_DOMGetNodeValue(GG_DOMGetBrotherElementByTagName(toiteNode1, "toitemstatus"));
							if(GG_DOMGetBrotherElementByTagName(toiteNode1, "imageid")){
							   imageid = SLIB_atoi(GG_DOMGetNodeValue(GG_DOMGetBrotherElementByTagName(toiteNode1, "imageid")))+1;
							   toitemstatus= "sync";
							}else{
							   imageid = -1;
							}
							if(SLIB_strcmp(relation_test,"primary") == 0){
							   XML_Primary_Handle_Date(i,relation,toitemid,toitemstatus);
							}else{
							   XML_Slave_Handle_Date(i,relation,toitemid,toitemstatus,imageid);
							}
						}else{
							toiterelation = GG_DOMGetNextBrother(toiterelation);
							toiteNode1 = GG_DOMGetFirstChild(toiterelation);
							toitemid = GG_DOMGetNodeValue(GG_DOMGetBrotherElementByTagName(toiteNode1, "toitemid"));
							relation_test = relation = GG_DOMGetNodeValue(GG_DOMGetBrotherElementByTagName(toiteNode1, "relation"));
							toitemstatus = GG_DOMGetNodeValue(GG_DOMGetBrotherElementByTagName(toiteNode1, "toitemstatus"));
							if(GG_DOMGetBrotherElementByTagName(toiteNode1, "imageid")){
							   imageid = SLIB_atoi(GG_DOMGetNodeValue(GG_DOMGetBrotherElementByTagName(toiteNode1, "imageid")))+1;
							   toitemstatus= "sync";
							}else{
							   imageid = -1;
							}
							if(SLIB_strcmp(relation_test,"primary") == 0){
							   XML_Primary_Handle_Date(i,relation,toitemid,toitemstatus);
							}else{
							   XML_Slave_Handle_Date(i,relation,toitemid,toitemstatus,imageid);
							}
						}
					}
				}else if(SLIB_strcmp(grouptype,"onlyone") == 0){
				
					for(j = 0;j < groupNum;j++){
						if(j == 0){
							toiterelation = GG_DOMGetFirstChild(toiteNode);
							toiterelation = GG_DOMGetBrotherElementByTagName(toiterelation, "relationship"); 
							toiteNode1 = GG_DOMGetFirstChild(toiterelation);
							toitemid = GG_DOMGetNodeValue(GG_DOMGetBrotherElementByTagName(toiteNode1, "toitemid"));
							relation_test = relation = GG_DOMGetNodeValue(GG_DOMGetBrotherElementByTagName(toiteNode1, "relation"));
							toitemstatus = GG_DOMGetNodeValue(GG_DOMGetBrotherElementByTagName(toiteNode1, "toitemstatus"));
							XML_Onlyone_Handle_Date(i,toitemid);
						}else{
							toiterelation = GG_DOMGetNextBrother(toiterelation);
							toiteNode1 = GG_DOMGetFirstChild(toiterelation);
							toitemid = GG_DOMGetNodeValue(GG_DOMGetBrotherElementByTagName(toiteNode1, "toitemid"));
							relation_test = relation = GG_DOMGetNodeValue(GG_DOMGetBrotherElementByTagName(toiteNode1, "relation"));
							toitemstatus = GG_DOMGetNodeValue(GG_DOMGetBrotherElementByTagName(toiteNode1, "toitemstatus"));
							XML_Onlyone_Handle_Date(i,toitemid);
						}
					}
				}else if(SLIB_strcmp(grouptype,"matrix") == 0){
					for(j = 0;j < groupNum;j++){
						if(j == 0){
							toiterelation = GG_DOMGetFirstChild(toiteNode);
							toiterelation = GG_DOMGetBrotherElementByTagName(toiterelation, "relationship"); 
							toiteNode1 = GG_DOMGetFirstChild(toiterelation);
							toitemid = GG_DOMGetNodeValue(GG_DOMGetBrotherElementByTagName(toiteNode1, "toitemid"));
							XML_Array_Handle_Date(i,toitemid);
							//LogMI("toitemid = %s",toitemid);
						}else{
							toiterelation = GG_DOMGetNextBrother(toiterelation);
							toiteNode1 = GG_DOMGetFirstChild(toiterelation);
							toitemid = GG_DOMGetNodeValue(GG_DOMGetBrotherElementByTagName(toiteNode1, "toitemid"));
							XML_Array_Handle_Date(i,toitemid);
							//LogMI("toitemid = %s",toitemid);
						}
					}
				}
			}else{//	!=0
				toiteNode = GG_DOMGetNextBrother(toiteNode);
				xml_button_controltype = GG_DOMGetNodeName(toiteNode);
				while(SLIB_strcmp(xml_button_controltype,"group") != 0){
					toiteNode = GG_DOMGetNextBrother(toiteNode);
					if(toiteNode){
						xml_button_controltype = GG_DOMGetNodeName(toiteNode);
					}else{
						LogMI("toiteNode1 = %s",GG_DOMGetNodeName(toiteNode));
						break;
					}
				}
				toiterelation = GG_DOMGetFirstChild(toiteNode);
				grouptype = GG_DOMGetNodeValue(GG_DOMGetBrotherElementByTagName(toiterelation, "grouptype"));
				//LogMI("grouptype = %s",grouptype);
				toiterelation = GG_DOMGetBrotherElementByTagName(toiterelation, "groupid");
				groupNum = GG_DOMGetElementsByTagName(toiteNode, "relationship");
				//LogMI("groupNum = %d",groupNum);
				if(SLIB_strcmp(grouptype,"normal") == 0){
					for(j = 0;j < groupNum;j++){
						if(j == 0){
							toiterelation = GG_DOMGetFirstChild(toiteNode);
							toiterelation = GG_DOMGetBrotherElementByTagName(toiterelation, "relationship"); 
							toiteNode1 = GG_DOMGetFirstChild(toiterelation);
							toitemid = GG_DOMGetNodeValue(GG_DOMGetBrotherElementByTagName(toiteNode1, "toitemid"));
							relation_test = relation = GG_DOMGetNodeValue(GG_DOMGetBrotherElementByTagName(toiteNode1, "relation"));
							toitemstatus = GG_DOMGetNodeValue(GG_DOMGetBrotherElementByTagName(toiteNode1, "toitemstatus"));
							if(GG_DOMGetBrotherElementByTagName(toiteNode1, "imageid")){
							   imageid = SLIB_atoi(GG_DOMGetNodeValue(GG_DOMGetBrotherElementByTagName(toiteNode1, "imageid")))+1;
							   toitemstatus= "sync";
							}else{
							   imageid = -1;
							}
							if(SLIB_strcmp(relation_test,"primary") == 0){
							   XML_Primary_Handle_Date(i,relation,toitemid,toitemstatus);
							}else{
							   XML_Slave_Handle_Date(i,relation,toitemid,toitemstatus,imageid);
							}
							/*toitecode = GG_DOMGetBrotherElementByTagName(toiteNode1, "item_code"); 
							toitecode = GG_DOMGetFirstChild(toitecode);
							toitemid = GG_DOMGetNodeValue(GG_DOMGetBrotherElementByTagName(toitecode, "item_code_datas"));
							if(GG_DOMGetBrotherElementByTagName(toitecode, "item_code_datas")){
							   LogMI("toitemid = %s",toitemid);
							}*/
						}else{
							toiterelation = GG_DOMGetNextBrother(toiterelation);
							toiteNode1 = GG_DOMGetBrotherElementByTagName(toiterelation, "relationship"); 
							toiteNode1 = GG_DOMGetFirstChild(toiteNode1);
							toitemid = GG_DOMGetNodeValue(GG_DOMGetBrotherElementByTagName(toiteNode1, "toitemid"));
							relation_test = relation = GG_DOMGetNodeValue(GG_DOMGetBrotherElementByTagName(toiteNode1, "relation"));
							toitemstatus = GG_DOMGetNodeValue(GG_DOMGetBrotherElementByTagName(toiteNode1, "toitemstatus"));
							if(GG_DOMGetBrotherElementByTagName(toiteNode1, "imageid")){
							   imageid = SLIB_atoi(GG_DOMGetNodeValue(GG_DOMGetBrotherElementByTagName(toiteNode1, "imageid")))+1;
							   toitemstatus= "sync";
							}else{
							   imageid = -1;
							}
							if(SLIB_strcmp(relation_test,"primary") == 0){
							   XML_Primary_Handle_Date(i,relation,toitemid,toitemstatus);
							}else{
							   XML_Slave_Handle_Date(i,relation,toitemid,toitemstatus,imageid);
							}
							/*toitecode = GG_DOMGetBrotherElementByTagName(toiteNode1, "item_code"); 
							toitecode = GG_DOMGetFirstChild(toitecode);
							toitemid = GG_DOMGetNodeValue(GG_DOMGetBrotherElementByTagName(toitecode, "item_code_datas"));
							if(GG_DOMGetBrotherElementByTagName(toitecode, "item_code_datas")){
							   LogMI("toitemid = %s",toitemid);
							}*/
						 }
					  }
				 }else if(SLIB_strcmp(grouptype,"onlyone") == 0){
					for(j = 0;j < groupNum;j++){
						if(j == 0){
							toiterelation = GG_DOMGetFirstChild(toiteNode);
							toiterelation = GG_DOMGetBrotherElementByTagName(toiterelation, "relationship"); 
							toiteNode1 = GG_DOMGetFirstChild(toiterelation);
							toitemid = GG_DOMGetNodeValue(GG_DOMGetBrotherElementByTagName(toiteNode1, "toitemid"));
							relation_test = relation = GG_DOMGetNodeValue(GG_DOMGetBrotherElementByTagName(toiteNode1, "relation"));
							toitemstatus = GG_DOMGetNodeValue(GG_DOMGetBrotherElementByTagName(toiteNode1, "toitemstatus"));
							XML_Onlyone_Handle_Date(i,toitemid);
						}else{
							toiterelation = GG_DOMGetNextBrother(toiterelation);
							toiteNode1 = GG_DOMGetFirstChild(toiterelation);
							toitemid = GG_DOMGetNodeValue(GG_DOMGetBrotherElementByTagName(toiteNode1, "toitemid"));
							relation_test = relation = GG_DOMGetNodeValue(GG_DOMGetBrotherElementByTagName(toiteNode1, "relation"));
							toitemstatus = GG_DOMGetNodeValue(GG_DOMGetBrotherElementByTagName(toiteNode1, "toitemstatus"));
							XML_Onlyone_Handle_Date(i,toitemid);
						}
					}
				}else if(SLIB_strcmp(grouptype,"matrix") == 0){
					for(j = 0;j < groupNum;j++){
						if(j == 0){
							toiterelation = GG_DOMGetFirstChild(toiteNode);
							toiterelation = GG_DOMGetBrotherElementByTagName(toiterelation, "relationship"); 
							toiteNode1 = GG_DOMGetFirstChild(toiterelation);
							toitemid = GG_DOMGetNodeValue(GG_DOMGetBrotherElementByTagName(toiteNode1, "toitemid"));
							XML_Array_Handle_Date(i,toitemid);
							//LogMI("toitemid = %s",toitemid);
						}else{
							toiterelation = GG_DOMGetNextBrother(toiterelation);
							toiteNode1 = GG_DOMGetFirstChild(toiterelation);
							toitemid = GG_DOMGetNodeValue(GG_DOMGetBrotherElementByTagName(toiteNode1, "toitemid"));
							XML_Array_Handle_Date(i,toitemid);
							//LogMI("toitemid = %s",toitemid);
						}
					}
				}
			}
		}
	}

}

static __s32 scrollingHostWndProc(__gui_msg_t * msg)
{
    ScrollingHostWinDescriptor * wndDpt;
    RECT source;
	RECT screen;
    int i;
    gg_handler page;
    char *name;
	
	char * xml_button_controltype;
    wndDpt = (ScrollingHostWinDescriptor *)GUI_WinGetAttr(msg->h_deswin);
    if ((!wndDpt) || (-1 == EWin_CheckInherit(&wndDpt->scrolling.ewii)) ||
        (!(wndDpt->scrolling.ewii.mask[0] & SCR_WNDINFOMASK_SHOST))) {
        LogE("Null or wrong scrolling host window private data");
        return -1;
    }

    switch (msg->id) {
    case GUI_MSG_CREATE:
		Save_Zu_Date(wndDpt->window_root);
		page = GG_DOMGetFirstChild(wndDpt->root);
		if(page){
			gg_handler lang_i = NULL;
			int lang;
			int baudrate;
			lang_i = GG_DOMGetBrotherElementByTagName(page, "langid");
			lang = SLIB_atoi(GG_DOMGetNodeValue(lang_i));
			if(lang){
				dsk_langres_set_type(EPDK_LANGUAGE_ENM_ENGLISH);
			}else{
				dsk_langres_set_type(EPDK_LANGUAGE_ENM_CHINESES);
			}
			//__msg("lanid=%d\n",lang);
			esDEV_Plugin("\\drv\\uart.drv", 2, (void *)9600, 0);
			/*if(inter_face){
				first_child = GG_DOMGetFirstChild(inter_face);
				if(first_child){
	                baudrate = SLIB_atoi(GG_DOMGetNodeValue(GG_DOMGetBrotherElementByTagName(first_child, "baudrate")));
					esDEV_Plugin("\\drv\\uart.drv", 1, (void *)baudrate, 0);
				}
			}*/
			name = GG_DOMGetNodeValue(GG_DOMGetBrotherElementByTagName(page, "suspend"));
			//__msg("name==%s\n",name);
			if(eLIBs_strcmp(name, "shutdown")==0){
                 wndDpt->page->suspend = 1;
			}else if(0 == eLIBs_strcmp(name, "clock")){
                 wndDpt->page->suspend = 2; 
			}else if(eLIBs_strcmp(name,"picture")==0){
                 wndDpt->page->suspend = 3; 
			}else{
                 wndDpt->page->suspend = 0;
			}
			//__msg("suspend==%d",wndDpt->scrolling.ewii.suspend);
			name = GG_DOMGetNodeValue(GG_DOMGetBrotherElementByTagName(page, "gesturedirection"));
			
			if(0 == eLIBs_strcmp(name,"leftright")){
                 //__msg("leftright");
				 baudrate = SLIB_atoi(GG_DOMGetNodeValue(GG_DOMGetBrotherElementByTagName(page, "pageslider")));
				 //__msg("len==%d",baudrate);
				 wndDpt->geatrue_direction = 1;
				 wndDpt->scrolling.ewii.gestureArgs.attr_x = GG_GESTURE_ATTR_LPRESS | GG_GESTURE_ATTR_INER_SBLOCK;
                 wndDpt->scrolling.ewii.gestureArgs.attr_y = 0;
                 wndDpt->scrolling.ewii.gestureArgs.hmsg_x[GG_GESTURE_MSG_DEST_LPRESS] = msg->h_deswin;
                 wndDpt->scrolling.ewii.gestureArgs.hmsg_x[GG_GESTURE_MSG_DEST_INER_SBLOCK] = msg->h_deswin;
                 wndDpt->scrolling.ewii.gestureArgs.steps_x = wndDpt->scrolling.ewii.wndRect.width;
                 wndDpt->scrolling.ewii.gestureArgs.steps_y = 0;
				 wndDpt->scrolling.ewii.mask[0] |= SCR_WNDINFOMASK_GESTURE;
		         wndDpt->scrolling.ewii.gestureArgs.start_x = 0;
		         wndDpt->scrolling.ewii.gestureArgs.start_y = 0;
				 
		         wndDpt->scrolling.ewii.gestureArgs.headboundary = 0;
		         wndDpt->scrolling.ewii.gestureArgs.tailboundary = 0;
		         wndDpt->scrolling.ewii.hWnd = msg->h_deswin;
				 if(baudrate){
				 	 MwIndicatorWndArgs args;
					 wndDpt->indicate_en = 1;
					 GG_CopyRect(&wndDpt->scrolling.wndRect, &wndDpt->scrolling.ewii.wndRect);
	                 GG_FillRect(&source, 0, 0, wndDpt->scrolling.wndRect.width, wndDpt->scrolling.wndRect.height);
					 GG_FillSize(&wndDpt->scrolling.ewii.lyrLowSize, wndDpt->scrolling.wndRect.width * (wndDpt->totalWins + ADDITIONAL_WINS), wndDpt->scrolling.wndRect.height);
	                 wndDpt->scrolling.ewii.lyrLow = EWin_LayerCreate("ScrollingLayerLow", &wndDpt->scrolling.ewii.lyrLowSize, &source, &source, 0);
	                 //GG_CopySize(&wndDpt->scrolling.ewii.lyrHigherSize, &wndDpt->scrolling.ewii.lyrLowSize);
	                 GG_FillRect(&source, 0, 0, wndDpt->scrolling.wndRect.width, wndDpt->scrolling.wndRect.height);
	                 GG_FillSize(&wndDpt->scrolling.ewii.lyrHigherSize, wndDpt->scrolling.wndRect.width * (wndDpt->totalWins + ADDITIONAL_WINS), wndDpt->scrolling.wndRect.height);
					 wndDpt->scrolling.ewii.lyrHigher = EWin_LayerCreate("ScrollingLayerHigher", &wndDpt->scrolling.ewii.lyrHigherSize, &source, &source, 1);
					 //GUI_LyrWinSetScnWindow(wndDpt->scrolling.ewii.lyrLow, &source);
					 GG_FillRect(&source, 0, wndDpt->scrolling.wndRect.height - baudrate, wndDpt->scrolling.wndRect.width, baudrate);
					 GG_FillRect(&screen, 0, 0, wndDpt->scrolling.wndRect.width, baudrate);
					 GG_FillSize(&wndDpt->scrolling.ewii.inilyrHigherSize, wndDpt->scrolling.wndRect.width, baudrate);
					 wndDpt->page->inilyrHigher = EWin_LayerCreate("indicatewindow", &wndDpt->scrolling.ewii.inilyrHigherSize, &source, &screen, 1);
					 args.lyrTop = wndDpt->page->inilyrHigher;
				     MwFillRect(&args.wndRect, 0, 0, wndDpt->scrolling.wndRect.width, baudrate);
				     args.winNr = wndDpt->totalWins;
				     wndDpt->indicatorWnd = MwIndicatorWndCreate("WindowIndicator", msg->h_deswin, &args);
				     MwIndicatorWndUpdate(wndDpt->page->inilyrHigher,0);
					 GG_FillRect(&source, 0, 0, wndDpt->scrolling.wndRect.width, wndDpt->scrolling.wndRect.height - baudrate);
					 GUI_LyrWinSetScnWindow(wndDpt->scrolling.ewii.lyrHigher,&source);
					 GUI_LyrWinSetScnWindow(wndDpt->scrolling.ewii.lyrLow,&source);
				}else{
                 GG_CopyRect(&wndDpt->scrolling.wndRect, &wndDpt->scrolling.ewii.wndRect);
                 GG_FillRect(&source, 0, 0, wndDpt->scrolling.wndRect.width, wndDpt->scrolling.wndRect.height);
				 GG_FillSize(&wndDpt->scrolling.ewii.lyrLowSize, wndDpt->scrolling.wndRect.width * (wndDpt->totalWins + ADDITIONAL_WINS), wndDpt->scrolling.wndRect.height);
                 wndDpt->scrolling.ewii.lyrLow = EWin_LayerCreate("ScrollingLayerLow", &wndDpt->scrolling.ewii.lyrLowSize, &source, &source, 0);
                 GG_CopySize(&wndDpt->scrolling.ewii.lyrHigherSize, &wndDpt->scrolling.ewii.lyrLowSize);
                 wndDpt->scrolling.ewii.lyrHigher = EWin_LayerCreate("ScrollingLayerHigher", &wndDpt->scrolling.ewii.lyrHigherSize, &source, &source, 1);
				}
				 wndDpt->hWins[0] = ScrollingBorderWndCreate_V0001("LeftBorderPage", &wndDpt->scrolling);
		         page = GG_DOMGetFirstChild(GG_DOMGetBrotherElementByTagName(wndDpt->root, "window"));
				 if(wndDpt->clock_en){
                    i = 2;
					wndDpt->scrolling.wndRect.x = wndDpt->scrolling.wndRect.width;
		            wndDpt->scrolling.ewii.gestureArgs.start_x = wndDpt->scrolling.wndRect.x;
					wndDpt->scrolling.ewii.gestureArgs.headboundary = 1;
		            wndDpt->scrolling.ewii.gestureArgs.tailboundary = 0;
					wndDpt->hWins[1] = DrawClockCreate("clock", &wndDpt->scrolling);
				 }else{
                    i = 1;
					esKRNL_TimeDly(50);
				 }
		         for (i; NULL != page; i++) {//GG_DOMGetBrotherElementByTagName(page, "page"))//song
		            wndDpt->scrolling.wndRect.x = i * wndDpt->scrolling.wndRect.width;
		            wndDpt->scrolling.ewii.gestureArgs.start_x = wndDpt->scrolling.wndRect.x;
		            if (1 == i) {
		                if(wndDpt->totalWins == 1){
							wndDpt->scrolling.ewii.gestureArgs.headboundary = 1;
			                wndDpt->scrolling.ewii.gestureArgs.tailboundary = 1;
						}else{
			                wndDpt->scrolling.ewii.gestureArgs.headboundary = 1;
			                wndDpt->scrolling.ewii.gestureArgs.tailboundary = 0;
						}
		            } else if (wndDpt->totalWins == i) {
		                wndDpt->scrolling.ewii.gestureArgs.headboundary = 0;
		                wndDpt->scrolling.ewii.gestureArgs.tailboundary = 1;
		            }else{
                        wndDpt->scrolling.ewii.gestureArgs.headboundary = 0;
		                wndDpt->scrolling.ewii.gestureArgs.tailboundary = 0;
					}
		            wndDpt->hWins[i] = (*wndDpt->create)(&wndDpt->scrolling, page);
					page = GG_DOMGetNextBrother(page);
					xml_button_controltype = GG_DOMGetNodeName(page);
					while(SLIB_strcmp(xml_button_controltype,"page") != 0){
						page = GG_DOMGetNextBrother(page);
						if(page){
							xml_button_controltype = GG_DOMGetNodeName(page);
						}else{
							break;
						}
					}
		         }
		         wndDpt->scrolling.wndRect.x = i * wndDpt->scrolling.wndRect.width;
		         wndDpt->scrolling.ewii.gestureArgs.start_x = wndDpt->scrolling.wndRect.x;
		         wndDpt->scrolling.ewii.gestureArgs.headboundary = 0;
		         wndDpt->scrolling.ewii.gestureArgs.tailboundary = 0;
		         wndDpt->hWins[i] = ScrollingBorderWndCreate_V0001("RightBorderPage", &wndDpt->scrolling);
		         if (i != (wndDpt->totalWins + 1)) {
		            LogE("Scrolling pages wrong (%d:%d)", i, wndDpt->totalWins);
		            return -1;
		         }
		         wndDpt->current = 1;
		         wndDpt->scrolling.wndRect.x = wndDpt->current * wndDpt->scrolling.wndRect.width;
		         wndDpt->scrolling.ewii.gestureArgs.start_x = wndDpt->scrolling.wndRect.x;
			}else if(0 == eLIBs_strcmp(name,"updown")){
                //__msg("updown");
				 wndDpt->geatrue_direction = 2;
				 wndDpt->scrolling.ewii.gestureArgs.attr_x = GG_GESTURE_ATTR_LPRESS;
                 wndDpt->scrolling.ewii.gestureArgs.attr_y = GG_GESTURE_ATTR_INER_SBLOCK;
                 wndDpt->scrolling.ewii.gestureArgs.hmsg_x[GG_GESTURE_MSG_DEST_LPRESS] = msg->h_deswin;
                 wndDpt->scrolling.ewii.gestureArgs.hmsg_y[GG_GESTURE_MSG_DEST_INER_SBLOCK] = msg->h_deswin;
                 wndDpt->scrolling.ewii.gestureArgs.steps_x = 0;
                 wndDpt->scrolling.ewii.gestureArgs.steps_y = wndDpt->scrolling.ewii.wndRect.height;
				 wndDpt->scrolling.ewii.mask[0] |= SCR_WNDINFOMASK_GESTURE;
		         wndDpt->scrolling.ewii.gestureArgs.start_x = 0;
		         wndDpt->scrolling.ewii.gestureArgs.start_y = 0;
		         wndDpt->scrolling.ewii.gestureArgs.headboundary = 0;
		         wndDpt->scrolling.ewii.gestureArgs.tailboundary = 0;
		         wndDpt->scrolling.ewii.hWnd = msg->h_deswin;
				 GG_CopyRect(&wndDpt->scrolling.wndRect, &wndDpt->scrolling.ewii.wndRect);
                 GG_FillRect(&source, 0, 0, wndDpt->scrolling.wndRect.width, wndDpt->scrolling.wndRect.height);
				 GG_FillSize(&wndDpt->scrolling.ewii.lyrLowSize, wndDpt->scrolling.wndRect.width,wndDpt->scrolling.wndRect.height *(wndDpt->totalWins + ADDITIONAL_WINS));
                 wndDpt->scrolling.ewii.lyrLow = EWin_LayerCreate("ScrollingLayerLow", &wndDpt->scrolling.ewii.lyrLowSize, &source, &source, 0);
                 GG_CopySize(&wndDpt->scrolling.ewii.lyrHigherSize, &wndDpt->scrolling.ewii.lyrLowSize);
                 wndDpt->scrolling.ewii.lyrHigher = EWin_LayerCreate("ScrollingLayerHigher", &wndDpt->scrolling.ewii.lyrHigherSize, &source, &source, 1);
				 wndDpt->hWins[0] = ScrollingBorderWndCreate_V0001("LeftBorderPage", &wndDpt->scrolling);
				 if(wndDpt->clock_en){
                    i = 2;
					wndDpt->scrolling.wndRect.y = wndDpt->scrolling.wndRect.height;
		            wndDpt->scrolling.ewii.gestureArgs.start_y = wndDpt->scrolling.wndRect.y;
					wndDpt->scrolling.ewii.gestureArgs.headboundary = 1;
		            wndDpt->scrolling.ewii.gestureArgs.tailboundary = 0;
					wndDpt->hWins[1] = DrawClockCreate("clock",&wndDpt->scrolling);
				 }else{
                    i = 1;
					esKRNL_TimeDly(50);
				 }
				 page = GG_DOMGetFirstChild(GG_DOMGetBrotherElementByTagName(wndDpt->root, "window"));
		         for (i; NULL != page; i++) {//GG_DOMGetBrotherElementByTagName(page, "page"))//song
		            wndDpt->scrolling.wndRect.y = i * wndDpt->scrolling.wndRect.height;
		            wndDpt->scrolling.ewii.gestureArgs.start_y = wndDpt->scrolling.wndRect.y;
		            if (1 == i) {
						if(wndDpt->totalWins == 1){
							wndDpt->scrolling.ewii.gestureArgs.headboundary = 1;
			                wndDpt->scrolling.ewii.gestureArgs.tailboundary = 1;
						}else{
			                wndDpt->scrolling.ewii.gestureArgs.headboundary = 1;
			                wndDpt->scrolling.ewii.gestureArgs.tailboundary = 0;
						}
		            } else if (wndDpt->totalWins == i) {
		                wndDpt->scrolling.ewii.gestureArgs.headboundary = 0;
		                wndDpt->scrolling.ewii.gestureArgs.tailboundary = 1;
		            }else{
                        wndDpt->scrolling.ewii.gestureArgs.headboundary = 0;
		                wndDpt->scrolling.ewii.gestureArgs.tailboundary = 0;
					}
		            wndDpt->hWins[i] = (*wndDpt->create)(&wndDpt->scrolling, page);
					page = GG_DOMGetNextBrother(page);
					xml_button_controltype = GG_DOMGetNodeName(page);
					while(SLIB_strcmp(xml_button_controltype,"page") != 0){
						page = GG_DOMGetNextBrother(page);
						if(page){
						    xml_button_controltype = GG_DOMGetNodeName(page);
						}else{
						    break;
						}
					}
		         }
		         wndDpt->scrolling.wndRect.y = i * wndDpt->scrolling.wndRect.height;
		         wndDpt->scrolling.ewii.gestureArgs.start_y = wndDpt->scrolling.wndRect.y;
		         wndDpt->scrolling.ewii.gestureArgs.headboundary = 0;
		         wndDpt->scrolling.ewii.gestureArgs.tailboundary = 0;
		         wndDpt->hWins[i] = ScrollingBorderWndCreate_V0001("RightBorderPage", &wndDpt->scrolling);
		         if (i != (wndDpt->totalWins + 1)) {
		            LogE("Scrolling pages wrong (%d:%d)", i, wndDpt->totalWins);
		            return -1;
		         }
		         wndDpt->current = 1;
		         wndDpt->scrolling.wndRect.y = wndDpt->current * wndDpt->scrolling.wndRect.height;
		         wndDpt->scrolling.ewii.gestureArgs.start_y = wndDpt->scrolling.wndRect.y;
			}else{
				 //__msg("close");
				 wndDpt->geatrue_direction = 0;
			     wndDpt->scrolling.ewii.gestureArgs.attr_x = GG_GESTURE_ATTR_LPRESS;
		         wndDpt->scrolling.ewii.gestureArgs.attr_y = 0;
		         wndDpt->scrolling.ewii.gestureArgs.hmsg_x[GG_GESTURE_MSG_DEST_LPRESS] = msg->h_deswin;
		         //wndDpt->scrolling.ewii.gestureArgs.hmsg_x[GG_GESTURE_MSG_DEST_INER_SBLOCK] = msg->h_deswin;
		         wndDpt->scrolling.ewii.gestureArgs.steps_x = 0;
		         wndDpt->scrolling.ewii.gestureArgs.steps_y = 0;
			     wndDpt->scrolling.ewii.mask[0] |= SCR_WNDINFOMASK_GESTURE;
                 wndDpt->scrolling.ewii.gestureArgs.start_x = 0;
                 wndDpt->scrolling.ewii.gestureArgs.start_y = 0;
                 wndDpt->scrolling.ewii.gestureArgs.headboundary = 0;
                 wndDpt->scrolling.ewii.gestureArgs.tailboundary = 0;
                 wndDpt->scrolling.ewii.hWnd = msg->h_deswin;
			     GG_CopyRect(&wndDpt->scrolling.wndRect, &wndDpt->scrolling.ewii.wndRect);
		         GG_FillRect(&source, 0, 0, wndDpt->scrolling.wndRect.width, wndDpt->scrolling.wndRect.height);
		         GG_FillSize(&wndDpt->scrolling.ewii.lyrLowSize, wndDpt->scrolling.wndRect.width * (wndDpt->totalWins + ADDITIONAL_WINS), wndDpt->scrolling.wndRect.height);
		         wndDpt->scrolling.ewii.lyrLow = EWin_LayerCreate("ScrollingLayerLow", &wndDpt->scrolling.ewii.lyrLowSize, &source, &source, 0);
		         GG_CopySize(&wndDpt->scrolling.ewii.lyrHigherSize, &wndDpt->scrolling.ewii.lyrLowSize);
		         wndDpt->scrolling.ewii.lyrHigher = EWin_LayerCreate("ScrollingLayerHigher", &wndDpt->scrolling.ewii.lyrHigherSize, &source, &source, 1);
				 wndDpt->hWins[0] = ScrollingBorderWndCreate_V0001("LeftBorderPage", &wndDpt->scrolling);
		         page = GG_DOMGetFirstChild(GG_DOMGetBrotherElementByTagName(wndDpt->root, "window"));
				 if(wndDpt->clock_en){
                    i = 2;
					wndDpt->scrolling.wndRect.x = wndDpt->scrolling.wndRect.width;
		            wndDpt->scrolling.ewii.gestureArgs.start_x = wndDpt->scrolling.wndRect.x;
					wndDpt->scrolling.ewii.gestureArgs.headboundary = 1;
		            wndDpt->scrolling.ewii.gestureArgs.tailboundary = 0;
					wndDpt->hWins[1] = DrawClockCreate("clock", &wndDpt->scrolling);
				 }else{
                    i = 1;
					esKRNL_TimeDly(50);
				 }
		         for (i; NULL != page; i++) {//GG_DOMGetBrotherElementByTagName(page, "page"))//song
		            wndDpt->scrolling.wndRect.x = i * wndDpt->scrolling.wndRect.width;
		            wndDpt->scrolling.ewii.gestureArgs.start_x = wndDpt->scrolling.wndRect.x;
		            if (1 == i) {
		                wndDpt->scrolling.ewii.gestureArgs.headboundary = 1;
		                wndDpt->scrolling.ewii.gestureArgs.tailboundary = 0;
		            } else if (wndDpt->totalWins == i) {
		                wndDpt->scrolling.ewii.gestureArgs.headboundary = 0;
		                wndDpt->scrolling.ewii.gestureArgs.tailboundary = 1;
		            }
		            wndDpt->hWins[i] = (*wndDpt->create)(&wndDpt->scrolling, page);
					page = GG_DOMGetNextBrother(page);
					xml_button_controltype = GG_DOMGetNodeName(page);
					while(SLIB_strcmp(xml_button_controltype,"page") != 0){
						page = GG_DOMGetNextBrother(page);
						if(page){
						    xml_button_controltype = GG_DOMGetNodeName(page);
						}else{
						    break;
						}
					}
		         }
		         wndDpt->scrolling.wndRect.x = i * wndDpt->scrolling.wndRect.width;
		         wndDpt->scrolling.ewii.gestureArgs.start_x = wndDpt->scrolling.wndRect.x;
		         wndDpt->scrolling.ewii.gestureArgs.headboundary = 0;
		         wndDpt->scrolling.ewii.gestureArgs.tailboundary = 0;
		         wndDpt->hWins[i] = ScrollingBorderWndCreate_V0001("RightBorderPage", &wndDpt->scrolling);
		         if (i != (wndDpt->totalWins + 1)) {
		            LogE("Scrolling pages wrong (%d:%d)", i, wndDpt->totalWins);
		            return -1;
		         }
		         wndDpt->current = 1;
		         wndDpt->scrolling.wndRect.x = wndDpt->current * wndDpt->scrolling.wndRect.width;
		         wndDpt->scrolling.ewii.gestureArgs.start_x = wndDpt->scrolling.wndRect.x;
			}
			
		}else{
             //__msg("page failure");
			 return 0;
		}
        wndDpt->page->lyrLow = wndDpt->scrolling.ewii.lyrLow ;
		wndDpt->page->lyrHigher= wndDpt->scrolling.ewii.lyrHigher ;
        // wndDpt->scrolling.ewii.wndRect, don't set, inherit from parent window
        GUI_LyrWinSetSrcWindow(wndDpt->scrolling.ewii.lyrLow, &wndDpt->scrolling.wndRect);
        GUI_LyrWinSetSrcWindow(wndDpt->scrolling.ewii.lyrHigher, &wndDpt->scrolling.wndRect);
        GUI_WinSetFocusChild(wndDpt->hWins[wndDpt->current]);
		//shell_flag = 1;
		GUI_LyrWinSetSta(wndDpt->scrolling.ewii.lyrLow, GUI_LYRWIN_STA_ON);
        GUI_LyrWinSetSta(wndDpt->scrolling.ewii.lyrHigher, GUI_LYRWIN_STA_ON);
		GUI_LyrWinSetSta(wndDpt->page->inilyrHigher, GUI_LYRWIN_STA_ON);
		wndDpt->root = GG_NULL;
		wndDpt->window_root = GG_NULL;
        break;
    case GUI_MSG_DESTROY:
        if (wndDpt->scrolling.ewii.lyrHigher) {
            GUI_LyrWinDelete(wndDpt->scrolling.ewii.lyrHigher);
        }
        if (wndDpt->scrolling.ewii.lyrLow) {
            GUI_LyrWinDelete(wndDpt->scrolling.ewii.lyrLow);
        }
		if(wndDpt->scrolling.ewii.inilyrHigher){
            GUI_LyrWinDelete(wndDpt->scrolling.ewii.inilyrHigher);
		}
        esMEMS_Mfree(0, wndDpt);
        break;
    case GUI_MSG_USER_GESTURE_MOVE:
#ifdef THIS_DEBUG
        //LogMI("Move direction: %d, distance: %d", HISWORD(msg->dwAddData2), LOSWORD(msg->dwAddData2));
#endif
        //__msg("gestrue = %d",wndDpt->geatrue_direction);
        if(wndDpt->geatrue_direction == 1){
            GG_FillRect(&source, LOSWORD(msg->dwAddData2), 0, wndDpt->scrolling.wndRect.width, wndDpt->scrolling.wndRect.height);
            GUI_LyrWinSetSrcWindow(wndDpt->scrolling.ewii.lyrLow, &source);
            GUI_LyrWinSetSrcWindow(wndDpt->scrolling.ewii.lyrHigher, &source);
        }else if(wndDpt->geatrue_direction == 2){
            GG_FillRect(&source, 0, LOSWORD(msg->dwAddData2), wndDpt->scrolling.wndRect.width, wndDpt->scrolling.wndRect.height);
		    GUI_LyrWinSetSrcWindow(wndDpt->scrolling.ewii.lyrLow, &source);
		    GUI_LyrWinSetSrcWindow(wndDpt->scrolling.ewii.lyrHigher, &source);
		}
        break;
    case GUI_MSG_USER_GESTURE_DONE:
#ifdef THIS_DEBUG
       //LogMI("Done direction: %d, distance: %d", HISWORD(msg->dwAddData2), LOSWORD(msg->dwAddData2));
#endif
       //模板保护会出错
        if(wndDpt->geatrue_direction == 1){
			if((wndDpt->clock_en == 1) && (HISWORD(msg->dwAddData2) == 100)){
                GG_FillRect(&source, LOSWORD(msg->dwAddData2) + wndDpt->scrolling.wndRect.width, 0, wndDpt->scrolling.wndRect.width, wndDpt->scrolling.wndRect.height);
				wndDpt->scrolling.wndRect.x = LOSWORD(msg->dwAddData2) + wndDpt->scrolling.wndRect.width;
			}else{
				GG_FillRect(&source, LOSWORD(msg->dwAddData2) + 0, 0, wndDpt->scrolling.wndRect.width, wndDpt->scrolling.wndRect.height);
				wndDpt->scrolling.wndRect.x = LOSWORD(msg->dwAddData2);
			}
            GUI_LyrWinSetSrcWindow(wndDpt->scrolling.ewii.lyrLow, &source);
            GUI_LyrWinSetSrcWindow(wndDpt->scrolling.ewii.lyrHigher, &source);
            wndDpt->scrolling.ewii.gestureArgs.start_x = wndDpt->scrolling.wndRect.x;
            wndDpt->current = wndDpt->scrolling.wndRect.x / wndDpt->scrolling.wndRect.width;
			if(wndDpt->indicate_en){
               MwIndicatorWndUpdate(wndDpt->indicatorWnd, wndDpt->current-1);
			}
        }else if(wndDpt->geatrue_direction == 2){
			if((wndDpt->clock_en == 1) && (HISWORD(msg->dwAddData2) == 100)){
				GG_FillRect(&source, 0, LOSWORD(msg->dwAddData2) + wndDpt->scrolling.wndRect.height, wndDpt->scrolling.wndRect.width, wndDpt->scrolling.wndRect.height);
				wndDpt->scrolling.wndRect.y = LOSWORD(msg->dwAddData2) + wndDpt->scrolling.wndRect.height;
			}else{
				GG_FillRect(&source, 0, LOSWORD(msg->dwAddData2) + 0, wndDpt->scrolling.wndRect.width, wndDpt->scrolling.wndRect.height);
				wndDpt->scrolling.wndRect.y = LOSWORD(msg->dwAddData2);
			}
		    GUI_LyrWinSetSrcWindow(wndDpt->scrolling.ewii.lyrLow, &source);
		    GUI_LyrWinSetSrcWindow(wndDpt->scrolling.ewii.lyrHigher, &source);
            wndDpt->scrolling.ewii.gestureArgs.start_y = wndDpt->scrolling.wndRect.y;
			wndDpt->fore_current = wndDpt->current;
            wndDpt->current = wndDpt->scrolling.wndRect.y / wndDpt->scrolling.wndRect.height;
		}
        GUI_WinSetFocusChild(wndDpt->hWins[wndDpt->current]);
        break;
	case GUI_MSG_SET_FOCUS://有
		if(wndDpt->clock_en){
            msg->h_deswin = wndDpt->hWins[wndDpt->current];
			msg->id = GUI_MSG_SET_FOCUS;
			GUI_PostMessage(msg);
		}
	    break;
	case GUI_MSG_SET_UNFOCUS:
		if(wndDpt->clock_en){
            msg->h_deswin = wndDpt->hWins[wndDpt->current];
			msg->id = GUI_MSG_SET_UNFOCUS;
			GUI_PostMessage(msg);
		}
		break;
    case GUI_MSG_USER_GESTURE_LPRESS:		
        	msg->h_deswin = wndDpt->scrolling.ewii.desktop;
        	msg->id = GUI_MSG_USER_WIN_CHANGE;
		if(wndDpt->clock_en){
			if(wndDpt->current==1){
	       		msg->dwAddData2 = HOME_SUBWIN_SETTING_TIME;
			}else{
				
               		msg->dwAddData2 = HOME_SUBWIN_UPDATE;
			}
		}else{
             		msg->dwAddData2 = HOME_SUBWIN_UPDATE;
		}
        	GUI_PostMessage(msg);
        	break;
    case GUI_MSG_USER_STANDY:
        break;
    default:
#ifdef THIS_DEBUG
        LogMI("Unhandled scrolling host window message (%d)", msg->id);
#endif
		GUI_ManWinDefaultProc(msg);
        break;
    }
    return 0;
}
H_WIN SCR_ScrollingHostBuilder(EWin_InheritInfo_ForHeader * inherit, BuilderCreate create, gg_handler windowNode)
{
	gg_handler window;
	gg_handler properties;
	int totalWins;
	int clock;
	
	
	ScrollingHostWinDescriptor * wndDpt;
	if (-1 == EWin_CheckInherit(inherit)) {
		return NULL;
	}
	window = GG_DOMGetBrotherElementByTagName(windowNode,"window");
	totalWins = GG_DOMGetElementsByTagName(window, "page");
	properties = GG_DOMGetFirstChild(windowNode);
	properties = GG_DOMGetBrotherElementByTagName(properties,"clock");
	
	if(properties){
	   totalWins++;
	   clock = 1;
	   //__msg("clock exist\n");
	}else{
	   clock = 0;
	   //__msg("clock not exist\n");
	}
	
#ifdef THIS_DEBUG
	LogD("Total windows (%d)", totalWins);
#endif
	if (totalWins < 1) {
		LogE("Argument error (%d)", totalWins);
		return NULL;
	}

	wndDpt = (ScrollingHostWinDescriptor *)esMEMS_Malloc(0, (sizeof(ScrollingHostWinDescriptor) + (totalWins * sizeof(H_WIN))));
	if (NULL == wndDpt) {
		LogE("Malloc failed");
		return NULL;
	}
	eLIBs_memset(wndDpt, 0, sizeof(ScrollingHostWinDescriptor));
	eLIBs_memcpy(wndDpt, inherit, sizeof(EWin_InheritInfo_ForHeader));
	wndDpt->scrolling.ewii.mask[0] = SCR_WNDINFOMASK_SHOST;
	
	wndDpt->create = create;
	wndDpt->root = windowNode;
	wndDpt->window_root = windowNode;
	wndDpt->totalWins = totalWins;
	wndDpt->clock_en = clock;
	wndDpt->page = inherit;
	return EWin_MainWindowCreate("ScrollingHostWindow", (EWin_InheritInfo_ForHeader *)wndDpt, scrollingHostWndProc);
}


