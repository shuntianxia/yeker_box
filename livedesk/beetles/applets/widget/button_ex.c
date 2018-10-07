#include "button_ex.h"

static __u32  last_key = 0xffffffff;

static __bool pt_in_rect(const RECT *prc, __s32 x, __s32 y)
{
	if(x >= 0 && x < prc->width && y >=0 && y < prc->height)
	{
		return EPDK_TRUE;
	}

	return EPDK_FALSE;
}

static int ButtonExProc(__gui_msg_t *msg)
{
	__u32	            	dwStyle;
	button_ex_data_t			*pData;
	dwStyle = GUI_WinGetStyle(msg->h_deswin);

	switch(msg->id)
	{
		case GUI_MSG_CREATE:
			pData = (button_ex_data_t *) esMEMS_Balloc(sizeof(button_ex_data_t));

			if(pData == NULL)
			{
				return EPDK_FAIL;
			}

			pData->draw_sts     = 0;
			pData->status       = 0;
			pData->hmem         = 0;
			pData->data         = (__u32)GUI_WinGetAttr(msg->h_deswin);
			GUI_WinSetAddData(msg->h_deswin, (__u32)pData);
			return 0;

		case GUI_MSG_DESTROY:
			pData = (button_ex_data_t *)GUI_WinGetAddData(msg->h_deswin);
			__msg("ButtonExProc GUI_MSG_DESTROY\n");
			esMEMS_Bfree((void *)GUI_WinGetAddData(msg->h_deswin), sizeof(button_ex_data_t));
			return 0;

		case GUI_MSG_ENABLE:
			if(msg->dwAddData1)
			{
				dwStyle &= ~WS_DISABLED;
				GUI_WinSetStyle(msg->h_deswin, dwStyle);
				if(BUTTON_DRAWSTATUS(msg->h_deswin) & BST_DISABLED) {
					BUTTON_DRAWSTATUS(msg->h_deswin) &= (~BST_DISABLED);
					GUI_InvalidateRect(msg->h_deswin, NULL, ORANGE_TRUE);
				}
			}
			else if(!msg->dwAddData1)
			{
				dwStyle |= WS_DISABLED;
				GUI_WinSetStyle(msg->h_deswin, dwStyle);
				if(!(BUTTON_DRAWSTATUS(msg->h_deswin) & BST_DISABLED)) {
					BUTTON_DRAWSTATUS(msg->h_deswin) |= BST_DISABLED;
					GUI_InvalidateRect(msg->h_deswin, NULL, ORANGE_TRUE);
				}
			}
			else
			{
				return 0;
			}

			return 0;

		case GUI_MSG_TOUCHINACTIVE:
		{
			if(BUTTON_DRAWSTATUS(msg->h_deswin) & BST_FOCUS)
			{
				BUTTON_DRAWSTATUS(msg->h_deswin) &= (~BST_FOCUS);
				GUI_InvalidateRect(msg->h_deswin, NULL, ORANGE_TRUE);
			}
		}

		return 0;

		case GUI_MSG_SET_UNFOCUS:
			BUTTON_STATUS(msg->h_deswin) &= (~BST_PUSHED);
			{
				__gui_notify_msg_t      notify_msg;
				notify_msg.hWnd     = msg->h_deswin;
				notify_msg.id       = GUI_WinGetItemId(msg->h_deswin);
				notify_msg.msgcode  = BN_KILLFOCUS;
				notify_msg.dwAddData = 0;
				GUI_NotifyParent(&notify_msg);
			}

			if(BUTTON_DRAWSTATUS(msg->h_deswin) & BST_FOCUS)
			{
				BUTTON_DRAWSTATUS(msg->h_deswin) &= (~BST_FOCUS);
				GUI_InvalidateRect(msg->h_deswin, NULL, ORANGE_TRUE);
			}

			return 0;

		case GUI_MSG_SET_FOCUS:
			BUTTON_STATUS(msg->h_deswin) |= BST_PUSHED;
			{
				__gui_notify_msg_t      notify_msg;
				notify_msg.hWnd     = msg->h_deswin;
				notify_msg.id       = GUI_WinGetItemId(msg->h_deswin);
				notify_msg.msgcode  = BN_SETFOCUS;
				notify_msg.dwAddData = 0;
				GUI_NotifyParent(&notify_msg);
			}

			if(!(BUTTON_DRAWSTATUS(msg->h_deswin) & BST_FOCUS))
			{
				BUTTON_DRAWSTATUS(msg->h_deswin) |= BST_FOCUS;
				GUI_InvalidateRect(msg->h_deswin, NULL, ORANGE_TRUE);
			}
			return 0;

		case GUI_MSG_TOUCH:
		{
			button_ex_para_t  	*user_data;

			user_data   = (button_ex_para_t *)GUI_WinGetAttr(msg->h_deswin);
			switch(msg->dwAddData1)
			{
				case GUI_MSG_TOUCH_DOWN:
				{
					if(GUI_WinGetCaptureWin() == msg->h_deswin)
					{
						break;
					}

					GUI_WinSetCaptureWin(msg->h_deswin);
					if(user_data->type == BUTTON_TYPE_PRESS) {
						BUTTON_STATUS(msg->h_deswin) |= BST_PUSHED;

						if(!(BUTTON_DRAWSTATUS(msg->h_deswin) & BST_FOCUS))
						{
							BUTTON_DRAWSTATUS(msg->h_deswin) |= BST_FOCUS;
							GUI_InvalidateRect(msg->h_deswin, NULL, ORANGE_TRUE);
						}

						{
							__gui_notify_msg_t      notify_msg;
							notify_msg.hWnd     = msg->h_deswin;
							notify_msg.id       = GUI_WinGetItemId(msg->h_deswin);
							notify_msg.msgcode  = BN_PUSHED;
							notify_msg.dwAddData = GUI_MSG_TOUCH_DOWN;
							GUI_NotifyParent(&notify_msg);
						}
					}
				}
				break;

				case GUI_MSG_TOUCH_LONGDOWN:
				{
					if(user_data->type == BUTTON_TYPE_PRESS) {
						BUTTON_STATUS(msg->h_deswin) |= BST_PUSHED;

						if(!(BUTTON_DRAWSTATUS(msg->h_deswin) & BST_FOCUS))
						{
							BUTTON_DRAWSTATUS(msg->h_deswin) |= BST_FOCUS;
							GUI_InvalidateRect(msg->h_deswin, NULL, ORANGE_TRUE);
						}

						{
							__gui_notify_msg_t      notify_msg;
							notify_msg.hWnd     = msg->h_deswin;
							notify_msg.id       = GUI_WinGetItemId(msg->h_deswin);
							notify_msg.msgcode  = BN_PUSHED;
							notify_msg.dwAddData = GUI_MSG_TOUCH_LONGDOWN;
							GUI_NotifyParent(&notify_msg);
						}
					}
				}
				break;

				case GUI_MSG_TOUCH_MOVE:
				{
					__gui_notify_msg_t      notify_msg;

					if(GUI_WinGetCaptureWin() != msg->h_deswin)
					{
						break;
					}
					if(user_data->type == BUTTON_TYPE_PRESS) {
						if(!(BUTTON_STATUS(msg->h_deswin) & BST_PUSHED))
						{
							BUTTON_STATUS(msg->h_deswin) |= BST_PUSHED;
						}
						if(!(BUTTON_DRAWSTATUS(msg->h_deswin) & BST_FOCUS))
						{
							BUTTON_DRAWSTATUS(msg->h_deswin) |= BST_FOCUS;
							GUI_InvalidateRect(msg->h_deswin, NULL, ORANGE_TRUE);
						}

						notify_msg.hWnd     = msg->h_deswin;
						notify_msg.id       = GUI_WinGetItemId(msg->h_deswin);
						notify_msg.msgcode  = BN_PUSHED;
						notify_msg.dwAddData = GUI_MSG_TOUCH_MOVE;
						GUI_NotifyParent(&notify_msg);
					}
				}
				break;

				case GUI_MSG_TOUCH_UP:
				{
					__s32               x;
					__s32               y;
					RECT                client_rect;
					__gui_rect_t        screct;

					if(GUI_WinGetCaptureWin() != msg->h_deswin)
					{
						break;
					}

					GUI_WinReleaseCapture();
					x = LOSWORD(msg->dwAddData2);
					y = HISWORD(msg->dwAddData2);
					GUI_WinGetClientRECT(msg->h_deswin, &client_rect);
					//SysRectToGuiRect(&client_rect, &screct);
					if(user_data->type == BUTTON_TYPE_PRESS) {
						if(pt_in_rect(&client_rect, x, y))
						{
							BUTTON_STATUS(msg->h_deswin) &= ~BST_PUSHED;
							//GUI_InvalidateRect (msg->h_deswin, NULL, ORANGE_TRUE);
							{
								__gui_notify_msg_t      notify_msg;
								notify_msg.hWnd     = msg->h_deswin;
								notify_msg.id       = GUI_WinGetItemId(msg->h_deswin);
								notify_msg.msgcode  = BN_CLICKED;
								notify_msg.dwAddData = last_key;
								GUI_NotifyParent(&notify_msg);
							}
						}
						else if(BUTTON_STATUS(msg->h_deswin) & BST_PUSHED)
						{
							BUTTON_STATUS(msg->h_deswin) &= ~BST_PUSHED;
							//GUI_InvalidateRect (msg->h_deswin, NULL, ORANGE_TRUE);
							{
								__gui_notify_msg_t      notify_msg;
								notify_msg.hWnd     = msg->h_deswin;
								notify_msg.id       = GUI_WinGetItemId(msg->h_deswin);
								notify_msg.msgcode  = BN_CLICKED;
								notify_msg.dwAddData = last_key;
								GUI_NotifyParent(&notify_msg);
							}
						}
					} else if(user_data->type == BUTTON_TYPE_ONOFF) {
						__here__;
						if(pt_in_rect(&client_rect, x, y)) {
							__here__;
							BUTTON_STATUS(msg->h_deswin) ^= BST_PUSHED;
							if(BUTTON_STATUS(msg->h_deswin) & BST_PUSHED) {
								__gui_notify_msg_t      notify_msg;
								notify_msg.hWnd     = msg->h_deswin;
								notify_msg.id       = GUI_WinGetItemId(msg->h_deswin);
								notify_msg.msgcode  = BN_PUSHED;
								notify_msg.dwAddData = last_key;
								GUI_NotifyParent(&notify_msg);
								BUTTON_DRAWSTATUS(msg->h_deswin) |= BST_FOCUS;
								__here__;
							} else {
								__gui_notify_msg_t      notify_msg;
								notify_msg.hWnd     = msg->h_deswin;
								notify_msg.id       = GUI_WinGetItemId(msg->h_deswin);
								notify_msg.msgcode  = BN_UNPUSHED;
								notify_msg.dwAddData = last_key;
								GUI_NotifyParent(&notify_msg);
								BUTTON_DRAWSTATUS(msg->h_deswin) &= (~BST_FOCUS);
								__here__;
							}
						}
						GUI_InvalidateRect (msg->h_deswin, NULL, ORANGE_TRUE);
					}
				}
				break;
			}
		}

		if(msg->dwAddData1 != GUI_MSG_TOUCH_UP)
		{
			last_key = msg->dwAddData1;
		}
		else
		{
			last_key = 0xffffffff;
		}

		return 0;

		case GUI_MSG_WIN_WAKEUP:
		case GUI_MSG_PAINT:
		{
			__gui_rect_t 	rcrect;
			RECT            fbrect;
			button_ex_data_t   *button_data;
			button_ex_para_t  	*user_data;
			GUI_RECT        rect;
			//button_data = (button_ex_para_t *)GUI_CtrlWinGetAddData(msg->h_deswin);
			user_data   = (button_ex_para_t *)GUI_WinGetAttr(msg->h_deswin);
			button_data = (button_ex_data_t *)GUI_WinGetAddData(msg->h_deswin);
			GUI_LyrWinSel((H_LYR)(GUI_WinGetLyrWin(msg->h_deswin)));
			GUI_WinGetClientFBRect(msg->h_deswin, &fbrect);

			if(GUI_WinGetStyle(msg->h_deswin) & WS_MEMDEV)
			{
				if(!button_data->hmem)
				{
#if APP_MEMDEV_EN
					button_data->hmem = GUI_MEMDEV_Create(fbrect.x, fbrect.y, fbrect.width, fbrect.height);
#endif
				}
			}

			SysRectToGuiRect(&fbrect, &rcrect);

			if(button_data->hmem)
			{
#if APP_MEMDEV_EN
				GUI_MEMDEV_Select(button_data->hmem);
#endif
			}// start memory device

			if(BUTTON_DRAWSTATUS(msg->h_deswin) & BST_DISABLED)
			{
				GUI_SetBkColor(user_data->bk_color);
				GUI_ClearRect(rcrect.left, rcrect.top,
				              rcrect.right, rcrect.bottom);

				if(user_data->disable_bmp)
				{
					GUI_BMP_Draw(user_data->disable_bmp, rcrect.left + user_data->bmp_pos.x,
					             rcrect.top + user_data->bmp_pos.y);
				}

				if(user_data->alpha_en)
				{
					GUI_OpenAlphaBlend();
				}

				GUI_CharSetToEncode(user_data->draw_code);
				GUI_SetFont(user_data->draw_font);
				GUI_SetDrawMode(GUI_DRAWMODE_TRANS);
				GUI_SetColor(user_data->distxt_color);
				rect.x0 = rcrect.left;
				rect.y0 = rcrect.top;
				rect.x1 = rcrect.right;
				rect.y1 = rcrect.bottom;

				if(!user_data->txt_align)
				{
					GUI_DispStringAt(user_data->text,
					                 rcrect.left + user_data->text_pos.x,
					                 rcrect.top + user_data->text_pos.y);
				}
				else
				{
					GUI_DispStringInRect(user_data->text, &rect, user_data->txt_align);
				}
			}
			else if(BUTTON_DRAWSTATUS(msg->h_deswin) & BST_FOCUS)
			{
				GUI_SetBkColor(user_data->bk_color);
				GUI_ClearRect(rcrect.left, rcrect.top,
				              rcrect.right, rcrect.bottom);

				if(user_data->focus_bmp)
				{
					GUI_BMP_Draw(user_data->focus_bmp, rcrect.left + user_data->bmp_pos.x,
					             rcrect.top + user_data->bmp_pos.y);
				}

				if(user_data->alpha_en)
				{
					GUI_OpenAlphaBlend();
				}

				GUI_CharSetToEncode(user_data->draw_code);
				GUI_SetFont(user_data->draw_font);
				GUI_SetDrawMode(GUI_DRAWMODE_TRANS);
				GUI_SetColor(user_data->ftxt_color);
				rect.x0 = rcrect.left;
				rect.y0 = rcrect.top;
				rect.x1 = rcrect.right;
				rect.y1 = rcrect.bottom;

				if(!user_data->txt_align)
				{
					GUI_DispStringAt(user_data->text,
					                 rcrect.left + user_data->text_pos.x,
					                 rcrect.top + user_data->text_pos.y);
				}
				else
				{
					GUI_DispStringInRect(user_data->text, &rect, user_data->txt_align);
				}
			}
			else
			{
				GUI_SetBkColor(user_data->bk_color);
				GUI_ClearRect(rcrect.left, rcrect.top,
				              rcrect.right, rcrect.bottom);

				if(user_data->unfocus_bmp)
				{
					GUI_BMP_Draw(user_data->unfocus_bmp, rcrect.left + user_data->bmp_pos.x,
					             rcrect.top + user_data->bmp_pos.y);
				}

				if(user_data->alpha_en)
				{
					GUI_OpenAlphaBlend();
				}

				GUI_CharSetToEncode(user_data->draw_code);
				GUI_SetFont(user_data->draw_font);
				GUI_SetDrawMode(GUI_DRAWMODE_TRANS);
				GUI_SetColor(user_data->uftxt_color);
				rect.x0 = rcrect.left;
				rect.y0 = rcrect.top;
				rect.x1 = rcrect.right;
				rect.y1 = rcrect.bottom;

				if(!user_data->txt_align)
				{
					GUI_DispStringAt(user_data->text,
					                 rcrect.left + user_data->text_pos.x,
					                 rcrect.top + user_data->text_pos.y);
				}
				else
				{
					GUI_DispStringInRect(user_data->text, &rect, user_data->txt_align);
				}
			}

			if(button_data->hmem)
			{
#if APP_MEMDEV_EN
				GUI_MEMDEV_CopyToLCD(button_data->hmem);            // copy from memory device to fb
				GUI_MEMDEV_Select(NULL);                      // stop memory device
				GUI_MEMDEV_Delete(button_data->hmem);
#endif
				button_data->hmem = NULL;
			}

			if(user_data->alpha_en)
			{
				GUI_CloseAlphaBlend();
			}

			GUI_LyrWinFlushFB(GUI_WinGetLyrWin(msg->h_deswin));
		}

		return 0;

		default:
			break;
	}

	return GUI_FrmWinDefaultProc(msg);
}

