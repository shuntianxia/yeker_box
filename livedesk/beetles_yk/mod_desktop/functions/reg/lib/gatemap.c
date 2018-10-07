/*
************************************************************************************************************************
*                                                       gate map
*
*                                  Copyright(C), 2006-2010, AllWinner Technology Co., Ltd.
*											       All Rights Reserved
*
* File Name   : gatemap.c
*
* Author      : Gary.Wang
*
* Version     : 1.0
*
* Date        :2010.10.12
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
* Gary.Wang     2010.10.12        1.0         build the file
*
************************************************************************************************************************
*/
#ifndef  __GATEMAP_C__
#define  __GATEMAP_C__


#include "reg_lib_i.h"


#define LION_GATEMAP_NAME_ON                     "ON"
#define LION_GATEMAP_NAME_OFF                    "OFF"                               


typedef struct gatemap_map_t
{
	char    name[ELIBS_KEY_VALUE_LEN+1];
	__bool  code;
}gatemap_map_t;


const static gatemap_map_t gatemap_map_tbl[ ] =
{
    { LION_GATEMAP_NAME_ON , EPDK_TRUE  },
    { LION_GATEMAP_NAME_OFF, EPDK_FALSE }
};

#define GATEMAP_NR           ( sizeof(gatemap_map_tbl) / sizeof(gatemap_map_tbl[0]) )



/*
****************************************************************************************************
*Name        : gatemap_name2enm
*Prototype   : __bool  gatemap_name2enm( const char *gatemap_name )
*Arguments   : gatemap_name   input. on or off.
*Return      : the enum value of the gate.
*Description : get the enum value of the gate through its name
*Other       :
****************************************************************************************************
*/
__bool  gatemap_name2enm( const char *gatemap_name )
{
	__u32  i;

    if( gatemap_name == NULL )
    {
        return EPDK_TRUE;
    }
    
    /* search in table */
	for( i = 0;  i < GATEMAP_NR;  i++ )
	{
		if( eLIBs_strcmp( gatemap_name, gatemap_map_tbl[i].name ) == 0 )
			return gatemap_map_tbl[i].code;
	}

	return EPDK_TRUE;
}



/*
****************************************************************************************************
*Name        : gatemap_enm2name
*Prototype   : __s32  gatemap_enm2name( __bool gatemap_enm, 
*                                              char *gatemap_name, __u32 size )
*Arguments   : gatemap_enm    input. the enum value of a gate
*              gatemap_name   output. a buffert to store the name of gate.
*              size           input. the buffer's size.
*Return      : ==0      succeed
*              !=0      fail
*Description : get the name of the gate through its enum value
*Other       :
****************************************************************************************************
*/
__s32  gatemap_enm2name( __bool gatemap_enm, char *gatemap_name, __u32 size )
{
	__u32  i;
	
	if( gatemap_name == NULL || size == 0 )
	{
		return -1;
    }
    
    /* search in table */
	for( i = 0;  i < GATEMAP_NR;  i++ )
	{
		if( gatemap_enm == gatemap_map_tbl[i].code )
		{
			eLIBs_strncpy( gatemap_name, gatemap_map_tbl[i].name, size );
			gatemap_name[size-1] = '\0';
			
			return 0;
		}
	}

	return -1;
}



#endif     //  ifndef __GATEMAP_C__

/* end of gatemap.c */
