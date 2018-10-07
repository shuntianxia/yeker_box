/*
************************************************************************************************************************
*                                            zoom in register table
*
*                                  Copyright(C), 2006-2010, AllWinner Technology Co., Ltd.
*											       All Rights Reserved
*
* File Name   : reg_zoom.c
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
#ifndef  __REG_ZOOM_C__
#define  __REG_ZOOM_C__


#include "..\\reg_i.h"


#define REG_ZOOM_REGNODE             "\\movie\\zoom"
#define REG_ZOOM_SET                 "zoom"
#define REG_ZOOM_KEY_CURRENT         "current"
#define REG_ZOOM_KEY_DEFAULT         "default"


static robin_zoom_e  _zoom_name2enm( const char *zoom_name );
static __s32  _zoom_enm2name( robin_zoom_e zoom_enm, char *zoom_name, __u32 size );


/*
****************************************************************************************************
*Name        : lion_reg_zoom_get
*Prototype   : robin_zoom_e lion_reg_zoom_get( void )
*Arguments   : void
*Return      : the current zoom in register table.
*Description : get the current zoom in register table.
*Other       :
****************************************************************************************************
*/
robin_zoom_e lion_reg_zoom_get( void )
{
    __hdle  h_node = NULL;
    char    key_value[ELIBS_KEY_VALUE_LEN+1];   
    robin_zoom_e  ret;
    
    /* open reg node */
	h_node = eLIBs_RegOpenNode( REG_ZOOM_REGNODE, ELIBS_REG_READONLY );
	if( h_node == NULL )
	{
		__wrn("Warning! Reg node %s cannot be opened!\n", REG_ZOOM_REGNODE);
	    goto error;
	}
    
    /* get key value */
    eLIBs_RegGetKeyValue( h_node, REG_ZOOM_SET, REG_ZOOM_KEY_CURRENT, key_value );
    
    /* convert the format of the key value */
    ret = _zoom_name2enm( key_value );
    
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
    return ROBIN_ZOOM_UNKNOWN;
}



