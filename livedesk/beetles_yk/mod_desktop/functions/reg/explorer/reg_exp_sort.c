/*
************************************************************************************************************************
*                                            mode mode in register table
*
*                                  Copyright(C), 2006-2010, AllWinner Technology Co., Ltd.
*											       All Rights Reserved
*
* File Name   : reg_sort_mode.c
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
#ifndef  __REG_EXP_SORT_C__
#define  __REG_EXP_SORT_C__


#include "..\\reg_i.h"


#define REG_EXP_SORT_REGNODE             "\\explorer\\sort_mode"
#define REG_EXP_SORT_SET                 "sort_mode"
#define REG_EXP_SORT_KEY_CURRENT         "current"
#define REG_EXP_SORT_KEY_DEFAULT         "default"


static cat_sort_type_t  _mode_name2enm( const char *mode_name );
static __s32  _mode_enm2name( cat_sort_type_t mode_enm, char *mode_name, __u32 size );


/*
****************************************************************************************************
*Name        : lion_reg_sort_mode_get
*Prototype   : cat_sort_type_t lion_reg_sort_mode_get( void )
*Arguments   : void
*Return      : the current mode in register table.
*Description : get the current mode in register table.
*Other       :
****************************************************************************************************
*/
cat_sort_type_t lion_reg_sort_mode_get( void )
{
    __hdle  h_node = NULL;
    char    key_value[ELIBS_KEY_VALUE_LEN+1];   
    cat_sort_type_t  ret;
    
    /* open reg node */
	h_node = eLIBs_RegOpenNode( REG_EXP_SORT_REGNODE, ELIBS_REG_READONLY );
	if( h_node == NULL )
	{
		__wrn("Warning! Reg node %s cannot be opened!\n", REG_EXP_SORT_REGNODE);
	    goto error;
	}
    
    /* get key value */
    eLIBs_RegGetKeyValue( h_node, REG_EXP_SORT_SET, REG_EXP_SORT_KEY_CURRENT, key_value );
    
    /* convert the format of the key value */
    ret = _mode_name2enm( key_value );
    
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
    return CAT_SORT_NAME;
}



