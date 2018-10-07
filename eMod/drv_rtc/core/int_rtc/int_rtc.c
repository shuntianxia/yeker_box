/*
*********************************************************************************************************
*                                                    ePDK
*                                    the Easy Portable/Player Develop Kits
*                                              ir keyboard driver
*
*                                (c) Copyright 2009-2010, kevin.z.m China
*                                              All Rights Reserved
*
* File    :  int_rtc.h
* By      : kevin.z.m
* Version : V1.00
* Date    : 2009-11-20 10:16
*********************************************************************************************************
*/

#include "int_rtc.h"

typedef struct
{
	__u32 year;
	__u32 month;
	__u32 day;
	__u32 hour;
	__u32 minute;
	__u32 second;
	__u32 msecond;
}__int_rtc_t;

static __int_rtc_t int_rtc;
static __s32  time_hdl = NULL;

static void int_rtc_timeout_proc(void *arg)
{
	arg = arg;
	int_rtc.msecond++;
	if (int_rtc.msecond > 10)
	{
		int_rtc.msecond = 0;
		int_rtc.second++;
		if (int_rtc.second > 59)
		{
			int_rtc.second = 0;
			int_rtc.minute++;
			if (int_rtc.minute > 59)
			{
				int_rtc.minute = 0;
				int_rtc.hour++;
				if (int_rtc.hour > 23)
					int_rtc.hour = 0;
			}
		}
	}
}

__s32 Int_rtc_init(void)
{
	__hw_tmr_type_t hw_tmr_type;
	
	int_rtc.year = 2017;
	int_rtc.month = 1;
	int_rtc.day = 1;
	int_rtc.hour = 0;
	int_rtc.minute = 0;
	int_rtc.second = 0;
	
	hw_tmr_type.precision = CSP_TMRC_TMR_PRECISION_MILLI_SECOND;
	hw_tmr_type.leastCount = 1;
	time_hdl = esTIME_RequestTimer(&hw_tmr_type, (__pCBK_t)int_rtc_timeout_proc, NULL, NULL);
	esTIME_StartTimer(time_hdl, 91, CSP_TMRC_TMR_MODE_CONTINU);
	
	return EPDK_OK;
}

__s32 Int_SetDate(__u32 year, __u32 month, __u32 day)
{
	int_rtc.year = year;
	int_rtc.month = month;
	int_rtc.day = day;
	
	return EPDK_OK;
}

__s32 Int_GetDate(__u32 *year, __u32 *month, __u32 *day)
{
	*year = int_rtc.year ;
	*month = int_rtc.month;
	*day = int_rtc.day;
	return EPDK_OK;
}

__s32 Int_SetTime(__u32 hour, __u32 minute, __u32 second)
{
	int_rtc.hour = hour;
	int_rtc.minute = minute;
	int_rtc.second = second;
	return EPDK_OK;
}

__s32 Int_GetTime(__u32 *hour, __u32 *minute, __u32 *second)
{
	__here__;
	*hour = int_rtc.hour;
	*minute = int_rtc.minute;
	*second = int_rtc.second;
	return EPDK_OK;
}

__s32 Int_AlarmEnable(void)
{
	 return EPDK_FAIL;
}

__s32 Int_AlarmDisable(void)
{
	 return EPDK_FAIL;
}

__s32 Int_AlarmInstIsr(void)
{
	 return EPDK_FAIL;
}
__s32 Int_AlarmUninstIsr(void)
{
	 return EPDK_FAIL;
}

__u32 Int_AlarmQueryINT(void)
{
	 return EPDK_FAIL;
}

__s32 Int_AlarmEnableINT(__u32 mode, __u32 attr)
{
	 return EPDK_FAIL;
}

__s32 Int_AlarmDisableINT(__u32 mode, __u32 attr)
{
	 return EPDK_FAIL;
}

__s32 Int_AlarmSetDuration(__u32 duration)
{
	 return EPDK_FAIL;
}

__u32 Int_AlarmQuery(void)
{
	 return EPDK_FAIL;
}

