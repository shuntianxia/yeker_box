/*
************************************************************************************************************************
*                                                     display gamma
*
*                                  Copyright(C), 2006-2010, AllWinner Technology Co., Ltd.
*											       All Rights Reserved
*
* File Name   : dsk_display_gamma.c
*
* Author      : Gary.Wang
*
* Version     : 1.0
*
* Date        : 2010.09.02
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
* Gary.Wang      2010.09.02        1.0         build the file
*
************************************************************************************************************************
*/
#ifndef  __DSK_DISPLAY_GAMMA_C__
#define  __DSK_DISPLAY_GAMMA_C__


#include "dsk_display_i.h"


/*
****************************************************************************************************
*Name        : dsk_set_gamma
*Prototype   : __s32 dsk_set_gamma( __u32 gamma )
*Arguments   : gamma       input. gamma value.
*Return      : EPDK_OK     succeed
*              EPDK_FAIL   fail
*Description : adjust the gamma of the display.
*Other       :
****************************************************************************************************
*/
__s32 dsk_set_gamma( __u32 gamma )
{
    /*
	__u32			i;
	__u8			gamma_a, gamma_r, gamma_g, gamma_b;
	__u8*			gamma_map;
	ES_FILE*		p_disp;	
	__u32 arg[3];
	
	gamma_map = (__u8 *)esMEMS_Malloc(0, 1024);
	if(gamma_map == NULL)
	{
		__err("malloc memery fail!\n");
		
		return EPDK_FAIL;
	}
	eLIBs_memset(gamma_map, 0, 1024);

	p_disp = eLIBs_fopen("b:\\DISP\\DISPLAY", "r+");
	if(!p_disp)
	{
		__err("open display device fail!\n");
		esMEMS_Mfree(0, gamma_map);
		
		return EPDK_FAIL;
	}
	gamma_a =  gamma >> 24;
	gamma_r = (gamma & 0x00ff0000) >> 16;
	gamma_g = (gamma & 0x0000ff00) >> 8;
	gamma_b =  gamma & 0x000000ff;	
	__msg("gamma_r = %d, gamma_g = %d, gamma_b = %d\n", gamma_r, gamma_g, gamma_b);
	for(i = 0; i < 256; i++)
	{
		*(gamma_map + 4*i	 ) = i * gamma_b / 10;		
		*(gamma_map + 4*i + 1) = i * gamma_g / 10;
		*(gamma_map + 4*i + 2) = i * gamma_r / 10;
		*(gamma_map + 4*i + 3) = i * gamma_a / 10;		
	}	
	
	arg[0] = (__u32)gamma_map;
	arg[1] = 0;
	arg[2] = 1024;
	eLIBs_fioctrl(p_disp, DISP_CMD_SET_GAMMA_TABLE, 0, (void *)arg);
	
	eLIBs_fclose(p_disp);
		
	esMEMS_Mfree(0, gamma_map);
	*/
	return  EPDK_OK;
}



/*
****************************************************************************************************
*Name        : dsk_gamma_on
*Prototype   : __s32 dsk_gamma_on(void)
*Arguments   : void
*Return      : EPDK_OK     succeed
*              EPDK_FAIL   fail
*Description : enable the adjustment of the gamma.
*Other       :
****************************************************************************************************
*/
__s32 dsk_gamma_on(void)
{
    /*
	ES_FILE*		p_disp;		

	p_disp = eLIBs_fopen("b:\\DISP\\DISPLAY", "r+");
	if(!p_disp)
	{
		__err("open display device fail!\n");	
		return EPDK_FAIL;
	}	
	
	eLIBs_fioctrl(p_disp, DISP_CMD_GAMMA_CORRECTION_ON, 0, 0);
	
	eLIBs_fclose(p_disp);
	*/
	return  EPDK_OK;
}



/*
****************************************************************************************************
*Name        : dsk_gamma_off
*Prototype   : __s32 dsk_gamma_off(void)
*Arguments   : void
*Return      : EPDK_OK     succeed
*              EPDK_FAIL   fail
*Description : disable the adjustment of the gamma.
*Other       :
****************************************************************************************************
*/
__s32 dsk_gamma_off(void)
{
    /*
	ES_FILE*		p_disp;		

	p_disp = eLIBs_fopen("b:\\DISP\\DISPLAY", "r+");
	if(!p_disp)
	{
		__err("open display device fail!\n");	
		return EPDK_FAIL;
	}

	eLIBs_fioctrl(p_disp, DISP_CMD_GAMMA_CORRECTION_OFF, 0, 0);
	
	eLIBs_fclose(p_disp);
	*/
	return  EPDK_OK;
}



#endif     //  ifndef __DSK_DISPLAY_GAMMA_C__

/* end of dsk_display_gamma.c */