H_WIN ButtonExCreate(H_WIN parent, H_LYR layer, button_ex_para_t *button_para)
{
    H_WIN hWin;
	__gui_framewincreate_para_t frmWndPara;

	eLIBs_memset(&frmWndPara, 0, sizeof(__gui_framewincreate_para_t));
    frmWndPara.name         = "button_ex";
    frmWndPara.dwStyle      = WS_VISIBLE | WS_MEMDEV;
	frmWndPara.dwExStyle    = WS_EX_NONE;
	frmWndPara.spCaption    = NULL;
	frmWndPara.hOwner       = NULL;
	frmWndPara.hHosting     = parent;
    frmWndPara.hLayer       = layer;
	frmWndPara.FrameWinProc = (__pGUI_WIN_CB)esKRNL_GetCallBack((__pCBK_t)ButtonExProc);
    frmWndPara.id            = button_para->id;
    frmWndPara.rect.x		= button_para->rect.x;
    frmWndPara.rect.y		= button_para->rect.y;
    frmWndPara.rect.width	= button_para->rect.width;
    frmWndPara.rect.height	= button_para->rect.height;
	frmWndPara.BkColor.alpha= 0;
	frmWndPara.BkColor.red  = 0;
	frmWndPara.BkColor.green= 0;
	frmWndPara.BkColor.blue = 0;
	frmWndPara.attr         = button_para;
	frmWndPara.dwReserved   = 0;
    return (GUI_FrmWinCreate(&frmWndPara));
}

