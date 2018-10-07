/*
************************************************************************************************************************
*                                            eq in register table
*
*                                  Copyright(C), 2006-2010, AllWinner Technology Co., Ltd.
*											       All Rights Reserved
*
* File Name   : reg_eq.c
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
#ifndef  __REG_EQ_C__
#define  __REG_EQ_C__


#include "..\\reg_i.h"


#define REG_EQ_REGNODE             "\\music\\eq"
#define REG_EQ_SET                 "eq"
#define REG_EQ_KEY_CURRENT         "current"
#define REG_EQ_KEY_DEFAULT         "default"


static __cedar_audio_eq_t  _eq_name2enm( const char *eq_name );
static __s32  _eq_enm2name( __cedar_audio_eq_t eq_enm, char *eq_name, __u32 size );


/*
****************************************************************************************************
*Name        : lion_reg_eq_get
*Prototype   : __cedar_audio_eq_t lion_reg_eq_get( void )
*Arguments   : void
*Return      : the current eq in register table.
*Description : get the current eq in register table.
*Other       :
****************************************************************************************************
*/
__cedar_audio_eq_t lion_reg_eq_get( void )
{
    __hdle  h_node = NULL;
    char    key_value[ELIBS_KEY_VALUE_LEN+1];   
    __cedar_audio_eq_t  ret;
    
    /* open reg node */
	h_node = eLIBs_RegOpenNode( REG_EQ_REGNODE, ELIBS_REG_READONLY );
	if( h_node == NULL )
	{
		__wrn("Warning! Reg node %s cannot be opened!\n", REG_EQ_REGNODE);
	    goto error;
	}
    
    /* get key value */
    eLIBs_RegGetKeyValue( h_node, REG_EQ_SET, REG_EQ_KEY_CURRENT, key_value );
    
    /* convert the format of the key value */
    ret = _eq_name2enm( key_value );
    
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
    return CEDAR_AUD_EQ_TYPE_;
}



