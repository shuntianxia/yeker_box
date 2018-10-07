/*
*********************************************************************************************************
*                                                    ePDK
*                                    the Easy Portable/Player Develop Kits
*                                             audio driver Module
*
*                                    (c) Copyright 2009-2010, kevin China
*                                             All Rights Reserved
*
* File    : ir_remote.c
* By      : victor
* Version : V1.0
* Date    : 2010-12-31
*********************************************************************************************************
*/

#include "drv_ir_remote_i.h"
#include "ir_remote.h"
#include "ir_remote_keymap.h"

 __u32 *ir_solution_keymapping = NULL;

__ir_key_setting_t *pIrkey = NULL;


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
static  void Irkey_keydown(__u32 keyvalue)
{
    __ir_key_msg_t msg;
    msg.key_value = ir_solution_keymapping[keyvalue];
    msg.flag = 1;

    DEV_IR_KEY_Ioctrl(0, DRV_IRKEY_CMD_PUTKEY, 0, &msg);
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
static  void Irkey_keyup(__u32 keyvalue)
{
    __ir_key_msg_t msg;
    msg.key_value = ir_solution_keymapping[keyvalue];
    msg.flag = 0;

    DEV_IR_KEY_Ioctrl(0, DRV_IRKEY_CMD_PUTKEY, 0, &msg);
    return;
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
__s32 Ir_key_Init(void)
{
    if(pIrkey == NULL)
    {
        pIrkey = (__ir_key_setting_t *)eLIBs_malloc(sizeof(KEYC_SETTING));
        if(pIrkey == NULL)
        {
            __err("ir key driver memory malloc error!\n");
            return EPDK_FAIL;
        }
    	eLIBs_memset(pIrkey, 0, sizeof(__ir_key_setting_t));
    }
    else
    {
        __wrn("ir key has been initial already!\n");
        return EPDK_FAIL;
    }
    
    //initial ir key 
    pIrkey->irq_flags    = 0;
    pIrkey->irq_no       = IR_IRQ_NUM;
    pIrkey->irq_prio     = 0;    
    
    pIrkey->remote_controller = NEC_REMOTE_CONTROLLER;
    pIrkey->user_code         = NEC_IR_ADDR_CODE1;/* 如果为0则认为不检查用户码 */
    pIrkey->holdkey           = 0;
    
    pIrkey->rp_keytime = 130; /* repeat key interval times: 95ms */
    pIrkey->keyup_times = 3; /* detect key up times: 连续按285ms抬起,单键按190ms抬起 ms*/
    
    pIrkey->key_down = Irkey_keydown;
    pIrkey->key_up   = Irkey_keyup;       

    if(pIrkey->user_code == NEC_IR_ADDR_CODE1)
    {
        ir_keymap[3]  = KPAD_MENU;
        ir_keymap[13] = KPAD_RETURN;/* learning */
        ir_keymap[18] = KPAD_UP;
        ir_keymap[20] = KPAD_RIGHT;
        ir_keymap[21] = KPAD_ENTER;
        ir_keymap[22] = KPAD_LEFT;
        ir_keymap[24] = KPAD_DOWN;        
    }
    ir_solution_keymapping = ir_keymap;
    
   return BSP_IR_key_init(pIrkey);
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
__s32 Ir_key_Exit(void)
{	
    __inf("lradc key drv exit!\n");
	if(pIrkey)
	{
	    BSP_IR_key_exit(pIrkey);
	    
        eLIBs_free(pIrkey);
        pIrkey = NULL;
        return EPDK_OK;
	}
	return  EPDK_FAIL;
}
