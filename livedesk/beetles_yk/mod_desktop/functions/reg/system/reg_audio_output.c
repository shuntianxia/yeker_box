/*
************************************************************************************************************************
*                                            audio output format in register table
*
*                                  Copyright(C), 2006-2010, AllWinner Technology Co., Ltd.
*											       All Rights Reserved
*
* File Name   : reg_audio_output.c
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
#ifndef  __REG_AUDIO_OUTPUT_C__
#define  __REG_AUDIO_OUTPUT_C__


#include "..\\reg_i.h"


#define REG_AUDIO_OUTPUT_REGNODE             "\\system\\audio_output"
#define REG_AUDIO_OUTPUT_SET                 "audio_output"
#define REG_AUDIO_OUTPUT_KEY_CURRENT         "current"
#define REG_AUDIO_OUTPUT_KEY_DEFAULT         "default"


static __lion_audio_output_type_e  _audio_output_name2enm( const char *audio_output_name );
static __s32  _audio_output_enm2name( __lion_audio_output_type_e audio_output_enm, char *audio_output_name, __u32 size );


/*
****************************************************************************************************
*Name        : lion_reg_audio_output_get
*Prototype   : __lion_audio_output_type_e lion_reg_audio_output_get( void )
*Arguments   : void
*Return      : the current audio output in register table.
*Description : get the current audio output in register table.
*Other       :
****************************************************************************************************
*/
__lion_audio_output_type_e lion_reg_audio_output_get( void )
{
    __hdle  h_node = NULL;
    char    key_value[ELIBS_KEY_VALUE_LEN+1];   
    __lion_audio_output_type_e  ret;
    
    /* open reg node */
	h_node = eLIBs_RegOpenNode( REG_AUDIO_OUTPUT_REGNODE, ELIBS_REG_READONLY );
	if( h_node == NULL )
	{
		__wrn("Warning! Reg node %s cannot be opened!\n", REG_AUDIO_OUTPUT_REGNODE);
	    goto error;
	}
    
    /* get key value */
    eLIBs_RegGetKeyValue( h_node, REG_AUDIO_OUTPUT_SET, REG_AUDIO_OUTPUT_KEY_CURRENT, key_value );
    
    /* convert the format of the key value */
    ret = _audio_output_name2enm( key_value );
    
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
    return LION_AUDIO_OUTPUT_UNKNOWN;
}



