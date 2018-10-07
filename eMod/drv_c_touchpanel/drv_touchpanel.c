/*
*********************************************************************************************************
*											        ePDK
*						            the Easy Portable/Player Develop Kits
*									           hello world sample
*
*						        (c) Copyright 2006-2007, Steven.ZGJ China
*											All	Rights Reserved
*
* File    : drv_touchpanel.c
* By      : jerry
* Version : V1.00
*********************************************************************************************************
*/
#include "drv_touchpanel_i.h"


#ifdef SIM_PC_WIN
typedef int (*Fun_tp_msg_cb)(void *msg);
extern  __declspec(dllimport) void SetTpMsgCallbackFun(Fun_tp_msg_cb cb_addr);
extern __declspec(dllimport) Fun_tp_msg_cb GetTpMsgCallbackFun();
#endif

__touchpanel_drv_t      touchpanel_drv;
_tp_private_data_set_t  tp_private;


static __s32 drv_tp_event(__input_dev_t *dev, __u32 type, __u32 code, __s32 value)
{
    __msg("keyborad feedback event [type:%d, code:%d, value:%d]\n",
                type, code, value);
 	return EPDK_OK;
}

static void input_set_abs_params(__input_dev_t *dev, int axis, int min, int max, int fuzz)
{
	__here__;
	dev->absmin[axis] = min;
	dev->absmax[axis] = max;
	dev->absfuzz[axis] = fuzz;

	dev->absbit[BIT_WORD(axis)] |= BIT_MASK(axis);
}

/*
****************************************************************************************************
*
*             DRV_IRKEY_MInit
*
*  Description:
*       DRV_IRKEY_MInit
*
*  Parameters:
*
*  Return value:
*       EPDK_OK
*       EPDK_FAIL
****************************************************************************************************
*/
__s32 DRV_TP_MInit(void)
{
    __wrn("TP MInit\n");
    __here__;
	eLIBs_memset(&touchpanel_drv, 0, sizeof(__touchpanel_drv_t));

    return EPDK_OK;
}
/*
****************************************************************************************************
*
*             DRV_IRKEY_MExit
*
*  Description:
*       DRV_IRKEY_MExit
*
*  Parameters:
*
*  Return value:
*       EPDK_OK
*       EPDK_FAIL
****************************************************************************************************
*/
__s32 DRV_TP_MExit(void)
{
    __wrn("TP MExit\n");
    __here__;
    eLIBs_memset(&touchpanel_drv, 0, sizeof(__touchpanel_drv_t));

    return EPDK_OK;
}
/*
****************************************************************************************************
*
*             DRV_IRKEY_MOpen
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
__mp *DRV_TP_MOpen(__u32 mid, __u32 mod)
{
    __s32 cpu_sr;

    __inf("TP MOpen = %d\n", mid);
    __here__;
    ENTER_CRITICAL(cpu_sr);
    if(touchpanel_drv.used)
    {
        __wrn("tp driver already openned by someone else!\n");
        EXIT_CRITICAL(cpu_sr);
        return (__mp *)0;
    }
    touchpanel_drv.used = 1;
    EXIT_CRITICAL(cpu_sr);

    touchpanel_drv.mid = mid;
    return (__mp *)&touchpanel_drv;
}

/*
****************************************************************************************************
*
*             DRV_IRKEY_MClose
*
*  Description:
*       DRV_IRKEY_MClose
*
*  Parameters:
*
*  Return value:
*       EPDK_OK
*       EPDK_FAIL
****************************************************************************************************
*/
__s32 DRV_TP_MClose(__mp *mp)
{
    __touchpanel_drv_t * drv = (__touchpanel_drv_t *)mp;
    __here__;
    __inf("TP MClose\n");

    if(drv->used == 0)
    {
        return EPDK_FAIL;
    }
    drv->used = 0;

	return EPDK_OK;
}

