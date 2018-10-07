#ifndef __GUI_BUTTON_EX_H__
#define __GUI_BUTTON_EX_H__

#include "beetles_app.h"

#define BS_PUSHBUTTON       0x00000000L
#define BS_USERBUTTON       0x00000008L

#define BS_TYPEMASK         0x0000000FL
#define BST_PUSHED          (1<<1)
#define BST_FOCUS           (1<<2)
#define BST_DISABLED		(1<<3)

#define BN_CLICKED          0
#define BN_PUSHED           1
#define BN_UNPUSHED         2
#define BN_DISABLE          4
#define BN_DOUBLECLICKED    5

#define BN_DBLCLK           BN_DOUBLECLICKED
#define BN_SETFOCUS         6
#define BN_KILLFOCUS        7

typedef enum button_ex_type {
	BUTTON_TYPE_PRESS,
	BUTTON_TYPE_ONOFF,
}button_ex_type_t;

typedef struct tagbtnexpara
{
	//H_WIN						win;
	RECT						rect;
	void        				*focus_bmp;
	void        				*unfocus_bmp;
	void						*disable_bmp;
	__pos_t     				bmp_pos;
	char        				*text;
	__pos_t     				text_pos;
	__u32			   			ftxt_color;
	__u32		   				uftxt_color;
	__u32						distxt_color;
	GUI_FONT                   *draw_font;
	__epdk_charset_enm_e        draw_code;   /*×Ö·û±àÂë*/
	__u32                       bk_color;
	__u32                       txt_align;
	__bool                      alpha_en;
	__s32						type;
	__s32						id;
} button_ex_para_t;

typedef struct {
	__s32 id;
	button_ex_type_t type;
	RECT rect;
	__s32 bmp_id[3];
	load_bmp_res_t bmp_res[3];
}button_ex_ui_t;

typedef struct tagBUTTONEXDATA
{
	GUI_MEMDEV_Handle   hmem;
	__u32               draw_sts;
	__u32               status;           /* button flags */
	__u32               data;             /* bitmap or icon of butotn. */
} button_ex_data_t;

typedef button_ex_data_t *pbuttonexdata;


#define BUTTON_DRAWSTATUS(pCtrl) (((pbuttonexdata)GUI_WinGetAddData(pCtrl))->draw_sts)
#define BUTTON_STATUS(pCtrl)   (((pbuttonexdata)GUI_WinGetAddData(pCtrl))->status)
//#define BUTTON_STSTRANS(pCtrl)  (BUTTON_OLDSTATUS(pCtrl) != BUTTON_STATUS(pCtrl))
#define BUTTON_DATA(pCtrl)     (((pbuttonexdata)GUI_WinGetAddData(pCtrl))->data)
__s32  ButtonExSetFocusBmp(H_WIN hbutton, void *bmp);
__s32  ButtonExSetUnFocusBmp(H_WIN hbutton, void *bmp);
__s32  ButtonExSetText(H_WIN hbutton, char *text);
__s32  ButtonExSetUnFcsTxtColor(H_WIN hbutton, __u32 color);
__s32  ButtonExSetFcsTxtColor(H_WIN hbutton, __u32  color);
__s32  ButtonExSetBkColor(H_WIN hbutton, __u32  color);
__s32  ButtonExSetBmpPos(H_WIN hbutton, __pos_t *pos);
__s32  ButtonExSetTxtPos(H_WIN hbutton, __pos_t *pos);
#endif /* #ifndef __GUI_BUTTON_H__ */