/*
****************************************************************************************************
*Name        : lion_reg_eq_set
*Prototype   : __s32 lion_reg_eq_set( __cedar_audio_eq_t eq )
*Arguments   : eq  input. eq.
*Return      : EPDK_OK    succeed
*              EPDK_FAIL  fail
*Description : set the current eq in register table.
*Other       :
****************************************************************************************************
*/
__s32 lion_reg_eq_set( __cedar_audio_eq_t eq )
{
    __hdle  h_node = NULL;
    char    key_value[ELIBS_KEY_VALUE_LEN+1];   
    __s32   ret;
    
    /* open reg node */
	h_node = eLIBs_RegOpenNode( REG_EQ_REGNODE, ELIBS_REG_READWRITE );
	if( h_node == NULL )
	{
		__wrn("Warning! Reg node %s cannot be opened!\n", REG_EQ_REGNODE);
	    goto error;
	}
    
    /* convert the format of the key value */
    ret = _eq_enm2name( eq, key_value, ELIBS_KEY_VALUE_LEN );
    if( ret != 0 )
    {
        goto error;
    }
    
    /* set key value */
    eLIBs_RegSetKeyValue( h_node, REG_EQ_SET, REG_EQ_KEY_CURRENT, key_value );
    
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
*Name        : lion_reg_eq_get_default
*Prototype   : __cedar_audio_eq_t lion_reg_eq_get_default( void )
*Arguments   : void
*Return      : the default eq in register table.
*Description : get the default eq in register table.
*Other       :
****************************************************************************************************
*/
__cedar_audio_eq_t lion_reg_eq_get_default( void )
{
    __hdle  h_node = NULL;
    char    key_value[ELIBS_KEY_VALUE_LEN+1];   
    __cedar_audio_eq_t  ret;
    
    /* open reg node */
	h_node = eLIBs_RegOpenNode( REG_EQ_REGNODE, ELIBS_REG_READONLY );
	if( h_node == NULL )
	{
		__wrn("Warning! Reg node %s cannot be opened!\n", REG_EQ_REGNODE);
	    goto error;
	}
    
    /* get key value */
    eLIBs_RegGetKeyValue( h_node, REG_EQ_SET, REG_EQ_KEY_DEFAULT, key_value );
    
    /* convert the format of the key value */
    ret = _eq_name2enm( key_value );
    
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
    return CEDAR_AUD_EQ_TYPE_;
}



/*
****************************************************************************************************
*Name        : lion_reg_eq_restore
*Prototype   : __s32 lion_reg_eq_restore( void )
*Arguments   : void
*Return      : EPDK_OK    succeed
*              EPDK_FAIL  fail
*Description : restore the default value of eq in register table.
*Other       :
****************************************************************************************************
*/
__s32 lion_reg_eq_restore( void )
{
    __hdle  h_node = NULL;
    char    key_value[ELIBS_KEY_VALUE_LEN+1];   
    
    /* open reg node */
	h_node = eLIBs_RegOpenNode( REG_EQ_REGNODE, ELIBS_REG_READWRITE );
	if( h_node == NULL )
	{
		__wrn("Warning! Reg node %s cannot be opened!\n", REG_EQ_REGNODE);
	    goto error;
	}

    /* get default value */
    eLIBs_RegGetKeyValue( h_node, REG_EQ_SET, REG_EQ_KEY_DEFAULT, key_value );

    /* set current value */
    eLIBs_RegSetKeyValue( h_node, REG_EQ_SET, REG_EQ_KEY_CURRENT, key_value );
    
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



#define LION_EQ_NAME_NORMAL             "NORMAL"
#define LION_EQ_NAME_DBB                "DBB"
#define LION_EQ_NAME_POP                "POP"
#define LION_EQ_NAME_ROCK               "ROCK"
#define LION_EQ_NAME_CLASSIC            "CLASSIC"
#define LION_EQ_NAME_JAZZ               "JAZZ"
#define LION_EQ_NAME_VOCAL              "VOCAL"
#define LION_EQ_NAME_DANCE              "DANCE"
#define LION_EQ_NAME_SOFT               "SOFT"



typedef struct _eq_map_t
{
	char                name[ELIBS_KEY_VALUE_LEN+1];
	__cedar_audio_eq_t  code;
}eq_map_t;


const static eq_map_t eq_map_tbl[ ] =
{
    { LION_EQ_NAME_NORMAL , CEDAR_AUD_EQ_TYPE_NORMAL  },
    { LION_EQ_NAME_DBB    , CEDAR_AUD_EQ_TYPE_DBB     },
    { LION_EQ_NAME_POP    , CEDAR_AUD_EQ_TYPE_POP     },
    { LION_EQ_NAME_ROCK   , CEDAR_AUD_EQ_TYPE_ROCK    },
    { LION_EQ_NAME_CLASSIC, CEDAR_AUD_EQ_TYPE_CLASSIC },
    { LION_EQ_NAME_JAZZ   , CEDAR_AUD_EQ_TYPE_JAZZ    },
    { LION_EQ_NAME_VOCAL  , CEDAR_AUD_EQ_TYPE_VOCAL   },
    { LION_EQ_NAME_DANCE  , CEDAR_AUD_EQ_TYPE_DANCE   },
    { LION_EQ_NAME_SOFT   , CEDAR_AUD_EQ_TYPE_SOFT    }
};

#define EQ_NR           ( sizeof(eq_map_tbl) / sizeof(eq_map_tbl[0]) )



/*
****************************************************************************************************
*Name        : _eq_name2enm
*Prototype   : __cedar_audio_eq_t  _eq_name2enm( const char *eq_name )
*Arguments   : eq_name   input. the name of a eq
*Return      : the enum value of the eq.
*Description : get the enum value of the eq through its name
*Other       :
****************************************************************************************************
*/
static __cedar_audio_eq_t  _eq_name2enm( const char *eq_name )
{
	__u32  i;

    if( eq_name == NULL )
    {
        return CEDAR_AUD_EQ_TYPE_;
    }
    
    /* search in table */
	for( i = 0;  i < EQ_NR;  i++ )
	{
		if( eLIBs_strcmp( eq_name, eq_map_tbl[i].name ) == 0 )
			return eq_map_tbl[i].code;
	}

	return CEDAR_AUD_EQ_TYPE_;
}



/*
****************************************************************************************************
*Name        : _eq_enm2name
*Prototype   : __s32  _eq_enm2name( __cedar_audio_eq_t eq_enm, 
*                                              char *eq_name, __u32 size )
*Arguments   : eq_enm    input. the enum value of a eq
*              eq_name   output. a buffert to store the name of a eq
*              size        input. the buffer's size.
*Return      : ==0      succeed
*              !=0      fail
*Description : get the name of the eq through its enum value
*Other       :
****************************************************************************************************
*/
static __s32  _eq_enm2name( __cedar_audio_eq_t eq_enm, char *eq_name, __u32 size )
{
	__u32  i;
	
	if( eq_name == NULL || size == 0 )
	{
		return -1;
    }
    
    /* search in table */
	for( i = 0;  i < EQ_NR;  i++ )
	{
		if( eq_enm == eq_map_tbl[i].code )
		{
			eLIBs_strncpy( eq_name, eq_map_tbl[i].name, size );
			eq_name[size-1] = '\0';
			
			return 0;
		}
	}

	return -1;
}




#endif     //  ifndef __REG_EQ_C__

/* end of reg_eq.c */
