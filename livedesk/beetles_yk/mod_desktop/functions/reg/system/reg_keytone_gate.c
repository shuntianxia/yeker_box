/*
************************************************************************************************************************
*                                            keytone gate in register table
*
*                                  Copyright(C), 2006-2010, AllWinner Technology Co., Ltd.
*											       All Rights Reserved
*
* File Name   : reg_keytone_gate.c
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
#ifndef  __REG_KEYTONE_GATE_C__
#define  __REG_KEYTONE_GATE_C__


#include "..\\reg_i.h"


#define REG_KEYTONE_GATE_REGNODE             "\\system\\keytone_gate"
#define REG_KEYTONE_GATE_SET                 "keytone_gate"
#define REG_KEYTONE_GATE_KEY_CURRENT         "current"
#define REG_KEYTONE_GATE_KEY_DEFAULT         "default"



/*
****************************************************************************************************
*Name        : lion_reg_keytone_gate_get
*Prototype   : __bool lion_reg_keytone_gate_get( void )
*Arguments   : void
*Return      : the current keytone gate in register table.
*Description : get the current keytone gate in register table.
*Other       :
****************************************************************************************************
*/
__bool lion_reg_keytone_gate_get( void )
{
    __hdle  h_node = NULL;
    char    key_value[ELIBS_KEY_VALUE_LEN+1];   
    __bool  ret;
    
    /* open reg node */
	h_node = eLIBs_RegOpenNode( REG_KEYTONE_GATE_REGNODE, ELIBS_REG_READONLY );
	if( h_node == NULL )
	{
		__wrn("Warning! Reg node %s cannot be opened!\n", REG_KEYTONE_GATE_REGNODE);
	    goto error;
	}
    
    /* get key value */
    eLIBs_RegGetKeyValue( h_node, REG_KEYTONE_GATE_SET, REG_KEYTONE_GATE_KEY_CURRENT, key_value );
    
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
*Name        : lion_reg_keytone_gate_set
*Prototype   : __s32 lion_reg_keytone_gate_set( __bool keytone_gate )
*Arguments   : keytone_gate  input. keytone gate.
*Return      : EPDK_OK    succeed
*              EPDK_FAIL  fail
*Description : set the current keytone gate in register table.
*Other       :
****************************************************************************************************
*/
__s32 lion_reg_keytone_gate_set( __bool keytone_gate )
{
    __hdle  h_node = NULL;
    char    key_value[ELIBS_KEY_VALUE_LEN+1];   
    __s32   ret;
    
    /* open reg node */
	h_node = eLIBs_RegOpenNode( REG_KEYTONE_GATE_REGNODE, ELIBS_REG_READWRITE );
	if( h_node == NULL )
	{
		__wrn("Warning! Reg node %s cannot be opened!\n", REG_KEYTONE_GATE_REGNODE);
	    goto error;
	}
    
    /* convert the format of the key value */
    ret = gatemap_enm2name( keytone_gate, key_value, ELIBS_KEY_VALUE_LEN );
    if( ret != 0 )
    {
        goto error;
    }
    
    /* set key value */
    eLIBs_RegSetKeyValue( h_node, REG_KEYTONE_GATE_SET, REG_KEYTONE_GATE_KEY_CURRENT, key_value );
    
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
*Name        : lion_reg_keytone_gate_get_default
*Prototype   : __bool lion_reg_keytone_gate_get_default( void )
*Arguments   : void
*Return      : the default keytone gate in register table.
*Description : get the default keytone gate in register table.
*Other       :
****************************************************************************************************
*/
__bool lion_reg_keytone_gate_get_default( void )
{
    __hdle  h_node = NULL;
    char    key_value[ELIBS_KEY_VALUE_LEN+1];   
    __bool  ret;
    
    /* open reg node */
	h_node = eLIBs_RegOpenNode( REG_KEYTONE_GATE_REGNODE, ELIBS_REG_READONLY );
	if( h_node == NULL )
	{
		__wrn("Warning! Reg node %s cannot be opened!\n", REG_KEYTONE_GATE_REGNODE);
	    goto error;
	}
    
    /* get key value */
    eLIBs_RegGetKeyValue( h_node, REG_KEYTONE_GATE_SET, REG_KEYTONE_GATE_KEY_DEFAULT, key_value );
    
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
*Name        : lion_reg_keytone_gate_restore
*Prototype   : __s32 lion_reg_keytone_gate_restore( void )
*Arguments   : void
*Return      : EPDK_OK    succeed
*              EPDK_FAIL  fail
*Description : restore the default value of keytone gate in register table.
*Other       :
****************************************************************************************************
*/
__s32 lion_reg_keytone_gate_restore( void )
{
    __hdle  h_node = NULL;
    char    key_value[ELIBS_KEY_VALUE_LEN+1];   
    
    /* open reg node */
	h_node = eLIBs_RegOpenNode( REG_KEYTONE_GATE_REGNODE, ELIBS_REG_READWRITE );
	if( h_node == NULL )
	{
		__wrn("Warning! Reg node %s cannot be opened!\n", REG_KEYTONE_GATE_REGNODE);
	    goto error;
	}

    /* get default value */
    eLIBs_RegGetKeyValue( h_node, REG_KEYTONE_GATE_SET, REG_KEYTONE_GATE_KEY_DEFAULT, key_value );

    /* set current value */
    eLIBs_RegSetKeyValue( h_node, REG_KEYTONE_GATE_SET, REG_KEYTONE_GATE_KEY_CURRENT, key_value );
    
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



#endif     //  ifndef __REG_KEYTONE_GATE_C__

/* end of reg_keytone_gate.c */
