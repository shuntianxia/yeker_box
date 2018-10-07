/*
**********************************************************************************************************************
*                                                    ePDK
*                                    the Easy Portable/Player Develop Kits
*                                              apps Sub-System
*
*                                   (c) Copyright 2007-2009, Steven.ZGJ.China
*                                             All Rights Reserved
*
* Moudle  : Shell
* File    : domediaplay.c
*
* Version : v2.0                                                                            2009.3.3
**********************************************************************************************************************
*/

#include <apps.h>

#define     DOFAILED            1       /* do command failed  */
#define     DOSUCCEED           0       /* do commmad succeed */

struct op {
	int op_type;               /* operation type, see Txxxx below */
	char **op_words;           /* arguments to a command */
	struct op *left;           /* op left ptr */
	struct op *right;          /* op right ptr */
	char *str;                 /* identifier for FOR and CASE */
};


#define MSG                 __msg
#define MESSAGE             __msg
#define WRN                 __wrn
#define WARNING             __wrn
#define ERR                 __err
#define ERROR               __err
#define LOG                 __log
#define INF                 __inf
#define INFORMATION         __inf

__s32   fileplayfinishedflag = 0;
__u32   mid_mod_cedar = 0;
__mp   *mod_cedar = 0;

static __s32 media_player_init(void);
static __s32 media_player_exit(void);
static void player_status_monitor_task(void *mod_cedar);
static __s32 play_media_file(const char *filepath);
static void player_set_audio(void);
__s32 media_player_main(const char *filepath)
{
    __s32           result;
    __u8            playermonitortaskprio = 0;

    //ES_FILE         *dis_file = 0;      //display file handle
   // RECT            video_window;       //video window position and size
    //__s32           video_layer_hdl = 0;//video layer handle
    //__u32           arg[3];

    LOG("=======================================================================\n");
    LOG("=======================| MEDIA PLAYER RUN |============================\n");
    LOG("=======================================================================\n");
    result = media_player_init();
    if(result)
    {
        WARNING("Init media player failed!\n");
		goto end;
    }
    
    playermonitortaskprio = esKRNL_TCreate(player_status_monitor_task, \
                            (void *)mod_cedar, 0x8000, (EPOS_curid << 8) | KRNL_priolevel4);
    if(!playermonitortaskprio)
    {
        WARNING("Create task failed!\n");
		goto end;
    }

    //==========================================================================
    // request video layer for cedar to play video
    //==========================================================================
    //open display file to get display handle
    //set aduio output
    //player_set_audio();
    
    //play media file
    if (play_media_file(filepath) != EPDK_OK)
    {
        WARNING("play media file failed!\n");
		goto end;
    }
    
    //wait media file play finished
    return EPDK_OK;
end:
	media_player_exit();
    esKRNL_TDelReq(playermonitortaskprio);
	return EPDK_FAIL;
}

static __s32 media_player_init(void)
{
    MESSAGE("open media player!\n");

    //install cedar module
    mid_mod_cedar = esMODS_MInstall("d:\\mod\\cedar.mod", 0);
    if(!mid_mod_cedar)
    {
        LOG("cedar.mod install fail!\n");
        return EPDK_FAIL;
    }
    LOG("install cedar.mod ok, mod_id:%x!\n", mid_mod_cedar);

    //open cedar module
    mod_cedar = esMODS_MOpen(mid_mod_cedar, 0);
    if(mod_cedar == 0)
    {
        LOG("mod_cedar.mod open fail!\n");
        return EPDK_FAIL;
    }
    LOG("open mod_cedar.mod ok!\n");

    return EPDK_OK;
}

static __s32 media_player_exit(void)
{
    MESSAGE("eixt media player!\n");
    
    //eixt and uninstall cedar module
    if(mod_cedar)
    {
        esMODS_MClose(mod_cedar);
        mod_cedar = 0;
    }
    if(mid_mod_cedar)
    {
        esMODS_MUninstall(mid_mod_cedar);
        mid_mod_cedar = 0;
    }

    return EPDK_OK;
}