void button_group_create(H_WIN parent, H_LYR layer, button_ex_ui_t *button_ex_ui, H_WIN *button, __s32 count)
{
	__u32 i;
	button_ex_para_t *button_para;

	button_para	= (button_ex_para_t *)esMEMS_Malloc(0, sizeof(button_ex_para_t) * count);
	if(!button_para) {
		__wrn("button_para malloc error\n");
		return;
	}
	eLIBs_memset(button_para, 0, sizeof(button_ex_para_t) * count);

	for(i = 0; i < count; i++)
	{
		button_para[i].type			= button_ex_ui[i].type;
		button_para[i].id			= button_ex_ui[i].id;
		button_para[i].rect.x		= button_ex_ui[i].rect.x;
		button_para[i].rect.y		= button_ex_ui[i].rect.y;
		button_para[i].rect.width		= button_ex_ui[i].rect.width;
		button_para[i].rect.height		= button_ex_ui[i].rect.height;
		button_para[i].unfocus_bmp	= button_ex_ui[i].bmp_res[0].data;
		button_para[i].focus_bmp		= button_ex_ui[i].bmp_res[1].data;
		button_para[i].disable_bmp		= button_ex_ui[i].bmp_res[2].data;
		button_para[i].bmp_pos.x		= 0;
		button_para[i].bmp_pos.y		= 0;
		button_para[i].bk_color		= 0x0;
		button[i] = ButtonExCreate(parent, layer, &button_para[i]);
	}
}

