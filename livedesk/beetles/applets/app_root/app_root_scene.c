/*
**************************************************************************************************************
*											         ePDK
*						            the Easy Portable/Player Develop Kits
*									           desktop system 
*
*						        	 (c) Copyright 2007-2010, ANDY, China
*											 All Rights Reserved
*
* File    		: app_root_win_scene.c
* By      		: Bayden
* Func		: app_root applets plugin interface define
* Version	: v1.0
* ============================================================================================================
* 2010-9-28 17:11:20  andy.zhang  create this file, implements the fundemental interface;
**************************************************************************************************************
*/

#include "beetles_app.h"
#include "app_root.h"
#include "app_root_scene.h"
#include "app_root_init.h"
#include "home/app_home.h"
#include "explorer/explorer.h"
//#include "movie/app_movie.h"
#include "music/music.h"
#include "photo/app_photo.h"
#include "setting/app_setting.h"
//#include "fm/app_fm.h"
//#include "ebook/app_ebook.h"
//#include "record/app_record.h"

typedef struct tag_root_ctrl
{
	H_WIN	h_app_home;		
	H_WIN	h_app_explorer;
	H_WIN	h_app_fm;	
	H_WIN	h_app_movie;
	H_WIN	h_app_music;
	H_WIN	h_app_photo;
	H_WIN	h_app_ebook;
	H_WIN	h_app_setting;		
	H_WIN	h_app_calendar;	
    H_WIN	h_app_record;	
    H_WIN   h_app_screensaver;
	root_para_t *root_para;
}root_ctrl_t;

/**********************************************************************************************************************/

typedef enum
{
    APP_ROOT_MSG_DO_ASYNC=GUI_MSG_+1,
}app_root_msg_id_t;

typedef __s32 (*p_app_root_fun)(void* arg);

static __krnl_event_t* g_root_scene_sem = NULL;

__s32 __root_scene_sem_init(void)
{
    if (0 == g_root_scene_sem)
	{
		g_root_scene_sem = esKRNL_SemCreate(1);
	}

    if(g_root_scene_sem)
    {
        return EPDK_OK;
    }
    else
    {
        return EPDK_FAIL;
    }
}

__s32 __root_scene_sem_deinit(void)
{
    if (g_root_scene_sem)
	{		
        __u8 err;
        
		esKRNL_SemDel(g_root_scene_sem, OS_DEL_ALWAYS, &err);		
		g_root_scene_sem = NULL;
	}
    
    return EPDK_OK;
}

__s32 __root_scene_sem_pend(void)
{
    __u8 err;

    if(g_root_scene_sem)
    {
        __msg("before esKRNL_SemPend(g_root_scene_sem\n");
	    esKRNL_SemPend(g_root_scene_sem, 0, &err);
        __msg("after esKRNL_SemPend(g_root_scene_sem\n");
    }

    return 0;
}

__s32 __root_scene_sem_accept(void)
{
    __u16 sem_nr;

    sem_nr = 0;

    if(g_root_scene_sem)
    {
	    sem_nr = esKRNL_SemAccept(g_root_scene_sem);
    }
    
    return sem_nr;
}

__s32 __root_scene_sem_post(void)
{
    if(g_root_scene_sem)
    {
        esKRNL_SemPost(g_root_scene_sem);
    }
    
    return 0;
}


//向hwin发送异步消息，执行pfun函数
static __s32 __app_do_async(H_WIN hwin, __u32 msg_id, p_app_root_fun pfun, void* arg)
{
    if(0 == hwin)
    {
        return EPDK_FAIL;
    }

    {		
		__gui_msg_t setmsg;
		setmsg.id			=  msg_id;		
		setmsg.h_srcwin		=  NULL;
		setmsg.h_deswin		=  hwin;
		setmsg.dwAddData1	=  (__u32)pfun;
		setmsg.dwAddData2	=  (__u32)arg;
		setmsg.p_arg		=  NULL;		

        __msg("setmsg.id=%x, setmsg.h_deswin=%x, setmsg.dwAddData1=%x\n, setmsg.dwAddData2=%x\n",
            setmsg.id, setmsg.h_deswin, setmsg.dwAddData1, setmsg.dwAddData2);
		GUI_SendNotifyMessage(&setmsg);	
	}   
    
    return EPDK_OK;
}

//向hwin发送setfocus消息，syn 为1代表需要直接调用GUI_WinSetFocusChild。
static void __app_set_focus_child(H_WIN hwin, __s32 syn)
{
	if (0 == hwin)
	{
		return;
	}

	if (1 == syn)
	{
		GUI_WinSetFocusChild(hwin);
	}
	else
	{		
		__gui_msg_t setmsg;
		setmsg.id			=  GUI_MSG_SET_FOCUS;		
		setmsg.h_srcwin		=  NULL;
		setmsg.h_deswin		=  hwin;
		setmsg.dwAddData1	=  NULL;
		setmsg.dwAddData2	=  NULL;
		setmsg.p_arg		=  NULL;			
		GUI_SendNotifyMessage(&setmsg);	
	}
	
}

static void __app_home_restore(__gui_msg_t* msg)
{
    root_ctrl_t   *root_ctrl;
    
    root_ctrl = (root_ctrl_t *)GUI_WinGetAddData(msg->h_deswin);
	if (root_ctrl->h_app_home)
	{
		__gui_msg_t mymsg;
		
		mymsg.h_deswin = root_ctrl->h_app_home;
		mymsg.id = DSK_APP_RESTORE;
		mymsg.dwAddData1 =0;
		mymsg.dwAddData2 = 0;
		GUI_SendNotifyMessage(&mymsg);
						
		mymsg.h_deswin = root_ctrl->h_app_home;
		mymsg.id = DSK_MSG_FS_PART_PLUGOUT;
		mymsg.dwAddData1 = msg->dwAddData1;
		mymsg.dwAddData2 = msg->dwAddData2;
		GUI_SendNotifyMessage(&mymsg);

		__app_set_focus_child(root_ctrl->h_app_home, 0);
	}
	else
	{
		root_ctrl->h_app_home = app_home_create(root_ctrl->root_para);
		__app_set_focus_child(root_ctrl->h_app_home, 0);
	}
}

static __s32 __app_root_is_bg_music_exist(__gui_msg_t *msg, H_WIN* hMusic)
{
    H_WIN root, child;
	char  winname[256];  
    __bool exist;
   
    root  = GUI_WinGetHandFromName(APP_ROOT);	
	child = GUI_WinGetFirstChild(root);

    exist = 0;
    *hMusic = 0;
	while( child != NULL )
	{
		eLIBs_memset(winname, 0, 256);
		GUI_WinGetName(child, winname);
				
		__msg("winname = %s \n", winname);
		
		if( 0 == eLIBs_strcmp( APP_MUSIC, winname))		
		{
            *hMusic = child;
			exist = 1;
            break;
		}
		
		child = GUI_WinGetNextBro(child);
	}

    return exist;
}


