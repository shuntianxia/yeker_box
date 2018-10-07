/*
************************************************************************************************************************
*                                                charset in register table
*
*                                  Copyright(C), 2006-2010, AllWinner Technology Co., Ltd.
*											       All Rights Reserved
*
* File Name   : reg_charset.c
*
* Author      : Gary.Wang
*
* Version     : 1.0
*
* Date        : 2010.08.26
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
* Gary.Wang      2010.08.26        1.0         build the file
*
************************************************************************************************************************
*/
#ifndef  __REG_CHARSET_C__
#define  __REG_CHARSET_C__


#include "..\\reg_i.h"


#define REG_CHARSET_REGNODE             "\\system\\charset"
#define REG_CHARSET_SET                 "charset"
#define REG_CHARSET_KEY_CURRENT         "current"
#define REG_CHARSET_KEY_DEFAULT         "default"


/*
****************************************************************************************************
*Name        : lion_reg_charset_get
*Prototype   : __epdk_charset_enm_e lion_reg_charset_get( void )
*Arguments   : void
*Return      : the current charset in register table.
*Description : get the current charset in register table.
*Other       :
****************************************************************************************************
*/
__epdk_charset_enm_e lion_reg_charset_get( void )
{
    __hdle  h_node = NULL;
    char    key_value[ELIBS_KEY_VALUE_LEN+1];   
    __epdk_charset_enm_e  ret;
    
    /* open reg node */
	h_node = eLIBs_RegOpenNode( REG_CHARSET_REGNODE, ELIBS_REG_READONLY );
	if( h_node == NULL )
	{
		__wrn("Warning! Reg node %s cannot be opened!\n", REG_CHARSET_REGNODE);
	    goto error;
	}
    
    /* get key value */
    eLIBs_RegGetKeyValue( h_node, REG_CHARSET_SET, REG_CHARSET_KEY_CURRENT, key_value );
    
    /* convert the format of the key value */
    ret = (__epdk_charset_enm_e)eLIBs_charset_name2enm( key_value );
    
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
    return EPDK_CHARSET_ENM_UNKNOWN;
}



/*
****************************************************************************************************
*Name        : lion_reg_charset_set
*Prototype   : __s32 lion_reg_charset_set( __epdk_charset_enm_e charset )
*Arguments   : charset  input. charset.
*Return      : EPDK_OK    succeed
*              EPDK_FAIL  fail
*Description : set the current charset in register table.
*Other       :
****************************************************************************************************
*/
__s32 lion_reg_charset_set( __epdk_charset_enm_e charset )
{
    __hdle  h_node = NULL;
    char    key_value[ELIBS_KEY_VALUE_LEN+1];   
    __s32   ret;
    
    /* open reg node */
	h_node = eLIBs_RegOpenNode( REG_CHARSET_REGNODE, ELIBS_REG_READWRITE );
	if( h_node == NULL )
	{
		__wrn("Warning! Reg node %s cannot be opened!\n", REG_CHARSET_REGNODE);
	    goto error;
	}
    
    /* convert the format of the key value */
    ret = eLIBs_charset_enm2name( charset, key_value );
    if( ret != 0 )
    {
        goto error;
    }
    
    /* set key value */
    eLIBs_RegSetKeyValue( h_node, REG_CHARSET_SET, REG_CHARSET_KEY_CURRENT, key_value );
    
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
*Name        : lion_reg_charset_get_default
*Prototype   : __epdk_charset_enm_e lion_reg_charset_get_default( void )
*Arguments   : void
*Return      : the default charset in register table.
*Description : get the default charset in register table.
*Other       :
****************************************************************************************************
*/
__epdk_charset_enm_e lion_reg_charset_get_default( void )
{
    __hdle  h_node = NULL;
    char    key_value[ELIBS_KEY_VALUE_LEN+1];   
    __epdk_charset_enm_e  ret;
    
    /* open reg node */
	h_node = eLIBs_RegOpenNode( REG_CHARSET_REGNODE, ELIBS_REG_READONLY );
	if( h_node == NULL )
	{
		__wrn("Warning! Reg node %s cannot be opened!\n", REG_CHARSET_REGNODE);
	    goto error;
	}
    
    /* get key value */
    eLIBs_RegGetKeyValue( h_node, REG_CHARSET_SET, REG_CHARSET_KEY_DEFAULT, key_value );
    
    /* convert the format of the key value */
    ret = (__epdk_charset_enm_e)eLIBs_charset_name2enm( key_value );
    
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
    return EPDK_CHARSET_ENM_UNKNOWN;
}



/*
****************************************************************************************************
*Name        : lion_reg_charset_restore
*Prototype   : __s32 lion_reg_charset_restore( void )
*Arguments   : void
*Return      : EPDK_OK    succeed
*              EPDK_FAIL  fail
*Description : restore the default value of charset in register table.
*Other       :
****************************************************************************************************
*/
__s32 lion_reg_charset_restore( void )
{
    __hdle  h_node = NULL;
    char    key_value[ELIBS_KEY_VALUE_LEN+1];   
    
    /* open reg node */
	h_node = eLIBs_RegOpenNode( REG_CHARSET_REGNODE, ELIBS_REG_READWRITE );
	if( h_node == NULL )
	{
		__wrn("Warning! Reg node %s cannot be opened!\n", REG_CHARSET_REGNODE);
	    goto error;
	}

    /* get default value */
    eLIBs_RegGetKeyValue( h_node, REG_CHARSET_SET, REG_CHARSET_KEY_DEFAULT, key_value );

    /* set current value */
    eLIBs_RegSetKeyValue( h_node, REG_CHARSET_SET, REG_CHARSET_KEY_CURRENT, key_value );
    
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



#endif     //  ifndef __REG_CHARSET_C__

/* end of reg_charset.c */
