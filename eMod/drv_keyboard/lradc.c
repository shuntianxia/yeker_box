/*
*********************************************************************************************************
*                                                    ePDK
*                                    the Easy Portable/Player Develop Kits
*                                             audio driver Module
*
*                                    (c) Copyright 2008-2009, Jerry China
*                                             All Rights Reserved
*
* File    : lradck.c
* By      : jerry
* Version : V1.0
* Date    : 2009-4-13
*********************************************************************************************************
*/
#include "drv_key_i.h"
#include "lradc.h"


LRADC_KEY_SCANCODE *solution_keymapping = NULL;

static KEYC_SETTING *pLradck = NULL;
//#define REG_RD32(reg)    (*((volatile __u32 *)(reg)))
/* solution key mapping */
static LRADC_KEY_SCANCODE pvp_keymapping[]={
/*   { Min,      Max,        Keycode         } */ 
    {0,     7,      KPAD_LEFT     },	// for C500 PER2 VOL+
	{8, 	14,	    KPAD_RIGHT     },	// for C500 PER2 VOL-
	{15, 	20,  	KPAD_MENU},
	{21,	26, 	KPAD_ENTER},			// for C500 PER2 ENTER
	{26,	33, 	KPAD_UP},				// for C500 PER2 HOME
//	{28, 	33, 	KPAD_UP        },
//	{34, 	39, 	KPAD_DOWN      },	
//	{40, 	46, 	KPAD_MENU      },	
//	{47, 	63, 	KPAD_RETURN    }
};
__s32 get_keymap_size(void)
{
    return sizeof(pvp_keymapping)/sizeof(pvp_keymapping[0]);    
}

/*
****************************************************************************************************
*
*  FunctionName:           lradckey_keydown
*
*  Description:
*              按键按下，调用ioctrl发送按键按下消息。回调函数。   
*
*  Parameters:
*       keyvalue :  按键值
*
*  Return value:
*            无。
*  Notes:
*           
****************************************************************************************************
*/
static  void lradckey_keydown(__u32 keyvalue)
{
    __key_msg_t msg;
    msg.key_value = keyvalue;
    msg.flag = 1;
	
	//eLIBs_printf("adda reg value =%x\n",REG_RD32(0xf1c23400+0x0c));
	
    DEV_KEYC_Ioctrl(0, DRV_KEY_CMD_PUTKEY, 0, &msg);
  
	return;
}

/*
****************************************************************************************************
*
*  FunctionName:           lradckey_keyup
*
*  Description:
*              按键抬起，调用ioctrl发送按键抬起消息。回调函数。   
*
*  Parameters:
*       keyvalue :  按键值
*
*  Return value:
*            无。
*  Notes:
*           
****************************************************************************************************
*/
static  void lradckey_keyup(__u32 keyvalue)
{
    __key_msg_t msg;
    msg.key_value = keyvalue;
    msg.flag = 0;

    DEV_KEYC_Ioctrl(0, DRV_KEY_CMD_PUTKEY, 0, &msg);

}

/*
****************************************************************************************************
*
*  FunctionName:           Lradc_key_Init
*
*  Description:
*             lradc 按键初始化。
*  Parameters:
*       无。
*
*  Return value:
*             成功返回EPDK_OK,失败返回EPDK_FAIL.
*  Notes:
*           
****************************************************************************************************
*/
__s32 Lradc_key_Init(void)
{  
    if(pLradck == NULL)
    {
        pLradck = (KEYC_SETTING *)eLIBs_malloc(sizeof(KEYC_SETTING));
        if(pLradck == NULL)
        {
            __err("keyc driver memory malloc error!\n");
            return EPDK_FAIL;
        }
    	eLIBs_memset(pLradck, 0, sizeof(KEYC_SETTING));
    }
    else
    {
        __wrn("lradc key has been initial!\n");
        return EPDK_FAIL;
    }
    
    //initial lradc key 
    pLradck->channel = LRADC_ONE_CHANNEL;
    
    pLradck->irq_flags    = 0;
    pLradck->irq_no       = LRADC_IRQ_NUM;
    pLradck->irq_prio     = 0;
    
    pLradck->key_adc  = LRADC_USED_ADC0;// ADC0
    pLradck->key_down = lradckey_keydown;
    pLradck->key_up   = lradckey_keyup;       

    pLradck->sample   = LRADC_SAMPLE_32HZ;// 32ms
    pLradck->voltageB = LRADC_LEVELB_1_8V;
    
    pLradck->pKeycode   = pvp_keymapping;
    solution_keymapping = pvp_keymapping;
    pLradck->kc_size    = sizeof(pvp_keymapping)/sizeof(pvp_keymapping[0]);
    
    
    pLradck->first_key_time  = 2; // 32ms*2 = 64ms
    pLradck->first_key_delay = 5; // 32ms*5 = 160ms    
    pLradck->repeat_key_time = 3; // 32ms*3 = 96ms
    
    pLradck->line_notifier = NULL;//none
    pLradck->line_adc      = LRADC_NOTUSED_ADC01;

    BSP_LRADCK_init(pLradck);
	return EPDK_OK;
}

/*
****************************************************************************************************
*
*  FunctionName:           Lradc_key_Exit
*
*  Description:
*              释放资源，调用bsp退出函数。
*
*  Parameters:
*       无。
*
*  Return value:
*            成功返回EPDK_OK,失败返回EPDK_FAIL
*  Notes:
*           
****************************************************************************************************
*/
__s32 Lradc_key_Exit(void)
{
    __inf("lradc key drv exit!\n");
	if(pLradck)
	{
	    __s32 ret;
	    ret = BSP_LRADCK_exit(pLradck);
	    if(ret == -1)
	    {
            __wrn("key exit failed!\n");
	    }
        eLIBs_free(pLradck);
        pLradck = NULL;
        return EPDK_OK;
	}
	return  EPDK_FAIL;
}

