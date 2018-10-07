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
* File    : startx.c
*
* By      : Steven
* Version : vx.xx.xx
* Date    : (please press f7 here)
**********************************************************************************************************************
*/

#include    "Esh_shell_private.h"

static __u8 mid_mod_lemon;
static __mp * mod_lemon;

static __u8 mid_mod_desktop;
static __mp * mod_desktop;

static __s32 __close_logo(void)
{
	ES_FILE      * p_disp;	
    __u32 arg[3];

	/*open display driver handle*/
	p_disp = eLIBs_fopen("b:\\DISP\\DISPLAY", "r+");
	if(!p_disp)
	{
		Esh_Wrn("open display device fail!\n");
		return EPDK_FAIL;
	}	
	//close logo
	arg[0] = logo_layer;
    arg[1] = 0;
    arg[2] = 0;
    eLIBs_fioctrl(p_disp,DISP_CMD_LAYER_CLOSE,0,(void*)arg);

	eLIBs_fioctrl(p_disp,DISP_CMD_LAYER_RELEASE,0,(void*)arg);

	eLIBs_fclose(p_disp);

	return EPDK_OK;
}

static void __exec_startx(void)
{
    __u32 bid, sid, pid, chip;
	__u32 arg[3];
	
	//__getc();
	
	//__close_logo();

	//esDEV_Plugin("\\drv\\display.drv", 0, 0, 1);
	//__inf(".Display Device Installed!...........................................................................................\n");
    {
        ES_FILE      * p_disp;
     /*__disp_colorkey_t   colorkey;
        //open display driver handle
        p_disp = eLIBs_fopen("b:\\DISP\\DISPLAY", "r+");
        if(!p_disp)
        {
            Esh_Wrn("open display device fail!\n");
            return ;
        }
        colorkey.ck_min.alpha = 0;
        colorkey.ck_min.red   = 0;
        colorkey.ck_min.green = 0;
        colorkey.ck_min.blue  = 0;
        colorkey.ck_max.alpha = 1;
        colorkey.ck_max.red   = 1;
        colorkey.ck_max.green = 1;
        colorkey.ck_max.blue  = 1;
        colorkey.ck_match   = 0x2a;
        eLIBs_fioctrl(p_disp, DISP_CMD_LCD_ON, 0, 0);
		{										
			arg[0] = (__u32)&colorkey;
			arg[1] = 0;
			arg[2] = 0;
        	eLIBs_fioctrl(p_disp, DISP_CMD_SET_COLORKEY, 0, (void *)arg);
        }
        eLIBs_fclose(p_disp);*/
    }
    esKRNL_TimeDly(2);
#if (MW_PROJ_MODEL == p35tp)
    __inf("No hdmi.drv\n");
#else
	esDEV_Plugin("\\drv\\hdmi.drv", 0, 0, 1);
#endif
	__here__
    mid_mod_lemon = esMODS_MInstall("d:\\mod\\orange.mod", 0);

	if(!mid_mod_lemon)
	{
		Esh_Error("orange.mod install fail!\n");
	}
	
	mod_lemon = esMODS_MOpen(mid_mod_lemon, 0);

	if(mod_lemon == 0)
	{
		Esh_Error("orange.mod start fail!\n");
	}
	__inf(".orange Installed and Openned!........................................................................................\n");
	
	mid_mod_desktop = esMODS_MInstall("d:\\mod\\desktop.mod", 0);

	if( !mid_mod_desktop )
	{
		Esh_Error("desktop.mod install fail!\n");
	}

	mod_desktop = esMODS_MOpen(mid_mod_desktop, 0);

	if( mod_desktop == 0)
	{
		Esh_Error("desktop.mod start fail!\n");
	}
	__inf(".Desktop Installed and Openned!........................................................................................\n");
#if 1
    /*enable device monitor */
    {
        ES_FILE     *pHwsc;

        pHwsc = eLIBs_fopen("b:\\HWSC\\hwsc", "rb+");
        if(pHwsc)
        {
            eLIBs_fioctrl(pHwsc, DEV_IOC_USR_HWSC_ENABLE_MONITOR, 0, NULL);
            eLIBs_fclose(pHwsc);
            __inf(".start device monitor .......................................\n");
        }
        else
        {
            __err("try to open b:\\HWSC\\hwsc failed!\n");
        }
    }
#endif
}
/*
*****************************************************************************
*                            dostartx
*
*  Description: start lemon.mod
*
*
*  Parameters:
*
*  Return value:
*****************************************************************************
*/
int dostartx(struct op *t , const char **args)
{
    /* do startx core: write by Steven */
    __exec_startx();

    /* no any error */
	return DOSUCCEED;
}
