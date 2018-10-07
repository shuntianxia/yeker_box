/*
************************************************************************************************************************
*                                           subtitle size in register table
*
*                                  Copyright(C), 2006-2010, AllWinner Technology Co., Ltd.
*											       All Rights Reserved
*
* File Name   : reg_backgrd.c
*
* Author      : Jackie.Chen
*
* Version     : 1.0
*
* Date        : 2010.12.17
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
* Jackie.Chen      2010.12.17        1.0         build the file

************************************************************************************************************************
*/
#ifndef  __REG_BACKGRD_C__
#define  __REG_BACKGRD_C__
	
	
#include "..\\reg_i.h"
	
	
#define REG_BACKGRD_REGNODE             "\\music\\backgrd"
#define REG_BACKGRD_SET                 "backgrd"
#define REG_BACKGRD_KEY_CURRENT         "current"
#define REG_BACKGRD_KEY_DEFAULT         "default"
	
	
	/*
	****************************************************************************************************
	*Name		 : lion_reg_backgrd_get
	*Prototype	 : __u32 lion_reg_backgrd_get( void )
	*Arguments	 : void
	*Return 	 : the current subtitle size in register table.
	*Description : get the current subtitle size in register table.
	*Other		 :
	****************************************************************************************************
	*/
	__u32  lion_reg_backgrd_get( void )
	{
		__hdle	h_node = NULL;
		char	key_value[ELIBS_KEY_VALUE_LEN+1];	
		__u32	ret;
		const char *p;
		
		/* open reg node */
		h_node = eLIBs_RegOpenNode( REG_BACKGRD_REGNODE, ELIBS_REG_READONLY );
		if( h_node == NULL )
		{
			__wrn("Warning! Reg node %s cannot be opened!\n", REG_BACKGRD_REGNODE);
			goto error;
		}
		
		/* get key value */
		eLIBs_RegGetKeyValue( h_node, REG_BACKGRD_SET, REG_BACKGRD_KEY_CURRENT, key_value );
		
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
	*Name		 : lion_reg_backgrd_set
	*Prototype	 : __s32 lion_reg_backgrd_set( __u32 value )
	*Arguments	 : value		 input. subtitle size value.
	*Return 	 : EPDK_OK	  succeed
	*			   EPDK_FAIL  fail
	*Description : set the current subtitle size in register table.
	*Other		 :
	****************************************************************************************************
	*/
	__s32 lion_reg_backgrd_set( __u32 value )
	{
		__hdle	h_node = NULL;
		char	key_value[ELIBS_KEY_VALUE_LEN+1];	
		
		/* open reg node */
		h_node = eLIBs_RegOpenNode( REG_BACKGRD_REGNODE, ELIBS_REG_READWRITE );
		if( h_node == NULL )
		{
			__wrn("Warning! Reg node %s cannot be opened!\n", REG_BACKGRD_REGNODE);
			goto error;
		}
		
		/* convert the format of the key value */
		eLIBs_int2str_dec( value, key_value );
		
		/* set key value */
		eLIBs_RegSetKeyValue( h_node, REG_BACKGRD_SET, REG_BACKGRD_KEY_CURRENT, key_value );
		
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
	*Name		 : lion_reg_backgrd_get_default
	*Prototype	 : __u32 lion_reg_backgrd_get_default( void )
	*Arguments	 : void
	*Return 	 : the default subtitle size in register table.
	*Description : get the default subtitle size in register table.
	*Other		 :
	****************************************************************************************************
	*/
	__u32 lion_reg_backgrd_get_default( void )
	{
		__hdle	h_node = NULL;
		char	key_value[ELIBS_KEY_VALUE_LEN+1];	
		__u32	ret;
		const char *p;
		
		/* open reg node */
		h_node = eLIBs_RegOpenNode( REG_BACKGRD_REGNODE, ELIBS_REG_READONLY );
		if( h_node == NULL )
		{
			__wrn("Warning! Reg node %s cannot be opened!\n", REG_BACKGRD_REGNODE);
			goto error;
		}
		
		/* get key value */
		eLIBs_RegGetKeyValue( h_node, REG_BACKGRD_SET, REG_BACKGRD_KEY_DEFAULT, key_value );
		
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
	*Name		 : lion_reg_backgrd_restore
	*Prototype	 : __s32 lion_reg_backgrd_restore( void )
	*Arguments	 : void
	*Return 	 : EPDK_OK	  succeed
	*			   EPDK_FAIL  fail
	*Description : restore the default value of subtitle size in register table.
	*Other		 :
	****************************************************************************************************
	*/
	__s32 lion_reg_backgrd_restore( void )
	{
		__hdle	h_node = NULL;
		char	key_value[ELIBS_KEY_VALUE_LEN+1];	
		
		/* open reg node */
		h_node = eLIBs_RegOpenNode( REG_BACKGRD_REGNODE, ELIBS_REG_READWRITE );
		if( h_node == NULL )
		{
			__wrn("Warning! Reg node %s cannot be opened!\n", REG_BACKGRD_REGNODE);
			goto error;
		}
	
		/* get default value */
		eLIBs_RegGetKeyValue( h_node, REG_BACKGRD_SET, REG_BACKGRD_KEY_DEFAULT, key_value );
	
		/* set current value */
		eLIBs_RegSetKeyValue( h_node, REG_BACKGRD_SET, REG_BACKGRD_KEY_CURRENT, key_value );
		
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
	
	
	
#endif     //  ifndef __REG_BACKGRD_C__
	
	/* end of reg_backgrd.c */



