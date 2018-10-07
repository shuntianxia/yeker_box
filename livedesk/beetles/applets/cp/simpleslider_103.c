
#include <apps.h>
#include "simpleslider_103.h"
#include "cp_i.h"

#define BYTESPERPIXEL       0x4
#define BITMAPHEADERSIZE    54
static int liangduzhi_value;
static int cant_paint;

typedef struct {
    MwSimpleSliderClass_103 cls; 

    int puzzleDataSize;
    void * puzzleData;
	int puzzleDataSize2;
    void * puzzleData2;
    // Be careful, this member must be the last one.
    int bmps[1];
} SimpleSliderDesc_103;
#if 0
void * MwSimpleSliderNew_103(MwSimpleSliderClass_103 * cls)
{
    SimpleSliderDesc_103 * create;
    __size_t sz;
    sz = sizeof(SimpleSliderDesc_103) + (1 * sizeof(int));
    create = (SimpleSliderDesc_103 *)esMEMS_Malloc(0, sz);
	liangduzhi_value = -1;
	cant_paint = -1;
    if (create) {
        eLIBs_memcpy((void *)&create->cls, (void *)cls, sizeof(MwSimpleSliderClass_103));
        create->puzzleDataSize = BYTESPERPIXEL * cls->bg_width * cls->bg_height;
        create->puzzleData = esMEMS_Malloc(0, create->puzzleDataSize);
		create->puzzleDataSize2 = BYTESPERPIXEL * cls->up_width * cls->up_height;
        create->puzzleData2 = esMEMS_Malloc(0, create->puzzleDataSize2);
        if (NULL == create->puzzleData) {
            esMEMS_Mfree(0, create);
            return NULL;
        }
    } else {
        return NULL;
    }
    return (void *)create;
}