/*
****************************************************************************************************
*
*             DRV_IRKEY_MRead
*
*  Description:
*       DRV_IRKEY_MRead
*
*  Parameters:
*
*  Return value:
*       size*n
*
****************************************************************************************************
*/
__u32 DRV_TP_MRead(void *pdata, __u32 size, __u32 n, __mp *mp)
{
	__here__;
    __wrn("do nothing!\n");
    return 0;
}
/*
****************************************************************************************************
*
*             DRV_IRKEY_MWrite
*
*  Description:
*       DRV_IRKEY_MWrite
*
*  Parameters:
*
*  Return value:
*       size*n
*
****************************************************************************************************
*/
__u32 DRV_TP_MWrite(const void *pdata, __u32 size, __u32 n, __mp *mp)
{
	__here__;
    __wrn("do nothing!\n");
    return 0;
}
/*
****************************************************************************************************
*
*             DRV_IRKEY_MIoctrl
*
*  Description:
*       DRV_IRKEY_MIoctrl
*
*  Parameters:
*
*  Return value:
*       EPDK_OK
*       EPDK_FAIL
****************************************************************************************************
*/
__s32 DRV_TP_MIoctrl(__mp *mp, __u32 cmd, __s32 aux, void *pbuffer)
{
    __s32 cpu_sr = 0;
#ifndef SIM_PC_WIN
    switch(cmd)
    {
		case DRV_CMD_PLUGIN:
		{
		    __input_dev_t   *pTpDev = NULL;

		    __here__;
		    if(touchpanel_init() == EPDK_FAIL)
		    {
		        return EPDK_FAIL;
		    }

		    ENTER_CRITICAL(cpu_sr);
		    if(touchpanel_drv.ndev)
		    {
		        __wrn("Driver cannot support more than one tp board!\n");
		        EXIT_CRITICAL(cpu_sr);
		        return EPDK_FAIL;
		    }
		    touchpanel_drv.ndev = 1;
		    EXIT_CRITICAL(cpu_sr);

		    pTpDev = (__input_dev_t *)eLIBs_malloc(sizeof(__input_dev_t));
		    if(NULL == pTpDev)
		    {
                __wrn("allocate memory for input device hanle fail!\n");
                touchpanel_exit();
                return EPDK_FAIL;
		    }

		    eLIBs_memset(pTpDev, 0, sizeof(__input_dev_t));

            /* general managment system information */
		    pTpDev->classname = INPUT_CLASS_NAME;
		    pTpDev->name      = INPUT_TP_DEV_NAME;
		    pTpDev->ops       = &touchpanel_dev_ops;
		    pTpDev->parg      = &(touchpanel_drv.tp_dev);/* for device manager */

		    pTpDev->id.product = 0x0001;
		    pTpDev->id.version = 0x0001;

            /* Touch Panel device */
		    pTpDev->device_class = INPUT_TS_CLASS;

            /* set keyboard support event types */
		    pTpDev->evbit[0]  = BIT_MASK(EV_KEY) | BIT_MASK(EV_ABS) | BIT_MASK(EV_SYN);

            /* set key support event codes */
	//	    __set_bit(BTN_TOUCH, pTpDev->keybit);
            /* set tp abs support event codes */
		    input_set_abs_params(pTpDev, ABS_X, 0, 0x7ff, 0);
		    input_set_abs_params(pTpDev, ABS_Y, 0, 0x7ff, 0);
		    input_set_abs_params(pTpDev, ABS_RUDDER, 0, 20, 0);/* direction */
		    input_set_abs_params(pTpDev, ABS_BRAKE, 0, 20, 0);/* speed */
		    input_set_abs_params(pTpDev, ABS_MISC, 0xf0, 0xf2, 0); /* compatible with old one */

		    /* set key event handler, handle the feedback events */
		    pTpDev->event = drv_tp_event;

		    /* register keyboard device to INPUT managemant system */
		    if(esINPUT_RegDev(pTpDev) != EPDK_OK)
		    {
                __wrn("register tp device to input system failed\n");
                eLIBs_free(pTpDev);
	        	return EPDK_FAIL;
		    }
		    /* attach pKeycDev to keydrv */
		    touchpanel_drv.tp_input_dev = pTpDev;

		    __wrn("TP Plugin OK\n");
		    return EPDK_OK;
		}
		case DRV_CMD_PLUGOUT:
		{
		    touchpanel_exit();
            /* unregister keyboard deivce from INPUT manage system,
             * free keyboard device handle.
             */
            if (touchpanel_drv.ndev)
            {
                __wrn("tp is using now\n");
                return EPDK_FAIL;
            }
            esINPUT_UnregDev(touchpanel_drv.tp_input_dev);
            eLIBs_free(touchpanel_drv.tp_input_dev);

            __wrn("TP Plugout OK\n");
            return EPDK_OK;
		}
		case DRV_CMD_GET_STATUS:
		{
		    if(touchpanel_drv.ndev)
		    {
		        return DRV_STA_BUSY;
		    }
		    else
		    {
		        return DRV_STA_FREE;
		    }
		}
		default:
			break;
	}
#else
    switch(cmd)
    {
		case DRV_CMD_PLUGIN:
		{
		    __input_dev_t   *pTpDev = NULL;

		    if(touchpanel_init() == EPDK_FAIL)
		    {
		        return EPDK_FAIL;
		    }

		    ENTER_CRITICAL(cpu_sr);
		    if(touchpanel_drv.ndev)
		    {
		        __wrn("Driver cannot support more than one tp board!\n");
		        EXIT_CRITICAL(cpu_sr);
		        return EPDK_FAIL;
		    }
		    touchpanel_drv.ndev = 1;
		    EXIT_CRITICAL(cpu_sr);

		    pTpDev = (__input_dev_t *)eLIBs_malloc(sizeof(__input_dev_t));
		    if(NULL == pTpDev)
		    {
                __wrn("allocate memory for input device hanle fail!\n");
                touchpanel_exit();
                return EPDK_FAIL;
		    }

		    eLIBs_memset(pTpDev, 0, sizeof(__input_dev_t));

            /* general managment system information */
		    pTpDev->classname = INPUT_CLASS_NAME;
		    pTpDev->name      = INPUT_TP_DEV_NAME;
		    pTpDev->ops       = &touchpanel_dev_ops;
		    pTpDev->parg      = &(touchpanel_drv.tp_dev);/* for device manager */

		    pTpDev->id.product = 0x0001;
		    pTpDev->id.version = 0x0001;

            /* Touch Panel device */
		    pTpDev->device_class = INPUT_TS_CLASS;

            /* set keyboard support event types */
		    pTpDev->evbit[0]  = BIT_MASK(EV_KEY) | BIT_MASK(EV_ABS) | BIT_MASK(EV_SYN);

            /* set key support event codes */
	//	    __set_bit(BTN_TOUCH, pTpDev->keybit);
            /* set tp abs support event codes */
		    input_set_abs_params(pTpDev, ABS_X, 0, 0x7ff, 0);
		    input_set_abs_params(pTpDev, ABS_Y, 0, 0x7ff, 0);
		    input_set_abs_params(pTpDev, ABS_RUDDER, 0, 20, 0);/* direction */
		    input_set_abs_params(pTpDev, ABS_BRAKE, 0, 20, 0);/* speed */
		    input_set_abs_params(pTpDev, ABS_MISC, 0xf0, 0xf2, 0); /* compatible with old one */

		    /* set key event handler, handle the feedback events */
		    pTpDev->event = drv_tp_event;

		    /* register keyboard device to INPUT managemant system */
		    if(esINPUT_RegDev(pTpDev) != EPDK_OK)
		    {
                __wrn("register tp device to input system failed\n");
                eLIBs_free(pTpDev);
	        	return EPDK_FAIL;
		    }
		    /* attach pKeycDev to keydrv */
		    touchpanel_drv.tp_input_dev = pTpDev;

		    __wrn("TP Plugin OK\n");
		    return EPDK_OK;
		}
		case DRV_CMD_PLUGOUT:
		{
		    touchpanel_exit();
            /* unregister keyboard deivce from INPUT manage system,
             * free keyboard device handle.
             */
            if (touchpanel_drv.ndev)
            {
                __wrn("tp is using now\n");
                return EPDK_FAIL;
            }
            esINPUT_UnregDev(touchpanel_drv.tp_input_dev);
            eLIBs_free(touchpanel_drv.tp_input_dev);

            __wrn("TP Plugout OK\n");
            return EPDK_OK;
		}
		case DRV_CMD_GET_STATUS:
		{
		    if(touchpanel_drv.ndev)
		    {
		        return DRV_STA_BUSY;
		    }
		    else
		    {
		        return DRV_STA_FREE;
		    }
		}
		default:
			break;
	}

#endif
	return EPDK_FAIL;
}




