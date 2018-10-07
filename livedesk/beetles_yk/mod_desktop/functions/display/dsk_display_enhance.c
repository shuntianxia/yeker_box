/*
************************************************************************************************************************
*                                                     		display enhance
*
*                                  Copyright(C), 2006-2010, AllWinner Technology Co., Ltd.
*									    All Rights Reserved
*
* File Name  : dsk_display_enhance.c
*
* Author      : Jackie.Chen
*
* Version     : 1.0
*
* Date         : 2011.01.21
*
* Description :
* 
* Others      : None at present.
*
*
* History     :
*
*  <Author>        <time>       <version>      <description>
*
* Jackie.Chen      2011.01.21        1.0         build the file
*
************************************************************************************************************************
*/

#include "dsk_display_i.h"

__s32 dsk_display_enhance_on(void)
{
    /*
	ES_FILE*		p_disp;		

	p_disp = eLIBs_fopen("b:\\DISP\\DISPLAY", "r+");
	if(!p_disp)
	{
		__err("open display device fail!\n");	
		return EPDK_FAIL;
	}	
	
	eLIBs_fioctrl(p_disp, DISP_CMD_ENHANCE_ON, 0, 0);
	
	eLIBs_fclose(p_disp);
	*/
	return  EPDK_OK;
}

__s32 dsk_display_enhance_off(void)
{
    /*
	ES_FILE*		p_disp;		

	p_disp = eLIBs_fopen("b:\\DISP\\DISPLAY", "r+");
	if(!p_disp)
	{
		__err("open display device fail!\n");	
		return EPDK_FAIL;
	}

	eLIBs_fioctrl(p_disp, DISP_CMD_ENHANCE_OFF, 0, 0);
	
	eLIBs_fclose(p_disp);
	*/
	return  EPDK_OK;
}

__s32 dsk_display_set_bright(__u32 bright)
{
    /*
	ES_FILE*		p_disp;		
	__u32 arg[3];
	
	p_disp = eLIBs_fopen("b:\\DISP\\DISPLAY", "r+");
	if(!p_disp)
	{
		__err("open display device fail!\n");	
		return EPDK_FAIL;
	}

	arg[0] = bright;
	arg[1] = 0;
	arg[2] = 0;
	
	eLIBs_fioctrl(p_disp, DISP_CMD_SET_BRIGHT, 0, (void *)arg);
	
	eLIBs_fclose(p_disp);
	*/
	return  EPDK_OK;
}

__s32 dsk_display_get_bright(void)
{
    /*
	ES_FILE*		p_disp;		
	__s32 			ret;
	
	p_disp = eLIBs_fopen("b:\\DISP\\DISPLAY", "r+");
	if(!p_disp)
	{
		__err("open display device fail!\n");	
		return EPDK_FAIL;
	}
	ret = eLIBs_fioctrl(p_disp, DISP_CMD_GET_BRIGHT, 0, 0);
	
	eLIBs_fclose(p_disp);
	
	return  ret;
	*/
	return  EPDK_OK;
}

__s32 dsk_display_set_contrast(__u32 contrast)
{
    /*
	ES_FILE*		p_disp;		
	__u32 arg[3];
	
	p_disp = eLIBs_fopen("b:\\DISP\\DISPLAY", "r+");
	if(!p_disp)
	{
		__err("open display device fail!\n");	
		return EPDK_FAIL;
	}

	arg[0] = contrast;
	arg[1] = 0;
	arg[2] = 0;
	
	eLIBs_fioctrl(p_disp, DISP_CMD_SET_CONTRAST, 0, (void *)arg);
	
	eLIBs_fclose(p_disp);
	*/
	return  EPDK_OK;
}

__s32 dsk_display_get_contrast(void)
{
    /*
	ES_FILE*		p_disp;		
	__s32 			ret;
	
	p_disp = eLIBs_fopen("b:\\DISP\\DISPLAY", "r+");
	if(!p_disp)
	{
		__err("open display device fail!\n");	
		return EPDK_FAIL;
	}

	
	ret = eLIBs_fioctrl(p_disp, DISP_CMD_GET_CONTRAST, 0, 0);
	
	eLIBs_fclose(p_disp);
	
	return  ret;
	*/
	return EPDK_OK;
}

__s32 dsk_display_set_saturation(__u32 saturation)
{
    /*
	ES_FILE*		p_disp;		
	__u32 arg[3];
	
	p_disp = eLIBs_fopen("b:\\DISP\\DISPLAY", "r+");
	if(!p_disp)
	{
		__err("open display device fail!\n");	
		return EPDK_FAIL;
	}

	arg[0] = saturation;
	arg[1] = 0;
	arg[2] = 0;
	
	eLIBs_fioctrl(p_disp, DISP_CMD_SET_SATURATION, 0, (void *)arg);
	
	eLIBs_fclose(p_disp);
	*/
	return  EPDK_OK;
}

__s32 dsk_display_get_saturation(void)
{
    /*
	ES_FILE*		p_disp;		
	__s32 			ret;
	
	p_disp = eLIBs_fopen("b:\\DISP\\DISPLAY", "r+");
	if(!p_disp)
	{
		__err("open display device fail!\n");	
		return EPDK_FAIL;
	}
	
	ret = eLIBs_fioctrl(p_disp, DISP_CMD_SET_SATURATION, 0, 0);
	
	eLIBs_fclose(p_disp);
	
	return  ret;
	*/
	return EPDK_OK;
}

__s32 dsk_display_set_hue(__u32 hue)
{
    /*
	ES_FILE*		p_disp;		
	__u32 arg[3];
	
	p_disp = eLIBs_fopen("b:\\DISP\\DISPLAY", "r+");
	if(!p_disp)
	{
		__err("open display device fail!\n");	
		return EPDK_FAIL;
	}

	arg[0] = hue;
	arg[1] = 0;
	arg[2] = 0;
	
	eLIBs_fioctrl(p_disp, DISP_CMD_SET_HUE, 0, (void *)arg);
	
	eLIBs_fclose(p_disp);
	*/
	return  EPDK_OK;
}

__s32 dsk_display_get_hue(void)
{
    /*
	ES_FILE*		p_disp;		
	__s32 			ret;
	
	p_disp = eLIBs_fopen("b:\\DISP\\DISPLAY", "r+");
	if(!p_disp)
	{
		__err("open display device fail!\n");	
		return EPDK_FAIL;
	}
	
	ret = eLIBs_fioctrl(p_disp, DISP_CMD_SET_HUE, 0, 0);
	
	eLIBs_fclose(p_disp);
	
	return  ret;
	*/
	return EPDK_OK;
}