int MwSimpleSliderDelete_103(void * handler)
{
    SimpleSliderDesc_103 * desc;
    if (NULL == handler) {
        return -1;
    }
    desc = (SimpleSliderDesc_103 *)handler;
    if (desc->puzzleData) {
        esMEMS_Mfree(0, desc->puzzleData);
    }
	if (desc->puzzleData2) {
        esMEMS_Mfree(0, desc->puzzleData2);
    }
    esMEMS_Mfree(0, handler);
    return 0;
}
//#define REGULAR_DRAW
int MwSimpleSliderDraw_103(void * handler, H_LYR layer, int x, int y, int current)
{
    SimpleSliderDesc_103 * desc;
    int paint_x;
	int paint_y;
    int blocks;
    HTHEME bmpTheme;
    void * bmpData;
    HBLOCK backbg;
    HBLOCK foreg;
    FB fb;

    if (NULL == handler) {
        return -1;
    }
    desc = (SimpleSliderDesc_103 *)handler;
    // To get the current display position
    current = current + desc->cls.up_height;
    current = (current > desc->cls.limit) ? desc->cls.limit : current;
	if(current >= desc->cls.up_height){
      blocks = current / desc->cls.step;
	  if(desc->cls.direc > 1){
	  	paint_y = desc->cls.limit - desc->cls.step * blocks;
		paint_x = 0;
	  }else{
	    paint_x = desc->cls.limit - desc->cls.step * blocks;
		paint_y = 0;
	  }
	}else{
	  current = desc->cls.up_height;
	  blocks = current / desc->cls.step;
	  if(desc->cls.direc > 1){
	  	paint_y = desc->cls.up_height;
		paint_x = 0;
	  }else{
	    paint_x = desc->cls.up_width;
		paint_y = 0;
	  }
	}
   if(cant_paint != current){
   	   GUI_LyrWinGetFB(layer, &fb);
       backbg = GUI_BlockCreateFrom(NULL, fb.size.width, fb.size.height, fb.addr[0], 0);
       GUI_LyrWinSel(layer);
	   if((blocks != liangduzhi_value)){
	        LogMI("blocks %d",blocks);
			bmpTheme = user_theme_open(desc->cls.bgbmp_piece);
		    bmpData = user_theme_hdl2buf(bmpTheme);
			GUI_BMP_Draw(bmpData, x,y);
			user_theme_close(bmpTheme);
		
		    bmpTheme = user_theme_open(desc->cls.bgbmp_button);
		    bmpData = user_theme_hdl2buf(bmpTheme);
		    eLIBs_memcpy(desc->puzzleData2, (void *)((int)bmpData + BITMAPHEADERSIZE), desc->puzzleDataSize2);
		    foreg = GUI_BlockCreateFrom(NULL, desc->cls.up_width, desc->cls.up_height, desc->puzzleData2, 0);
		    GUI_BlockBitBlit(backbg,  x + paint_x, y + paint_y ,foreg, NULL, BLITFLAG_COLORALPHA, NULL);
			user_theme_close(bmpTheme);
		    GUI_BlockDelete(foreg);
		    liangduzhi_value = blocks;
		}else if(current == (desc->cls.up_height)){
		    LogMI("blocks %d",blocks);
			bmpTheme = user_theme_open(desc->cls.bgbmp_piece);
		    bmpData = user_theme_hdl2buf(bmpTheme);
			GUI_BMP_Draw(bmpData, x,y);
			user_theme_close(bmpTheme);
		
		    bmpTheme = user_theme_open(desc->cls.bgbmp_button);
		    bmpData = user_theme_hdl2buf(bmpTheme);
		    eLIBs_memcpy(desc->puzzleData2, (void *)((int)bmpData + BITMAPHEADERSIZE), desc->puzzleDataSize2);
		    foreg = GUI_BlockCreateFrom(NULL, desc->cls.up_width, desc->cls.up_height, desc->puzzleData2, 0);
		    GUI_BlockBitBlit(backbg,  x + paint_x, y + paint_y ,foreg, NULL, BLITFLAG_COLORALPHA, NULL);
			user_theme_close(bmpTheme);
		    GUI_BlockDelete(foreg);
		}
	 GUI_BlockDelete(backbg);
	 cant_paint = current;
   }
    return 0;
}
//#define REGULAR_DRAW
int MwSimpleSliderDraw_103_2(void * handler, H_LYR layer, int x, int y, int current)//shuzhi
{
	SimpleSliderDesc_103 * desc;
	int paint_x;
	int paint_y;
	int blocks;
	HTHEME bmpTheme;
	void * bmpData;
	HBLOCK backbg;
	HBLOCK foreg;
	FB fb;

	if (NULL == handler) {
		return -1;
	}
	desc = (SimpleSliderDesc_103 *)handler;
	// To get the current display position
	current = current + desc->cls.up_width;
	current = (current > desc->cls.limit) ? desc->cls.limit : current;
	if(current >= desc->cls.up_width){
	  blocks = current / desc->cls.step;
	  if(desc->cls.direc > 1){
		paint_y = desc->cls.limit - desc->cls.step * blocks;
		paint_x = 0;
	  }else{
		paint_x = desc->cls.limit - desc->cls.step * blocks;
		paint_y = 0;
	  }
	}else{
	  current = desc->cls.up_width;
	  blocks = current / desc->cls.step;
	  if(desc->cls.direc > 1){
		paint_y = desc->cls.up_height;
		paint_x = 0;
	  }else{
		paint_x = desc->cls.up_width;
		paint_y = 0;
	  }
	}
   if(cant_paint != current){
	   GUI_LyrWinGetFB(layer, &fb);
	   backbg = GUI_BlockCreateFrom(NULL, fb.size.width, fb.size.height, fb.addr[0], 0);
	   GUI_LyrWinSel(layer);
	   if((blocks != liangduzhi_value)){
			LogMI("blocks %d",blocks);
			bmpTheme = user_theme_open(desc->cls.bgbmp_piece);
			bmpData = user_theme_hdl2buf(bmpTheme);
			GUI_BMP_Draw(bmpData, x,y);
			user_theme_close(bmpTheme);
		
			bmpTheme = user_theme_open(desc->cls.bgbmp_button);
			bmpData = user_theme_hdl2buf(bmpTheme);
			eLIBs_memcpy(desc->puzzleData2, (void *)((int)bmpData + BITMAPHEADERSIZE), desc->puzzleDataSize2);
			foreg = GUI_BlockCreateFrom(NULL, desc->cls.up_width, desc->cls.up_height, desc->puzzleData2, 0);
			GUI_BlockBitBlit(backbg,  x + paint_x, y + paint_y ,foreg, NULL, BLITFLAG_COLORALPHA, NULL);
			user_theme_close(bmpTheme);
			GUI_BlockDelete(foreg);
			liangduzhi_value = blocks;
		}else if(current == (desc->cls.up_height)){
			LogMI("blocks %d",blocks);
			bmpTheme = user_theme_open(desc->cls.bgbmp_piece);
			bmpData = user_theme_hdl2buf(bmpTheme);
			GUI_BMP_Draw(bmpData, x,y);
			user_theme_close(bmpTheme);
		
			bmpTheme = user_theme_open(desc->cls.bgbmp_button);
			bmpData = user_theme_hdl2buf(bmpTheme);
			eLIBs_memcpy(desc->puzzleData2, (void *)((int)bmpData + BITMAPHEADERSIZE), desc->puzzleDataSize2);
			foreg = GUI_BlockCreateFrom(NULL, desc->cls.up_width, desc->cls.up_height, desc->puzzleData2, 0);
			GUI_BlockBitBlit(backbg,  x + paint_x, y + paint_y ,foreg, NULL, BLITFLAG_COLORALPHA, NULL);
			user_theme_close(bmpTheme);
			GUI_BlockDelete(foreg);
		}
	 GUI_BlockDelete(backbg);
	 cant_paint = current;
   }
	return 0;
}
#endif


