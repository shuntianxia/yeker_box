

#ifndef H_CONTROL_103_BENNIS_20121024
#define H_CONTROL_103_BENNIS_20121024

typedef enum {
    LEFT_RIGHT_103 = 0,
    RIGHT_LEFT_103,
    BOTTOM_UP_103,
    UP_BOTTOM_103
} MwDrawDirection_103;

typedef struct {
    MwDrawDirection_103 direc;
    int bg_width;
	int bg_height;
	int up_width;
	int up_height;
    int limit;
	int limit_min;
    int step;
	int bgbmp_piece;
	int bgbmp_button;

} MwSimpleSliderClass_103;

void * MwSimpleSliderNew_103(MwSimpleSliderClass_103 * cls);
int MwSimpleSliderDelete_103(void * handler);
int MwSimpleSliderDraw_103(void * handler, H_LYR layer, int x, int y, int current);
int MwSimpleSliderDraw_103_2(void * handler, H_LYR layer, int x, int y, int current);//shuzhi

#endif


