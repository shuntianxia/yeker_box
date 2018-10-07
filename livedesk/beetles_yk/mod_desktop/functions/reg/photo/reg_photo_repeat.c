/*
************************************************************************************************************************
*                                            repeat mode in register table
*
*                                  Copyright(C), 2006-2010, AllWinner Technology Co., Ltd.
*											       All Rights Reserved
*
* File Name   : reg_photo_repeat.c
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
#ifndef  __REG_PHOTO_REPEAT_C__
#define  __REG_PHOTO_REPEAT_C__


#include "..\\reg_i.h"


#define REG_PHOTO_REPEAT_REGNODE             "\\photo\\repeat"
#define REG_PHOTO_REPEAT_SET                 "repeat"
#define REG_PHOTO_REPEAT_KEY_CURRENT         "current"
#define REG_PHOTO_REPEAT_KEY_DEFAULT         "default"


static __anole_playorder_t  _repeat_name2enm( const char *repeat_name );
static __s32  _repeat_enm2name( __anole_playorder_t repeat_enm, char *repeat_name, __u32 size );


/*
****************************************************************************************************
*Name        : lion_reg_photo_repeat_get
*Prototype   : __anole_playorder_t lion_reg_photo_repeat_get( void )
*Arguments   : void
*Return      : the current repeat in register table.
*Description : get the current repeat in register table.
*Other       :
****************************************************************************************************
*/
__anole_playorder_t lion_reg_photo_repeat_get( void )
{
    __hdle  h_node = NULL;
    char    key_value[ELIBS_KEY_VALUE_LEN+1];   
    __anole_playorder_t  ret;
    
    /* open reg node */
	h_node = eLIBs_RegOpenNode( REG_PHOTO_REPEAT_REGNODE, ELIBS_REG_READONLY );
	if( h_node == NULL )
	{
		__wrn("Warning! Reg node %s cannot be opened!\n", REG_PHOTO_REPEAT_REGNODE);
	    goto error;
	}
    
    /* get key value */
    eLIBs_RegGetKeyValue( h_node, REG_PHOTO_REPEAT_SET, REG_PHOTO_REPEAT_KEY_CURRENT, key_value );
    
    /* convert the format of the key value */
    ret = _repeat_name2enm( key_value );
    
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
    return ANOLE_PLAY_NEXT;
}



