/*
************************************************************************************************************************
*                                            display format in register table
*
*                                  Copyright(C), 2006-2010, AllWinner Technology Co., Ltd.
*											       All Rights Reserved
*
* File Name   : reg_display.c
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
#ifndef  __REG_DISPLAY_C__
#define  __REG_DISPLAY_C__


#include "..\\reg_i.h"


#define REG_DISPLAY_REGNODE             "\\system\\display"
#define REG_DISPLAY_SET                 "display"
#define REG_DISPLAY_KEY_CURRENT         "current"
#define REG_DISPLAY_KEY_DEFAULT         "default"


static __lion_disp_format_e  _display_name2enm( const char *display_name );
static __s32  _display_enm2name( __lion_disp_format_e display_enm, char *display_name, __u32 size );


/*
****************************************************************************************************
*Name        : lion_reg_display_get
*Prototype   : __lion_disp_format_e lion_reg_display_get( void )
*Arguments   : void
*Return      : the current display in register table.
*Description : get the current display in register table.
*Other       :
****************************************************************************************************
*/
__lion_disp_format_e lion_reg_display_get( void )
{
    __hdle  h_node = NULL;
    char    key_value[ELIBS_KEY_VALUE_LEN+1];   
    __lion_disp_format_e  ret;
    
    /* open reg node */
	h_node = eLIBs_RegOpenNode( REG_DISPLAY_REGNODE, ELIBS_REG_READONLY );
	if( h_node == NULL )
	{
		__wrn("Warning! Reg node %s cannot be opened!\n", REG_DISPLAY_REGNODE);
	    goto error;
	}
    
    /* get key value */
    eLIBs_RegGetKeyValue( h_node, REG_DISPLAY_SET, REG_DISPLAY_KEY_CURRENT, key_value );
    
    /* convert the format of the key value */
    ret = _display_name2enm( key_value );
    
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
    return LION_DISP_UNKNOWN;
}



