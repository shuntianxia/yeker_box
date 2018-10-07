/*
**************************************************************************************************************
*											         ePDK
*						            the Easy Portable/Player Develop Kits
*									           desktop system 
*
*						        	 (c) Copyright 2007-2010, ANDY, China
*											 All Rights Reserved
*
* File    	: mod_init_i.h
* By      	: Andy.zhang
* Func		: 
* Version	: v1.0
* ============================================================================================================
* 2010-9-25 17:10:55  andy.zhang  create this file, implements the fundemental interface;
**************************************************************************************************************
*/

#include "mod_init.h"
#include "init.h"

__init_mod_t init_data;


__s32 INIT_MInit(void)
{
    return EPDK_OK;
}


__s32 INIT_MExit(void)
{
    return EPDK_OK;
}


__mp *INIT_MOpen(__u32 mid, __u32 mod)
{      
    __msg("----------------------INIT_MOpen --------------------------\n");
    init_data.mid = mid;
    init_data.reserved = 0;
    process_init();
    __msg("----------------------INIT_MOpen end --------------------------\n");
    
    return (__mp *)&init_data;
}


__s32 INIT_MClose(__mp *mp)
{    
    return EPDK_OK;
}


__u32 INIT_MRead(void *pdata, __u32 size, __u32 n, __mp *mp)
{
    return size * n;
}

__u32 INIT_MWrite(const void *pdata, __u32 size, __u32 n, __mp *mp)
{   
    return size * n;
}

__s32 INIT_MIoctrl(__mp *mp, __u32 cmd, __s32 aux, void *pbuffer)
{   
    return EPDK_OK;
}    



