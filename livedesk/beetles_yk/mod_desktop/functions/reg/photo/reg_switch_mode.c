/*
************************************************************************************************************************
*                                            switch mode in register table
*
*                                  Copyright(C), 2006-2010, AllWinner Technology Co., Ltd.
*											       All Rights Reserved
*
* File Name   : reg_switch_mode.c
*
* Author      : Gary.Wang
*
* Version     : 1.0
*
* Date        : 2010.09.01
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
* Gary.Wang      2010.09.01        1.0         build the file
*
************************************************************************************************************************
*/
#ifndef  __REG_SWITCH_MODE_C__
#define  __REG_SWITCH_MODE_C__


#include "..\\reg_i.h"


#define REG_SWITCH_MODE_REGNODE             "\\photo\\switch_mode"
#define REG_SWITCH_MODE_SET                 "switch_mode"
#define REG_SWITCH_MODE_KEY_CURRENT         "current"
#define REG_SWITCH_MODE_KEY_DEFAULT         "default"


static __willow_switch_mode_t  _switch_mode_name2enm( const char *switch_mode_name );
static __s32  _switch_mode_enm2name( __willow_switch_mode_t switch_mode_enm, char *switch_mode_name, __u32 size );


/*
****************************************************************************************************
*Name        : lion_reg_switch_mode_get
*Prototype   : __willow_switch_mode_t lion_reg_switch_mode_get( void )
*Arguments   : void
*Return      : the current switch mode in register table.
*Description : get the current switch mode in register table.
*Other       :
****************************************************************************************************
*/
__willow_switch_mode_t lion_reg_switch_mode_get( void )
{
    __hdle  h_node = NULL;
    char    key_value[ELIBS_KEY_VALUE_LEN+1];   
    __willow_switch_mode_t  ret;
    
    /* open reg node */
	h_node = eLIBs_RegOpenNode( REG_SWITCH_MODE_REGNODE, ELIBS_REG_READONLY );
	if( h_node == NULL )
	{
		__wrn("Warning! Reg node %s cannot be opened!\n", REG_SWITCH_MODE_REGNODE);
	    goto error;
	}
    
    /* get key value */
    eLIBs_RegGetKeyValue( h_node, REG_SWITCH_MODE_SET, REG_SWITCH_MODE_KEY_CURRENT, key_value );
    
    /* convert the format of the key value */
    ret = _switch_mode_name2enm( key_value );
    
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
    return DEFAULT_SWITCH;
}