/*
****************************************************************************************************
*Name        : lion_reg_photo_repeat_set
*Prototype   : __s32 lion_reg_photo_repeat_set( __anole_playorder_t repeat )
*Arguments   : repeat  input. repeat.
*Return      : EPDK_OK    succeed
*              EPDK_FAIL  fail
*Description : set the current repeat in register table.
*Other       :
****************************************************************************************************
*/
__s32 lion_reg_photo_repeat_set( __anole_playorder_t repeat )
{
    __hdle  h_node = NULL;
    char    key_value[ELIBS_KEY_VALUE_LEN+1];   
    __s32   ret;
    
    /* open reg node */
	h_node = eLIBs_RegOpenNode( REG_PHOTO_REPEAT_REGNODE, ELIBS_REG_READWRITE );
	if( h_node == NULL )
	{
		__wrn("Warning! Reg node %s cannot be opened!\n", REG_PHOTO_REPEAT_REGNODE);
	    goto error;
	}
    
    /* convert the format of the key value */
    ret = _repeat_enm2name( repeat, key_value, ELIBS_KEY_VALUE_LEN );
    if( ret != 0 )
    {
        goto error;
    }
    
    /* set key value */
    eLIBs_RegSetKeyValue( h_node, REG_PHOTO_REPEAT_SET, REG_PHOTO_REPEAT_KEY_CURRENT, key_value );
    
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
*Name        : lion_reg_photo_repeat_get_default
*Prototype   : __anole_playorder_t lion_reg_photo_repeat_get_default( void )
*Arguments   : void
*Return      : the default repeat in register table.
*Description : get the default repeat in register table.
*Other       :
****************************************************************************************************
*/
__anole_playorder_t lion_reg_photo_repeat_get_default( void )
{
    __hdle  h_node = NULL;
    char    key_value[ELIBS_KEY_VALUE_LEN+1];   
    __anole_playorder_t  ret;
    
    /* open reg node */
	h_node = eLIBs_RegOpenNode( REG_PHOTO_REPEAT_REGNODE, ELIBS_REG_READONLY );
	if( h_node == NULL )
	{
		__wrn("Warning! Reg node %s cannot be opened!\n", REG_PHOTO_REPEAT_REGNODE);
	    goto error;
	}
    
    /* get key value */
    eLIBs_RegGetKeyValue( h_node, REG_PHOTO_REPEAT_SET, REG_PHOTO_REPEAT_KEY_DEFAULT, key_value );
    
    /* convert the format of the key value */
    ret = _repeat_name2enm( key_value );
    
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
    return ANOLE_PLAY_NEXT;
}



/*
****************************************************************************************************
*Name        : lion_reg_photo_repeat_restore
*Prototype   : __s32 lion_reg_photo_repeat_restore( void )
*Arguments   : void
*Return      : EPDK_OK    succeed
*              EPDK_FAIL  fail
*Description : restore the default value of repeat in register table.
*Other       :
****************************************************************************************************
*/
__s32 lion_reg_photo_repeat_restore( void )
{
    __hdle  h_node = NULL;
    char    key_value[ELIBS_KEY_VALUE_LEN+1];   
    
    /* open reg node */
	h_node = eLIBs_RegOpenNode( REG_PHOTO_REPEAT_REGNODE, ELIBS_REG_READWRITE );
	if( h_node == NULL )
	{
		__wrn("Warning! Reg node %s cannot be opened!\n", REG_PHOTO_REPEAT_REGNODE);
	    goto error;
	}

    /* get default value */
    eLIBs_RegGetKeyValue( h_node, REG_PHOTO_REPEAT_SET, REG_PHOTO_REPEAT_KEY_DEFAULT, key_value );

    /* set current value */
    eLIBs_RegSetKeyValue( h_node, REG_PHOTO_REPEAT_SET, REG_PHOTO_REPEAT_KEY_CURRENT, key_value );
    
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



#define LION_REPEAT_NAME_NEXT               		"NEXT"
#define LION_REPEAT_NAME_PREV              			"PREV"
#define LION_REPEAT_NAME_RANDOM              	    "RANDOM"


typedef struct _repeat_map_t
{
	char             name[ELIBS_KEY_VALUE_LEN+1];
	__anole_playorder_t  code;
}repeat_map_t;


const static repeat_map_t repeat_map_tbl[ ] =
{
    { LION_REPEAT_NAME_NEXT , ANOLE_PLAY_NEXT  },
    { LION_REPEAT_NAME_PREV, ANOLE_PLAY_PREV },
    { LION_REPEAT_NAME_RANDOM, ANOLE_PLAY_RANDOM },
};

#define REPEAT_NR           ( sizeof(repeat_map_tbl) / sizeof(repeat_map_tbl[0]) )



/*
****************************************************************************************************
*Name        : _repeat_name2enm
*Prototype   : __anole_playorder_t  _repeat_name2enm( const char *repeat_name )
*Arguments   : repeat_name   input. the name of a repeat
*Return      : the enum value of the repeat.
*Description : get the enum value of the repeat through its name
*Other       :
****************************************************************************************************
*/
static __anole_playorder_t  _repeat_name2enm( const char *repeat_name )
{
	__u32  i;

    if( repeat_name == NULL )
    {
        return ANOLE_PLAY_NEXT;
    }
    
    /* search in table */
	for( i = 0;  i < REPEAT_NR;  i++ )
	{
		if( eLIBs_strcmp( repeat_name, repeat_map_tbl[i].name ) == 0 )
			return repeat_map_tbl[i].code;
	}

	return ANOLE_PLAY_NEXT;
}



/*
****************************************************************************************************
*Name        : _repeat_enm2name
*Prototype   : __s32  _repeat_enm2name( __anole_playorder_t repeat_enm, 
*                                              char *repeat_name, __u32 size )
*Arguments   : repeat_enm    input. the enum value of a repeat
*              repeat_name   output. a buffert to store the name of a repeat
*              size           input. the buffer's size.
*Return      : ==0      succeed
*              !=0      fail
*Description : get the name of the repeat through its enum value
*Other       :
****************************************************************************************************
*/
static __s32  _repeat_enm2name( __anole_playorder_t repeat_enm, char *repeat_name, __u32 size )
{
	__u32  i;
	
	if( repeat_name == NULL || size == 0 )
	{
		return -1;
    }
    
    /* search in table */
	for( i = 0;  i < REPEAT_NR;  i++ )
	{
		if( repeat_enm == repeat_map_tbl[i].code )
		{
			eLIBs_strncpy( repeat_name, repeat_map_tbl[i].name, size );
			repeat_name[size-1] = '\0';
			
			return 0;
		}
	}

	return -1;
}




#endif     //  ifndef __REG_PHOTO_REPEAT_C__

/* end of reg_photo_repeat.c */
