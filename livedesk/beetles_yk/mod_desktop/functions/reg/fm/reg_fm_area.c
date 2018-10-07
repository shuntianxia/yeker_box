/*
************************************************************************************************************************
*                                            area mode in register table
*
*                                  Copyright(C), 2006-2010, AllWinner Technology Co., Ltd.
*											       All Rights Reserved
*
* File Name   : reg_fm_area.c
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
#ifndef  __REG_FM_AREA_C__
#define  __REG_FM_AREA_C__


#include "..\\reg_i.h"

#define REG_FM_AREA_REGNODE             "\\fm\\area"
#define REG_FM_AREA_SET                 "area"
#define REG_FM_AREA_KEY_CURRENT         "current"
#define REG_FM_AREA_KEY_DEFAULT         "default"


static __lion_fm_area_t  _area_name2enm( const char *area_name );
static __s32  _area_enm2name( __lion_fm_area_t area_enm, char *area_name, __u32 size );


/*
****************************************************************************************************
*Name        : lion_reg_fm_area_get
*Prototype   : __lion_fm_area_t lion_reg_fm_area_get( void )
*Arguments   : void
*Return      : the current area in register table.
*Description : get the current area in register table.
*Other       :
****************************************************************************************************
*/
__lion_fm_area_t lion_reg_fm_area_get( void )
{
    __hdle  h_node = NULL;
    char    key_value[ELIBS_KEY_VALUE_LEN+1];   
    __lion_fm_area_t  ret;
    
    /* open reg node */
	h_node = eLIBs_RegOpenNode( REG_FM_AREA_REGNODE, ELIBS_REG_READONLY );
	if( h_node == NULL )
	{
		__wrn("Warning! Reg node %s cannot be opened!\n", REG_FM_AREA_REGNODE);
	    goto error;
	}
    
    /* get key value */
    eLIBs_RegGetKeyValue( h_node, REG_FM_AREA_SET, REG_FM_AREA_KEY_CURRENT, key_value );
    
    /* convert the format of the key value */
    ret = _area_name2enm( key_value );
    
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
    return LION_FM_AREA_;
}



