/*
************************************************************************************************************************
*                                            ebooktitle color in register table
*
*                                  Copyright(C), 2006-2010, AllWinner Technology Co., Ltd.
*											       All Rights Reserved
*
* File Name   : reg_ebook_color.c
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
#ifndef  __REG_EBOOK_COLOR_C__
#define  __REG_EBOOK_COLOR_C__


#include "..\\reg_i.h"


#define REG_EBOOK_COLOR_REGNODE             "\\ebook\\color"
#define REG_EBOOK_COLOR_SET                 "color"
#define REG_EBOOK_COLOR_KEY_CURRENT         "current"
#define REG_EBOOK_COLOR_KEY_DEFAULT         "default"


static __u32  _ebook_color_name2enm( const char *ebook_color_name );
static __s32  _ebook_color_enm2name( __u32 ebook_color_enm, char *ebook_color_name, __u32 size );


/*
****************************************************************************************************
*Name        : lion_reg_ebook_color_get
*Prototype   : __u32 lion_reg_ebook_color_get( void )
*Arguments   : void
*Return      : the current ebooktitle color in register table. The color follows GUI's color format.
*Description : get the current ebooktitle color in register table.
*Other       :
****************************************************************************************************
*/
__u32 lion_reg_ebook_color_get( void )
{
    __hdle  h_node = NULL;
    char    key_value[ELIBS_KEY_VALUE_LEN+1];   
    __u32  ret;
    
    /* open reg node */
	h_node = eLIBs_RegOpenNode( REG_EBOOK_COLOR_REGNODE, ELIBS_REG_READONLY );
	if( h_node == NULL )
	{
		__wrn("Warning! Reg node %s cannot be opened!\n", REG_EBOOK_COLOR_REGNODE);
	    goto error;
	}
    
    /* get key value */
    eLIBs_RegGetKeyValue( h_node, REG_EBOOK_COLOR_SET, REG_EBOOK_COLOR_KEY_CURRENT, key_value );
    
    /* convert the format of the key value */
    ret = _ebook_color_name2enm( key_value );
    
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
    return GUI_WHITE;
}



