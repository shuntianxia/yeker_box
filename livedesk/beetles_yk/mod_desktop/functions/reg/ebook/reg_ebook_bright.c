/*
************************************************************************************************************************
*                                            app restore flag in register table
*
*                                  Copyright(C), 2006-2010, AllWinner Technology Co., Ltd.
*											       All Rights Reserved
*
* File Name   : reg_ebook_bright.c
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

#ifndef  __REG_BRIGHT_C__
#define  __REG_BRIGHT_C__

#include "..\\reg_i.h"

#define REGNODE_EBOOK_BRIGHT "\\ebook\\ebook_bright"

__lion_bright_t lion_reg_ebook_bright_get(void)
{

    __hdle                  h_node;
    __lion_bright_t          ret;

    char  key_value[ ELIBS_KEY_VALUE_LEN+1];


	h_node = eLIBs_RegOpenNode(REGNODE_EBOOK_BRIGHT, ELIBS_REG_READWRITE);
	if(!h_node)
	{
		__wrn("reg node cannot be opened!\n");
		ret = LION_BRIGHT_LEVEL11;
	    goto _exit;
	}

    eLIBs_RegGetKeyValue(h_node, "ebook_bright", "current", key_value);

    if(!eLIBs_strcmp(key_value, "level1"))
    {
		ret = LION_BRIGHT_LEVEL1;
    }
	else if(!eLIBs_strcmp(key_value, "level2"))
    {
		ret = LION_BRIGHT_LEVEL2;
    }
	else if(!eLIBs_strcmp(key_value, "level3"))
    {
		ret = LION_BRIGHT_LEVEL3;
    }
	else if(!eLIBs_strcmp(key_value, "level4"))
    {
		ret = LION_BRIGHT_LEVEL4;
    }
	else if(!eLIBs_strcmp(key_value, "level5"))
    {
		ret = LION_BRIGHT_LEVEL5;
    }
    else if(!eLIBs_strcmp(key_value, "level6"))
    {
		ret = LION_BRIGHT_LEVEL6;
    }
    else if(!eLIBs_strcmp(key_value, "level7"))
    {
		ret = LION_BRIGHT_LEVEL7;
    }
    else if(!eLIBs_strcmp(key_value, "level8"))
    {
		ret = LION_BRIGHT_LEVEL8;
    }
    else if(!eLIBs_strcmp(key_value, "level9"))
    {
		ret = LION_BRIGHT_LEVEL9;
    }
    else if(!eLIBs_strcmp(key_value, "level10"))
    {
		ret = LION_BRIGHT_LEVEL10;
    }
    else if(!eLIBs_strcmp(key_value, "level11"))
    {
		ret = LION_BRIGHT_LEVEL11;
    }
    else if(!eLIBs_strcmp(key_value, "level12"))
    {
		ret = LION_BRIGHT_LEVEL12;
    }
    else if(!eLIBs_strcmp(key_value, "level13"))
    {
		ret = LION_BRIGHT_LEVEL13;
    }
    else if(!eLIBs_strcmp(key_value, "level14"))
    {
		ret = LION_BRIGHT_LEVEL14;
    }
    else if(!eLIBs_strcmp(key_value, "level15"))
    {
		ret = LION_BRIGHT_LEVEL15;
    }
    else
    {
        __wrn("bright not set!\n");
		ret = LION_BRIGHT_LEVEL11;
    }
_exit:
    eLIBs_RegCloseNode(h_node);
    return ret;
}

/*
**********************************************************************************************************************
*                                               lion_set_bright
*
* Description:
*
* Arguments  :
*
* Returns    :
*
* Notes      :
*
**********************************************************************************************************************
*/
__s32 lion_reg_ebook_bright_set(__lion_bright_t bright)
{
    __hdle  h_node;

    char  key_value[ ELIBS_KEY_VALUE_LEN+1];


	h_node = eLIBs_RegOpenNode(REGNODE_EBOOK_BRIGHT, ELIBS_REG_READWRITE);
	if( h_node == NULL )
	{
		__wrn("reg node cannot be opened!\n");
		return EPDK_FAIL;
	}

    if(bright == LION_BRIGHT_LEVEL1)
    {
        eLIBs_strcpy(key_value, "level1");
    }
    else if(bright == LION_BRIGHT_LEVEL2)
    {
        eLIBs_strcpy(key_value, "level2");
    }
	else if(bright == LION_BRIGHT_LEVEL3)
    {
        eLIBs_strcpy(key_value, "level3");
    }
	else if(bright == LION_BRIGHT_LEVEL4)
    {
        eLIBs_strcpy(key_value, "level4");
    }
	else if(bright == LION_BRIGHT_LEVEL5)
    {
        eLIBs_strcpy(key_value, "level5");
    }
    else if(bright == LION_BRIGHT_LEVEL6)
    {
        eLIBs_strcpy(key_value, "level6");
    }
    else if(bright == LION_BRIGHT_LEVEL7)
    {
        eLIBs_strcpy(key_value, "level7");
    }
	else if(bright == LION_BRIGHT_LEVEL8)
    {
        eLIBs_strcpy(key_value, "level8");
    }
	else if(bright == LION_BRIGHT_LEVEL9)
    {
        eLIBs_strcpy(key_value, "level9");
    }
	else if(bright == LION_BRIGHT_LEVEL10)
    {
        eLIBs_strcpy(key_value, "level10");
    }
    else if(bright == LION_BRIGHT_LEVEL11)
    {
        eLIBs_strcpy(key_value, "level11");
    }
    else if(bright == LION_BRIGHT_LEVEL12)
    {
        eLIBs_strcpy(key_value, "level12");
    }
	else if(bright == LION_BRIGHT_LEVEL13)
    {
        eLIBs_strcpy(key_value, "level13");
    }
	else if(bright == LION_BRIGHT_LEVEL14)
    {
        eLIBs_strcpy(key_value, "level14");
    }
	else if(bright == LION_BRIGHT_LEVEL15)
    {
        eLIBs_strcpy(key_value, "level15");
    }
    else
    {
        eLIBs_strcpy(key_value, "level11");
    }

    eLIBs_RegSetKeyValue(h_node, "ebook_bright", "current", key_value);

    eLIBs_RegCloseNode(h_node);
    return EPDK_OK;
}

