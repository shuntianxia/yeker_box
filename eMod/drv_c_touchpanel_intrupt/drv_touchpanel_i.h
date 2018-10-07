/*
*********************************************************************************************************
*											        ePDK
*						            the Easy Portable/Player Develop Kits
*									          ir keyboard driver
*
*						        (c) Copyright 2006-2007, Steven.ZGJ China
*											All	Rights Reserved
*
* File    : drv_touchpanel_i.h
* By      : jerry
* Version : V1.00
*********************************************************************************************************
*/
#ifndef	_DRV_TOUCHPANEL_I_H_
#define	_DRV_TOUCHPANEL_I_H_

#include "epdk.h"

#define INPUT_TP_DEV_NAME  "CTP"

#define INPUT_CHIP_NAME   "ft52XX"
#define INPUT_CHIP_DEV_ID  0x48	

#define    TP_ABS(x)            (((x)>0)?(x):(-(x)))
#define    TP_MAX(a,b)          (((a) > (b)) ? (a) : (b))
#define    TP_MIN(a,b)          (((a) < (b)) ? (a) : (b))

#define BITS_PER_BYTE       8
#define DIV_ROUND_UP(n,d)   (((n) + (d) - 1) / (d))
#define BITS_PER_LONG       (sizeof(long) * BITS_PER_BYTE)
#define BIT(nr)             (1UL << (nr))
#define BIT_MASK(nr)        (1UL << ((nr) % BITS_PER_LONG))
#define BIT_WORD(nr)        ((nr) / BITS_PER_LONG)
#define BITS_TO_LONGS(nr)   DIV_ROUND_UP(nr, BITS_PER_LONG)

#define    TP_DATA_BUFFER_MASK     (0x3f) /* ÑÚÂë */
#define    TP_DATA_BUFFER_SIZE     (0x40) 

/////////´¥ÃþÆÁ¶¯×÷////////
#define TCHSCR_ACTION_NULL              0       //¿Õ°×¶¯×÷»òÎÞ·¨Ê¶±ðµÄ¶¯×÷
#define TCHSCR_ACTION_CLICK             1       //´¥ÃþÆÁµ¥»÷
#define TCHSCR_ACTION_DOWN              (0x02)  //´¥ÃþÆÁÏÂ»¬
#define TCHSCR_ACTION_UP                (0x12)  //´¥ÃþÆÁÉÏ»¬
#define TCHSCR_ACTION_LEFT              (0x03)  //´¥ÃþÆÁ×ó»¬
#define TCHSCR_ACTION_RIGHT             (0x13)  //´¥ÃþÆÁÓÒ»¬
#define TCHSCR_ACTION_ANTICLOCKWISE     (0x04)  //´¥ÃþÆÁÄæÊ±Õë»¬¶¯
#define TCHSCR_ACTION_CLOCKWISE         (0x14)  //´¥ÃþÆÁË³Ê±Õë»¬¶¯
#define TCHSCR_ACTION_LD2RU             (0x05)  //´¥ÃþÆÁÐ±Ïß»¬¶¯,×óÏÂµ½ÓÒÉÏ
#define TCHSCR_ACTION_RU2LD             (0x15)  //´¥ÃþÆÁÐ±Ïß»¬¶¯,ÓÒÉÏµ½×óÏÂ
#define TCHSCR_ACTION_LU2RD             (0x06)  //´¥ÃþÆÁ·´Ð±Ïß»¬¶¯,×óÉÏµ½ÓÒÏÂ
#define TCHSCR_ACTION_RD2LU             (0x16)  //´¥ÃþÆÁ·´Ð±Ïß»¬¶¯,ÓÒÏÂµ½×óÉÏ

#define RIGHTWARD                       (TCHSCR_ACTION_RIGHT)
#define LEFTWARD                        (TCHSCR_ACTION_LEFT)
#define UPWARD                          (TCHSCR_ACTION_UP)
#define DOWNWARD                        (TCHSCR_ACTION_DOWN)

typedef struct tag_tp_point
{
    __s32 x[TP_DATA_BUFFER_SIZE];
    __s32 y[TP_DATA_BUFFER_SIZE];
}
__tp_point_t;

typedef struct STRUCT_TOUCHPANEL_DEV
{
    __u32 used;

}__touchpanel_dev_t;

typedef struct __TP_INFO_SET
{
    __u32   pertime;            //ÃèÊö¼ä¸ô¶àÉÙ´Î·¢ËÍÒ»´ÎÏûÏ¢
    __u32   offset;             //ÃèÊöx»òÕßy·½ÏòÆ«²î¶àÉÙÏñËØÈÏÎªÊÇÒ»´Îmove¶¯×÷
}__tp_info_set_t;


typedef struct STRUCT_TOUCHPANEL_DRV
{
    __u32               mid;
    __u8                used;
    __u8                ndev;    
    __touchpanel_dev_t  tp_dev;    
    __input_dev_t       *tp_input_dev;   
     
}__touchpanel_drv_t;


typedef struct TP_ADJUST_PT_SET
{
    __u16   disp_x;               //ÆÁÄ»ÉÏµÄx×ø±ê
    __u16   disp_y;               //ÆÁÄ»ÉÏµÄy×ø±ê
    __u16   tp_x;                 //TP×ª»»µÄx×ø±ê
    __u16   tp_y;                 //TP×ª»»µÄy×ø±ê
}_tp_adjust_pt_set_t;

//´ËÊý¾Ý½á¹¹±£Áô
typedef struct TP_PRIVATE_DATA_SET
{
	
    __u8              main_tid;           //Ö÷Ïß³ÌµÄid
    __u8              data_tid;
    __krnl_event_t   *tp_sem_data;        //ÓÃÓÚÍ¨ÖªÊý¾Ý²É¼¯ÒÑ¾­ok
    __krnl_event_t   *tp_sem_main;        //ÓÃÓÚÍ¨Öª²É¼¯Êý¾ÝÏß³Ì
	
	ES_FILE 	   *iic_file;

    __hdle h_gpio_reset;
    __hdle h_gpio_irq;

    __s16             xpre;               //Ç°Ò»´ÎµãµÄx×ø±ê
    __s16             ypre;               //Ç°Ò»´ÎµãµÄy×ø±ê
    __s16             x_old;
    __s16             y_old;


	__u8             x_y_change;
	__u8			 x_pol;
	__u8			 y_pol;
}_tp_private_data_set_t;


extern  __dev_devop_t  touchpanel_dev_ops;
extern __tp_info_set_t tp_info ;

__s32 DRV_TP_MInit(void);
__s32 DRV_TP_MExit(void);
__mp* DRV_TP_MOpen(__u32 mid, __u32 mode);
__s32 DRV_TP_MClose(__mp *mp);
__u32 DRV_TP_MRead(void *pdata, __u32 size, __u32 n, __mp *mp);
__u32 DRV_TP_MWrite(const void *pdata, __u32 size, __u32 n, __mp *mp);
__s32 DRV_TP_MIoctrl(__mp *mp, __u32 cmd, __s32 aux, void *pbuffer);



#endif	/* _DRV_TOUCHPANEL_I_H_ */
