/*
************************************************************************************************************************
*                                            lyric mode in register table
*
*                                  Copyright(C), 2006-2010, AllWinner Technology Co., Ltd.
*											       All Rights Reserved
*
* File Name   : reg_lyric_mode.c
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
#ifndef  __REG_LYRIC_MODE_C__
#define  __REG_LYRIC_MODE_C__


#include "..\\reg_i.h"


#define REG_LYRIC_MODE_REGNODE             "\\music\\lyric_mode"
#define REG_LYRIC_MODE_SET                 "lyric_mode"
#define REG_LYRIC_MODE_KEY_CURRENT         "current"
#define REG_LYRIC_MODE_KEY_DEFAULT         "default"


static __txtbox_mode_t  _lyric_mode_name2enm( const char *lyric_mode_name );
static __s32  _lyric_mode_enm2name( __txtbox_mode_t lyric_mode_enm, char *lyric_mode_name, __u32 size );


/*
****************************************************************************************************
*Name        : lion_reg_lyric_mode_get
*Prototype   : __txtbox_mode_t lion_reg_lyric_mode_get( void )
*Arguments   : void
*Return      : the current lyric mode in register table.
*Description : get the current lyric mode in register table.
*Other       :
****************************************************************************************************
*/
__txtbox_mode_t lion_reg_lyric_mode_get( void )
{
    __hdle  h_node = NULL;
    char    key_value[ELIBS_KEY_VALUE_LEN+1];   
    __txtbox_mode_t  ret;
    
    /* open reg node */
	h_node = eLIBs_RegOpenNode( REG_LYRIC_MODE_REGNODE, ELIBS_REG_READONLY );
	if( h_node == NULL )
	{
		__wrn("Warning! Reg node %s cannot be opened!\n", REG_LYRIC_MODE_REGNODE);
	    goto error;
	}
    
    /* get key value */
    eLIBs_RegGetKeyValue( h_node, REG_LYRIC_MODE_SET, REG_LYRIC_MODE_KEY_CURRENT, key_value );
    
    /* convert the format of the key value */
    ret = _lyric_mode_name2enm( key_value );
    
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
    return LINEROLL_MODE;
}