/*
****************************************************************************************************
*Name        : lion_reg_audio_output_set
*Prototype   : __s32 lion_reg_audio_output_set( __lion_audio_output_type_e audio_output )
*Arguments   : audio_output  input. audio output.
*Return      : EPDK_OK    succeed
*              EPDK_FAIL  fail
*Description : set the current audio output in register table.
*Other       :
****************************************************************************************************
*/
__s32 lion_reg_audio_output_set( __lion_audio_output_type_e audio_output )
{
    __hdle  h_node = NULL;
    char    key_value[ELIBS_KEY_VALUE_LEN+1];   
    __s32   ret;
    
    /* open reg node */
	h_node = eLIBs_RegOpenNode( REG_AUDIO_OUTPUT_REGNODE, ELIBS_REG_READWRITE );
	if( h_node == NULL )
	{
		__wrn("Warning! Reg node %s cannot be opened!\n", REG_AUDIO_OUTPUT_REGNODE);
	    goto error;
	}
    
    /* convert the format of the key value */
    ret = _audio_output_enm2name( audio_output, key_value, ELIBS_KEY_VALUE_LEN );
    if( ret != 0 )
    {
        goto error;
    }
    
    /* set key value */
    eLIBs_RegSetKeyValue( h_node, REG_AUDIO_OUTPUT_SET, REG_AUDIO_OUTPUT_KEY_CURRENT, key_value );
    
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
*Name        : lion_reg_audio_output_get_default
*Prototype   : __lion_audio_output_type_e lion_reg_audio_output_get_default( void )
*Arguments   : void
*Return      : the default audio output in register table.
*Description : get the default audio output in register table.
*Other       :
****************************************************************************************************
*/
__lion_audio_output_type_e lion_reg_audio_output_get_default( void )
{
    __hdle  h_node = NULL;
    char    key_value[ELIBS_KEY_VALUE_LEN+1];   
    __lion_audio_output_type_e  ret;
    
    /* open reg node */
	h_node = eLIBs_RegOpenNode( REG_AUDIO_OUTPUT_REGNODE, ELIBS_REG_READONLY );
	if( h_node == NULL )
	{
		__wrn("Warning! Reg node %s cannot be opened!\n", REG_AUDIO_OUTPUT_REGNODE);
	    goto error;
	}
    
    /* get key value */
    eLIBs_RegGetKeyValue( h_node, REG_AUDIO_OUTPUT_SET, REG_AUDIO_OUTPUT_KEY_DEFAULT, key_value );
    
    /* convert the format of the key value */
    ret = _audio_output_name2enm( key_value );
    
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
    return LION_AUDIO_OUTPUT_UNKNOWN;
}



/*
****************************************************************************************************
*Name        : lion_reg_audio_output_restore
*Prototype   : __s32 lion_reg_audio_output_restore( void )
*Arguments   : void
*Return      : EPDK_OK    succeed
*              EPDK_FAIL  fail
*Description : restore the default value of audio output in register table.
*Other       :
****************************************************************************************************
*/
__s32 lion_reg_audio_output_restore( void )
{
    __hdle  h_node = NULL;
    char    key_value[ELIBS_KEY_VALUE_LEN+1];   
    
    /* open reg node */
	h_node = eLIBs_RegOpenNode( REG_AUDIO_OUTPUT_REGNODE, ELIBS_REG_READWRITE );
	if( h_node == NULL )
	{
		__wrn("Warning! Reg node %s cannot be opened!\n", REG_AUDIO_OUTPUT_REGNODE);
	    goto error;
	}

    /* get default value */
    eLIBs_RegGetKeyValue( h_node, REG_AUDIO_OUTPUT_SET, REG_AUDIO_OUTPUT_KEY_DEFAULT, key_value );

    /* set current value */
    eLIBs_RegSetKeyValue( h_node, REG_AUDIO_OUTPUT_SET, REG_AUDIO_OUTPUT_KEY_CURRENT, key_value );
    
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



#define LION_AUDIO_OUTPUT_NAME_NORMAL                     "NORMAL"
#define LION_AUDIO_OUTPUT_NAME_SPDIF_LPCM                 "SPDIF_LPCM"                               
#define LION_AUDIO_OUTPUT_NAME_SPDIF_RAWDATA              "SPDIF_RAWDATA"                           


typedef struct _audio_output_map_t
{
	char                        name[ELIBS_KEY_VALUE_LEN+1];
	__lion_audio_output_type_e  code;
}audio_output_map_t;


const static audio_output_map_t audio_output_map_tbl[ ] =
{
    { LION_AUDIO_OUTPUT_NAME_NORMAL       , LION_AUDIO_OUTPUT_NORMAL        },
    { LION_AUDIO_OUTPUT_NAME_SPDIF_LPCM   , LION_AUDIO_OUTPUT_SPDIF_LPCM    },
    { LION_AUDIO_OUTPUT_NAME_SPDIF_RAWDATA, LION_AUDIO_OUTPUT_SPDIF_RAWDATA }
};

#define AUDIO_OUTPUT_NR           ( sizeof(audio_output_map_tbl) / sizeof(audio_output_map_tbl[0]) )



/*
****************************************************************************************************
*Name        : _audio_output_name2enm
*Prototype   : __lion_audio_output_type_e  _audio_output_name2enm( const char *audio_output_name )
*Arguments   : audio_output_name   input. the name of a audio output
*Return      : the enum value of the audio output.
*Description : get the enum value of the audio output through its name
*Other       :
****************************************************************************************************
*/
static __lion_audio_output_type_e  _audio_output_name2enm( const char *audio_output_name )
{
	__u32  i;

    if( audio_output_name == NULL )
    {
        return LION_AUDIO_OUTPUT_UNKNOWN;
    }
    
    /* search in table */
	for( i = 0;  i < AUDIO_OUTPUT_NR;  i++ )
	{
		if( eLIBs_strcmp( audio_output_name, audio_output_map_tbl[i].name ) == 0 )
			return audio_output_map_tbl[i].code;
	}

	return LION_AUDIO_OUTPUT_UNKNOWN;
}



/*
****************************************************************************************************
*Name        : _audio_output_enm2name
*Prototype   : __s32  _audio_output_enm2name( __lion_audio_output_type_e audio_output_enm, 
*                                              char *audio_output_name, __u32 size )
*Arguments   : audio_output_enm    input. the enum value of a audio output
*              audio_output_name   output. a buffert to store the name of a audio output
*              size           input. the buffer's size.
*Return      : ==0      succeed
*              !=0      fail
*Description : get the name of the audio output through its enum value
*Other       :
****************************************************************************************************
*/
static __s32  _audio_output_enm2name( __lion_audio_output_type_e audio_output_enm, char *audio_output_name, __u32 size )
{
	__u32  i;
	
	if( audio_output_name == NULL || size == 0 )
	{
		return -1;
    }
    
    /* search in table */
	for( i = 0;  i < AUDIO_OUTPUT_NR;  i++ )
	{
		if( audio_output_enm == audio_output_map_tbl[i].code )
		{
			eLIBs_strncpy( audio_output_name, audio_output_map_tbl[i].name, size );
			audio_output_name[size-1] = '\0';
			
			return 0;
		}
	}

	return -1;
}




#endif     //  ifndef __REG_AUDIO_OUTPUT_C__

/* end of reg_audio_output.c */
