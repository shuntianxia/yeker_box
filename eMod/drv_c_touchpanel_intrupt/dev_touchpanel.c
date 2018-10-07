/*
*********************************************************************************************************
*											        ePDK
*						            the Easy Portable/Player Develop Kits
*									           touchpanel driver
*
*						        (c) Copyright 2006-2007, Steven.ZGJ China
*											All	Rights Reserved
*
* File    : dev_touchpanel.c
* By      : Jerry
* Version : V1.00
*********************************************************************************************************
*/

#include "drv_touchpanel_i.h"

/*
****************************************************************************************************
*
*             DEV_KEY_Open
*
*  Description:
*       DRV_MOpen
*
*  Parameters:
*
*  Return value:
*       EPDK_OK
*       EPDK_FAIL
****************************************************************************************************
*/
__hdle DEV_TP_Open(void * open_arg, __u32 mode)
{
    __touchpanel_dev_t  *tp_dev = (__touchpanel_dev_t *)open_arg;
    __s32 cpu_sr = 0;
    __here__;
    if(tp_dev == NULL)
    {
        __wrn("open para is NULL!\n");
        return (__hdle)0;
    }
    
    if(tp_dev->used == 0)
    {
        ENTER_CRITICAL(cpu_sr);
        tp_dev->used = 1;
        EXIT_CRITICAL(cpu_sr);
    }
    else
    {
        __wrn("tp dev have already opened by someone!\n");
        return (__hdle)0;
    }

    return (__hdle)tp_dev;
}

/*
****************************************************************************************************
*
*             DEV_KEY_Close
*
*  Description:
*       DRV_irkey_MClose
*
*  Parameters:
*
*  Return value:
*       EPDK_OK
*       EPDK_FAIL
****************************************************************************************************
*/
__s32 DEV_TP_Close(__hdle hTouchPanel)
{
    __touchpanel_dev_t  *tp_dev = (__touchpanel_dev_t *)hTouchPanel;
    __s32 cpu_sr = 0;
    __here__;
    if(tp_dev == NULL)
    {
        __wrn("hdle is NULL!\n");
        return EPDK_FAIL;
    }
    
    if(tp_dev->used == 1)
    {
        ENTER_CRITICAL(cpu_sr);
        tp_dev->used = 0;
        EXIT_CRITICAL(cpu_sr);
    }
    else
    {
        __wrn("tp dev doesn't opened!\n");
        return EPDK_FAIL;
    }
    
	return EPDK_OK;
}
/*
****************************************************************************************************
*
*             DEV_KEY_Read
*
*  Description:
*       DRV_irkey_MRead
*
*  Parameters:
*
*  Return value:
*       size*n
*
****************************************************************************************************
*/
__u32 DEV_TP_Read(void *pdata, __u32 size, __u32 n, __hdle hTouchPanel)
{
    __here__;
    return 0;
}
/*
****************************************************************************************************
*
*             DEV_KEY_Write
*
*  Description:
*       DRV_irkey_MWrite
*
*  Parameters:
*
*  Return value:
*       size*n
*
****************************************************************************************************
*/
__u32 DEV_TP_Write(const void *pdata, __u32 size, __u32 n, __hdle hTouchPanel)
{
    return size * n;
}
/*
****************************************************************************************************
*
*             DEV_KEY_Ioctrl
*
*  Description:
*       DRV_irkey_MIoctrl
*
*  Parameters:
*
*  Return value:
*       EPDK_OK
*       EPDK_FAIL
****************************************************************************************************
*/
__s32 DEV_TP_Ioctrl(__hdle hTouchPanel, __u32 cmd, __s32 aux, void *pbuffer)
{
    __touchpanel_dev_t  *tp_dev = (__touchpanel_dev_t *)hTouchPanel;
    __s32           ret = EPDK_OK;
    __s32           cpu_sr;

    if(tp_dev == NULL)
    {
        __wrn("hdle is null \n");
        return EPDK_FAIL;
    }

    switch(cmd)
    {
        case DRV_TP_CMD_SET_OFFSET_INFO:
            tp_info.offset = aux;
            break;
        case DRV_TP_CMD_GET_OFFSET_INFO:
            ret = tp_info.offset;
            break;
            
        case DRV_TP_CMD_SET_MSG_PERTIME:
            tp_info.pertime = aux;
            break;
        case DRV_TP_CMD_GET_MSG_PERTIME:
            ret = tp_info.pertime;
            break;
        //暂时这个命令还是空的
        case DRV_TP_CMD_SET_WORKMODE:
            ENTER_CRITICAL(cpu_sr);
            EXIT_CRITICAL(cpu_sr);
            break;

        default:
        {
            __wrn("Unknown TP Command\n");
            break;
        }
    }
	return ret;
}

__dev_devop_t   touchpanel_dev_ops =
{
    DEV_TP_Open,
    DEV_TP_Close,
    DEV_TP_Read,
    DEV_TP_Write,
    DEV_TP_Ioctrl
};
