#include "beetles_app.h"
#include "app_font.h"

typedef struct font_res_s
{
	__s32 size;
	GUI_FONT *font;
}font_res_t;

static font_res_t font_res[] =
{
	{11, NULL},
	{18, NULL},
	{20, NULL},
	{21, NULL},
	{22, NULL},
	{23, NULL},
	{25, NULL},
	{26, NULL},
	{30, NULL},
	{39, NULL},
	{40, NULL},
	{100, NULL},
};

void font_res_init()
{
	int i;
	GUI_TTF_ATTR attr;

	eLIBs_strcpy(attr.filepath, BEETLES_APP_ROOT"res\\fonts\\font.ttf");
	for(i = 0; i < (sizeof(font_res)/sizeof(font_res[0])); i++) {
		attr.height = font_res[i].size;
		font_res[i].font = GUI_TTF_CreateFont(&attr);
	}
}

void font_res_deinit()
{
	int i;
	
	GUI_SetFont(GUI_GetDefaultFont());
	for(i = 0; i < (sizeof(font_res)/sizeof(font_res[0])); i++) {
		if(font_res[i].font) {
			GUI_TTF_Done(font_res[i].font);
			font_res[i].font = NULL;
		}
	}
}

GUI_FONT *app_get_font(__s32 size)
{
	int i;

	for(i = 0; i < (sizeof(font_res)/sizeof(font_res[0])); i++) {
		if(size == font_res[i].size) {
			return font_res[i].font;
		}
	}
	return NULL;
}

H_LYR app_32bpp_layer_create(RECT LayerRect, __u8 pipe)
{
	H_LYR layer = NULL;
	
	FB  fb =
	{
	    {0, 0},                                   		/* size      */
	    {0, 0, 0},                                      /* buffer    */
	    {FB_TYPE_RGB, {PIXEL_COLOR_ARGB8888, 0, (__rgb_seq_t)0}},    /* fmt       */
	    
	};

	__disp_layer_para_t lstlyr =
	{
	    DISP_LAYER_WORK_MODE_NORMAL,
	    0,                                              /* ck_mode   */
	    0,                                              /* alpha_en  */
	    0,                                              /* alpha_val */
	    1,                                              /* pipe      */
	    0xff,                                           /* prio      */
	    {0, 0, 0, 0},                           		/* screen    */
	    {0, 0, 0, 0},                               	/* source    */
	    DISP_LAYER_OUTPUT_CHN_DE_CH1,                   /* channel   */
	   NULL                                            /* fb        */
    };	                                      
    __layerwincreate_para_t lyrcreate_info =  
    {                                         
        "APP_MAIN",                       
      	NULL,                                 
      	GUI_LYRWIN_STA_SUSPEND,               
      	GUI_LYRWIN_NORMAL                     
    };	
	
	
    fb.size.width		= LayerRect.width;            
    fb.size.height		= LayerRect.height;	        
    fb.fmt.fmt.rgb.pixelfmt = PIXEL_COLOR_ARGB8888; //PIXEL_MONO_8BPP; //PIXEL_COLOR_ARGB8888;
	
	lstlyr.src_win.x  		= 0;
	lstlyr.src_win.y  		= 0;

	lstlyr.src_win.width 	= LayerRect.width;

	lstlyr.src_win.height 	= LayerRect.height;
	
	lstlyr.scn_win.x		= LayerRect.x;
	lstlyr.scn_win.y		= LayerRect.y;
	
	lstlyr.scn_win.width 	= LayerRect.width;

	lstlyr.scn_win.height 	= LayerRect.height;
	
	lstlyr.pipe = pipe;		//1
	lstlyr.fb = &fb;
	lyrcreate_info.lyrpara = &lstlyr;
	
	layer = GUI_LyrWinCreate(&lyrcreate_info);
	
	if( !layer )
	{
		__err("app music layer create error !\n");
	}
	return layer;	
}

H_WIN cache_layer_create(__u32 width, __u32 height, __s32 pipe)
{
	FB  fb =
	{
	    {0, 0},                                   		/* size      */
	    {0, 0, 0},                                      /* buffer    */
	    {FB_TYPE_RGB, {PIXEL_COLOR_ARGB8888, 0, 0}},    /* fmt       */
	};
	__disp_layer_para_t para =
	{
	    DISP_LAYER_WORK_MODE_NORMAL,                    /* mode      */
	    0,                                              /* ck_mode   */
	    0,                                              /* alpha_en  */
	    0xff,                                           /* alpha_val */
	    1,                                              /* pipe      */
	    0xff,                                           /* prio      */
	    {0, 0, 0, 0},                           		/* screen    */
	    {0, 0, 0, 0},                               	/* source    */
	    DISP_LAYER_OUTPUT_CHN_DE_CH1,                   /* channel   */
	    NULL                                            /* fb        */
	};
		
	__layerwincreate_para_t create_info = 
	{
		"layer",
		NULL,
		GUI_LYRWIN_STA_SLEEP,
		0
	};
		
	fb.size.width  = width;
	fb.size.height = height;

	para.pipe			= pipe;
	
	para.scn_win.x 		= 0;
	para.scn_win.y 		= 0;
	para.scn_win.width 	= width;	
	para.scn_win.height = height;	

	para.src_win.x 		= 0;
	para.src_win.y 		= 0;
	para.src_win.width 	= width;	
	para.src_win.height = height;	

	para.fb = &fb;
	create_info.lyrpara = &para;	
	
	return (GUI_LyrWinCreate(&create_info));
}