static void __app_root_delete_bg_music(__gui_msg_t *msg)
{
    H_WIN root, child, next_child;
	char  winname[256]; 
    root_para_t *root_para;
	root_ctrl_t   *root_ctrl;

    root_ctrl = (root_ctrl_t   *)GUI_WinGetAddData(msg->h_deswin);
	root_para = root_ctrl->root_para;
		
	root  = GUI_WinGetHandFromName(APP_ROOT);	
    if(!root)
    {
        __err("GUI_WinGetHandFromName fail...\n");
        return ;
    }
	child = GUI_WinGetFirstChild(root);
	
	while( child != NULL )
	{
		eLIBs_memset(winname, 0, 256);
		GUI_WinGetName(child, winname);

        next_child = GUI_WinGetNextBro(child);
				
		__msg("winname = %s \n", winname);
		
		if( 0 == eLIBs_strcmp( APP_MUSIC, winname))		
		{
			GUI_ManWinDelete(child);
            root_ctrl->h_app_music = NULL;
            break;
		}
		
		child = next_child;
	} 	
}

static void __app_root_create_bg_music(__gui_msg_t *msg)
{
    H_WIN root, child;
	char  winname[256];  
    __bool exist;
    root_para_t *root_para;
	root_ctrl_t   *root_ctrl;

    root_ctrl = (root_ctrl_t   *)GUI_WinGetAddData(msg->h_deswin);
	root_para = root_ctrl->root_para;

    root  = GUI_WinGetHandFromName(APP_ROOT);	
	child = GUI_WinGetFirstChild(root);

    exist = 0;
	while( child != NULL )
	{
		eLIBs_memset(winname, 0, 256);
		GUI_WinGetName(child, winname);
				
		__msg("winname = %s \n", winname);
		
		if( 0 == eLIBs_strcmp( APP_MUSIC, winname))		
		{
			exist = 1;
            break;
		}
		
		child = GUI_WinGetNextBro(child);
	}

    if(0 == exist)
    {        				
		root_para->data = MUSICPLAYER_BACKGROUND;
		root_ctrl->root_para->h_parent= GUI_WinGetParent(root_ctrl->h_app_home);                                                      
		root_ctrl->h_app_music =  app_music_create(root_para);		
    }
}


