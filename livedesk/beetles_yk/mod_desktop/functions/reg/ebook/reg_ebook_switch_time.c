/*
************************************************************************************************************************
*                                           switch time in register table
*
*                                  Copyright(C), 2006-2010, AllWinner Technology Co., Ltd.
*											       All Rights Reserved
*
* File Name   : reg_ebook_switch_time.c
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
#ifndef  __REG_EBOOK_SWITCH_TIME_C__
#define  __REG_EBOOK_SWITCH_TIME_C__


#include "..\\reg_i.h"


#define REG_EBOOK_SWITCH_TIME_REGNODE             "\\ebook\\switch_time"
#define REG_EBOOK_SWITCH_TIME_SET                 "switch_time"
#define REG_EBOOK_SWITCH_TIME_KEY_CURRENT         "current"
#define REG_EBOOK_SWITCH_TIME_KEY_DEFAULT         "default"


/*
****************************************************************************************************
*Name        : lion_reg_ebook_switch_time_get
*Prototype   : __s32 lion_reg_ebook_switch_time_get( void )
*Arguments   : void
*Return      : the current switch time in register table. unit:s.
*Description : get the current switch time in register table.
*Other       :
****************************************************************************************************
*/
__s32  lion_reg_ebook_switch_time_get( void )
{
    __hdle  h_node = NULL;
    char    key_value[ELIBS_KEY_VALUE_LEN+1];   
    __u32   ret;
    const char *p;
    
    /* open reg node */
	h_node = eLIBs_RegOpenNode( REG_EBOOK_SWITCH_TIME_REGNODE, ELIBS_REG_READONLY );
	if( h_node == NULL )
	{
		__wrn("Warning! Reg node %s cannot be opened!\n", REG_EBOOK_SWITCH_TIME_REGNODE);
	    goto error;
	}
    
    /* get key value */
    eLIBs_RegGetKeyValue( h_node, REG_EBOOK_SWITCH_TIME_SET, REG_EBOOK_SWITCH_TIME_KEY_CURRENT, key_value );
    
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
*Name        : lion_reg_ebook_switch_time_set
*Prototype   : __s32 lion_reg_ebook_switch_time_set( __s32 time )
*Arguments   : time         input. switch time, unit:s.
*Return      : EPDK_OK    succeed
*              EPDK_FAIL  fail
*Description : set the current switch time in register table.
*Other       :
****************************************************************************************************
*/
__s32 lion_reg_ebook_switch_time_set( __s32 time )
{
    __hdle  h_node = NULL;
    char    key_value[ELIBS_KEY_VALUE_LEN+1];   
    
    /* open reg node */
	h_node = eLIBs_RegOpenNode( REG_EBOOK_SWITCH_TIME_REGNODE, ELIBS_REG_READWRITE );
	if( h_node == NULL )
	{
		__wrn("Warning! Reg node %s cannot be opened!\n", REG_EBOOK_SWITCH_TIME_REGNODE);
	    goto error;
	}
    
    /* convert the format of the key value */
    eLIBs_int2str_dec( time, key_value );
    
    /* set key value */
    eLIBs_RegSetKeyValue( h_node, REG_EBOOK_SWITCH_TIME_SET, REG_EBOOK_SWITCH_TIME_KEY_CURRENT, key_value );
    
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
*Name        : lion_reg_ebook_switch_time_get_default
*Prototype   : __s32 lion_reg_ebook_switch_time_get_default( void )
*Arguments   : void
*Return      : the default switch time in register table. unit:s.
*Description : get the default switch time in register table.
*Other       :
****************************************************************************************************
*/
__s32 lion_reg_ebook_switch_time_get_default( void )
{
    __hdle  h_node = NULL;
    char    key_value[ELIBS_KEY_VALUE_LEN+1];   
    __u32   ret;
    const char *p;
    
    /* open reg node */
	h_node = eLIBs_RegOpenNode( REG_EBOOK_SWITCH_TIME_REGNODE, ELIBS_REG_READONLY );
	if( h_node == NULL )
	{
		__wrn("Warning! Reg node %s cannot be opened!\n", REG_EBOOK_SWITCH_TIME_REGNODE);
	    goto error;
	}
    
    /* get key value */
    eLIBs_RegGetKeyValue( h_node, REG_EBOOK_SWITCH_TIME_SET, REG_EBOOK_SWITCH_TIME_KEY_DEFAULT, key_value );
    
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
*Name        : lion_reg_ebook_switch_time_restore
*Prototype   : __s32 lion_reg_ebook_switch_time_restore( void )
*Arguments   : void
*Return      : EPDK_OK    succeed
*              EPDK_FAIL  fail
*Description : restore the default value of switch time in register table.
*Other       :
****************************************************************************************************
*/
__s32 lion_reg_ebook_switch_time_restore( void )
{
    __hdle  h_node = NULL;
    char    key_value[ELIBS_KEY_VALUE_LEN+1];   
    
    /* open reg node */
	h_node = eLIBs_RegOpenNode( REG_EBOOK_SWITCH_TIME_REGNODE, ELIBS_REG_READWRITE );
	if( h_node == NULL )
	{
		__wrn("Warning! Reg node %s cannot be opened!\n", REG_EBOOK_SWITCH_TIME_REGNODE);
	    goto error;
	}

    /* get default value */
    eLIBs_RegGetKeyValue( h_node, REG_EBOOK_SWITCH_TIME_SET, REG_EBOOK_SWITCH_TIME_KEY_DEFAULT, key_value );

    /* set current value */
    eLIBs_RegSetKeyValue( h_node, REG_EBOOK_SWITCH_TIME_SET, REG_EBOOK_SWITCH_TIME_KEY_CURRENT, key_value );
    
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



#endif     //  ifndef __REG_EBOOK_SWITCH_TIME_C__

/* end of reg_ebook_switch_time.c */
