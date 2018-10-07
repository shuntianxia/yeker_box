/*
*********************************************************************************************************
*                                                    ePDK
*                                    the Easy Portable/Player Develop Kits
*                                              ir keyboard driver
*
*                                (c) Copyright 2009-2010, kevin.z.m China
*                                              All Rights Reserved
*
* File    : ext_rtc.h
* By      : kevin.z.m
* Version : V1.00
* Date    : 2009-11-20 10:16
*********************************************************************************************************
*/

#ifndef __INT_RTC_H__
#define __INT_RTC_H__

#include "epdk.h"

//philips pcf8563 function
extern __s32 Int_rtc_init(void);
extern __s32 Int_SetDate(__u32 year, __u32 month, __u32 day);
extern __s32 Int_GetDate(__u32 *year, __u32 *month, __u32 *day);
extern __s32 Int_SetTime(__u32 hour, __u32 minute, __u32 second);
extern __s32 Int_GetTime(__u32 *hour, __u32 *minute, __u32 *second);
extern __s32 Int_AlarmEnable(void);
extern __s32 Int_AlarmDisable(void);
extern __s32 Int_AlarmInstIsr(void);
extern __s32 Int_AlarmUninstIsr(void);
extern __u32 Int_AlarmQueryINT(void);
extern __s32 Int_AlarmEnableINT(__u32 mode, __u32 attr);
extern __s32 Int_AlarmDisableINT(__u32 mode, __u32 attr);
extern __s32 Int_AlarmSetDuration(__u32 duration);
extern __u32 Int_AlarmQuery(void);


#endif //__EXT_RTC_H__