//进入应用之前把背景设置为默认背景
static void __app_root_change_to_default_bg(void)
{    
    reg_init_para_t* para;
    para = dsk_reg_get_para_by_app(REG_APP_INIT);
    if(para)
    {
        para->bgd_flag_in_main = gscene_bgd_get_flag();
        if(EPDK_TRUE == para->bgd_flag_in_main)//当前是默认背景，则不做任何事情
        {                                       
        }
        else//当前不是默认背景，修改为默认背景
        {
            gscene_bgd_set_flag(EPDK_TRUE);
            gscene_bgd_refresh();
        }                                    
    }
    else
    {
        __msg("dsk_reg_get_para_by_app fail...\n");
    }
}
#if 0
static __s32 app_root_command_proc(__gui_msg_t *msg)
{
	root_para_t *root_para;
	root_ctrl_t   *root_ctrl;
	__gui_msg_t mymsg;

	root_ctrl = (root_ctrl_t   *)GUI_WinGetAddData(msg->h_deswin);
	root_para = root_ctrl->root_para;

    __msg("*****************app_root_command_proc**************\n");

	switch (LOWORD(msg->dwAddData1))
	{
		case APP_HOME_ID:
		{
			switch (HIWORD(msg->dwAddData1))
			{
				case SWITCH_TO_OTHER_APP:
				{
					switch(msg->dwAddData2)
					{						
						case ID_HOME_FM:
						{                            
                            gscene_hbar_set_state(HBAR_ST_HIDE);
                            __app_root_change_to_default_bg();
                            
                            //删除后台音乐
                            __app_root_delete_bg_music(msg);
                            
							__msg("**********enter fm**********\n");							
							root_ctrl->root_para->h_parent= GUI_WinGetParent(root_ctrl->h_app_home);
							root_ctrl->h_app_fm=  app_fm_create(root_para);
							GUI_WinSetFocusChild(root_ctrl->h_app_fm);
                            break;                          
						}
						break;
						case ID_HOME_MOVIE:
						{   
                            gscene_hbar_set_state(HBAR_ST_HIDE);
                            __app_root_change_to_default_bg();
                            
							__msg("**********enter movie explorer**********\n");							                           

							__msg("root type=%d\n", msg->dwReserved);
                            root_para->root_type = msg->dwReserved;//RAT_TF;
                            root_para->explr_root = msg->dwReserved;
                            root_para->data = ID_EXPLORER_MOVIE;
                            root_ctrl->root_para->h_parent= GUI_WinGetParent(root_ctrl->h_app_home);
                            root_ctrl->h_app_explorer =  app_explorer_create(root_para);
                            __msg("root_ctrl->h_app_explorer = %x\n", root_ctrl->h_app_explorer);
                            GUI_WinSetFocusChild(root_ctrl->h_app_explorer);
							#if EPDK_AUTO_PLAY_MOVIE_ENABLE		
							{
								__gui_msg_t new_msg ;
								eLIBs_printf("send enter key down message to explorer win\n");
								new_msg.h_deswin = root_ctrl->h_app_explorer;
								new_msg.id = GUI_MSG_KEY;
								new_msg.dwAddData1 = GUI_MSG_KEY_ENTER;
								new_msg.dwAddData2 = KEY_DOWN_ACTION;
								GUI_SendAsyncMessage(&new_msg);
								esKRNL_TimeDly(20);

								eLIBs_printf("send enter key up message to explorer win\n");
								new_msg.h_deswin = root_ctrl->h_app_explorer;
								new_msg.id = GUI_MSG_KEY;
								new_msg.dwAddData1 = GUI_MSG_KEY_ENTER;
								new_msg.dwAddData2 = KEY_UP_ACTION;
								GUI_SendAsyncMessage(&new_msg);
							}
							#endif

                            break;							
						}	
						break;
						case ID_HOME_PHOTO:
						{               
                            gscene_hbar_set_state(HBAR_ST_HIDE);
                            __app_root_change_to_default_bg();
                            
							__msg("**********enter photo explorer**********\n");														
							
							__msg("root type=%d\n", msg->dwReserved);
                            root_para->root_type = msg->dwReserved;
                            root_para->explr_root = msg->dwReserved;
                            root_para->data = ID_EXPLORER_PHOTO;
                            root_ctrl->root_para->h_parent= GUI_WinGetParent(root_ctrl->h_app_home);
                            root_ctrl->h_app_explorer =  app_explorer_create(root_para);
                            __msg("root_ctrl->h_app_explorer = %x\n", root_ctrl->h_app_explorer);
                            GUI_WinSetFocusChild(root_ctrl->h_app_explorer);
                            break;
						}
						break;
						case ID_HOME_MUSIC:
						{       
                            __s32 ret;
                            __s32 exist;
                            H_WIN hmusic;
                            
                            gscene_hbar_set_state(HBAR_ST_HIDE);
                            __app_root_change_to_default_bg();

                            root_para->explorer_play_music = 0;
                            root_para->root_type = msg->dwReserved;                     
                            root_para->data = MUSICPLAYER_NORMAL;
                            root_ctrl->root_para->h_parent= GUI_WinGetParent(root_ctrl->h_app_home);                                                      

                            exist = __app_root_is_bg_music_exist(msg, &hmusic);
                            if(1 == exist)//重建后台音乐
                            {
                                //ret = app_music_restore(hmusic, root_para->root_type); 
                                ret = app_music_restore(hmusic, root_para); 
                                if(EPDK_OK == ret)
                                {
                                    if(root_ctrl->h_app_music != hmusic)
                                    {
                                        __msg("*******root_ctrl->h_app_music != hmusic********\n");
                                    }
                                    root_ctrl->h_app_music = hmusic;
                                }
                                else
                                {
                                    __msg("*****app_music_restore fail*******\n");
                                    __app_root_delete_bg_music(msg);
                                    __here__;
                                    root_ctrl->h_app_music =  app_music_create(root_para);
                                    __here__;
                                }
                            }
                            else
                            {
                                root_ctrl->h_app_music =  app_music_create(root_para);							    
                            }
                            GUI_WinSetFocusChild(root_ctrl->h_app_music);                                                       
                            
                           // gscene_bgd_set_state(BGD_STATUS_HIDE);
							
						}
						break;
						case ID_HOME_EBOOK:
						{                 
                            gscene_hbar_set_state(HBAR_ST_HIDE);
                            __app_root_change_to_default_bg();
                            
							root_para->root_type = msg->dwReserved;
							root_para->explr_root = msg->dwReserved;
							root_para->data = ID_EXPLORER_EBOOK;
							root_ctrl->root_para->h_parent= GUI_WinGetParent(root_ctrl->h_app_home);
							root_ctrl->h_app_explorer=  app_explorer_create(root_para);
							GUI_WinSetFocusChild(root_ctrl->h_app_explorer);
						}
						break;
                        case ID_HOME_RECORD:
                        {
                            gscene_hbar_set_state(HBAR_ST_HIDE);
                            __app_root_change_to_default_bg();

                            //删除后台音乐
                            __app_root_delete_bg_music(msg);

                            __msg("**********ID_HOME_RECORD**********\n");	
                            root_ctrl->root_para->root_type = msg->dwReserved;//RAT_USB或者RAT_TF
                            root_ctrl->root_para->record_opt = 0;//正常录音
							root_ctrl->root_para->h_parent= GUI_WinGetParent(root_ctrl->h_app_home);
							root_ctrl->h_app_record=  app_record_create(root_para);
							GUI_WinSetFocusChild(root_ctrl->h_app_record);
                            break;
                        }
						case ID_HOME_OTHERS:
						{                                                                          
                            gscene_hbar_set_state(HBAR_ST_HIDE);
                            //删除后台音乐
                            //__app_root_delete_bg_music(msg);
                            
							__msg("**********enter other apps**********\n");
							if (0 == msg->dwReserved)
							{
                                __app_root_change_to_default_bg();
                                
								__msg("**********enter setting**********\n");	
                                root_ctrl->root_para->setting_opt = 0;//通用设置
								root_ctrl->root_para->h_parent= GUI_WinGetParent(root_ctrl->h_app_home);
								root_ctrl->h_app_setting=  app_setting_create(root_para);
								GUI_WinSetFocusChild(root_ctrl->h_app_setting);
								break;
							}
							else if (1 == msg->dwReserved)
							{
                                __app_root_change_to_default_bg();
                                
								__msg("**********enter setting**********\n");							
                                root_ctrl->root_para->setting_opt = 1;//电源管理
								root_ctrl->root_para->h_parent= GUI_WinGetParent(root_ctrl->h_app_home);
								root_ctrl->h_app_setting=  app_setting_create(root_para);
								GUI_WinSetFocusChild(root_ctrl->h_app_setting);
								break;
							}							
							else if (2 == msg->dwReserved)
							{                                                               								
#if BEETLES_RTC_EXIST
                                __msg("**********enter calendar**********\n");  

                                __app_root_change_to_default_bg();
								root_ctrl->root_para->h_parent= GUI_WinGetParent(root_ctrl->h_app_home);
								root_ctrl->h_app_calendar=  app_calendar_create(root_para);
								GUI_WinSetFocusChild(root_ctrl->h_app_calendar);                                

#else

                                __msg("**********enter explorer manage**********\n");							
								gscene_hbar_set_state(HBAR_ST_HIDE);
                                __app_root_change_to_default_bg();
                                
    							__msg("**********enter movie explorer**********\n");							                           

    							__msg("root type=%d\n", msg->dwReserved);
                                root_para->root_type = RAT_TF;//msg->dwReserved;//RAT_TF;
                                root_para->explr_root = RAT_TF;//msg->dwReserved;
                                root_para->data = ID_EXPLORER_ALL;
                                root_ctrl->root_para->h_parent= GUI_WinGetParent(root_ctrl->h_app_home);
                                root_ctrl->h_app_explorer =  app_explorer_create(root_para);
                                __msg("root_ctrl->h_app_explorer = %x\n", root_ctrl->h_app_explorer);
                                GUI_WinSetFocusChild(root_ctrl->h_app_explorer);

#endif
								break;
							}
                            else if (3 == msg->dwReserved)
							{      
#if BEETLES_RTC_EXIST                                
								__msg("**********enter explorer manage**********\n");							
								gscene_hbar_set_state(HBAR_ST_HIDE);
                                __app_root_change_to_default_bg();
                                
    							__msg("**********enter movie explorer**********\n");							                           

    							__msg("root type=%d\n", msg->dwReserved);
                                root_para->root_type = RAT_TF;//msg->dwReserved;//RAT_TF;
                                root_para->explr_root = RAT_TF;//msg->dwReserved;
                                root_para->data = ID_EXPLORER_ALL;
                                root_ctrl->root_para->h_parent= GUI_WinGetParent(root_ctrl->h_app_home);
                                root_ctrl->h_app_explorer =  app_explorer_create(root_para);
                                __msg("root_ctrl->h_app_explorer = %x\n", root_ctrl->h_app_explorer);
                                GUI_WinSetFocusChild(root_ctrl->h_app_explorer);
                                
#else

                                __err("sub menu focus item invalid...\n");

#endif
								break;
							}
							else
							{
								__err("sub menu focus item invalid...\n");
								break;
							}
						}
						break;
						default:
							break;
					}
				}
					break;
				default:
					break;
			}
		}
		break;
		case APP_EXPLORER_ID:
		{
			switch(HIWORD(msg->dwAddData1))
			{
			case SWITCH_TO_OTHER_APP:
				{
					switch( msg->dwAddData2 )
					{
					case EXPLR_SW_TO_MAIN:
						{                            
                            if(root_ctrl->h_app_explorer)
                            {
                                GUI_ManWinDelete(root_ctrl->h_app_explorer);
			                    root_ctrl->h_app_explorer = 0;
                            }
                            __here__;

                            __app_home_restore(msg);
						}
						break;
					case EXPLR_SW_TO_MOVIE:
						{                            
                            //删除后台音乐
                            __app_root_delete_bg_music(msg);
                            
							__msg("**********explorer to movie**********\n");
							
							//root_para->root_type = msg->dwReserved;
							root_para->data = 0;
							root_para->root_type = root_para->explr_root  ;
							root_ctrl->h_app_movie = app_movie_create(root_para);
							GUI_WinSetFocusChild(root_ctrl->h_app_movie);							
						}
						break;
					case EXPLR_SW_TO_PHOTO:
						{
                            //删除后台音乐
                            //__app_root_delete_bg_music(msg);
                            
							__msg("**********explorer to photo**********\n");
							
							//root_para->root_type = msg->dwReserved;
							root_para->data = 0;
							root_para->root_type = root_para->explr_root  ;
							root_ctrl->h_app_photo = app_photo_create(root_para);
							GUI_WinSetFocusChild(root_ctrl->h_app_photo);
							
						}
						break;
					case EXPLR_SW_TO_MUSIC:
						{       
	                            __s32 ret;
	                            __s32 exist;
	                            H_WIN hmusic;
	                            
	                            gscene_hbar_set_state(HBAR_ST_HIDE);
	                            __app_root_change_to_default_bg();
					//eLIBs_strncpy( root_para->music_file_name , (const char *) msg->dwReserved, sizeof(root_para->music_file_name) ) ;
					//__msg( "root_para->music_file_name =%s\n",root_para->music_file_name );

					root_para->explorer_play_music = 1;
	                            root_para->root_type = root_para->explr_root  ;     

					//root_para->root_type = root_para->explr_root;
					__msg("app_root_scene, root_para->root_type = %d\n", root_para->root_type);								
	                            root_para->data = MUSICPLAYER_NORMAL;
	                            root_ctrl->root_para->h_parent= GUI_WinGetParent(root_ctrl->h_app_home);                                                      

	                            exist = __app_root_is_bg_music_exist(msg, &hmusic);
	                            if(1 == exist)//重建后台音乐
	                            {
	                                //ret = app_music_restore(hmusic, root_para->root_type); 
	                                ret = app_music_restore(hmusic, root_para); 
	                                if(EPDK_OK == ret)
	                                {
	                                    if(root_ctrl->h_app_music != hmusic)
	                                    {
	                                        __msg("*******root_ctrl->h_app_music != hmusic********\n");
	                                    }
	                                    root_ctrl->h_app_music = hmusic;
	                                }
	                                else
	                                {
	                                    __msg("*****app_music_restore fail*******\n");
	                                    __app_root_delete_bg_music(msg);
	                                    __here__;
	                                    root_ctrl->h_app_music =  app_music_create(root_para);
	                                    __here__;
	                                }
	                            }
	                            else
	                            {
	                                root_ctrl->h_app_music =  app_music_create(root_para);							    
	                            }
	                            GUI_WinSetFocusChild(root_ctrl->h_app_music);                                                       
	                            
	                           // gscene_bgd_set_state(BGD_STATUS_HIDE);
								
							}
							break ;
					case EXPLR_SW_TO_EBOOK:
						{                            
                            //删除后台音乐
                            //__app_root_delete_bg_music(msg);
							__msg("**********explorer to ebook**********\n");
							
							//root_para->root_type = msg->dwReserved;
							root_para->data = 0;
							root_para->root_type = root_para->explr_root  ;   

							root_ctrl->h_app_ebook= app_ebook_create(root_para);
							__here__;
							GUI_WinSetFocusChild(root_ctrl->h_app_ebook);
							__here__;
							
						}						
						break;
					default:
                        {
                            __err("media type err...\n");
					    }
						break;
					}
					root_ctrl->h_app_explorer = NULL;//浏览器已自己删除
				}
				break;
			default:
				break;
			}
		}
		break;
		case APP_MOVIE_ID:
		{
            __msg("**********movie to explorer**********\n");
            __msg("msg->dwAddData1=%d\n", msg->dwAddData1);
            __msg("msg->dwAddData2=%d\n", msg->dwAddData2);
            __msg("msg->dwReserved=%d\n", msg->dwReserved);
			switch (HIWORD(msg->dwAddData1))
			{
				case SWITCH_TO_EXPLORER:
				{
                    GUI_ManWinDelete(root_ctrl->h_app_movie);
        			root_ctrl->h_app_movie = 0;
                    __here__;
                    root_para->root_type = root_para->explr_root;
                    root_para->data = ID_EXPLORER_MOVIE;
                    root_ctrl->root_para->h_parent= GUI_WinGetParent(root_ctrl->h_app_home);
                    root_ctrl->h_app_explorer =  app_explorer_create(root_para);
                    GUI_WinSetFocusChild(root_ctrl->h_app_explorer);
                    break;
				}
                case SWITCH_TO_MMENU:
				{
                    GUI_ManWinDelete(root_ctrl->h_app_movie);
        			root_ctrl->h_app_movie = 0;
                    __here__;
                    __app_home_restore(msg);
                    
                    break;
                }
				case VIDEO_SW_TO_AUDIO:
				{
                    GUI_ManWinDelete(root_ctrl->h_app_movie);
        			root_ctrl->h_app_movie = 0;
                    DebugPrintf("music switch to movie ...........root_para->explr_root=%d\n",root_para->explr_root);
                    root_para->root_type = root_para->explr_root;
                    root_para->data = MUSICPLAYER_NORMAL;
                    root_ctrl->root_para->h_parent= GUI_WinGetParent(root_ctrl->h_app_home);
                    root_ctrl->h_app_music=  app_music_create(root_para);
                    GUI_WinSetFocusChild(root_ctrl->h_app_music);
                    break;
				}	
                default:
                {
                    break;
                }
			}
			            			            	                    
            break;
            
		}		
		case APP_MUSIC_ID:
		{
			__msg("**********receive msg from music,msg->dwAddData2=%d**********\n", msg->dwAddData2);

            switch (msg->dwAddData2)
			{
				case SWITCH_TO_MMENU:
				{
                    if(MUSICPLAYER_BACKGROUND == msg->dwReserved)//音乐后台，不删除音乐
                    {
                        __msg("music background...\n");
                    }
                    else
                    {
                        __msg("delete music...\n");
                        GUI_ManWinDelete(root_ctrl->h_app_music);
			            root_ctrl->h_app_music = 0;                                  
                    }   
                    
                    __app_home_restore(msg);
                    
                    break;
				}
                case CLOSE_BG_MUSIC:
                {
                    __msg("delete background music...\n");
                    if(root_ctrl->h_app_music)
                    {
                        GUI_ManWinDelete(root_ctrl->h_app_music);
    		            root_ctrl->h_app_music = 0;  
                    }
                    break;
                }
				case AUDIO_SW_TO_VIDEO:
				{
                    GUI_ManWinDelete(root_ctrl->h_app_music);
        			root_ctrl->h_app_music = 0;
                    DebugPrintf("video switch to music ...........\n");
                    root_para->root_type = msg->dwReserved;
                    root_ctrl->root_para->h_parent= GUI_WinGetParent(root_ctrl->h_app_home);
                    root_ctrl->h_app_movie=  app_movie_create(root_para);
                    GUI_WinSetFocusChild(root_ctrl->h_app_movie);
                    break;
				}	

                default:
                {
                    break;
                }
            }            									
		}
		break;
		case APP_PHOTO_ID:
		{
            switch (HIWORD(msg->dwAddData1))
			{
				case SWITCH_TO_EXPLORER:
				{
                    //删除后台音乐
                    // __app_root_delete_bg_music(msg);
                    
                    __msg("**********photo to explorer**********\n");
			
        			GUI_ManWinDelete(root_ctrl->h_app_photo);
        			root_ctrl->h_app_photo = 0;
                    
                    gscene_bgd_set_flag(EPDK_TRUE);
                    //gscene_bgd_refresh();
                    
        			__here__;
                    root_para->root_type = root_para->explr_root;
                    root_para->data = ID_EXPLORER_PHOTO;
                    root_ctrl->root_para->h_parent= GUI_WinGetParent(root_ctrl->h_app_home);
                    root_ctrl->h_app_explorer =  app_explorer_create(root_para);
                    GUI_WinSetFocusChild(root_ctrl->h_app_explorer);  
                    break;
				}
                case OPEN_BG_MUSIC:
                {
                    __app_root_create_bg_music(msg);
                    break;
                }
                case CLOSE_BG_MUSIC:
                {
                    __app_root_delete_bg_music(msg);
                    break;
                }
                case SWITCH_TO_MMENU:
				{
                    GUI_ManWinDelete(root_ctrl->h_app_photo);
        			root_ctrl->h_app_photo= 0;
                    __here__;
                    __app_home_restore(msg);
                    
                    break;
                }
                default:
                {
                    break;
                }
            }		
						
			break;
		}
		case APP_EBOOK_ID:
		{
            switch (HIWORD(msg->dwAddData1))
			{
				case SWITCH_TO_EXPLORER:
				{
                    //删除后台音乐
                    //__app_root_delete_bg_music(msg);
                    
                    __msg("**********ebook to explorer**********\n");
			
        			GUI_ManWinDelete(root_ctrl->h_app_ebook);
        			root_ctrl->h_app_ebook = 0;

                    //gscene_bgd_set_flag(EPDK_TRUE);
                    //gscene_bgd_refresh();
        			
        			root_para->root_type = root_para->explr_root;
        			root_para->data = ID_EXPLORER_EBOOK;
        			root_ctrl->h_app_explorer =  app_explorer_create(root_para);
        			GUI_WinSetFocusChild(root_ctrl->h_app_explorer);
                    break;
				}
                case OPEN_BG_MUSIC:
                {
                    __app_root_create_bg_music(msg);
                    break;
                }
                case CLOSE_BG_MUSIC:
                {
                    __app_root_delete_bg_music(msg);
                    break;
                }
                case SWITCH_TO_MMENU:
				{
                    GUI_ManWinDelete(root_ctrl->h_app_ebook);
        			root_ctrl->h_app_ebook = 0;
                    __here__;
                    __app_home_restore(msg);
                    
                    break;
                }
                default:
                {
                    break;
                }
            }							
			
			break;
		}
				
		case APP_FM_ID:
		{
            switch (HIWORD(msg->dwAddData1))
			{
				case SWITCH_TO_MMENU:
				{
                    __msg("**********fm to home**********\n");
					
        			GUI_ManWinDelete(root_ctrl->h_app_fm);
        			root_ctrl->h_app_fm = 0;
                    
        			__app_home_restore(msg);

                    {//重新初始化按键音
                        __set_keytone_t state;

                        dsk_keytone_get_state(&state);
                        dsk_keytone_exit();
                        dsk_keytone_init(BEETLES_APP_ROOT"res\\sounds\\chord.wav");                    
                        dsk_keytone_set_state(state);
                    }
                    break;
				}
                case SWITCH_TO_OTHER_APP:
				{
					switch(LOWORD(msg->dwAddData2))
					{
					    case FM_SW_TO_RECORD:
						{
                            __here__;                            

                            GUI_ManWinDelete(root_ctrl->h_app_fm);
        			        root_ctrl->h_app_fm = 0;

                            //__app_root_change_to_default_bg();//这里不是通过主界面进入的，不记录进入ap之前背景的状态。
                                
							__msg("**********FM_SW_TO_RECORD**********\n");	
                            root_ctrl->root_para->root_type = HIWORD(msg->dwAddData2);//RAT_USB或者RAT_TF
                            root_ctrl->root_para->record_opt = 1;//fm录音
							root_ctrl->root_para->h_parent= GUI_WinGetParent(root_ctrl->h_app_home);
							root_ctrl->h_app_record=  app_record_create(root_para);
							GUI_WinSetFocusChild(root_ctrl->h_app_record);

                            {//重新初始化按键音
                                __set_keytone_t state;

                                dsk_keytone_get_state(&state);
                                dsk_keytone_exit();
                                dsk_keytone_init(BEETLES_APP_ROOT"res\\sounds\\chord.wav");                    
                                dsk_keytone_set_state(state);
                            }
                            break;
					    }
                        default:
                        {
                            break;
                        }
					}
                }
                default:
                {
                    break;
                }
            }										
					
			break;
		}
        case APP_RECORD_ID:
		{
            switch (HIWORD(msg->dwAddData1))
			{
				case SWITCH_TO_MMENU:
				{
                    __msg("**********record to home**********\n");
					
        			GUI_ManWinDelete(root_ctrl->h_app_record);
        			root_ctrl->h_app_record = 0;

        			__app_home_restore(msg);
                    
                    break;
				}
                case SWITCH_TO_OTHER_APP:
				{
					switch(LOWORD(msg->dwAddData2))
					{
                        case RECORD_SW_TO_FM:
                        {
                            __here__;
                            //__app_root_change_to_default_bg();//这里不是通过主界面进入的，不记录进入ap之前背景的状态。

                            GUI_ManWinDelete(root_ctrl->h_app_record);
        			        root_ctrl->h_app_record = 0;
                                
							__msg("**********FM_SW_TO_RECORD**********\n");	                                                        
							root_ctrl->root_para->h_parent= GUI_WinGetParent(root_ctrl->h_app_home);
							root_ctrl->h_app_fm = app_fm_create(root_para);
							GUI_WinSetFocusChild(root_ctrl->h_app_fm);
                            break;
                        }
                        default:
                        {
                            break;
                        }
					}                    
                }
                default:
                {
                    break;
                }
            }										
					
			break;
		}
        case APP_CALENDAR_ID:
		{
            switch (HIWORD(msg->dwAddData1))
			{
				case SWITCH_TO_MMENU:
				{
                    __msg("**********calendar to home**********\n");
					
        			GUI_ManWinDelete(root_ctrl->h_app_calendar);
        			root_ctrl->h_app_calendar = 0;

        			__app_home_restore(msg);
                    
                    break;
				}
                default:
                {
                    break;
                }
            }										
					
			break;
		}
		case APP_SETTING_ID:
		{
			switch(HIWORD(msg->dwAddData1))
			{
			case SWITCH_TO_OTHER_APP:
				{
					switch(msg->dwAddData2)
					{
					case SETTING_SW_TO_MAIN:
						{
							__msg("**********setting to home**********\n");
							GUI_ManWinDelete(root_ctrl->h_app_setting);
        			        root_ctrl->h_app_setting= 0;
							__app_home_restore(msg);
							
						}
						break;
					case SETTING_SW_TO_MULTIMEDIA:
					{
						
					}
						break;
					case SETTING_SW_TO_PROMPT_UPGRADE:
					{

					}
					break;
					case SETTING_SW_TO_PROMPT_DEFAULT:
					{
					}						
					break;
					case EXPLR_SW_TO_UPGRADE:
					{
	
					}						
					break;
					default:
						break;
					}
					//root_ctrl->h_app_setting = NULL;
				}
					break;
				default:
					break;
			}	
			break;
		}
		default:
			break;
	}

	return EPDK_OK;
}
#endif
static __s32 __app_root_broadcast_msg(__gui_msg_t *msg)
{
    H_WIN root, child;
	char  winname[256];  
    __gui_msg_t set_msg;

    eLIBs_memcpy(&set_msg, msg, sizeof(__gui_msg_t));
        
	root  = msg->h_deswin;	
	child = GUI_WinGetFirstChild(root);
	
	while( child != NULL )
	{
		eLIBs_memset(winname, 0, 256);
		GUI_WinGetName(child, winname);
				
		__msg("app_root_broadcast_msg msg->id=%d,winname = %s \n", 
            msg->id, winname);

        set_msg.h_deswin = child;
		GUI_SendMessage(&set_msg);
        // GUI_SendNotifyMessage(&set_msg);
        __here__;
		
		child = GUI_WinGetNextBro(child);
	} 	
    
    return EPDK_OK;
}