/*
****************************************************************************************************
*Name        : lion_reg_ebook_color_set
*Prototype   : __s32 lion_reg_ebook_color_set( __u32 ebook_color )
*Arguments   : ebook_color  input. ebooktitle color. The color follows GUI's color format.
*Return      : EPDK_OK    succeed
*              EPDK_FAIL  fail
*Description : set the current ebooktitle color in register table.
*Other       :
****************************************************************************************************
*/
__s32 lion_reg_ebook_color_set( __u32 ebook_color )
{
    __hdle  h_node = NULL;
    char    key_value[ELIBS_KEY_VALUE_LEN+1];   
    __s32   ret;
    
    /* open reg node */
	h_node = eLIBs_RegOpenNode( REG_EBOOK_COLOR_REGNODE, ELIBS_REG_READWRITE );
	if( h_node == NULL )
	{
		__wrn("Warning! Reg node %s cannot be opened!\n", REG_EBOOK_COLOR_REGNODE);
	    goto error;
	}
    
    /* convert the format of the key value */
    ret = _ebook_color_enm2name( ebook_color, key_value, ELIBS_KEY_VALUE_LEN );
    if( ret != 0 )
    {
        goto error;
    }
    
    /* set key value */
    eLIBs_RegSetKeyValue( h_node, REG_EBOOK_COLOR_SET, REG_EBOOK_COLOR_KEY_CURRENT, key_value );
    
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
*Name        : lion_reg_ebook_color_get_default
*Prototype   : __u32 lion_reg_ebook_color_get_default( void )
*Arguments   : void
*Return      : the default ebooktitle color in register table. The color follows GUI's color format.
*Description : get the default ebooktitle color in register table.
*Other       :
****************************************************************************************************
*/
__u32 lion_reg_ebook_color_get_default( void )
{
    __hdle  h_node = NULL;
    char    key_value[ELIBS_KEY_VALUE_LEN+1];   
    __u32  ret;
    
    /* open reg node */
	h_node = eLIBs_RegOpenNode( REG_EBOOK_COLOR_REGNODE, ELIBS_REG_READONLY );
	if( h_node == NULL )
	{
		__wrn("Warning! Reg node %s cannot be opened!\n", REG_EBOOK_COLOR_REGNODE);
	    goto error;
	}
    
    /* get key value */
    eLIBs_RegGetKeyValue( h_node, REG_EBOOK_COLOR_SET, REG_EBOOK_COLOR_KEY_DEFAULT, key_value );
    
    /* convert the format of the key value */
    ret = _ebook_color_name2enm( key_value );
    
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
    return GUI_WHITE;
}



/*
****************************************************************************************************
*Name        : lion_reg_ebook_color_restore
*Prototype   : __s32 lion_reg_ebook_color_restore( void )
*Arguments   : void
*Return      : EPDK_OK    succeed
*              EPDK_FAIL  fail
*Description : restore the default value of ebooktitle color in register table.
*Other       :
****************************************************************************************************
*/
__s32 lion_reg_ebook_color_restore( void )
{
    __hdle  h_node = NULL;
    char    key_value[ELIBS_KEY_VALUE_LEN+1];   
    
    /* open reg node */
	h_node = eLIBs_RegOpenNode( REG_EBOOK_COLOR_REGNODE, ELIBS_REG_READWRITE );
	if( h_node == NULL )
	{
		__wrn("Warning! Reg node %s cannot be opened!\n", REG_EBOOK_COLOR_REGNODE);
	    goto error;
	}

    /* get default value */
    eLIBs_RegGetKeyValue( h_node, REG_EBOOK_COLOR_SET, REG_EBOOK_COLOR_KEY_DEFAULT, key_value );

    /* set current value */
    eLIBs_RegSetKeyValue( h_node, REG_EBOOK_COLOR_SET, REG_EBOOK_COLOR_KEY_CURRENT, key_value );
    
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



#define LION_EBOOK_COLOR_NAME_RED                   "RED"
#define LION_EBOOK_COLOR_NAME_BLUE                  "BLUE"
#define LION_EBOOK_COLOR_NAME_GREEN                 "GREEN"
#define LION_EBOOK_COLOR_NAME_WHITE                 "WHITE"
#define LION_EBOOK_COLOR_NAME_BLACK                 "BLACK"
#define LION_EBOOK_COLOR_NAME_GRAY                  "GRAY"
#define LION_EBOOK_COLOR_NAME_CYAN                  "CYAN"
#define LION_EBOOK_COLOR_NAME_MAGENTA               "MAGENTA"
#define LION_EBOOK_COLOR_NAME_YELLOW                "YELLOW"


typedef struct _ebook_color_map_t
{
	char   name[ELIBS_KEY_VALUE_LEN+1];
	__u32  code;
}ebook_color_map_t;


const static ebook_color_map_t ebook_color_map_tbl[ ] =
{
    { LION_EBOOK_COLOR_NAME_RED    , GUI_RED     },
    { LION_EBOOK_COLOR_NAME_BLUE   , GUI_BLUE    },
    { LION_EBOOK_COLOR_NAME_GREEN  , GUI_GREEN   },
    { LION_EBOOK_COLOR_NAME_WHITE  , GUI_WHITE   },
    { LION_EBOOK_COLOR_NAME_BLACK  , GUI_BLACK   },
    { LION_EBOOK_COLOR_NAME_GRAY   , GUI_GRAY    },
    { LION_EBOOK_COLOR_NAME_CYAN   , GUI_CYAN    },
    { LION_EBOOK_COLOR_NAME_MAGENTA, GUI_MAGENTA },
    { LION_EBOOK_COLOR_NAME_YELLOW , GUI_YELLOW  }
};

#define EBOOK_COLOR_NR           ( sizeof(ebook_color_map_tbl) / sizeof(ebook_color_map_tbl[0]) )



/*
****************************************************************************************************
*Name        : _ebook_color_name2enm
*Prototype   : __u32  _ebook_color_name2enm( const char *ebook_color_name )
*Arguments   : ebook_color_name   input. the name of a ebooktitle color
*Return      : the enum value of the ebooktitle color. return GUI_WHITE, if fail.
*Description : get the enum value of the ebooktitle color through its name
*Other       :
****************************************************************************************************
*/
static __u32  _ebook_color_name2enm( const char *ebook_color_name )
{
	__u32  i;

    if( ebook_color_name == NULL )
    {
        return GUI_WHITE;
    }
    
    /* search in table */
	for( i = 0;  i < EBOOK_COLOR_NR;  i++ )
	{
		if( eLIBs_strcmp( ebook_color_name, ebook_color_map_tbl[i].name ) == 0 )
			return ebook_color_map_tbl[i].code;
	}

	return GUI_WHITE;
}



/*
****************************************************************************************************
*Name        : _ebook_color_enm2name
*Prototype   : __s32  _ebook_color_enm2name( __u32 ebook_color_enm, 
*                                              char *ebook_color_name, __u32 size )
*Arguments   : ebook_color_enm    input. the enum value of a ebooktitle color
*              ebook_color_name   output. a buffert to store the name of a ebooktitle color
*              size           input. the buffer's size.
*Return      : ==0      succeed
*              !=0      fail
*Description : get the name of the ebooktitle color through its enum value
*Other       :
****************************************************************************************************
*/
static __s32  _ebook_color_enm2name( __u32 ebook_color_enm, char *ebook_color_name, __u32 size )
{
	__u32  i;
	
	if( ebook_color_name == NULL || size == 0 )
	{
		return -1;
    }
    
    /* search in table */
	for( i = 0;  i < EBOOK_COLOR_NR;  i++ )
	{
		if( ebook_color_enm == ebook_color_map_tbl[i].code )
		{
			eLIBs_strncpy( ebook_color_name, ebook_color_map_tbl[i].name, size );
			ebook_color_name[size-1] = '\0';
			
			return 0;
		}
	}

	return -1;
}




#endif     //  ifndef __REG_EBOOK_COLOR_C__

/* end of reg_ebook_color.c */
