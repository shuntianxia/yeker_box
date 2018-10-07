
#ifndef H_TRANSFORM_BENNIS_20121221
#define H_TRANSFORM_BENNIS_20121221

#define CLOCKHANDS          8

void * picswap_quadrant_begin(__u32 clockhands[CLOCKHANDS], int offset, int radius);
int picswap_quadrant_end(void * handler);
void * picget_clock_pic(void * handler, int clock, int * length, int * start_x, int * start_y);


#endif


