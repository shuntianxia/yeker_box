/*
*********************************************************************************************************
*											        ePDK
*						            the Easy Portable/Player Develop Kits
*									           hello world sample
*
*						        (c) Copyright 2006-2007, Steven.ZGJ China
*											All	Rights Reserved
*
* File    : pfhead.h
* By      : Steven.ZGJ
* Version : V1.00
*********************************************************************************************************
*/
#include "drv_touchpanel_i.h"

#pragma arm section rodata="MAGIC"
const __mods_mgsec_t modinfo =
{
    {'e','P','D','K','.','m','o','d'},		//.magic
    0x01000000,                				//.version
    EMOD_TYPE_DRV_TP,                       //.type
    0xF0000,                                //.heapaddr
    0x400,                      			//.heapsize
	{                                       //.mif
	    &DRV_TP_MInit,
	    &DRV_TP_MExit,
	    &DRV_TP_MOpen,
	    &DRV_TP_MClose,
	    &DRV_TP_MRead,
    	&DRV_TP_MWrite,
	    &DRV_TP_MIoctrl
	}
};
#pragma arm section
