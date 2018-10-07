/*
************************************************************************************************************************
*                                            detect gate in register table
*
*                                  Copyright(C), 2006-2010, AllWinner Technology Co., Ltd.
*											       All Rights Reserved
*
* File Name   : reg_detect_gate.c
*
* Author      : Gary.Wang
*
* Version     : 1.0
*
* Date        : 2010.08.31
*
* Description :
* 
* Others      : None at present.
*
*
* History     :
*
*  <Author>        <time>       <version>      <description>
*
* Gary.Wang      2010.08.31        1.0         build the file
*
************************************************************************************************************************
*/
#ifndef  __REG_DETECT_GATE_C__
#define  __REG_DETECT_GATE_C__


#include "..\\reg_i.h"


#define REG_DETECT_GATE_REGNODE             "\\system\\detect_gate"
#define REG_DETECT_GATE_SET                 "detect_gate"
#define REG_DETECT_GATE_KEY_CURRENT         "current"
#define REG_DETECT_GATE_KEY_DEFAULT         "default"


/*
****************************************************************************************************
*Name        : lion_reg_detect_gate_get
*Prototype   : __bool lion_reg_detect_gate_get( void )
*Arguments   : void
*Return      : the current detect gate in register table.
*Description : get the current detect gate in register table.
*Other       :
****************************************************************************************************
*/
__bool lion_reg_detect_gate_get( void )
{
    __hdle  h_node = NULL;
    char    key_value[ELIBS_KEY_VALUE_LEN+1];   
    __bool  ret;
    
    /* open reg node */
	h_node = eLIBs_RegOpenNode( REG_DETECT_GATE_REGNODE, ELIBS_REG_READONLY );
	if( h_node == NULL )
	{
		__wrn("Warning! Reg node %s cannot be opened!\n", REG_DETECT_GATE_REGNODE);
	    goto error;
	}
    
    /* get key value */
    eLIBs_RegGetKeyValue( h_node, REG_DETECT_GATE_SET, REG_DETECT_GATE_KEY_CURRENT, key_value );
    
    /* convert the format of the key value */
    ret = gatemap_name2enm( key_value );
    
    /* close reg node */
    eLIBs_RegCloseNode( h_node );
    h_node = NULL;
    
    return ret;
    
error:
    if( h_node != NULL )
    {
        eLIBs_RegCloseNode( h_node );
        h_node = NULL;
    }
    return EPDK_TRUE;
}



/*
****************************************************************************************************
*Name        : lion_reg_detect_gate_set
*Prototype   : __s32 lion_reg_detect_gate_set( __bool detect_gate )
*Arguments   : detect_gate  input. detect gate.
*Return      : EPDK_OK    succeed
*              EPDK_FAIL  fail
*Description : set the current detect gate in register table.
*Other       :
****************************************************************************************************
*/
__s32 lion_reg_detect_gate_set( __bool detect_gate )
{
    __hdle  h_node = NULL;
    char    key_value[ELIBS_KEY_VALUE_LEN+1];   
    __s32   ret;
    
    /* open reg node */
	h_node = eLIBs_RegOpenNode( REG_DETECT_GATE_REGNODE, ELIBS_REG_READWRITE );
	if( h_node == NULL )
	{
		__wrn("Warning! Reg node %s cannot be opened!\n", REG_DETECT_GATE_REGNODE);
	    goto error;
	}
    
    /* convert the format of the key value */
    ret = gatemap_enm2name( detect_gate, key_value, ELIBS_KEY_VALUE_LEN );
    if( ret != 0 )
    {
        goto error;
    }
    
    /* set key value */
    eLIBs_RegSetKeyValue( h_node, REG_DETECT_GATE_SET, REG_DETECT_GATE_KEY_CURRENT, key_value );
    
    /* close reg node */
    eLIBs_RegCloseNode( h_node );
    h_node = NULL;
    
    return EPDK_OK;
    
error:
    if( h_node != NULL )
    {
        eLIBs_RegCloseNode( h_node );
        h_node = NULL;
    }
    return EPDK_FAIL;
}



/*
****************************************************************************************************
*Name        : lion_reg_detect_gate_get_default
*Prototype   : __bool lion_reg_detect_gate_get_default( void )
*Arguments   : void
*Return      : the default detect gate in register table.
*Description : get the default detect gate in register table.
*Other       :
****************************************************************************************************
*/
__bool lion_reg_detect_gate_get_default( void )
{
    __hdle  h_node = NULL;
    char    key_value[ELIBS_KEY_VALUE_LEN+1];   
    __bool  ret;
    
    /* open reg node */
	h_node = eLIBs_RegOpenNode( REG_DETECT_GATE_REGNODE, ELIBS_REG_READONLY );
	if( h_node == NULL )
	{
		__wrn("Warning! Reg node %s cannot be opened!\n", REG_DETECT_GATE_REGNODE);
	    goto error;
	}
    
    /* get key value */
    eLIBs_RegGetKeyValue( h_node, REG_DETECT_GATE_SET, REG_DETECT_GATE_KEY_DEFAULT, key_value );
    
    /* convert the format of the key value */
    ret = gatemap_name2enm( key_value );
    
    /* close reg node */
    eLIBs_RegCloseNode( h_node );
    h_node = NULL;
    
    return ret;
    
error:
    if( h_node != NULL )
    {
        eLIBs_RegCloseNode( h_node );
        h_node = NULL;
    }
    return EPDK_TRUE;
}



/*
****************************************************************************************************
*Name        : lion_reg_detect_gate_restore
*Prototype   : __s32 lion_reg_detect_gate_restore( void )
*Arguments   : void
*Return      : EPDK_OK    succeed
*              EPDK_FAIL  fail
*Description : restore the default value of detect gate in register table.
*Other       :
****************************************************************************************************
*/
__s32 lion_reg_detect_gate_restore( void )
{
    __hdle  h_node = NULL;
    char    key_value[ELIBS_KEY_VALUE_LEN+1];   
    
    /* open reg node */
	h_node = eLIBs_RegOpenNode( REG_DETECT_GATE_REGNODE, ELIBS_REG_READWRITE );
	if( h_node == NULL )
	{
		__wrn("Warning! Reg node %s cannot be opened!\n", REG_DETECT_GATE_REGNODE);
	    goto error;
	}

    /* get default value */
    eLIBs_RegGetKeyValue( h_node, REG_DETECT_GATE_SET, REG_DETECT_GATE_KEY_DEFAULT, key_value );

    /* set current value */
    eLIBs_RegSetKeyValue( h_node, REG_DETECT_GATE_SET, REG_DETECT_GATE_KEY_CURRENT, key_value );
    
    /* close reg node */
    eLIBs_RegCloseNode( h_node );
    h_node = NULL;
    
    return EPDK_OK;
    
error:
    if( h_node != NULL )
    {
        eLIBs_RegCloseNode( h_node );
        h_node = NULL;
    }
    return EPDK_FAIL;
}



#endif     //  ifndef __REG_DETECT_GATE_C__

/* end of reg_detect_gate.c */
