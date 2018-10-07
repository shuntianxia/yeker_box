#ifndef __TTF_FONT_H__
#define __APP_FONT_H__

typedef struct tag_bmp_res
{
	void *data;
	HTHEME handle;
}bmp_res_t;

void font_res_init();
void font_res_deinit();
GUI_FONT *app_get_font(__s32 size);

#endif