/*
****************************************************************************************************
*Name        : lion_reg_switch_mode_set
*Prototype   : __s32 lion_reg_switch_mode_set( __willow_switch_mode_t switch_mode )
*Arguments   : switch_mode  input. switch mode.
*Return      : EPDK_OK    succeed
*              EPDK_FAIL  fail
*Description : set the current switch mode in register table.
*Other       :
****************************************************************************************************
*/
__s32 lion_reg_switch_mode_set( __willow_switch_mode_t switch_mode )
{
    __hdle  h_node = NULL;
    char    key_value[ELIBS_KEY_VALUE_LEN+1];   
    __s32   ret;
    
    /* open reg node */
	h_node = eLIBs_RegOpenNode( REG_SWITCH_MODE_REGNODE, ELIBS_REG_READWRITE );
	if( h_node == NULL )
	{
		__wrn("Warning! Reg node %s cannot be opened!\n", REG_SWITCH_MODE_REGNODE);
	    goto error;
	}
    
    /* convert the format of the key value */
    ret = _switch_mode_enm2name( switch_mode, key_value, ELIBS_KEY_VALUE_LEN );
    if( ret != 0 )
    {
        goto error;
    }
    
    /* set key value */
    eLIBs_RegSetKeyValue( h_node, REG_SWITCH_MODE_SET, REG_SWITCH_MODE_KEY_CURRENT, key_value );
    
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
*Name        : lion_reg_switch_mode_get_default
*Prototype   : __willow_switch_mode_t lion_reg_switch_mode_get_default( void )
*Arguments   : void
*Return      : the default switch mode in register table.
*Description : get the default switch mode in register table.
*Other       :
****************************************************************************************************
*/
__willow_switch_mode_t lion_reg_switch_mode_get_default( void )
{
    __hdle  h_node = NULL;
    char    key_value[ELIBS_KEY_VALUE_LEN+1];   
    __willow_switch_mode_t  ret;
    
    /* open reg node */
	h_node = eLIBs_RegOpenNode( REG_SWITCH_MODE_REGNODE, ELIBS_REG_READONLY );
	if( h_node == NULL )
	{
		__wrn("Warning! Reg node %s cannot be opened!\n", REG_SWITCH_MODE_REGNODE);
	    goto error;
	}
    
    /* get key value */
    eLIBs_RegGetKeyValue( h_node, REG_SWITCH_MODE_SET, REG_SWITCH_MODE_KEY_DEFAULT, key_value );
    
    /* convert the format of the key value */
    ret = _switch_mode_name2enm( key_value );
    
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
    return DEFAULT_SWITCH;
}



/*
****************************************************************************************************
*Name        : lion_reg_switch_mode_restore
*Prototype   : __s32 lion_reg_switch_mode_restore( void )
*Arguments   : void
*Return      : EPDK_OK    succeed
*              EPDK_FAIL  fail
*Description : restore the default value of switch mode in register table.
*Other       :
****************************************************************************************************
*/
__s32 lion_reg_switch_mode_restore( void )
{
    __hdle  h_node = NULL;
    char    key_value[ELIBS_KEY_VALUE_LEN+1];   
    
    /* open reg node */
	h_node = eLIBs_RegOpenNode( REG_SWITCH_MODE_REGNODE, ELIBS_REG_READWRITE );
	if( h_node == NULL )
	{
		__wrn("Warning! Reg node %s cannot be opened!\n", REG_SWITCH_MODE_REGNODE);
	    goto error;
	}

    /* get default value */
    eLIBs_RegGetKeyValue( h_node, REG_SWITCH_MODE_SET, REG_SWITCH_MODE_KEY_DEFAULT, key_value );

    /* set current value */
    eLIBs_RegSetKeyValue( h_node, REG_SWITCH_MODE_SET, REG_SWITCH_MODE_KEY_CURRENT, key_value );
    
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



#define LION_SWITCH_MODE_NAME_DEFAULT_SWITCH      "DEFAULT"
#define LION_SWITCH_MODE_NAME_FADE_IN_OUT         "FADE_IN_OUT"
#define LION_SWITCH_MODE_NAME_PERSIAN_BLIND_H     "BLIND_H"
#define LION_SWITCH_MODE_NAME_PERSIAN_BLIND_V     "BLIND_V"
#define LION_SWITCH_MODE_NAME_SLID_UP             "SLID_UP"
#define LION_SWITCH_MODE_NAME_SLID_DOWN           "SLID_DOWN"
#define LION_SWITCH_MODE_NAME_SLID_LEFT           "SLID_LEFT"
#define LION_SWITCH_MODE_NAME_SLID_RIGHT          "SLID_RIGHT"
#define LION_SWITCH_MODE_NAME_STRETCH_UP          "STRETCH_UP"
#define LION_SWITCH_MODE_NAME_STRETCH_DOWN        "STRETCH_DOWN"
#define LION_SWITCH_MODE_NAME_STRETCH_LEFT        "STRETCH_LEFT"
#define LION_SWITCH_MODE_NAME_STRETCH_RIGHT       "STRETCH_RIGHT"
#define LION_SWITCH_MODE_NAME_MOSAIC              "MOSAIC"
#define LION_SWITCH_MODE_NAME_ROOM_IN             "ROOM_IN"
#define LION_SWITCH_MODE_NAME_ROOM_OUT            "ROOM_OUT"

                              
                              
typedef struct _switch_mode_map_t
{
	char                    name[ELIBS_KEY_VALUE_LEN+1];
	__willow_switch_mode_t  code;
}switch_mode_map_t;


const static switch_mode_map_t switch_mode_map_tbl[ ] =
{
    { LION_SWITCH_MODE_NAME_DEFAULT_SWITCH , DEFAULT_SWITCH  },
    { LION_SWITCH_MODE_NAME_FADE_IN_OUT    , FADE_IN_OUT     },
    { LION_SWITCH_MODE_NAME_PERSIAN_BLIND_H, PERSIAN_BLIND_H },
    { LION_SWITCH_MODE_NAME_PERSIAN_BLIND_V, PERSIAN_BLIND_V },
    { LION_SWITCH_MODE_NAME_SLID_UP        , SLID_UP         },
    { LION_SWITCH_MODE_NAME_SLID_DOWN      , SLID_DOWN       },
    { LION_SWITCH_MODE_NAME_SLID_LEFT      , SLID_LEFT       },
    { LION_SWITCH_MODE_NAME_SLID_RIGHT     , SLID_RIGHT      },
    { LION_SWITCH_MODE_NAME_STRETCH_UP     , STRETCH_UP      },
    { LION_SWITCH_MODE_NAME_STRETCH_DOWN   , STRETCH_DOWN    },
    { LION_SWITCH_MODE_NAME_STRETCH_LEFT   , STRETCH_LEFT    },
    { LION_SWITCH_MODE_NAME_STRETCH_RIGHT  , STRETCH_RIGHT   },
    { LION_SWITCH_MODE_NAME_MOSAIC         , MOSAIC          },
    { LION_SWITCH_MODE_NAME_ROOM_IN        , ROOM_IN         },
    { LION_SWITCH_MODE_NAME_ROOM_OUT       , ROOM_OUT        }
};

#define SWITCH_MODE_NR           ( sizeof(switch_mode_map_tbl) / sizeof(switch_mode_map_tbl[0]) )



/*
****************************************************************************************************
*Name        : _switch_mode_name2enm
*Prototype   : __willow_switch_mode_t  _switch_mode_name2enm( const char *switch_mode_name )
*Arguments   : switch_mode_name   input. the name of a switch mode
*Return      : the enum value of the switch mode.
*Description : get the enum value of the switch mode through its name
*Other       :
****************************************************************************************************
*/
static __willow_switch_mode_t  _switch_mode_name2enm( const char *switch_mode_name )
{
	__u32  i;

    if( switch_mode_name == NULL )
    {
        return DEFAULT_SWITCH;
    }
    
    /* search in table */
	for( i = 0;  i < SWITCH_MODE_NR;  i++ )
	{
		if( eLIBs_strcmp( switch_mode_name, switch_mode_map_tbl[i].name ) == 0 )
			return switch_mode_map_tbl[i].code;
	}

	return DEFAULT_SWITCH;
}



/*
****************************************************************************************************
*Name        : _switch_mode_enm2name
*Prototype   : __s32  _switch_mode_enm2name( __willow_switch_mode_t switch_mode_enm, 
*                                              char *switch_mode_name, __u32 size )
*Arguments   : switch_mode_enm    input. the enum value of a switch mode
*              switch_mode_name   output. a buffert to store the name of a switch mode
*              size           input. the buffer's size.
*Return      : ==0      succeed
*              !=0      fail
*Description : get the name of the switch mode through its enum value
*Other       :
****************************************************************************************************
*/
static __s32  _switch_mode_enm2name( __willow_switch_mode_t switch_mode_enm, char *switch_mode_name, __u32 size )
{
	__u32  i;
	
	if( switch_mode_name == NULL || size == 0 )
	{
		return -1;
    }
    
    /* search in table */
	for( i = 0;  i < SWITCH_MODE_NR;  i++ )
	{
		if( switch_mode_enm == switch_mode_map_tbl[i].code )
		{
			eLIBs_strncpy( switch_mode_name, switch_mode_map_tbl[i].name, size );
			switch_mode_name[size-1] = '\0';
			
			return 0;
		}
	}

	return -1;
}




#endif     //  ifndef __REG_SWITCH_MODE_C__

/* end of reg_switch_mode.c */
