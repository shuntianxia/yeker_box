/*
**************************************************************************************************************
*											         ePDK
*						            the Easy Portable/Player Develop Kits
*									           desktop system 
*
* File    	: 	music_player.h
*			
* By      	: 	Libaiao
* Func	: 
* Version	: 	v1.0
* ============================================================================================================
* 2010-5-25 15:24:52  Libaiao  create this file, implements the fundemental interface;
**************************************************************************************************************
*/

#ifndef __VOLUMEBAR_H__
#define __VOLUMEBAR_H__

#include "apps.h"
#define MUSIC_VOLBAR_WIN "volume bar win"

//VolumeBar UI ²ÎÊı
typedef struct VolumeBarUI_s
{
	RECT Background;		//±³¾°ÇøÓò´óĞ¡.(Õû¸öVolumeBarµÄÇøÓò´óĞ¡)	
							//VolumeBar»á¸ù¾İ¸Ã²ÎÊı´´½¨Í¼²ã
	__u32 BG_bmp_id;		//±³¾°Í¼Æ¬id
							//ÏÂÃæ×ø±ê²ÎÊı¶¼ÊÇÏà¶ÔÓÚBackground.(¼´´Ó0,0¿ªÊ¼)
	RECT LeftSpeaker;		//×ó±ßIcon×ø±ê,	
	__u32 LS_bmp_id;		//×ó±ßiconÍ¼Æ¬ID
	
	RECT BarBody;			//ÒôÁ¿Ìõbody×ø±êĞÅÏ¢	
	__u32 BB_bmp_id;		//ÒôÁ¿ÌõbodyÍ¼Æ¬id,, ×¢Òâbody¿í¶È»ò¸ß¶ÈÓ¦ÊÇpointµÄÕûÊı±¶.
	
	RECT Point;				//ÒôÁ¿Ìõ½ø¶Èµã×ø±êĞÅÏ¢
	__u32 PO_bmp_id;		//½ø¶ÈµãÍ¼Æ¬ID
	
	RECT RightSpeaker;		//ÓÒ±ßÀ®°È×ø±êĞÅÏ¢
	__u32 RS_bmp_id;		//ÓÒ±ßÀ®°ÈÍ¼Æ¬id
	
	RECT MuteSpeaker;		//¾²ÒôÍ¼±ê
	__u32 MS_bmp_id;		//¾²ÒôÍ¼±êidºÅÂë
	
	RECT VolNumber;			//ÒôÁ¿ÊıÖµ					
																//for volume bar private, user doesn't have to initial.				
}VolumeBarUI_t;

//VolumeBar¿ØÖÆ±äÁ¿²ÎÊı
typedef struct VolumeBarCtrlVar_s
{	
	__u32 ViewAlign;					//GUI_TA_HORIZONTAL = Ë®Æ½ÏÔÊ¾ÒôÁ¿Ìõ
										//GUI_TA_VERTICAL   = ´¹Ö±ÏÔÊ¾ÒôÁ¿Ìõ																																
	__u32 vol_max;						//×î´óÒôÁ¿Öµ.(ÈôĞ¡ÓÚµ±Ç°ÏµÍ³ÉèÖÃÒôÁ¿£¬ÔòÒÔ¸ÃÒôÁ¿Îª×¼).	
	__u32 AutoExitTime;					//×Ô¶¯ÍË³öÊ±¼ä£¬ÒÔSecondÎªµ¥Î»		
	
	H_WIN hParent;						//¸¸´°¿Ú¾ä±ú, should be manager win.
	H_LYR UserLayer;					//ÓÃ»§Í¼²ã,µ±ÏµÍ³ÒÑ¾­Ã»ÓĞ¿ÕÏĞÍ¼²ãÍ¨µÀÊ±£¬ĞèÒª´«µİ¸Ã²ÎÊı¸øVolumeBar(ÎªÁËÍ¸Ã÷ÏÔÊ¾)
	__s32 Pipe;							//ÓÃ»§¿ÉÒÔÖ¸¶¨VolumeBarÍ¼²ãµÄÍ¨µÀ.
	void *attr;							//ÓÃ»§²ÎÊıÖ¸Õë	
}VolumeBarCtrlVar_t;

