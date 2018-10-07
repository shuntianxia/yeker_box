/*
************************************************************************************************************************
*                                            channel in register table
*
*                                  Copyright(C), 2006-2010, AllWinner Technology Co., Ltd.
*											       All Rights Reserved
*
* File Name   : reg_channel.c
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
#ifndef  __REG_CHANNEL_C__
#define  __REG_CHANNEL_C__


#include "..\\reg_i.h"


#define REG_CHANNEL_REGNODE             "\\system\\channel"
#define REG_CHANNEL_SET                 "channel"
#define REG_CHANNEL_KEY_CURRENT         "current"
#define REG_CHANNEL_KEY_DEFAULT         "default"


static __audio_dev_chan_mode_t  _channel_name2enm( const char *channel_name );
static __s32  _channel_enm2name( __audio_dev_chan_mode_t channel_enm, char *channel_name, __u32 size );


/*
****************************************************************************************************
*Name        : lion_reg_channel_get
*Prototype   : __audio_dev_chan_mode_t lion_reg_channel_get( void )
*Arguments   : void
*Return      : the current channel in register table.
*Description : get the current channel in register table.
*Other       :
****************************************************************************************************
*/
__audio_dev_chan_mode_t lion_reg_channel_get( void )
{
    __hdle  h_node = NULL;
    char    key_value[ELIBS_KEY_VALUE_LEN+1];   
    __audio_dev_chan_mode_t  ret;
    
    /* open reg node */
	h_node = eLIBs_RegOpenNode( REG_CHANNEL_REGNODE, ELIBS_REG_READONLY );
	if( h_node == NULL )
	{
		__wrn("Warning! Reg node %s cannot be opened!\n", REG_CHANNEL_REGNODE);
	    goto error;
	}
    
    /* get key value */
    eLIBs_RegGetKeyValue( h_node, REG_CHANNEL_SET, REG_CHANNEL_KEY_CURRENT, key_value );
    
    /* convert the format of the key value */
    ret = _channel_name2enm( key_value );
    
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
    return AUDIO_DEV_CHANNEL_STEREO;
}



