

#ifndef H_SLIDER_BENNIS_20130221
#define H_SLIDER_BENNIS_20130221

typedef enum {
    LEFT_RIGHT = 0,
    RIGHT_LEFT,
    BOTTOM_UP,
    UP_BOTTOM
} DrawDirection;

typedef struct {
    DrawDirection direc;
    int bmp_nr;
    OFFSET start_point;
    SIZE puzzle;
    int limit;
    int bgbmp_piece;
} SimpleSliderClass;

void * SimpleSliderNew(SimpleSliderClass * cls, int * bmps);
int SimpleSliderDelete(void * handler);
int SimpleSliderDraw(void * handler, H_LYR layer, RECT * rect, int current);
int SimpleSliderRedraw(void * handler, H_LYR layer, RECT * rect);

#endif