static __s32  __app_root_send_msg2focus_child(__gui_msg_t *msg)
{
    H_WIN root, child;	
    __gui_msg_t set_msg;

    eLIBs_memcpy(&set_msg, msg, sizeof(__gui_msg_t));
		
	root  = msg->h_deswin;	
	child = GUI_WinGetFocusChild(root);
	if(child)
	{	
    	GUI_SendMessage(&set_msg);
	}
    
    return EPDK_OK;
}

static __hdle           g_pe0 = 0;
static __hdle           g_pe1 = 0;
static __hdle           g_pe2 = 0;
static __hdle           g_pe3 = 0;

static __s32 __app_request_pins(void)
{
    __s32            ret;
    user_gpio_set_t  gpio_set[1];  
    
	/* 申请input_row1 */
    eLIBs_memset(gpio_set, 0, sizeof(user_gpio_set_t));
    ret = esCFG_GetKeyValue("matrixkey_para", "input_row1", (int *)gpio_set, sizeof(user_gpio_set_t)/4);

    __msg("ret= %d\n", ret);

    {        
        __u32 *pTPCtrl = NULL;
        __u32 data = 0;
        pTPCtrl = (__u32 *)0xf1c24800; //必须打开AD模块，PE2口才会有效
        data = *pTPCtrl; 
        __msg("tp ctrl registor is %d\n", data); 
        data |= 0x07;
        *pTPCtrl = data; 
        __msg("tp ctrl registor is %d\n", data);
    }

    if(!g_pe0)
    {
        /* 申请tv_en */
        eLIBs_memset(gpio_set, 0, sizeof(user_gpio_set_t));
        ret = esCFG_GetKeyValue("test_peport", "test_pea", (int *)gpio_set, sizeof(user_gpio_set_t)/4);
        if (!ret)
        {
            g_pe0 = esPINS_PinGrpReq(gpio_set, 1);
            if (!g_pe0)
            {
                __msg("request output_col pin failed\n");
                return EPDK_FAIL;
            }
        }
        else
        {
            __msg("fetch para from script failed\n");
            return EPDK_FAIL;
        }
    }

    if(!g_pe1)
    {
        /* 申请tv_en */
        eLIBs_memset(gpio_set, 0, sizeof(user_gpio_set_t));
        ret = esCFG_GetKeyValue("test_peport", "test_peb", (int *)gpio_set, sizeof(user_gpio_set_t)/4);
        if (!ret)
        {
            g_pe1 = esPINS_PinGrpReq(gpio_set, 1);
            if (!g_pe1)
            {
                __msg("request output_col pin failed\n");
                return EPDK_FAIL;
            }
        }
        else
        {
            __msg("fetch para from script failed\n");
            return EPDK_FAIL;
        }
    }

    if(!g_pe2)
    {
        /* 申请tv_en */
        eLIBs_memset(gpio_set, 0, sizeof(user_gpio_set_t));
        ret = esCFG_GetKeyValue("test_peport", "test_pec", (int *)gpio_set, sizeof(user_gpio_set_t)/4);
        if (!ret)
        {
            g_pe2 = esPINS_PinGrpReq(gpio_set, 1);
            if (!g_pe2)
            {
                __msg("request output_col pin failed\n");
                return EPDK_FAIL;
            }
        }
        else
        {
            __msg("fetch para from script failed\n");
            return EPDK_FAIL;
        }
    }

    if(!g_pe3)
    {
        /* 申请tv_en */
        eLIBs_memset(gpio_set, 0, sizeof(user_gpio_set_t));
        ret = esCFG_GetKeyValue("test_peport", "test_ped", (int *)gpio_set, sizeof(user_gpio_set_t)/4);
        if (!ret)
        {
            g_pe3 = esPINS_PinGrpReq(gpio_set, 1);
            if (!g_pe3)
            {
                __msg("request output_col pin failed\n");
                return EPDK_FAIL;
            }
        }
        else
        {
            __msg("fetch para from script failed\n");
            return EPDK_FAIL;
        }
    }

    return EPDK_OK;
}