/*
****************************************************************************************************
*Name        : lion_reg_channel_set
*Prototype   : __s32 lion_reg_channel_set( __audio_dev_chan_mode_t channel )
*Arguments   : channel  input. channel.
*Return      : EPDK_OK    succeed
*              EPDK_FAIL  fail
*Description : set the current channel in register table.
*Other       :
****************************************************************************************************
*/
__s32 lion_reg_channel_set( __audio_dev_chan_mode_t channel )
{
    __hdle  h_node = NULL;
    char    key_value[ELIBS_KEY_VALUE_LEN+1];   
    __s32   ret;
    
    /* open reg node */
	h_node = eLIBs_RegOpenNode( REG_CHANNEL_REGNODE, ELIBS_REG_READWRITE );
	if( h_node == NULL )
	{
		__wrn("Warning! Reg node %s cannot be opened!\n", REG_CHANNEL_REGNODE);
	    goto error;
	}
    
    /* convert the format of the key value */
    ret = _channel_enm2name( channel, key_value, ELIBS_KEY_VALUE_LEN );
    if( ret != 0 )
    {
        goto error;
    }
    
    /* set key value */
    eLIBs_RegSetKeyValue( h_node, REG_CHANNEL_SET, REG_CHANNEL_KEY_CURRENT, key_value );
    
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
*Name        : lion_reg_channel_get_default
*Prototype   : __audio_dev_chan_mode_t lion_reg_channel_get_default( void )
*Arguments   : void
*Return      : the default channel in register table.
*Description : get the default channel in register table.
*Other       :
****************************************************************************************************
*/
__audio_dev_chan_mode_t lion_reg_channel_get_default( void )
{
    __hdle  h_node = NULL;
    char    key_value[ELIBS_KEY_VALUE_LEN+1];   
    __audio_dev_chan_mode_t  ret;
    
    /* open reg node */
	h_node = eLIBs_RegOpenNode( REG_CHANNEL_REGNODE, ELIBS_REG_READONLY );
	if( h_node == NULL )
	{
		__wrn("Warning! Reg node %s cannot be opened!\n", REG_CHANNEL_REGNODE);
	    goto error;
	}
    
    /* get key value */
    eLIBs_RegGetKeyValue( h_node, REG_CHANNEL_SET, REG_CHANNEL_KEY_DEFAULT, key_value );
    
    /* convert the format of the key value */
    ret = _channel_name2enm( key_value );
    
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
    return AUDIO_DEV_CHANNEL_STEREO;
}



/*
****************************************************************************************************
*Name        : lion_reg_channel_restore
*Prototype   : __s32 lion_reg_channel_restore( void )
*Arguments   : void
*Return      : EPDK_OK    succeed
*              EPDK_FAIL  fail
*Description : restore the default value of channel in register table.
*Other       :
****************************************************************************************************
*/
__s32 lion_reg_channel_restore( void )
{
    __hdle  h_node = NULL;
    char    key_value[ELIBS_KEY_VALUE_LEN+1];   
    
    /* open reg node */
	h_node = eLIBs_RegOpenNode( REG_CHANNEL_REGNODE, ELIBS_REG_READWRITE );
	if( h_node == NULL )
	{
		__wrn("Warning! Reg node %s cannot be opened!\n", REG_CHANNEL_REGNODE);
	    goto error;
	}

    /* get default value */
    eLIBs_RegGetKeyValue( h_node, REG_CHANNEL_SET, REG_CHANNEL_KEY_DEFAULT, key_value );

    /* set current value */
    eLIBs_RegSetKeyValue( h_node, REG_CHANNEL_SET, REG_CHANNEL_KEY_CURRENT, key_value );
    
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



#define LION_CHANNEL_NAME_STEREO              "STEREO"
#define LION_CHANNEL_NAME_LEFT                "LEFT"                           
#define LION_CHANNEL_NAME_RIGHT               "RIGHT"                            


typedef struct _channel_map_t
{
	char                     name[ELIBS_KEY_VALUE_LEN+1];
	__audio_dev_chan_mode_t  code;
}channel_map_t;


const static channel_map_t channel_map_tbl[ ] =
{
    { LION_CHANNEL_NAME_STEREO, AUDIO_DEV_CHANNEL_STEREO },
    { LION_CHANNEL_NAME_LEFT  , AUDIO_DEV_CHANNEL_LEFT   },
    { LION_CHANNEL_NAME_RIGHT , AUDIO_DEV_CHANNEL_RIGHT  }
};

#define CHANNEL_NR           ( sizeof(channel_map_tbl) / sizeof(channel_map_tbl[0]) )



/*
****************************************************************************************************
*Name        : _channel_name2enm
*Prototype   : __audio_dev_chan_mode_t  _channel_name2enm( const char *channel_name )
*Arguments   : channel_name   input. the name of a channel
*Return      : the enum value of the channel. return LION_CHANNEL_NAME_STEREO, if fail.
*Description : get the enum value of the channel through its name
*Other       :
****************************************************************************************************
*/
static __audio_dev_chan_mode_t  _channel_name2enm( const char *channel_name )
{
	__u32  i;

    if( channel_name == NULL )
    {
        return AUDIO_DEV_CHANNEL_STEREO;
    }
    
    /* search in table */
	for( i = 0;  i < CHANNEL_NR;  i++ )
	{
		if( eLIBs_strcmp( channel_name, channel_map_tbl[i].name ) == 0 )
			return channel_map_tbl[i].code;
	}

	return AUDIO_DEV_CHANNEL_STEREO;
}



/*
****************************************************************************************************
*Name        : _channel_enm2name
*Prototype   : __s32  _channel_enm2name( __audio_dev_chan_mode_t channel_enm, 
*                                              char *channel_name, __u32 size )
*Arguments   : channel_enm    input. the enum value of a channel
*              channel_name   output. a buffert to store the name of a channel
*              size           input. the buffer's size.
*Return      : ==0      succeed
*              !=0      fail
*Description : get the name of the channel through its enum value
*Other       :
****************************************************************************************************
*/
static __s32  _channel_enm2name( __audio_dev_chan_mode_t channel_enm, char *channel_name, __u32 size )
{
	__u32  i;
	
	if( channel_name == NULL || size == 0 )
	{
		return -1;
    }
    
    /* search in table */
	for( i = 0;  i < CHANNEL_NR;  i++ )
	{
		if( channel_enm == channel_map_tbl[i].code )
		{
			eLIBs_strncpy( channel_name, channel_map_tbl[i].name, size );
			channel_name[size-1] = '\0';
			
			return 0;
		}
	}

	return -1;
}




#endif     //  ifndef __REG_CHANNEL_C__

/* end of reg_channel.c */
