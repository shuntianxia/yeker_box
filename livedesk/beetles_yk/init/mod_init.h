/*
**************************************************************************************************************
*											         ePDK
*						            the Easy Portable/Player Develop Kits
*									           init system 
*
*						        	 (c) Copyright 2007-2010, ANDY, China
*											 All Rights Reserved
*
* File    	: mod_init.h
* By      	: Andy.zhang
* Func		: 
* Version	: v1.0
* ============================================================================================================
* 2010-9-25 17:09:34  andy.zhang  create this file, implements the fundemental interface;
**************************************************************************************************************
*/

#ifndef __MOD_INIT_H__
#define __MOD_INIT_H__

#include "apps.h"


typedef struct
{
    __u32 mid; 
    __u32 reserved;
}__init_mod_t; 



__s32 INIT_MInit(void);
__s32 INIT_MExit(void);
__mp* INIT_MOpen(__u32 mid, __u32 mode);
__s32 INIT_MClose(__mp *mp);
__u32 INIT_MRead(void *pdata, __u32 size, __u32 n, __mp *mp); 
__u32 INIT_MWrite(const void *pdata, __u32 size, __u32 n, __mp *mp);
__s32 INIT_MIoctrl(__mp *mp, __u32 cmd, __s32 aux, void *pbuffer);


#endif /* __MOD_INIT_H__ */