static __s32 __app_pullup_pe(void)
{  
    __s32            ret;
    user_gpio_set_t  gpio_set[16];  

    if(!g_pe0 ||!g_pe1 ||!g_pe2 ||!g_pe3 )
    {
        __msg("__app_pullup_pe fail...\n");
        return EPDK_FAIL;
    }

    __msg("__app_pullup_pe...\n");
    esPINS_WritePinData(g_pe0, 1, 0);
    esPINS_WritePinData(g_pe1, 1, 0);
    esPINS_WritePinData(g_pe2, 1, 0);
    esPINS_WritePinData(g_pe3, 1, 0);
		
	return EPDK_OK;
}

static __s32 __app_pulldown_pe(void)
{  
     __s32            ret;
    user_gpio_set_t  gpio_set[16];  

    if(!g_pe0 ||!g_pe1 ||!g_pe2 ||!g_pe3 )
    {
        __msg("__app_pullup_pe fail...\n");
        return EPDK_FAIL;
    }

    __msg("__app_pulldown_pe...\n");
    esPINS_WritePinData(g_pe0, 0, 0);
    esPINS_WritePinData(g_pe1, 0, 0);
    esPINS_WritePinData(g_pe2, 0, 0);
    esPINS_WritePinData(g_pe3, 0, 0);
		
	return EPDK_OK;
}