/*
****************************************************************************************************
*Name        : lion_reg_zoom_set
*Prototype   : __s32 lion_reg_zoom_set( robin_zoom_e zoom )
*Arguments   : zoom  input. zoom.
*Return      : EPDK_OK    succeed
*              EPDK_FAIL  fail
*Description : set the current zoom in register table.
*Other       :
****************************************************************************************************
*/
__s32 lion_reg_zoom_set( robin_zoom_e zoom )
{
    __hdle  h_node = NULL;
    char    key_value[ELIBS_KEY_VALUE_LEN+1];   
    __s32   ret;
    
    /* open reg node */
	h_node = eLIBs_RegOpenNode( REG_ZOOM_REGNODE, ELIBS_REG_READWRITE );
	if( h_node == NULL )
	{
		__wrn("Warning! Reg node %s cannot be opened!\n", REG_ZOOM_REGNODE);
	    goto error;
	}
    
    /* convert the format of the key value */
    ret = _zoom_enm2name( zoom, key_value, ELIBS_KEY_VALUE_LEN );
    if( ret != 0 )
    {
        goto error;
    }
    
    /* set key value */
    eLIBs_RegSetKeyValue( h_node, REG_ZOOM_SET, REG_ZOOM_KEY_CURRENT, key_value );
    
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
*Name        : lion_reg_zoom_get_default
*Prototype   : robin_zoom_e lion_reg_zoom_get_default( void )
*Arguments   : void
*Return      : the default zoom in register table.
*Description : get the default zoom in register table.
*Other       :
****************************************************************************************************
*/
robin_zoom_e lion_reg_zoom_get_default( void )
{
    __hdle  h_node = NULL;
    char    key_value[ELIBS_KEY_VALUE_LEN+1];   
    robin_zoom_e  ret;
    
    /* open reg node */
	h_node = eLIBs_RegOpenNode( REG_ZOOM_REGNODE, ELIBS_REG_READONLY );
	if( h_node == NULL )
	{
		__wrn("Warning! Reg node %s cannot be opened!\n", REG_ZOOM_REGNODE);
	    goto error;
	}
    
    /* get key value */
    eLIBs_RegGetKeyValue( h_node, REG_ZOOM_SET, REG_ZOOM_KEY_DEFAULT, key_value );
    
    /* convert the format of the key value */
    ret = _zoom_name2enm( key_value );
    
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
    return ROBIN_ZOOM_UNKNOWN;
}



/*
****************************************************************************************************
*Name        : lion_reg_zoom_restore
*Prototype   : __s32 lion_reg_zoom_restore( void )
*Arguments   : void
*Return      : EPDK_OK    succeed
*              EPDK_FAIL  fail
*Description : restore the default value of zoom in register table.
*Other       :
****************************************************************************************************
*/
__s32 lion_reg_zoom_restore( void )
{
    __hdle  h_node = NULL;
    char    key_value[ELIBS_KEY_VALUE_LEN+1];   
    
    /* open reg node */
	h_node = eLIBs_RegOpenNode( REG_ZOOM_REGNODE, ELIBS_REG_READWRITE );
	if( h_node == NULL )
	{
		__wrn("Warning! Reg node %s cannot be opened!\n", REG_ZOOM_REGNODE);
	    goto error;
	}

    /* get default value */
    eLIBs_RegGetKeyValue( h_node, REG_ZOOM_SET, REG_ZOOM_KEY_DEFAULT, key_value );

    /* set current value */
    eLIBs_RegSetKeyValue( h_node, REG_ZOOM_SET, REG_ZOOM_KEY_CURRENT, key_value );
    
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



#define LION_ZOOM_NAME_FULL_SCREEN_V                         "FULL_SCREEN_V"
#define LION_ZOOM_NAME_FULL_SCREEN_S                         "FULL_SCREEN_S"
#define LION_ZOOM_NAME_ORIGIN_SIZE                           "ORIGIN_SIZE"
#define LION_ZOOM_NAME_4R3                                   "4R3"
#define LION_ZOOM_NAME_16R9                                  "16R9"
#define LION_ZOOM_NAME_FIT_VIEW                              "FIT_VIEW"



typedef struct _zoom_map_t
{
	char          name[ELIBS_KEY_VALUE_LEN+1];
	robin_zoom_e  code;
}zoom_map_t;


const static zoom_map_t zoom_map_tbl[ ] =
{
    { LION_ZOOM_NAME_FULL_SCREEN_V, ROBIN_ZOOM_FULL_SCREEN_VIDEO_RATIO  },
    { LION_ZOOM_NAME_FULL_SCREEN_S, ROBIN_ZOOM_FULL_SCREEN_SCREEN_RATIO },
    { LION_ZOOM_NAME_ORIGIN_SIZE  , ROBIN_ZOOM_ORIGIN_SIZE              },
    { LION_ZOOM_NAME_4R3          , ROBIN_ZOOM_4R3                      },
    { LION_ZOOM_NAME_16R9         , ROBIN_ZOOM_16R9                     },
    { LION_ZOOM_NAME_FIT_VIEW     , ROBIN_ZOOM_FIT_VIEW                 }
};

#define ZOOM_NR           ( sizeof(zoom_map_tbl) / sizeof(zoom_map_tbl[0]) )



/*
****************************************************************************************************
*Name        : _zoom_name2enm
*Prototype   : robin_zoom_e  _zoom_name2enm( const char *zoom_name )
*Arguments   : zoom_name   input. the name of a zoom
*Return      : the enum value of the zoom.
*Description : get the enum value of the zoom through its name
*Other       :
****************************************************************************************************
*/
static robin_zoom_e  _zoom_name2enm( const char *zoom_name )
{
	__u32  i;

    if( zoom_name == NULL )
    {
        return ROBIN_ZOOM_UNKNOWN;
    }
    
    /* search in table */
	for( i = 0;  i < ZOOM_NR;  i++ )
	{
		if( eLIBs_strcmp( zoom_name, zoom_map_tbl[i].name ) == 0 )
			return zoom_map_tbl[i].code;
	}

	return ROBIN_ZOOM_UNKNOWN;
}



/*
****************************************************************************************************
*Name        : _zoom_enm2name
*Prototype   : __s32  _zoom_enm2name( robin_zoom_e zoom_enm, 
*                                              char *zoom_name, __u32 size )
*Arguments   : zoom_enm    input. the enum value of a zoom
*              zoom_name   output. a buffert to store the name of a zoom
*              size        input. the buffer's size.
*Return      : ==0      succeed
*              !=0      fail
*Description : get the name of the zoom through its enum value
*Other       :
****************************************************************************************************
*/
static __s32  _zoom_enm2name( robin_zoom_e zoom_enm, char *zoom_name, __u32 size )
{
	__u32  i;
	
	if( zoom_name == NULL || size == 0 )
	{
		return -1;
    }
    
    /* search in table */
	for( i = 0;  i < ZOOM_NR;  i++ )
	{
		if( zoom_enm == zoom_map_tbl[i].code )
		{
			eLIBs_strncpy( zoom_name, zoom_map_tbl[i].name, size );
			zoom_name[size-1] = '\0';
			
			return 0;
		}
	}

	return -1;
}




#endif     //  ifndef __REG_ZOOM_C__

/* end of reg_zoom.c */