void button_group_destroy(H_WIN *button, __s32 count)
{
	int i;
	button_ex_para_t *button_para;

	for(i = 0; i < count; i++) {
		button_para = GUI_WinGetAttr(button[i]);
		esMEMS_Mfree(0, button_para);
		GUI_FrmWinDelete(button[i]);
	}
}

__s32  ButtonExSetFocusBmp(H_WIN hbutton, void *bmp)
{
	button_ex_para_t  	*user_data;

	if((!hbutton) || (!bmp))
	{
		__wrn("input parameter error!\n");
		return ORANGE_FAIL;
	}

	user_data   = (button_ex_para_t *)(((button_ex_data_t *)GUI_WinGetAddData(hbutton))->data);
	user_data->focus_bmp = bmp;
	return ORANGE_OK;
}

__s32  ButtonExSetUnFocusBmp(H_WIN hbutton, void *bmp)
{
	button_ex_para_t  	*user_data;

	if((!hbutton) || (!bmp))
	{
		__wrn("input parameter error!\n");
		return ORANGE_FAIL;
	}

	user_data   = (button_ex_para_t *)(((button_ex_data_t *)GUI_WinGetAddData(hbutton))->data);
	user_data->unfocus_bmp = bmp;
	return ORANGE_OK;
}

__s32  ButtonExSetDisableBmp(H_WIN hbutton, void *bmp)
{
	button_ex_para_t  	*user_data;

	if((!hbutton) || (!bmp))
	{
		__wrn("input parameter error!\n");
		return ORANGE_FAIL;
	}

	user_data   = (button_ex_para_t *)(((button_ex_data_t *)GUI_WinGetAddData(hbutton))->data);
	user_data->disable_bmp = bmp;
	return ORANGE_OK;
}

