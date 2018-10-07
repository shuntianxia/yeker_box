/*
************************************************************************************************************************
*                                           close screen time in register table
*
*                                  Copyright(C), 2006-2010, AllWinner Technology Co., Ltd.
*											       All Rights Reserved
*
* File Name   : reg_close_scn.c
*
* Author      : Gary.Wang
*
* Version     : 1.0
*
* Date        : 2010.08.28
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
* Gary.Wang      2010.08.28        1.0         build the file
*
************************************************************************************************************************
*/
#ifndef  __REG_CLOSE_SCN_C__
#define  __REG_CLOSE_SCN_C__


#include "..\\reg_i.h"


#define REG_CLOSE_SCN_REGNODE             "\\system\\close_scn"
#define REG_CLOSE_SCN_SET                 "close_scn"
#define REG_CLOSE_SCN_KEY_CURRENT         "current"
#define REG_CLOSE_SCN_KEY_DEFAULT         "default"


/*
****************************************************************************************************
*Name        : lion_reg_close_scn_get
*Prototype   : __u32 lion_reg_close_scn_get( void )
*Arguments   : void
*Return      : the current close screen time in register table. unit: s, 0 means "forbid closing 
*              screen".
*Description : get the current close screen time in register table.
*Other       :
****************************************************************************************************
*/
__u32  lion_reg_close_scn_get( void )
{
    __hdle  h_node = NULL;
    char    key_value[ELIBS_KEY_VALUE_LEN+1];   
    __u32   ret;
    const char *p;
    
    /* open reg node */
	h_node = eLIBs_RegOpenNode( REG_CLOSE_SCN_REGNODE, ELIBS_REG_READONLY );
	if( h_node == NULL )
	{
		__wrn("Warning! Reg node %s cannot be opened!\n", REG_CLOSE_SCN_REGNODE);
	    goto error;
	}
    
    /* get key value */
    eLIBs_RegGetKeyValue( h_node, REG_CLOSE_SCN_SET, REG_CLOSE_SCN_KEY_CURRENT, key_value );
    
    /* convert the format of the key value */
    ret = eLIBs_strtol( key_value, &p, 0 );
    if( p == key_value )
    {
        __wrn("Warning! Fail in converting string %s to number.\n", key_value);
        goto error;
    }
    
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
    return 0;
}



/*
****************************************************************************************************
*Name        : lion_reg_close_scn_set
*Prototype   : __s32 lion_reg_close_scn_set( __u32 time )
*Arguments   : time         input. close screen time, unit: s, 0 means "forbid closing screen".
*Return      : EPDK_OK    succeed
*              EPDK_FAIL  fail
*Description : set the current close screen time in register table.
*Other       :
****************************************************************************************************
*/
__s32 lion_reg_close_scn_set( __u32 time )
{
    __hdle  h_node = NULL;
    char    key_value[ELIBS_KEY_VALUE_LEN+1];   
    
    /* open reg node */
	h_node = eLIBs_RegOpenNode( REG_CLOSE_SCN_REGNODE, ELIBS_REG_READWRITE );
	if( h_node == NULL )
	{
		__wrn("Warning! Reg node %s cannot be opened!\n", REG_CLOSE_SCN_REGNODE);
	    goto error;
	}
    
    /* convert the format of the key value */
    eLIBs_int2str_dec( time, key_value );
    
    /* set key value */
    eLIBs_RegSetKeyValue( h_node, REG_CLOSE_SCN_SET, REG_CLOSE_SCN_KEY_CURRENT, key_value );
    
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
*Name        : lion_reg_close_scn_get_default
*Prototype   : __u32 lion_reg_close_scn_get_default( void )
*Arguments   : void
*Return      : the default close screen time in register table. unit: s, 0 means "forbid closing 
*              screen".
*Description : get the default close screen time in register table.
*Other       :
****************************************************************************************************
*/
__u32 lion_reg_close_scn_get_default( void )
{
    __hdle  h_node = NULL;
    char    key_value[ELIBS_KEY_VALUE_LEN+1];   
    __u32   ret;
    const char *p;
    
    /* open reg node */
	h_node = eLIBs_RegOpenNode( REG_CLOSE_SCN_REGNODE, ELIBS_REG_READONLY );
	if( h_node == NULL )
	{
		__wrn("Warning! Reg node %s cannot be opened!\n", REG_CLOSE_SCN_REGNODE);
	    goto error;
	}
    
    /* get key value */
    eLIBs_RegGetKeyValue( h_node, REG_CLOSE_SCN_SET, REG_CLOSE_SCN_KEY_DEFAULT, key_value );
    
    /* convert the format of the key value */
    ret = eLIBs_strtol( key_value, &p, 0 );
    if( p == key_value )
    {
        __wrn("Warning! Fail in converting string %s to number.\n", key_value);
        goto error;
    }
        
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
    return 0;
}



/*
****************************************************************************************************
*Name        : lion_reg_close_scn_restore
*Prototype   : __s32 lion_reg_close_scn_restore( void )
*Arguments   : void
*Return      : EPDK_OK    succeed
*              EPDK_FAIL  fail
*Description : restore the default value of close screen time in register table.
*Other       :
****************************************************************************************************
*/
__s32 lion_reg_close_scn_restore( void )
{
    __hdle  h_node = NULL;
    char    key_value[ELIBS_KEY_VALUE_LEN+1];   
    
    /* open reg node */
	h_node = eLIBs_RegOpenNode( REG_CLOSE_SCN_REGNODE, ELIBS_REG_READWRITE );
	if( h_node == NULL )
	{
		__wrn("Warning! Reg node %s cannot be opened!\n", REG_CLOSE_SCN_REGNODE);
	    goto error;
	}

    /* get default value */
    eLIBs_RegGetKeyValue( h_node, REG_CLOSE_SCN_SET, REG_CLOSE_SCN_KEY_DEFAULT, key_value );

    /* set current value */
    eLIBs_RegSetKeyValue( h_node, REG_CLOSE_SCN_SET, REG_CLOSE_SCN_KEY_CURRENT, key_value );
    
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



#endif     //  ifndef __REG_CLOSE_SCN_C__

/* end of reg_close_scn.c */
