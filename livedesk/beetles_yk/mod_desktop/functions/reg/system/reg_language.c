/*
************************************************************************************************************************
*                                                language in register table
*
*                                  Copyright(C), 2006-2010, AllWinner Technology Co., Ltd.
*											       All Rights Reserved
*
* File Name   : reg_language.c
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
#ifndef  __REG_LANGUAGE_C__
#define  __REG_LANGUAGE_C__


#include "..\\reg_i.h"


#define REG_LANGUAGE_REGNODE             "\\system\\language"
#define REG_LANGUAGE_SET                 "language"
#define REG_LANGUAGE_KEY_CURRENT         "current"
#define REG_LANGUAGE_KEY_DEFAULT         "default"


/*
****************************************************************************************************
*Name        : lion_reg_language_get
*Prototype   : __epdk_language_enm_e lion_reg_language_get( void )
*Arguments   : void
*Return      : the current language in register table.
*Description : get the current language in register table.
*Other       :
****************************************************************************************************
*/
__epdk_language_enm_e lion_reg_language_get( void )
{
    __hdle  h_node = NULL;
    char    key_value[ELIBS_KEY_VALUE_LEN+1];   
    __epdk_language_enm_e  ret;
    
    /* open reg node */
	h_node = eLIBs_RegOpenNode( REG_LANGUAGE_REGNODE, ELIBS_REG_READONLY );
	if( h_node == NULL )
	{
		__wrn("Warning! Reg node %s cannot be opened!\n", REG_LANGUAGE_REGNODE);
	    goto error;
	}
    
    /* get key value */
    eLIBs_RegGetKeyValue( h_node, REG_LANGUAGE_SET, REG_LANGUAGE_KEY_CURRENT, key_value );
    
    /* convert the format of the key value */
    ret = eLIBs_language_name2enm( key_value );
    
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
    return EPDK_LANGUAGE_ENM_UNKNOWN;
}



/*
****************************************************************************************************
*Name        : lion_reg_language_set
*Prototype   : __s32 lion_reg_language_set( __epdk_language_enm_e language )
*Arguments   : language  input. language.
*Return      : EPDK_OK    succeed
*              EPDK_FAIL  fail
*Description : set the current language in register table.
*Other       :
****************************************************************************************************
*/
__s32 lion_reg_language_set( __epdk_language_enm_e language )
{
    __hdle  h_node = NULL;
    char    key_value[ELIBS_KEY_VALUE_LEN+1];   
    __s32   ret;
    
    /* open reg node */
	h_node = eLIBs_RegOpenNode( REG_LANGUAGE_REGNODE, ELIBS_REG_READWRITE );
	if( h_node == NULL )
	{
		__wrn("Warning! Reg node %s cannot be opened!\n", REG_LANGUAGE_REGNODE);
	    goto error;
	}
    
    /* convert the format of the key value */
    ret = eLIBs_language_enm2name( language, key_value, ELIBS_KEY_VALUE_LEN );
    if( ret != 0 )
    {
        goto error;
    }
    
    /* set key value */
    eLIBs_RegSetKeyValue( h_node, REG_LANGUAGE_SET, REG_LANGUAGE_KEY_CURRENT, key_value );
    
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
*Name        : lion_reg_language_get_default
*Prototype   : __epdk_language_enm_e lion_reg_language_get_default( void )
*Arguments   : void
*Return      : the default language in register table.
*Description : get the default language in register table.
*Other       :
****************************************************************************************************
*/
__epdk_language_enm_e lion_reg_language_get_default( void )
{
    __hdle  h_node = NULL;
    char    key_value[ELIBS_KEY_VALUE_LEN+1];   
    __epdk_language_enm_e  ret;
    
    /* open reg node */
	h_node = eLIBs_RegOpenNode( REG_LANGUAGE_REGNODE, ELIBS_REG_READONLY );
	if( h_node == NULL )
	{
		__wrn("Warning! Reg node %s cannot be opened!\n", REG_LANGUAGE_REGNODE);
	    goto error;
	}
    
    /* get key value */
    eLIBs_RegGetKeyValue( h_node, REG_LANGUAGE_SET, REG_LANGUAGE_KEY_DEFAULT, key_value );
    
    /* convert the format of the key value */
    ret = eLIBs_language_name2enm( key_value );
    
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
    return EPDK_LANGUAGE_ENM_UNKNOWN;
}



/*
****************************************************************************************************
*Name        : lion_reg_language_restore
*Prototype   : __s32 lion_reg_language_restore( void )
*Arguments   : void
*Return      : EPDK_OK    succeed
*              EPDK_FAIL  fail
*Description : restore the default value of language in register table.
*Other       :
****************************************************************************************************
*/
__s32 lion_reg_language_restore( void )
{
    __hdle  h_node = NULL;
    char    key_value[ELIBS_KEY_VALUE_LEN+1];   
    
    /* open reg node */
	h_node = eLIBs_RegOpenNode( REG_LANGUAGE_REGNODE, ELIBS_REG_READWRITE );
	if( h_node == NULL )
	{
		__wrn("Warning! Reg node %s cannot be opened!\n", REG_LANGUAGE_REGNODE);
	    goto error;
	}

    /* get default value */
    eLIBs_RegGetKeyValue( h_node, REG_LANGUAGE_SET, REG_LANGUAGE_KEY_DEFAULT, key_value );

    /* set current value */
    eLIBs_RegSetKeyValue( h_node, REG_LANGUAGE_SET, REG_LANGUAGE_KEY_CURRENT, key_value );
    
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



#endif     //  ifndef __REG_LANGUAGE_C__

/* end of reg_language.c */