__s32  ButtonSetText(H_WIN hbutton, char *text)
{
	button_ex_para_t  	*user_data;

	if((!hbutton) || (!text))
	{
		__wrn("input parameter error!\n");
		return ORANGE_FAIL;
	}

	user_data   = (button_ex_para_t *)(((button_ex_data_t *)GUI_WinGetAddData(hbutton))->data);
	user_data->text = text;
	return ORANGE_OK;
}

__s32  ButtonSetUnFcsTxtColor(H_WIN hbutton, __u32 color)
{
	button_ex_para_t  	*user_data;

	if((!hbutton))
	{
		__wrn("input parameter error!\n");
		return ORANGE_FAIL;
	}

	user_data   = (button_ex_para_t *)(((button_ex_data_t *)GUI_WinGetAddData(hbutton))->data);
	user_data->uftxt_color = color;
	return ORANGE_OK;
}

__s32  ButtonSetFcsTxtColor(H_WIN hbutton, __u32  color)
{
	button_ex_para_t  	*user_data;

	if((!hbutton))
	{
		__wrn("input parameter error!\n");
		return ORANGE_FAIL;
	}

	user_data   = (button_ex_para_t *)(((button_ex_data_t *)GUI_WinGetAddData(hbutton))->data);
	user_data->ftxt_color = color;
	return ORANGE_OK;
}