__s32 app_root_win_proc(__gui_msg_t *msg)
{
    __msg("app_root_win_proc msg->id=%d, msg->h_deswin=%x, msg->dwAddData1=%d, msg->dwAddData2=%d\n"
        ,msg->id, msg->h_deswin, msg->dwAddData1, msg->dwAddData2);
	switch(msg->id)
	{
		case GUI_MSG_CREATE:
 		{			
			root_para_t *root_para;
			root_ctrl_t   *root_ctrl;				

			root_para = esMEMS_Balloc(sizeof(root_para_t));
            if(!root_para)
            {
                return EPDK_FAIL;
            }
			eLIBs_memset(root_para, 0, sizeof(root_para_t));

			root_ctrl = esMEMS_Balloc(sizeof(root_ctrl_t));
            if(!root_ctrl)
            {
                esMEMS_Bfree(root_para, sizeof(root_para_t));
                return EPDK_FAIL;
            }
			eLIBs_memset(root_ctrl, 0, sizeof(root_ctrl_t));           
            
			root_para->h_parent		= msg->h_deswin;
			root_para->font			= SWFFont;
			root_para->root_type	= 0;
			root_ctrl->h_app_home	= app_setting_create(root_para);
            __msg("root_ctrl->h_app_home = %x\n", root_ctrl->h_app_home);
            
			GUI_WinSetFocusChild(root_ctrl->h_app_home);
			
			root_ctrl->root_para = root_para;
			GUI_WinSetAddData(msg->h_deswin, (__u32)root_ctrl);                       

            __root_scene_sem_init();           
            
 		}
		return EPDK_OK;
		case GUI_MSG_CLOSE:
		{
			GUI_ManWinDelete(msg->h_deswin);
		}
		return EPDK_OK;
		case GUI_MSG_DESTROY:
 		{
			root_para_t *root_para;
			root_ctrl_t   *root_ctrl;
			
			root_ctrl = (root_ctrl_t *)GUI_WinGetAddData(msg->h_deswin);
			if (!root_ctrl)
			{
				__msg("******err****\n");
                return EPDK_OK;
			}
			
			root_para = (root_para_t *)root_ctrl->root_para;
			if (!root_para)
			{
				__msg("******err****\n");
                return EPDK_OK;
			}

			esMEMS_Bfree(root_para, sizeof(root_para_t));
			esMEMS_Bfree(root_ctrl, sizeof(root_ctrl_t));	

            __root_scene_sem_deinit();
 		}
		return EPDK_OK;
		case GUI_MSG_COMMAND:
 		{           
 			//app_root_command_proc(msg);
            
			return EPDK_OK;
 		}					
		case GUI_MSG_KEY:
		{
            __bool audio_engine_is_open;            
            
			__msg("**********************before app_root_win_proc GUI_MSG_KEY**************************\n");                      
            
            audio_engine_is_open = 0;

            if(!audio_engine_is_open)
            {
                __here__;
                if(EPDK_TRUE == dsk_radio_rcv_is_open())
                {
                    __here__;
                    audio_engine_is_open = 1;    
                }
            }

            //测试deflicker
            if(0)//112350
            {
                static __bool bup = 1;
                
                if(KEY_UP_ACTION == msg->dwAddData2
                    &&GUI_MSG_KEY_VADD == msg->dwAddData1)
                {
                    ES_FILE* fp;
                    
                    fp = eLIBs_fopen("b:\\disp\\display", "r+");
                    if(fp)
                    {
                        __u32 args[3]={0};
                        
                        args[0] = bup?1:0;
                        __msg("DISP_CMD_SET_DE_FLICKER, args[0]=%d\n", args[0]);
                        eLIBs_fioctrl(fp, DISP_CMD_SET_DE_FLICKER, 0, args);
                        eLIBs_fclose(fp);
                        bup = !bup;
                    }
                    else
                    {
                        __msg("open disp drv fail...\n");
                    }                                        
                }
            }

            //测试PE口作为gpio输出，拉高拉低
            if(0)//112350
            {
                if(KEY_UP_ACTION == msg->dwAddData2
                    &&GUI_MSG_KEY_VADD == msg->dwAddData1)
                {
                    static __bool bfirst = 1;
                    static __bool bup = 1;

                    if(bfirst)
                    {
                        __app_request_pins();
                        bfirst = 0;
                    }
                    if(bup)
                    {
                        __app_pullup_pe();
                    }
                    else
                    {
                        __app_pulldown_pe();
                    }

                    bup = !bup;
                }
            }

            __here__;

            if(!audio_engine_is_open)
            {
                __here__;
                if(EPDK_TRUE == robin_is_open())
                {
                    //__cedar_status_t status;

                    //__here__;
                    //status = robin_get_fsm_status();
                    //__msg("status = %d\n", status);
                    
                    //if(CEDAR_STAT_PAUSE != status && CEDAR_STAT_STOP != status)
                    {
                        __here__;
                        audio_engine_is_open = 1;
                    }
                }
            }

            __here__;

            if(!audio_engine_is_open)
            {
                if(EPDK_TRUE == dsk_wkm_is_open())
                {
                    //__cedar_status_t status;

                    //status = dsk_wkm_get_fsm_state();
                    //if(CEDAR_STAT_PAUSE != status && CEDAR_STAT_STOP != status)
                    {
                        audio_engine_is_open = 1;
                    }               
                }
            }

            __here__;
            if(!audio_engine_is_open)
            {
                __u32 key_type;

                __here__;
                key_type = dsk_keytone_get_key_type();
                if(KEY_DOWN_ACTION == msg->dwAddData2)
                {
                    __here__;
                    if(KEYTONE_KEY_TYPE_DOWN & key_type)
                    {
                        __here__;
                        dsk_keytone_on();
                        __here__;
                    }            
                }
                else if(KEY_UP_ACTION == msg->dwAddData2)
                {
                    __here__;
                    if(KEYTONE_KEY_TYPE_UP & key_type)
                    {
                        __here__;
                        dsk_keytone_on();
                        __here__;
                    }   
                }
                else if(KEY_REPEAT_ACTION == msg->dwAddData2)
                {
                    __here__;
                    if(KEYTONE_KEY_TYPE_REPEAT& key_type)
                    {
                        __here__;
                        dsk_keytone_on();
                        __here__;
                    }   
                }               
            }

			#if EPDK_TRANS_KEY_LONGLEFT2LONGVADD
			if(GUI_MSG_KEY_LONGLEFT == msg->dwAddData1)
			{
				DebugPrintf("trans longleft key to long vadd ...\n");
				msg->dwAddData1 = GUI_MSG_KEY_LONGVADD ;
			}
			#endif
            __msg("after app_root_win_proc GUI_MSG_KEY\n");
			break;
		}
		case GUI_MSG_TOUCH:			
		{
			__msg("app_root_win_proc GUI_MSG_TOUCH\n");
			//break;
			msg->h_deswin = GUI_WinGetFocusChild(msg->h_deswin);
			return GUI_SendMessage(msg);	
		}		
		case DSK_MSG_ENTER_SCREENSAVER:
		{	
			root_para_t *root_para;
			root_ctrl_t   *root_ctrl;
			reg_system_para_t *system_para;

			root_ctrl = (root_ctrl_t *)GUI_WinGetAddData(msg->h_deswin);
			root_para = root_ctrl->root_para;
           	system_para = (reg_system_para_t *)dsk_reg_get_para_by_app(REG_APP_SYSTEM);
        	if (root_ctrl->h_app_home)
        	//	GUI_ManWinDelete(root_ctrl->h_app_remote);
			{
			    __gui_msg_t msg;

				//return;
				eLIBs_memset(&msg, 0, sizeof(__gui_msg_t));
				msg.id 			= DSK_MSG_ENTER_SCREENSAVER;
				msg.h_srcwin 	= NULL;
				msg.h_deswin	= root_ctrl->h_app_home;
				msg.dwAddData1	= (__u32)0;
				msg.dwAddData2	= (__u32)0;
				
				GUI_SendMessage(&msg);	
			}

           	//dsk_display_set_lcd_bright(system_para->standby_backlight);
           	
        	root_ctrl->h_app_screensaver = app_clock_create(root_para);
            GUI_WinSetFocusChild(root_ctrl->h_app_screensaver);

			//dsk_display_set_lcd_bright(system_para->standby_backlight);
			break;
		}	
		case DSK_MSG_CLOSE_SCREENSAVER:
		{
			root_para_t *root_para;
			root_ctrl_t   *root_ctrl;
			reg_system_para_t *system_para;
			
			if (root_ctrl->h_app_screensaver == NULL)
				break;
			
			root_ctrl = (root_ctrl_t *)GUI_WinGetAddData(msg->h_deswin);
			root_para = root_ctrl->root_para;
			system_para = (reg_system_para_t *)dsk_reg_get_para_by_app(REG_APP_SYSTEM);
			if (root_ctrl->h_app_screensaver)
        		GUI_ManWinDelete(root_ctrl->h_app_screensaver);
        	root_ctrl->h_app_screensaver = NULL;

        	if (root_ctrl->h_app_home)
        	{
			    __gui_msg_t msg;

				//return;
				eLIBs_memset(&msg, 0, sizeof(__gui_msg_t));
				msg.id 			= DSK_MSG_CLOSE_SCREENSAVER;
				msg.h_srcwin 	= NULL;
				msg.h_deswin	= root_ctrl->h_app_home;
				msg.dwAddData1	= (__u32)0;
				msg.dwAddData2	= (__u32)0;
				
				GUI_SendMessage(&msg);	
			}
        	/*
			if (check_remote_setting() == EPDK_OK)
            {
            	root_ctrl->h_app_remote = app_remote_create(root_para);
           		GUI_WinSetFocusChild(root_ctrl->h_app_remote);	
            }
            else
            {
	            root_ctrl->h_app_setting = app_setting_create(root_para);
	            GUI_WinSetFocusChild(root_ctrl->h_app_setting);	
        	}*/
        	//dsk_display_set_lcd_bright(system_para->backlight);
			break;
		}
		case DSK_MSG_FS_PART_PLUGOUT:
 		{
 			root_ctrl_t   *root_ctrl;
 			H_WIN root, child, next_child;
 			char  winname[256];          

            __msg("**************app_root_win_proc, DSK_MSG_FS_PART_PLUGOUT**************\n");
            if(('D' == msg->dwAddData2)&&('E' == msg->dwAddData2)&&('Z' == msg->dwAddData2))
            {
                __msg("internal disk, not need kill apps, msg->dwAddData1=%d\n", msg->dwAddData2);
                __root_scene_sem_post();
                return EPDK_OK;
            }           

            __msg("app_root_broadcast_msg, msg->dwAddData2=%d\n", msg->dwAddData2);
            __app_root_broadcast_msg(msg);	                      

            __msg("********before __app_do_async***********\n");
            __msg("rat_clear_all_list_by_root=%x,msg->dwAddData2=%x\n", (__u32)rat_clear_all_list_by_root, msg->dwAddData2);
            __app_do_async(msg->h_deswin, APP_ROOT_MSG_DO_ASYNC, rat_clear_all_list_by_root, (void*)(msg->dwAddData2));
            __msg("********after __app_do_async***********\n");
            
            return EPDK_OK;
 		}		
		case DSK_MSG_FS_PART_PLUGIN:
		{
 			root_ctrl_t   *root_ctrl;
 			root_ctrl = (root_ctrl_t *)GUI_WinGetAddData(msg->h_deswin);

            __msg("app_root_win_proc, DSK_MSG_FS_PART_PLUGIN\n");
            if(('D' == msg->dwAddData1)&&('E' == msg->dwAddData1)&&('Z' == msg->dwAddData1))
            {
                __msg("internal disk, not need notify home app, msg->dwAddData1=%d\n", msg->dwAddData1);
                return EPDK_OK;
            }

            __msg("app_root_broadcast_msg, msg->dwAddData1=%d\n", msg->dwAddData1);
            __app_root_broadcast_msg(msg);
             			
            return EPDK_OK;
		}               		        
        case DSK_MSG_VOLTAGE_CHANGE:        
        {
            __msg("app_root_broadcast_msg, msg->dwAddData1=%d\n", msg->dwAddData1);
            __app_root_broadcast_msg(msg);
            return EPDK_OK;
        }		
        case DSK_MSG_TVDAC_PLUGOUT:
        case DSK_MSG_HDMI_PLUGOUT:
        {
            __msg("__app_root_broadcast_msg, msg->dwAddData1=%d\n", msg->dwAddData1);
            __app_root_broadcast_msg(msg);
            return EPDK_OK;
        }
        case APP_ROOT_MSG_DO_ASYNC:
        {            
            p_app_root_fun pfun;

            __msg("msg->dwAddData1=%x, msg->dwAddData2=%x\n",
                msg->dwAddData1, msg->dwAddData2);
            
            if(!msg->dwAddData1)
            {
                return EPDK_OK;
            }
            
            pfun = (p_app_root_fun)(msg->dwAddData1);
            __msg("pfun=%x, msg->dwAddData2=%x\n", pfun, msg->dwAddData2);
            __here__;
            pfun((void*)(msg->dwAddData2));
            __here__;
            return EPDK_OK;
        }
	}
	
	return GUI_ManWinDefaultProc(msg); 
}