/*
**********************************************************************************************************************
*                                               lion_get_defaultbright
*
* Description:
*
* Arguments  :
*
* Returns    :
*
* Notes      :
*
**********************************************************************************************************************
*/
__lion_bright_t lion_reg_ebook_bright_get_default(void)
{

    __hdle                  h_node;
    __lion_bright_t          ret;

    char  key_value[ ELIBS_KEY_VALUE_LEN+1];


	h_node = eLIBs_RegOpenNode(REGNODE_EBOOK_BRIGHT, ELIBS_REG_READWRITE);
	if(!h_node)
	{
		__wrn("reg node cannot be opened!\n");
		ret = LION_BRIGHT_LEVEL11;
	    goto _exit;
	}

    eLIBs_RegGetKeyValue(h_node, "ebook_bright", "default", key_value);

    if(!eLIBs_strcmp(key_value, "level1"))
    {
		ret = LION_BRIGHT_LEVEL1;
    }
	else if(!eLIBs_strcmp(key_value, "level2"))
    {
		ret = LION_BRIGHT_LEVEL2;
    }
	else if(!eLIBs_strcmp(key_value, "level3"))
    {
		ret = LION_BRIGHT_LEVEL3;
    }
	else if(!eLIBs_strcmp(key_value, "level4"))
    {
		ret = LION_BRIGHT_LEVEL4;
    }
	else if(!eLIBs_strcmp(key_value, "level5"))
    {
		ret = LION_BRIGHT_LEVEL5;
    }
    else if(!eLIBs_strcmp(key_value, "level6"))
    {
		ret = LION_BRIGHT_LEVEL6;
    }
	else if(!eLIBs_strcmp(key_value, "level7"))
    {
		ret = LION_BRIGHT_LEVEL7;
    }
	else if(!eLIBs_strcmp(key_value, "level8"))
    {
		ret = LION_BRIGHT_LEVEL8;
    }
	else if(!eLIBs_strcmp(key_value, "level9"))
    {
		ret = LION_BRIGHT_LEVEL9;
    }
	else if(!eLIBs_strcmp(key_value, "level10"))
    {
		ret = LION_BRIGHT_LEVEL10;
    }
    else if(!eLIBs_strcmp(key_value, "level11"))
    {
		ret = LION_BRIGHT_LEVEL11;
    }
	else if(!eLIBs_strcmp(key_value, "level12"))
    {
		ret = LION_BRIGHT_LEVEL12;
    }
	else if(!eLIBs_strcmp(key_value, "level13"))
    {
		ret = LION_BRIGHT_LEVEL13;
    }
	else if(!eLIBs_strcmp(key_value, "level14"))
    {
		ret = LION_BRIGHT_LEVEL14;
    }
	else if(!eLIBs_strcmp(key_value, "level15"))
    {
		ret = LION_BRIGHT_LEVEL15;
    }
    else
    {
        __wrn("bright not set!\n");
		ret = LION_BRIGHT_LEVEL11;
    }
_exit:
    eLIBs_RegCloseNode(h_node);
    return ret;
}

/*
**********************************************************************************************************************
*                                               lion_get_defaultbright
*
* Description:
*
* Arguments  :
*
* Returns    :
*
* Notes      :
*
**********************************************************************************************************************
*/
__s32 lion_reg_ebook_bright_restore( void )
{
    __hdle  h_node = NULL;
    char    key_value[ELIBS_KEY_VALUE_LEN+1];   
    
    /* open reg node */
	h_node = eLIBs_RegOpenNode( REGNODE_EBOOK_BRIGHT, ELIBS_REG_READWRITE );
	if( h_node == NULL )
	{
		__wrn("Warning! Reg node %s cannot be opened!\n", REGNODE_EBOOK_BRIGHT);
	    goto error;
	}

    /* get default value */
    eLIBs_RegGetKeyValue( h_node, "ebook_bright", "default", key_value );

    /* set current value */
    eLIBs_RegSetKeyValue( h_node, "ebook_bright", "current", key_value );
    
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

#endif