/*
****************************************************************************************************
*Name        : lion_reg_lyric_mode_set
*Prototype   : __s32 lion_reg_lyric_mode_set( __txtbox_mode_t lyric_mode )
*Arguments   : lyric_mode  input. lyric mode.
*Return      : EPDK_OK    succeed
*              EPDK_FAIL  fail
*Description : set the current lyric mode in register table.
*Other       :
****************************************************************************************************
*/
__s32 lion_reg_lyric_mode_set( __txtbox_mode_t lyric_mode )
{
    __hdle  h_node = NULL;
    char    key_value[ELIBS_KEY_VALUE_LEN+1];   
    __s32   ret;
    
    /* open reg node */
	h_node = eLIBs_RegOpenNode( REG_LYRIC_MODE_REGNODE, ELIBS_REG_READWRITE );
	if( h_node == NULL )
	{
		__wrn("Warning! Reg node %s cannot be opened!\n", REG_LYRIC_MODE_REGNODE);
	    goto error;
	}
    
    /* convert the format of the key value */
    ret = _lyric_mode_enm2name( lyric_mode, key_value, ELIBS_KEY_VALUE_LEN );
    if( ret != 0 )
    {
        goto error;
    }
    
    /* set key value */
    eLIBs_RegSetKeyValue( h_node, REG_LYRIC_MODE_SET, REG_LYRIC_MODE_KEY_CURRENT, key_value );
    
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
*Name        : lion_reg_lyric_mode_get_default
*Prototype   : __txtbox_mode_t lion_reg_lyric_mode_get_default( void )
*Arguments   : void
*Return      : the default lyric mode in register table.
*Description : get the default lyric mode in register table.
*Other       :
****************************************************************************************************
*/
__txtbox_mode_t lion_reg_lyric_mode_get_default( void )
{
    __hdle  h_node = NULL;
    char    key_value[ELIBS_KEY_VALUE_LEN+1];   
    __txtbox_mode_t  ret;
    
    /* open reg node */
	h_node = eLIBs_RegOpenNode( REG_LYRIC_MODE_REGNODE, ELIBS_REG_READONLY );
	if( h_node == NULL )
	{
		__wrn("Warning! Reg node %s cannot be opened!\n", REG_LYRIC_MODE_REGNODE);
	    goto error;
	}
    
    /* get key value */
    eLIBs_RegGetKeyValue( h_node, REG_LYRIC_MODE_SET, REG_LYRIC_MODE_KEY_DEFAULT, key_value );
    
    /* convert the format of the key value */
    ret = _lyric_mode_name2enm( key_value );
    
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
    return LINEROLL_MODE;
}



/*
****************************************************************************************************
*Name        : lion_reg_lyric_mode_restore
*Prototype   : __s32 lion_reg_lyric_mode_restore( void )
*Arguments   : void
*Return      : EPDK_OK    succeed
*              EPDK_FAIL  fail
*Description : restore the default value of lyric mode in register table.
*Other       :
****************************************************************************************************
*/
__s32 lion_reg_lyric_mode_restore( void )
{
    __hdle  h_node = NULL;
    char    key_value[ELIBS_KEY_VALUE_LEN+1];   
    
    /* open reg node */
	h_node = eLIBs_RegOpenNode( REG_LYRIC_MODE_REGNODE, ELIBS_REG_READWRITE );
	if( h_node == NULL )
	{
		__wrn("Warning! Reg node %s cannot be opened!\n", REG_LYRIC_MODE_REGNODE);
	    goto error;
	}

    /* get default value */
    eLIBs_RegGetKeyValue( h_node, REG_LYRIC_MODE_SET, REG_LYRIC_MODE_KEY_DEFAULT, key_value );

    /* set current value */
    eLIBs_RegSetKeyValue( h_node, REG_LYRIC_MODE_SET, REG_LYRIC_MODE_KEY_CURRENT, key_value );
    
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



#define LION_LYRIC_MODE_NAME_KALAOK                   "KALAOK"
#define LION_LYRIC_MODE_NAME_LINEROLL                 "LINEROLL"                               


typedef struct _lyric_mode_map_t
{
	char             name[ELIBS_KEY_VALUE_LEN+1];
	__txtbox_mode_t  code;
}lyric_mode_map_t;


const static lyric_mode_map_t lyric_mode_map_tbl[ ] =
{
    { LION_LYRIC_MODE_NAME_KALAOK  , KALAOK_MODE   },
    { LION_LYRIC_MODE_NAME_LINEROLL, LINEROLL_MODE }
};

#define LYRIC_MODE_NR           ( sizeof(lyric_mode_map_tbl) / sizeof(lyric_mode_map_tbl[0]) )



/*
****************************************************************************************************
*Name        : _lyric_mode_name2enm
*Prototype   : __txtbox_mode_t  _lyric_mode_name2enm( const char *lyric_mode_name )
*Arguments   : lyric_mode_name   input. the name of a lyric mode
*Return      : the enum value of the lyric mode.
*Description : get the enum value of the lyric mode through its name
*Other       :
****************************************************************************************************
*/
static __txtbox_mode_t  _lyric_mode_name2enm( const char *lyric_mode_name )
{
	__u32  i;

    if( lyric_mode_name == NULL )
    {
        return LINEROLL_MODE;
    }
    
    /* search in table */
	for( i = 0;  i < LYRIC_MODE_NR;  i++ )
	{
		if( eLIBs_strcmp( lyric_mode_name, lyric_mode_map_tbl[i].name ) == 0 )
			return lyric_mode_map_tbl[i].code;
	}

	return LINEROLL_MODE;
}



/*
****************************************************************************************************
*Name        : _lyric_mode_enm2name
*Prototype   : __s32  _lyric_mode_enm2name( __txtbox_mode_t lyric_mode_enm, 
*                                              char *lyric_mode_name, __u32 size )
*Arguments   : lyric_mode_enm    input. the enum value of a lyric mode
*              lyric_mode_name   output. a buffert to store the name of a lyric mode
*              size              input. the buffer's size.
*Return      : ==0      succeed
*              !=0      fail
*Description : get the name of the lyric mode through its enum value
*Other       :
****************************************************************************************************
*/
static __s32  _lyric_mode_enm2name( __txtbox_mode_t lyric_mode_enm, char *lyric_mode_name, __u32 size )
{
	__u32  i;
	
	if( lyric_mode_name == NULL || size == 0 )
	{
		return -1;
    }
    
    /* search in table */
	for( i = 0;  i < LYRIC_MODE_NR;  i++ )
	{
		if( lyric_mode_enm == lyric_mode_map_tbl[i].code )
		{
			eLIBs_strncpy( lyric_mode_name, lyric_mode_map_tbl[i].name, size );
			lyric_mode_name[size-1] = '\0';
			
			return 0;
		}
	}

	return -1;
}




#endif     //  ifndef __REG_LYRIC_MODE_C__

/* end of reg_lyric_mode.c */
