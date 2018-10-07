/*
**************************************************************************************************************
*											         ePDK
*						            the Easy Portable/Player Develop Kits
*									           desktop system 
*
*						        	 (c) Copyright 2007-2010, ANDY, China
*											 All Rights Reserved
*
* File    	: dsk_updating.c
* By      	: Andy.zhang
* Func		: desktop firmware updating
* Version	: v1.0
* ============================================================================================================
* 2010-3-1 14:01:42  andy.zhang  create this file, implements the fundemental interface;
**************************************************************************************************************
*/

#include "..\\functions_i.h"

static char *g_fw_path = 0;

static void update(void* p_arg);



/*
****************************************************************************************************
*Name        : dsk_fw_update
*Prototype   : __s32 dsk_fw_update(  )
*Arguments   : file        input. the fireware file's full path.
*Return      : EPDK_OK     succeed
*              EPDK_FAIL   fail
*Description : update system with the fireware file.
*Other       :
****************************************************************************************************
*/

__s32 dsk_fw_update(const char *path,H_LYR lyr)
{    
    //__u8 tid;
    
#ifndef SIM_PC_WIN    

	__s32 len;
    __gui_msg_t msgex;
    
	if(g_fw_path)
    {
        __msg("updating...\n");
        return EPDK_FAIL;
    }

    len = eLIBs_strlen(path);
	g_fw_path = (char *)eLIBs_malloc(len+2);
	if(!g_fw_path)
	{
		__msg("store firmware path error \n");
		return EPDK_FAIL;
	} 
	eLIBs_memset(g_fw_path, 0, len+2);
    eLIBs_strcpy(g_fw_path, path);
	
    //tid = esKRNL_TCreate(update, 0, 0x400, KRNL_priolevel4);
    //if(!tid)
    //{
    //    __msg("esKRNL_TCreate fail...\n");
    //    return EPDK_FAIL;
    //}

    update((void *)lyr);

#endif	
    
    return EPDK_OK;
}

static void update(void* p_arg)
{
    __s32 len;
	__u8 	mid_update;
	__mp* 	mod_update;
    __s32 progress;   
	H_LYR lyr ;
	char buff[5];

	lyr = (H_LYR)p_arg;
	mid_update = esMODS_MInstall("d:\\mod\\update.mod", 0);
	if( mid_update == 0 )
	{
		__err("update mod install error \n");
		goto err1;
	} 
	
	mod_update = esMODS_MOpen(mid_update, 0);
	if( mod_update == NULL)
	{
		__err("open update mode error \n");
		esMODS_MUninstall(mid_update);
		goto err1;
	}
	
	__msg("mod_update = %x,file=%s\n",mod_update,g_fw_path);
	
	esMODS_MIoctrl(mod_update, UPDATE_CMD_START, 0, (void *)g_fw_path);	
	
	__msg(" ioctrl mod update complete \n");
    
    while (1)
	{
	    progress = esMODS_MIoctrl(mod_update, UPDATE_CMD_CHECK_PROG, 0, 0);
	    if (-1  == progress)
	    {
	        __wrn("error occur\n");
	        break;
	    }
	    
	    __msg("current progress is %d%%\n", progress);
	    if (100 == progress)
	    {
	        __msg("update complete\n");
            esMODS_MIoctrl(mod_update, UPDATE_CMD_STOP, 0, 0);
	    }else{
	    	//GUI_LyrWinSel(lyr);
			//eLIBs_sprintf(buff, "%02d%",progress);
			//GUI_DispStringAt(buff,12,52);
		}
		esKRNL_TimeDly(5);
	}
	
	esMODS_MClose(mod_update);
	
	esMODS_MUninstall(mid_update);

    if(g_fw_path)
    {
        eLIBs_free(g_fw_path);
        g_fw_path = 0;
    }   

err1:;

    //esKRNL_TDel(OS_PRIO_SELF);
    
	return ;
}

__s32 dsk_start_fw_update(const char *path)
{
    __s32 len;
    __gui_msg_t msgex;
    
	if(g_fw_path)
    {
        __msg("updating...\n");
        return EPDK_FAIL;
    }

    len = eLIBs_strlen(path);
	g_fw_path = (char *)eLIBs_malloc(len+2);
	if(!g_fw_path)
	{
		__msg("store firmware path error \n");
		return EPDK_FAIL;
	} 
	eLIBs_memset(g_fw_path, 0, len+2);
    eLIBs_strcpy(g_fw_path, path);
	
	return EPDK_OK;
}