__s32 app_root_win_proc_test(__gui_msg_t *msg)
{
    __msg("app_root_win_proc_test msg->id=%d, msg->h_deswin=%x, msg->dwAddData1=%d, msg->dwAddData2=%d\n"
        ,msg->id, msg->h_deswin, msg->dwAddData1, msg->dwAddData2);
	switch(msg->id)
	{
		case GUI_MSG_CREATE:
 		{			
			__msg("app_root_win_proc_test GUI_MSG_CREATE\n");
 		}
		return EPDK_OK;
		case GUI_MSG_CLOSE:
		{
			GUI_ManWinDelete(msg->h_deswin);
		}
		return EPDK_OK;
		case GUI_MSG_DESTROY:
 		{
			__msg("app_root_win_proc_test GUI_MSG_DESTROY\n");			
 		}
		return EPDK_OK;
		case GUI_MSG_COMMAND:
 		{
 			__msg("app_root_win_proc_test GUI_MSG_COMMAND\n");			

			return EPDK_OK;
 		}					
		case GUI_MSG_KEY:
		{
			__msg("app_root_win_proc_test GUI_MSG_KEY\n");		
			return EPDK_OK;
		}
	}
	
	return GUI_ManWinDefaultProc(msg); 
}


/* 创建根窗口 */
H_WIN app_root_wincreate(Activity *activity)
{
	H_WIN  						hManWin;
	
	__gui_manwincreate_para_t  	create_info;	
	
	eLIBs_memset(&create_info, 0, sizeof(__gui_manwincreate_para_t));
	
	create_info.attr            = activity; 
	create_info.hParent			= activity_get_root_win(activity) ;
	create_info.hHosting 		= NULL;   
    create_info.ManWindowProc   = (__pGUI_WIN_CB)esKRNL_GetCallBack((__pCBK_t)app_root_win_proc);
    create_info.name            = APP_ROOT; 	
	hManWin 					= GUI_ManWinCreate(&create_info);

    __msg("root win = %x\n", hManWin);
	
	if(hManWin == NULL)
    {        
        __wrn("home: create movie windows failed!\n");      
        return NULL;
    }
	
	return hManWin;
}