//ÊÂ¼ş¿ØÖÆ²ÎÊı
typedef struct VolumeBarEvent_s
{
	__u32 VolAddKey;					//ÓÃ»§Ö¸¶¨ÓÃÄÇ¸ö¼üÏìÓ¦ÒôÁ¿¼Ó¶¯×÷. ±ÈÈç = GUI_MSG_KEY_RIGHT
	__u32 VolAddRepeatKey;				//ÏìÓ¦ÖØ¸´¼ü
	__u32 VolDecKey;					//ÓÃ»§Ö¸¶¨ÓÃÄÇ¸ö¼üÏìÓ¦ÒôÁ¿¼õ¶¯×÷. ±ÈÈç = GUI_MSG_KEY_LEFT 
	__u32 VolDecRepeatKey;				//ÏìÓ¦ÖØ¸´¼ü
	__u32 VolExitKey;					//ÍË³öÒôÁ¿¿ØÖÆ¼ü
	
	__s32 (*OnVolAdd)(void *Handle); 	//ÒôÁ¿¼ÓÊ±,ÓÃ»§µÄÊÂ¼ş´¦Àíº¯Êı
	__s32 (*OnVolDec)(void *Handle);	//ÒôÁ¿¼õÊ±,ÓÃ»§µÄÊÂ¼ş´¦Àíº¯Ê
	__s32 (*OnExit)(void *Handle);		//
	
}VolumeBarEvent_t;

typedef struct  VolumeBarPara_s
{		
	VolumeBarUI_t  			UI;				//ÒôÁ¿¿ØÖÆÌõ  UI²ÎÊı
	VolumeBarCtrlVar_t		CtrlVar;		//ÒôÁ¿¿ØÖÆÌõ  ±äÁ¿¿ØÖÆ²ÎÊı
	VolumeBarEvent_t        Event;			//ÒôÁ¿¿ØÖÆÌõ  ÊÂ¼ş´¦Àí²ÎÊı
}VolumeBarPara_t;


typedef struct VolumeBar_s
{
	VolumeBarPara_t  Para;				//User´«µİ½øÀ´µÄ²ÎÊı

	
										//ÒÔÏÂÎªVoluemBarµÄË½ÓĞ²ÎÊı

	H_WIN   H_Framewin;					//frame window
	H_LYR 	VB_Layer;					//VolumeBarÍ¼²ã£¬ÓÃ»§ÎŞĞè³õÊ¼»¯ 	

	__u32   TimerID;
	HTHEME 	BG_Handle;					//±³¾°Í¼Æ¬¾ä±ú
	void*   BG_bmp;					//±³¾°Í¼Æ¬µØÖ·

	HTHEME 	LS_Handle;					//LeftSpeakerÍ¼Æ¬¾ä±ú
	void*   LS_bmp;						//×ó±ßÀ®°ÈiconÍ¼Æ¬µØÖ·
	
	HTHEME 	BB_Handle;					//ÒôÁ¿ÌõbodyÍ¼Æ¬¾ä±ú
	void*   BB_bmp;						//ÒôÁ¿ÌõbodyÍ¼Æ¬bufferµØÖ·

	HTHEME 	PO_Handle;					//½ø¶Èµã¾ä±ú
	void*   PO_bmp; 					//½ø¶ÈµãÍ¼Æ¬ĞÅÏ¢
	
	HTHEME 	RS_Handle;					//RightSpeaker icon ¾ä±ú
	void*   RS_bmp;						//ÓÒ±ßÀ®°ÈÍ¼Æ¬µØÖ·
	
	HTHEME 	MS_Handle;					//mute speaker icon ¾ä±ú
	void*   MS_bmp;						//¾²ÒôÍ¼±êµØÖ·					
}VolumeBar_t;

#define C_VOLUMEBAR_FRAMEWIN_ID			(APP_MUSIC_ID + 20)

#define	C_VB_AUTO_EXIT_TIME				100

extern VolumeBar_t* VolumeBarCreate(VolumeBarPara_t *Para);
extern __s32 VolumeBarDelete(VolumeBar_t *This);
extern __s32 VolumeBarSetFocus(VolumeBar_t *This);
extern __s32 VolumeBarShow(VolumeBar_t *This);

#endif