__s32  ButtonSetBkColor(H_WIN hbutton, __u32  color)
{
	button_ex_para_t  	*user_data;

	if((!hbutton))
	{
		__wrn("input parameter error!\n");
		return ORANGE_FAIL;
	}

	user_data   = (button_ex_para_t *)(((button_ex_data_t *)GUI_WinGetAddData(hbutton))->data);
	user_data->bk_color = color;
	return ORANGE_OK;
}

__s32  ButtonSetBmpPos(H_WIN hbutton, __pos_t *pos)
{
	button_ex_para_t  	*user_data;

	if((!hbutton) || (!pos))
	{
		__wrn("input parameter error!\n");
		return ORANGE_FAIL;
	}

	user_data   = (button_ex_para_t *)(((button_ex_data_t *)GUI_WinGetAddData(hbutton))->data);
	user_data->bmp_pos.x = pos->x;
	user_data->bmp_pos.y = pos->y;
	return ORANGE_OK;
}

__s32  ButtonSetTxtPos(H_WIN hbutton, __pos_t *pos)
{
	button_ex_para_t  	*user_data;

	if((!hbutton) || (!pos))
	{
		__wrn("input parameter error!\n");
		return ORANGE_FAIL;
	}

	user_data   = (button_ex_para_t *)(((button_ex_data_t *)GUI_WinGetAddData(hbutton))->data);
	user_data->text_pos.x = pos->x;
	user_data->text_pos.y = pos->y;
	return ORANGE_OK;
}
