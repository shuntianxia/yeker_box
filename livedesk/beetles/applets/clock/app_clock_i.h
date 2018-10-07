/**************************************************************
******************************************************************/
#ifndef __APP_CLOCK_I_H__
#define __APP_CLOCK_I_H__

#include "beetles_app.h"
#include "app_font.h"

#define CLOCK_LAYER_X		0
#define CLOCK_LAYER_Y		0
#define CLOCK_LAYER_W		(720-CLOCK_LAYER_X)
#define CLOCK_LAYER_H		(720-CLOCK_LAYER_Y)

typedef struct tag_app_clock
{
	H_LYR lyr;
	H_WIN h_parent;
	H_WIN frm_win;
	GUI_FONT *font;
}app_clock_t;

typedef enum 
{
	CLOCK_PAINT_ALL,
	CLOCK_PAINT_DATE,
}clock_disp_id_t;

H_WIN app_clock_frm_create(app_clock_t *para);
#endif