static __s32 play_media_file(const char *filepath)
{
    __s32   result;
    __cedar_media_file_inf  media_file;
    
    //set default media file to play, input media file to cedar for playing
    eLIBs_memcpy(&media_file.file_path, filepath, eLIBs_strlen(filepath) + 1);
    media_file.tag_inf_validflag = 0;
    MESSAGE("Try to play:%s\n", media_file.file_path);
    if(mod_cedar)
    {
        result = esMODS_MIoctrl(mod_cedar, CEDAR_CMD_SET_MEDIAFILE, 0, &media_file);
        if(result < 0)
        {
            WARNING("Input media file to cedar failed!\n");
            return EPDK_FAIL;
        }
        result = esMODS_MIoctrl(mod_cedar, CEDAR_CMD_PLAY, 0, 0);
        if(result < 0)
        {
            WARNING("Send 'PLAY' command to cedar failed!!!\n");
            return EPDK_FAIL;
        }
		fileplayfinishedflag = 1;
    }
    return EPDK_OK;
}

static void player_status_monitor_task(void *mod_cedar)
{
    __s32 status;

    INFORMATION("player_status_monitor_task thread run\n");
    while(1)
    {
    	if(esKRNL_TDelReq(OS_PRIO_SELF) == OS_TASK_DEL_REQ)
    	{
	    	INFORMATION("player_status_monitor_task thread : Bye!\n");
    		break;
    	}
        
        //get cedar status
        if(fileplayfinishedflag){
	        status = esMODS_MIoctrl(mod_cedar, CEDAR_CMD_GET_STATUS, 0, 0);
	        if(status == CEDAR_STAT_STOP)
	        {
	            INFORMATION("Media play finished\n");
	            break;
	        }
        }
        esKRNL_TimeDly(50);
	}
	 //close and uninstall cedar module
    media_player_exit();
	esKRNL_TDel(OS_PRIO_SELF);
}

static void player_set_audio(void)
{
    ES_FILE *pAudio;
    pAudio = eLIBs_fopen("b:\\AUDIO\\PLAY", "r+");//b:\\AUDIO\\CTRL
	if(pAudio)
	{
        __inf("set audio volume to 20!\n");
        //eLIBs_fioctrl(pAudio, AUDIO_DEV_CMD_SET_VOLUME, 100, (void *)AUDIO_DEV_PLAY);
        eLIBs_fioctrl(pAudio, AUDIO_DEV_CMD_SET_VOLUME, 100, 0);
        eLIBs_fclose(pAudio);
	}else{
       __inf("set audio fail\n");
	}
}

		

extern __hdle OSAL_GPIO_Request(user_gpio_set_t *gpio_list, __u32 group_count_max);
extern __s32 OSAL_GPIO_DevSetONEPIN_IO_STATUS(__hdle devpin,  __u32 if_set_to_output_status,  const char *gpio_name);
extern __s32 OSAL_GPIO_DevWRITE_ONEPIN_DATA(__hdle devpin, __u32 value_to_gpio, const char *gpio_name);
extern __s32 OSAL_GPIO_Release(__hdle devpins,   __s32 if_release_to_default_status);

static __hdle hdl_mute;

int MwInitAudioMute(void)
{
    user_gpio_set_t  gpio_set;

    gpio_set.port = 1;
    gpio_set.port_num = 1;
    gpio_set.mul_sel = 1;
    gpio_set.pull = 1;
    gpio_set.drv_level = 1;
    gpio_set.data = 0;
    hdl_mute = OSAL_GPIO_Request(&gpio_set, 1);
    OSAL_GPIO_DevSetONEPIN_IO_STATUS(hdl_mute, 1, 0);
    return 0;
}

int MwExitAudioMute(void)
{
    OSAL_GPIO_Release(hdl_mute, 2);
    return 0;
}

int MwAudioMute(int on)
{
    OSAL_GPIO_DevWRITE_ONEPIN_DATA(hdl_mute, on & 0x01, 0);
    return 0;
}