/*
****************************************************************************************************
*Name        : lion_reg_display_set
*Prototype   : __s32 lion_reg_display_set( __lion_disp_format_e display )
*Arguments   : display  input. display.
*Return      : EPDK_OK    succeed
*              EPDK_FAIL  fail
*Description : set the current display in register table.
*Other       :
****************************************************************************************************
*/
__s32 lion_reg_display_set( __lion_disp_format_e display )
{
    __hdle  h_node = NULL;
    char    key_value[ELIBS_KEY_VALUE_LEN+1];   
    __s32   ret;
    
    /* open reg node */
	h_node = eLIBs_RegOpenNode( REG_DISPLAY_REGNODE, ELIBS_REG_READWRITE );
	if( h_node == NULL )
	{
		__wrn("Warning! Reg node %s cannot be opened!\n", REG_DISPLAY_REGNODE);
	    goto error;
	}
    
    /* convert the format of the key value */
    ret = _display_enm2name( display, key_value, ELIBS_KEY_VALUE_LEN );
    if( ret != 0 )
    {
        goto error;
    }
    
    /* set key value */
    eLIBs_RegSetKeyValue( h_node, REG_DISPLAY_SET, REG_DISPLAY_KEY_CURRENT, key_value );
    
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
*Name        : lion_reg_display_get_default
*Prototype   : __lion_disp_format_e lion_reg_display_get_default( void )
*Arguments   : void
*Return      : the default display in register table.
*Description : get the default display in register table.
*Other       :
****************************************************************************************************
*/
__lion_disp_format_e lion_reg_display_get_default( void )
{
    __hdle  h_node = NULL;
    char    key_value[ELIBS_KEY_VALUE_LEN+1];   
    __lion_disp_format_e  ret;
    
    /* open reg node */
	h_node = eLIBs_RegOpenNode( REG_DISPLAY_REGNODE, ELIBS_REG_READONLY );
	if( h_node == NULL )
	{
		__wrn("Warning! Reg node %s cannot be opened!\n", REG_DISPLAY_REGNODE);
	    goto error;
	}
    
    /* get key value */
    eLIBs_RegGetKeyValue( h_node, REG_DISPLAY_SET, REG_DISPLAY_KEY_DEFAULT, key_value );
    
    /* convert the format of the key value */
    ret = _display_name2enm( key_value );
    
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
    return LION_DISP_UNKNOWN;
}



/*
****************************************************************************************************
*Name        : lion_reg_display_restore
*Prototype   : __s32 lion_reg_display_restore( void )
*Arguments   : void
*Return      : EPDK_OK    succeed
*              EPDK_FAIL  fail
*Description : restore the default value of display in register table.
*Other       :
****************************************************************************************************
*/
__s32 lion_reg_display_restore( void )
{
    __hdle  h_node = NULL;
    char    key_value[ELIBS_KEY_VALUE_LEN+1];   
    
    /* open reg node */
	h_node = eLIBs_RegOpenNode( REG_DISPLAY_REGNODE, ELIBS_REG_READWRITE );
	if( h_node == NULL )
	{
		__wrn("Warning! Reg node %s cannot be opened!\n", REG_DISPLAY_REGNODE);
	    goto error;
	}

    /* get default value */
    eLIBs_RegGetKeyValue( h_node, REG_DISPLAY_SET, REG_DISPLAY_KEY_DEFAULT, key_value );

    /* set current value */
    eLIBs_RegSetKeyValue( h_node, REG_DISPLAY_SET, REG_DISPLAY_KEY_CURRENT, key_value );
    
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



#define LION_DISP_NAME_TV_PAL                     "TV_PAL"                                  
#define LION_DISP_NAME_TV_NTSC                    "TV_NTSC"                                 
#define LION_DISP_NAME_TV_PAL_SVIDEO              "TV_PAL_SVIDEO"                           
#define LION_DISP_NAME_TV_NTSC_SVIDEO             "TV_NTSC_SVIDEO"                          
#define LION_DISP_NAME_TV_PAL_CVBS_SVIDEO         "TV_PAL_CVBS_SVIDEO"                      
#define LION_DISP_NAME_TV_NTSC_CVBS_SVIDEO        "TV_NTSC_CVBS_SVIDEO"                   
#define LION_DISP_NAME_TV_480I                    "TV_480I"                                 
#define LION_DISP_NAME_TV_576I                    "TV_576I"                                 
#define LION_DISP_NAME_TV_480P                    "TV_480P"                                 
#define LION_DISP_NAME_TV_576P                    "TV_576P"                                 
#define LION_DISP_NAME_TV_720P_50HZ               "TV_720P_50HZ"                            
#define LION_DISP_NAME_TV_720P_60HZ               "TV_720P_60HZ"                            
#define LION_DISP_NAME_TV_1080I_50HZ              "TV_1080I_50HZ"                           
#define LION_DISP_NAME_TV_1080I_60HZ              "TV_1080I_60HZ"                           
#define LION_DISP_NAME_TV_1080P_24HZ              "TV_1080P_24HZ"                           
#define LION_DISP_NAME_TV_1080P_50HZ              "TV_1080P_50HZ"                           
#define LION_DISP_NAME_TV_1080P_60HZ              "TV_1080P_60HZ"                           
#define LION_DISP_NAME_HDMI_480I                  "HDMI_480I"                               
#define LION_DISP_NAME_HDMI_576I                  "HDMI_576I"                               
#define LION_DISP_NAME_HDMI_480P                  "HDMI_480P"                               
#define LION_DISP_NAME_HDMI_576P                  "HDMI_576P"                               
#define LION_DISP_NAME_HDMI_720P_50HZ             "HDMI_720P_50HZ"                          
#define LION_DISP_NAME_HDMI_720P_60HZ             "HDMI_720P_60HZ"                          
#define LION_DISP_NAME_HDMI_1080I_50HZ            "HDMI_1080I_50HZ"                         
#define LION_DISP_NAME_HDMI_1080I_60HZ            "HDMI_1080I_60HZ"                         
#define LION_DISP_NAME_HDMI_1080P_24HZ            "HDMI_1080P_24HZ"                         
#define LION_DISP_NAME_HDMI_1080P_50HZ            "HDMI_1080P_50HZ"                         
#define LION_DISP_NAME_HDMI_1080P_60HZ            "HDMI_1080P_60HZ"                         
#define LION_DISP_NAME_VGA_H1680_V1050            "VGA_H1680_V1050"                         
#define LION_DISP_NAME_VGA_H1440_V900             "VGA_H1440_V900"                          
#define LION_DISP_NAME_VGA_H1360_V768             "VGA_H1360_V768"                          
#define LION_DISP_NAME_VGA_H1280_V1024            "VGA_H1280_V1024"                         
#define LION_DISP_NAME_VGA_H1024_V768             "VGA_H1024_V768"                          
#define LION_DISP_NAME_VGA_H800_V600              "VGA_H800_V600"                           
#define LION_DISP_NAME_VGA_H640_V480              "VGA_H640_V480"                           
#define LION_DISP_NAME_VGA_H1440_V900_RB          "VGA_H1440_V900_RB"                       
#define LION_DISP_NAME_VGA_H1680_V1050_RB         "VGA_H1680_V1050_RB"                      
#define LION_DISP_NAME_VGA_H1920_V1080_RB         "VGA_H1920_V1080_RB"                      
#define LION_DISP_NAME_VGA_H1920_V1080   	      "VGA_H1920_V1080"  	                    
#define LION_DISP_NAME_LCD                        "LCD"                                     
                    
                                                
typedef struct _display_map_t
{
	char                  name[ELIBS_KEY_VALUE_LEN+1];
	__lion_disp_format_e  code;
}display_map_t;


const static display_map_t display_map_tbl[ ] =
{
    { LION_DISP_NAME_TV_PAL             , LION_DISP_TV_PAL              },
    { LION_DISP_NAME_TV_NTSC            , LION_DISP_TV_NTSC             },
    { LION_DISP_NAME_TV_PAL_SVIDEO      , LION_DISP_TV_PAL_SVIDEO       },
    { LION_DISP_NAME_TV_NTSC_SVIDEO     , LION_DISP_TV_NTSC_SVIDEO      },
    { LION_DISP_NAME_TV_PAL_CVBS_SVIDEO , LION_DISP_TV_PAL_CVBS_SVIDEO  },
    { LION_DISP_NAME_TV_NTSC_CVBS_SVIDEO, LION_DISP_TV_NTSC_CVBS_SVIDEO },
    { LION_DISP_NAME_TV_480I            , LION_DISP_TV_480I             },
    { LION_DISP_NAME_TV_576I            , LION_DISP_TV_576I             },
    { LION_DISP_NAME_TV_480P            , LION_DISP_TV_480P             },
    { LION_DISP_NAME_TV_576P            , LION_DISP_TV_576P             },
    { LION_DISP_NAME_TV_720P_50HZ       , LION_DISP_TV_720P_50HZ        },
    { LION_DISP_NAME_TV_720P_60HZ       , LION_DISP_TV_720P_60HZ        },
    { LION_DISP_NAME_TV_1080I_50HZ      , LION_DISP_TV_1080I_50HZ       },
    { LION_DISP_NAME_TV_1080I_60HZ      , LION_DISP_TV_1080I_60HZ       },
    { LION_DISP_NAME_TV_1080P_24HZ      , LION_DISP_TV_1080P_24HZ       },
    { LION_DISP_NAME_TV_1080P_50HZ      , LION_DISP_TV_1080P_50HZ       },
    { LION_DISP_NAME_TV_1080P_60HZ      , LION_DISP_TV_1080P_60HZ       },
    { LION_DISP_NAME_HDMI_480I          , LION_DISP_HDMI_480I           },
    { LION_DISP_NAME_HDMI_576I          , LION_DISP_HDMI_576I           },
    { LION_DISP_NAME_HDMI_480P          , LION_DISP_HDMI_480P           },
    { LION_DISP_NAME_HDMI_576P          , LION_DISP_HDMI_576P           },
    { LION_DISP_NAME_HDMI_720P_50HZ     , LION_DISP_HDMI_720P_50HZ      },
    { LION_DISP_NAME_HDMI_720P_60HZ     , LION_DISP_HDMI_720P_60HZ      },
    { LION_DISP_NAME_HDMI_1080I_50HZ    , LION_DISP_HDMI_1080I_50HZ     },
    { LION_DISP_NAME_HDMI_1080I_60HZ    , LION_DISP_HDMI_1080I_60HZ     },
    { LION_DISP_NAME_HDMI_1080P_24HZ    , LION_DISP_HDMI_1080P_24HZ     },
    { LION_DISP_NAME_HDMI_1080P_50HZ    , LION_DISP_HDMI_1080P_50HZ     },
    { LION_DISP_NAME_HDMI_1080P_60HZ    , LION_DISP_HDMI_1080P_60HZ     },
    { LION_DISP_NAME_VGA_H1680_V1050    , LION_DISP_VGA_H1680_V1050     },
    { LION_DISP_NAME_VGA_H1440_V900     , LION_DISP_VGA_H1440_V900      },
    { LION_DISP_NAME_VGA_H1360_V768     , LION_DISP_VGA_H1360_V768      },
    { LION_DISP_NAME_VGA_H1280_V1024    , LION_DISP_VGA_H1280_V1024     },
    { LION_DISP_NAME_VGA_H1024_V768     , LION_DISP_VGA_H1024_V768      },
    { LION_DISP_NAME_VGA_H800_V600      , LION_DISP_VGA_H800_V600       },
    { LION_DISP_NAME_VGA_H640_V480      , LION_DISP_VGA_H640_V480       },
    { LION_DISP_NAME_VGA_H1440_V900_RB  , LION_DISP_VGA_H1440_V900_RB   },
    { LION_DISP_NAME_VGA_H1680_V1050_RB , LION_DISP_VGA_H1680_V1050_RB  },
    { LION_DISP_NAME_VGA_H1920_V1080_RB , LION_DISP_VGA_H1920_V1080_RB  },
    { LION_DISP_NAME_VGA_H1920_V1080    , LION_DISP_VGA_H1920_V1080     },
    { LION_DISP_NAME_LCD                , LION_DISP_LCD                 }
};

#define DISPLAY_NR           ( sizeof(display_map_tbl) / sizeof(display_map_tbl[0]) )



/*
****************************************************************************************************
*Name        : _display_name2enm
*Prototype   : __lion_disp_format_e  _display_name2enm( const char *display_name )
*Arguments   : display_name   input. the name of a display
*Return      : the enum value of the display.
*Description : get the enum value of the display through its name
*Other       :
****************************************************************************************************
*/
static __lion_disp_format_e  _display_name2enm( const char *display_name )
{
	__u32  i;

    if( display_name == NULL )
    {
        return LION_DISP_UNKNOWN;
    }
    
    /* search in table */
	for( i = 0;  i < DISPLAY_NR;  i++ )
	{
		if( eLIBs_strcmp( display_name, display_map_tbl[i].name ) == 0 )
			return display_map_tbl[i].code;
	}

	return LION_DISP_UNKNOWN;
}



/*
****************************************************************************************************
*Name        : _display_enm2name
*Prototype   : __s32  _display_enm2name( __lion_disp_format_e display_enm, 
*                                              char *display_name, __u32 size )
*Arguments   : display_enm    input. the enum value of a display
*              display_name   output. a buffert to store the name of a display
*              size           input. the buffer's size.
*Return      : ==0      succeed
*              !=0      fail
*Description : get the name of the display through its enum value
*Other       :
****************************************************************************************************
*/
static __s32  _display_enm2name( __lion_disp_format_e display_enm, char *display_name, __u32 size )
{
	__u32  i;
	
	if( display_name == NULL || size == 0 )
	{
		return -1;
    }
    
    /* search in table */
	for( i = 0;  i < DISPLAY_NR;  i++ )
	{
		if( display_enm == display_map_tbl[i].code )
		{
			eLIBs_strncpy( display_name, display_map_tbl[i].name, size );
			display_name[size-1] = '\0';
			
			return 0;
		}
	}

	return -1;
}




#endif     //  ifndef __REG_DISPLAY_C__

/* end of reg_display.c */