/*
****************************************************************************************************
*Name        : lion_reg_fm_area_set
*Prototype   : __s32 lion_reg_fm_area_set( __lion_fm_area_t area )
*Arguments   : area  input. area.
*Return      : EPDK_OK    succeed
*              EPDK_FAIL  fail
*Description : set the current area in register table.
*Other       :
****************************************************************************************************
*/
__s32 lion_reg_fm_area_set( __lion_fm_area_t area )
{
    __hdle  h_node = NULL;
    char    key_value[ELIBS_KEY_VALUE_LEN+1];   
    __s32   ret;
    
    /* open reg node */
	h_node = eLIBs_RegOpenNode( REG_FM_AREA_REGNODE, ELIBS_REG_READWRITE );
	if( h_node == NULL )
	{
		__wrn("Warning! Reg node %s cannot be opened!\n", REG_FM_AREA_REGNODE);
	    goto error;
	}
    
    /* convert the format of the key value */
    ret = _area_enm2name( area, key_value, ELIBS_KEY_VALUE_LEN );
    if( ret != 0 )
    {
        goto error;
    }
    
    /* set key value */
    eLIBs_RegSetKeyValue( h_node, REG_FM_AREA_SET, REG_FM_AREA_KEY_CURRENT, key_value );
    
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
*Name        : lion_reg_fm_area_get_default
*Prototype   : __lion_fm_area_t lion_reg_fm_area_get_default( void )
*Arguments   : void
*Return      : the default area in register table.
*Description : get the default area in register table.
*Other       :
****************************************************************************************************
*/
__lion_fm_area_t lion_reg_fm_area_get_default( void )
{
    __hdle  h_node = NULL;
    char    key_value[ELIBS_KEY_VALUE_LEN+1];   
    __lion_fm_area_t  ret;
    
    /* open reg node */
	h_node = eLIBs_RegOpenNode( REG_FM_AREA_REGNODE, ELIBS_REG_READONLY );
	if( h_node == NULL )
	{
		__wrn("Warning! Reg node %s cannot be opened!\n", REG_FM_AREA_REGNODE);
	    goto error;
	}
    
    /* get key value */
    eLIBs_RegGetKeyValue( h_node, REG_FM_AREA_SET, REG_FM_AREA_KEY_DEFAULT, key_value );
    
    /* convert the format of the key value */
    ret = _area_name2enm( key_value );
    
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
    return LION_FM_AREA_;
}



/*
****************************************************************************************************
*Name        : lion_reg_fm_area_restore
*Prototype   : __s32 lion_reg_fm_area_restore( void )
*Arguments   : void
*Return      : EPDK_OK    succeed
*              EPDK_FAIL  fail
*Description : restore the default value of area in register table.
*Other       :
****************************************************************************************************
*/
__s32 lion_reg_fm_area_restore( void )
{
    __hdle  h_node = NULL;
    char    key_value[ELIBS_KEY_VALUE_LEN+1];   
    
    /* open reg node */
	h_node = eLIBs_RegOpenNode( REG_FM_AREA_REGNODE, ELIBS_REG_READWRITE );
	if( h_node == NULL )
	{
		__wrn("Warning! Reg node %s cannot be opened!\n", REG_FM_AREA_REGNODE);
	    goto error;
	}

    /* get default value */
    eLIBs_RegGetKeyValue( h_node, REG_FM_AREA_SET, REG_FM_AREA_KEY_DEFAULT, key_value );

    /* set current value */
    eLIBs_RegSetKeyValue( h_node, REG_FM_AREA_SET, REG_FM_AREA_KEY_CURRENT, key_value );
    
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



#define LION_AREA_JAPAN                         "JAPAN"
#define LION_AREA_SCHOOL                        "SCHOOL"
#define LION_AREA_US_EUP                        "US_EUP"


typedef struct _area_map_t
{
	char             name[ELIBS_KEY_VALUE_LEN+1];
	__lion_fm_area_t  code;
}area_map_t;


const static area_map_t area_map_tbl[ ] =
{
    { LION_AREA_JAPAN , LION_FM_AREA_JAPAN  },
    { LION_AREA_SCHOOL, LION_FM_AREA_SCHOOL },
    { LION_AREA_US_EUP, LION_FM_AREA_US_EUP },
};

#define REPEAT_NR           ( sizeof(area_map_tbl) / sizeof(area_map_tbl[0]) )



/*
****************************************************************************************************
*Name        : _area_name2enm
*Prototype   : __lion_fm_area_t  _area_name2enm( const char *area_name )
*Arguments   : area_name   input. the name of a area
*Return      : the enum value of the area.
*Description : get the enum value of the area through its name
*Other       :
****************************************************************************************************
*/
static __lion_fm_area_t  _area_name2enm( const char *area_name )
{
	__u32  i;

    if( area_name == NULL )
    {
        return LION_FM_AREA_;
    }
    
    /* search in table */
	for( i = 0;  i < REPEAT_NR;  i++ )
	{
		if( eLIBs_strcmp( area_name, area_map_tbl[i].name ) == 0 )
			return area_map_tbl[i].code;
	}

	return LION_FM_AREA_;
}



/*
****************************************************************************************************
*Name        : _area_enm2name
*Prototype   : __s32  _area_enm2name( __lion_fm_area_t area_enm, 
*                                              char *area_name, __u32 size )
*Arguments   : area_enm    input. the enum value of a area
*              area_name   output. a buffert to store the name of a area
*              size           input. the buffer's size.
*Return      : ==0      succeed
*              !=0      fail
*Description : get the name of the area through its enum value
*Other       :
****************************************************************************************************
*/
static __s32  _area_enm2name( __lion_fm_area_t area_enm, char *area_name, __u32 size )
{
	__u32  i;
	
	if( area_name == NULL || size == 0 )
	{
		return -1;
    }
    
    /* search in table */
	for( i = 0;  i < REPEAT_NR;  i++ )
	{
		if( area_enm == area_map_tbl[i].code )
		{
			eLIBs_strncpy( area_name, area_map_tbl[i].name, size );
			area_name[size-1] = '\0';
			
			return 0;
		}
	}

	return -1;
}




#endif     //  ifndef __REG_FM_AREA_C__

/* end of reg_fm_area.c */