/*
****************************************************************************************************
*Name        : lion_reg_sort_mode_set
*Prototype   : __s32 lion_reg_sort_mode_set( cat_sort_type_t mode )
*Arguments   : mode  input. mode.
*Return      : EPDK_OK    succeed
*              EPDK_FAIL  fail
*Description : set the current mode in register table.
*Other       :
****************************************************************************************************
*/
__s32 lion_reg_sort_mode_set( cat_sort_type_t mode )
{
    __hdle  h_node = NULL;
    char    key_value[ELIBS_KEY_VALUE_LEN+1];   
    __s32   ret;
    
    /* open reg node */
	h_node = eLIBs_RegOpenNode( REG_EXP_SORT_REGNODE, ELIBS_REG_READWRITE );
	if( h_node == NULL )
	{
		__wrn("Warning! Reg node %s cannot be opened!\n", REG_EXP_SORT_REGNODE);
	    goto error;
	}
    
    /* convert the format of the key value */
    ret = _mode_enm2name( mode, key_value, ELIBS_KEY_VALUE_LEN );
    if( ret != 0 )
    {
        goto error;
    }
    
    /* set key value */
    eLIBs_RegSetKeyValue( h_node, REG_EXP_SORT_SET, REG_EXP_SORT_KEY_CURRENT, key_value );
    
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
*Name        : lion_reg_sort_mode_get_default
*Prototype   : cat_sort_type_t lion_reg_sort_mode_get_default( void )
*Arguments   : void
*Return      : the default mode in register table.
*Description : get the default mode in register table.
*Other       :
****************************************************************************************************
*/
cat_sort_type_t lion_reg_sort_mode_get_default( void )
{
    __hdle  h_node = NULL;
    char    key_value[ELIBS_KEY_VALUE_LEN+1];   
    cat_sort_type_t  ret;
    
    /* open reg node */
	h_node = eLIBs_RegOpenNode( REG_EXP_SORT_REGNODE, ELIBS_REG_READONLY );
	if( h_node == NULL )
	{
		__wrn("Warning! Reg node %s cannot be opened!\n", REG_EXP_SORT_REGNODE);
	    goto error;
	}
    
    /* get key value */
    eLIBs_RegGetKeyValue( h_node, REG_EXP_SORT_SET, REG_EXP_SORT_KEY_DEFAULT, key_value );
    
    /* convert the format of the key value */
    ret = _mode_name2enm( key_value );
    
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
    return CAT_SORT_NAME;
}



/*
****************************************************************************************************
*Name        : lion_reg_sort_mode_restore
*Prototype   : __s32 lion_reg_sort_mode_restore( void )
*Arguments   : void
*Return      : EPDK_OK    succeed
*              EPDK_FAIL  fail
*Description : restore the default value of mode in register table.
*Other       :
****************************************************************************************************
*/
__s32 lion_reg_sort_mode_restore( void )
{
    __hdle  h_node = NULL;
    char    key_value[ELIBS_KEY_VALUE_LEN+1];   
    
    /* open reg node */
	h_node = eLIBs_RegOpenNode( REG_EXP_SORT_REGNODE, ELIBS_REG_READWRITE );
	if( h_node == NULL )
	{
		__wrn("Warning! Reg node %s cannot be opened!\n", REG_EXP_SORT_REGNODE);
	    goto error;
	}

    /* get default value */
    eLIBs_RegGetKeyValue( h_node, REG_EXP_SORT_SET, REG_EXP_SORT_KEY_DEFAULT, key_value );

    /* set current value */
    eLIBs_RegSetKeyValue( h_node, REG_EXP_SORT_SET, REG_EXP_SORT_KEY_CURRENT, key_value );
    
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

#define LION_SORT_NAME_NAME               "NAME"
#define LION_SORT_NAME_TIME              "TIME"
#define LION_SORT_NAME_SIZE              "SIZE"
#define LION_SORT_NAME_FORMAT                "FORMAT"


typedef struct _mode_map_t
{
	char             name[ELIBS_KEY_VALUE_LEN+1];
	cat_sort_type_t  code;
}_mode_map_t;


const static _mode_map_t mode_map_tbl[ ] =
{
    { LION_SORT_NAME_NAME , CAT_SORT_NAME  },
    { LION_SORT_NAME_TIME, CAT_SORT_TIME },
    { LION_SORT_NAME_SIZE, CAT_SORT_SIZE },
    { LION_SORT_NAME_FORMAT  , CAT_SORT_FORMAT   },
};

#define REPEAT_NR           ( sizeof(mode_map_tbl) / sizeof(mode_map_tbl[0]) )



/*
****************************************************************************************************
*Name        : _mode_name2enm
*Prototype   : cat_sort_type_t  _mode_name2enm( const char *mode_name )
*Arguments   : mode_name   input. the name of a mode
*Return      : the enum value of the mode.
*Description : get the enum value of the mode through its name
*Other       :
****************************************************************************************************
*/
static cat_sort_type_t  _mode_name2enm( const char *mode_name )
{
	__u32  i;

    if( mode_name == NULL )
    {
        return CAT_SORT_NAME;
    }
    
    /* search in table */
	for( i = 0;  i < REPEAT_NR;  i++ )
	{
		if( eLIBs_strcmp( mode_name, mode_map_tbl[i].name ) == 0 )
			return mode_map_tbl[i].code;
	}

	return CAT_SORT_NAME;
}



/*
****************************************************************************************************
*Name        : _mode_enm2name
*Prototype   : __s32  _mode_enm2name( cat_sort_type_t mode_enm, 
*                                              char *mode_name, __u32 size )
*Arguments   : mode_enm    input. the enum value of a mode
*              mode_name   output. a buffert to store the name of a mode
*              size           input. the buffer's size.
*Return      : ==0      succeed
*              !=0      fail
*Description : get the name of the mode through its enum value
*Other       :
****************************************************************************************************
*/
static __s32  _mode_enm2name( cat_sort_type_t mode_enm, char *mode_name, __u32 size )
{
	__u32  i;
	
	if( mode_name == NULL || size == 0 )
	{
		return -1;
    }
    
    /* search in table */
	for( i = 0;  i < REPEAT_NR;  i++ )
	{
		if( mode_enm == mode_map_tbl[i].code )
		{
			eLIBs_strncpy( mode_name, mode_map_tbl[i].name, size );
			mode_name[size-1] = '\0';
			
			return 0;
		}
	}

	return -1;
}




#endif     //  ifndef __REG_EXP_SORT_C__

/* end of reg_sort_mode.c */
