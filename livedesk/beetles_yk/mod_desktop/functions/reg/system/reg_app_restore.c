/*
************************************************************************************************************************
*                                            app restore flag in register table
*
*                                  Copyright(C), 2006-2010, AllWinner Technology Co., Ltd.
*											       All Rights Reserved
*
* File Name   : reg_app_restore.c
*
* Author      : Jackie.chen
*
* Version     : 1.0
*
* Date        : 2010.11.25
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
* Jackie.chen     2010.11.25        1.0         build the file
*
************************************************************************************************************************
*/

#ifndef  __REG_APP_RESTORE_C__
#define  __REG_APP_RESTORE_C__

#include "..\\reg_i.h"

#define REG_APP_RESTORE_NODE "\\system\\app_restore"
#define REG_APP_RESTORE_SET  "app_restore"

typedef struct _app_map_t
{
	lion_app_e  		enm;
	char                name[ELIBS_KEY_VALUE_LEN+1];	
}app_map_t;

typedef struct
{
	__bool 	bool;
	char	chr[ELIBS_KEY_VALUE_LEN+1];
}_bool_char_t;

const static app_map_t app_map_tbl[ ] =
{
    { LION_APP_HOME             , "home"        },
    { LION_APP_EXPLORER         , "explorer"    },
    { LION_APP_MOVIE      		, "movie"       },
    { LION_APP_MUSIC     		, "music"      	},
    { LION_APP_PHOTO 			, "photo"  		},
    { LION_APP_EBOOK			, "ebook" 		},
    { LION_APP_DVD            	, "dvd"         },
    { LION_APP_FM            	, "fm"          },
    { LION_APP_GPS            	, "gps"         },
    { LION_APP_DV            	, "dv"          },
    { LION_APP_RECORDER       	, "recorder"    }    
};

const static _bool_char_t bool_char_tbl[] = 
{
	{0, 	"0"},
	{1, 	"1"}
};

#define APP_NUM (sizeof(app_map_tbl)/sizeof(app_map_tbl[0]))
#define BOOL_CHAR_NUM (sizeof(bool_char_tbl)/sizeof(bool_char_tbl[0]))

static __s32 _app_enm2name(lion_app_e app_enm, char * appname, __u32 size)
{
	__u32  i;
	
	if( appname == NULL || size == 0 )
	{
		return -1;
    }
    
    /* search in table */
	for( i = 0;  i < APP_NUM;  i++ )
	{
		if( app_enm == app_map_tbl[i].enm )
		{
			eLIBs_strncpy( appname, app_map_tbl[i].name, size );
			appname[size-1] = '\0';
			
			return 0;
		}
	}

	return -1;
}

static __s32 _bool2char(__bool bool, char * chr, __u32 size)
{
	__u32  i;
	
	if( chr == NULL || size == 0 )
	{
		return -1;
    }
    
    /* search in table */
	for( i = 0;  i < BOOL_CHAR_NUM;  i++ )
	{
		if( bool == bool_char_tbl[i].bool )
		{
			eLIBs_strncpy( chr, bool_char_tbl[i].chr, size );
			chr[size-1] = '\0';
			
			return 0;
		}
	}

	return -1;
}

static __bool _char2bool(char * chr)
{
	__u32  i;
	__bool ret;
	
	if( chr == NULL )
	{
		return -1;
    }
    
    /* search in table */
	for( i = 0;  i < BOOL_CHAR_NUM;  i++ )
	{
		if( eLIBs_strcmp(bool_char_tbl[i].chr, chr) == 0 )
		{
			ret = bool_char_tbl[i].bool;
			
			return ret;
		}
	}

	return -1;
}

__bool	lion_get_app_restore_flag( lion_app_e app_enm )
{
	__hdle  h_node = NULL;
    char	key_value[ELIBS_KEY_VALUE_LEN+1];   
    char    appname[ELIBS_KEY_VALUE_LEN+1];
	__bool  ret;
    
    /* open reg node */
	h_node = eLIBs_RegOpenNode( REG_APP_RESTORE_NODE, ELIBS_REG_READONLY );
	if( h_node == NULL )
	{
		__wrn("Warning! Reg node %s cannot be opened!\n", REG_APP_RESTORE_NODE);
	    goto error;
	}
    
    /* get key value */
	_app_enm2name(app_enm, appname, ELIBS_KEY_VALUE_LEN);
    eLIBs_RegGetKeyValue( h_node, REG_APP_RESTORE_SET, appname, key_value );    
    ret = _char2bool(key_value);
	
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

__s32	lion_set_app_restore_flag( lion_app_e app_enm, __bool flag )
{
	__hdle  h_node = NULL;      
	char	key_value[ELIBS_KEY_VALUE_LEN+1];
    char    appname[ELIBS_KEY_VALUE_LEN+1];
	__s32   ret;
    
    /* open reg node */
	h_node = eLIBs_RegOpenNode( REG_APP_RESTORE_NODE, ELIBS_REG_READWRITE );
	if( h_node == NULL )
	{
		__wrn("Warning! Reg node %s cannot be opened!\n", REG_APP_RESTORE_NODE);
	    goto error;
	}
    
    /* convert the format of the key value */
    ret = _app_enm2name(app_enm, appname, ELIBS_KEY_VALUE_LEN);
    if( ret != 0 )
    {
        goto error;
    }
    
    /* set key value */
	_bool2char(flag, key_value, ELIBS_KEY_VALUE_LEN);
    eLIBs_RegSetKeyValue( h_node, REG_APP_RESTORE_SET, appname, key_value );
    
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

__s32	lion_set_all_app_restore( void )
{
	__u32 i;
	for( i = 0;  i < APP_NUM;  i++ )
	{
		lion_set_app_restore_flag(app_map_tbl[i].enm, 1);		
	}

	return 0;
}

#endif
