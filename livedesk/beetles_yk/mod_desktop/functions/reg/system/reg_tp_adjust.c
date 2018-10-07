/*
*********************************************************************************************************
*											        ePDK
*						            the Easy Portable/Player Develop Kits
*									           calendar app sample
*
*						        	(c) Copyright 2006-2009, ANDY, China
*											 All Rights Reserved
*
* File    : volume_reg.c
* By      :lyn
* Version : V1.00
*********************************************************************************************************
*/

#include "..\\reg_i.h"

#define REGNODE_ADJUST  	"\\system\\tp_adjust"

/*
**********************************************************************************************************************
*                                               lion_get_adjust_flag
*
* Description: 
*
* Arguments  : 
*
* Returns    : 
*
* Notes      : 
*
**********************************************************************************************************************
*/
__u32 lion_tp_adjust_flag_get(void)
{
 	__hdle                  h_node;
    char  key_value[ ELIBS_KEY_VALUE_LEN+1];
    __u32 flag;
    
	h_node = eLIBs_RegOpenNode(REGNODE_ADJUST, ELIBS_REG_READWRITE);
	if( h_node == NULL )
	{
		__wrn("reg node cannot be opened!\n");
		return 0xffffffff;
	}

    if(eLIBs_RegGetKeyValue(h_node, "adjust", "adjust", key_value) == EPDK_FAIL)
	{
		eLIBs_RegCloseNode(h_node);
		return 0xffffffff;
	};
	
    flag = key_value[0] - 0x30;    
    eLIBs_RegCloseNode(h_node);
	
    return flag;
}



/*
**********************************************************************************************************************
*                                               lion_set_adjust_flag
*
* Description: 
*
* Arguments  : 
*
* Returns    : 
*
* Notes      : 
*
**********************************************************************************************************************
*/
__s32 lion_tp_adjust_flag_set(__u32 flag)
{
    __hdle  h_node;
    
    char  key_value[ ELIBS_KEY_VALUE_LEN+1];
    
	h_node = eLIBs_RegOpenNode(REGNODE_ADJUST, ELIBS_REG_READWRITE);
	if( h_node == NULL )
	{
		__wrn("reg node cannot be opened!\n");
		return EPDK_FAIL;
	}

	key_value[0] = flag+0x30;
	key_value[1] = '\0';
	
    eLIBs_RegSetKeyValue(h_node, "adjust", "adjust", key_value);
    
    eLIBs_RegCloseNode(h_node); 
	
    return EPDK_OK;
}

