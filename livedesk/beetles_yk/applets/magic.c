/*
*********************************************************************************************************
*											        ePDK
*						            the Easy Portable/Player Develop Kits
*									           hello world sample
*
*						        (c) Copyright 2006-2007, RICHARD, China
*											All	Rights Reserved
*
* File    : magic.c
* By      : Richard.x
* Version : V1.00
*********************************************************************************************************
*/
#include "epdk.h"

extern __s32 app_main(void *p_arg);
#pragma arm section rodata="MAGIC"
const __exec_mgsec_t exfinfo =
{
    {'e','P','D','K','.','e','x','f'},		//.magic
    0x01000000,                				//.version
    0,	               						//.type
    0x1F00000,                            	//.heapaddr
    0x400,                      			//.heapsize
    app_main,  						   	//.maintask
    0x4000,                      			//.mtskstksize
    KRNL_priolevel4               			//.mtskpriolevel
};
#pragma arm section

