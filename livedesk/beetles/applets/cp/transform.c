
#include "cp_i.h"
#include "transform.h"

#ifdef THIS_DEBUG
#undef THIS_DEBUG
#endif

#ifdef MW_PROJ_DEBUG
//#define THIS_DEBUG
#endif

#define BYTESPERPIXEL       4
#define BITMAPHEADERSIZE    54
#define BITMAPWIDTH         18
#define BITMAPHEIGHT        22

typedef struct {
    char * output;
    HTHEME clockhands[CLOCKHANDS];
    void * handdata[CLOCKHANDS];
    int width;
    int height;
    int offset;
    int radius;
    int length;
    int start_x;
    int start_y;
} swapqua_t;

//int swap_quadrant_end(void * handler);

static inline int GUI__Read32(const char * pData) {
    int value;
    value = *pData;
    value |= (*(pData + 1) << 8);
    value |= ((int)*(pData + 2) << 16);
    value |= ((int)*(pData + 3) << 24);
    return value;
}

static inline int swap_quadrant_getwh(char * pic, int * width, int * height)
{
    if (!pic) {
        LogW("Null argument");
        return -1;
    }
    *width = GUI__Read32((char *)((int)pic + BITMAPWIDTH));
    *height = GUI__Read32((char *)((int)pic + BITMAPHEIGHT));
    *height = -(*height);
    return 0;
}

void * picswap_quadrant_begin(__u32 clockhands[CLOCKHANDS], int offset, int radius)
{
    int w;
    int h;
    int i;
    swapqua_t * handler = esMEMS_Malloc(0, sizeof(swapqua_t));
    if (!handler) {
        LogW("Malloc failed");
        goto error_end;
    }
    eLIBs_memset(handler, 0, sizeof(swapqua_t));

    handler->offset = offset;
    handler->radius = radius;
    handler->length = offset + radius;
    handler->output = (char *)esMEMS_Balloc(handler->length * handler->length * BYTESPERPIXEL);
    if (!handler->output) {
        LogW("Malloc failed");
        goto error_end;
    }
    handler->clockhands[0] = dsk_theme_open(clockhands[0]);
    handler->handdata[0] = dsk_theme_hdl2buf(handler->clockhands[0]);
    if (swap_quadrant_getwh(handler->handdata[0], &handler->width, &handler->height)) goto error_end;
    if (handler->length > GG_MATH_MIN(handler->width, handler->height)) {
        LogW("Length = %d, Width = %d, Height = %d", handler->length, handler->width, handler->height);
        goto error_end;
    }
    if (handler->radius > GG_MATH_HALF(GG_MATH_MIN(handler->width, handler->height))) {
        LogW("Radius = %d, Width = %d, Height = %d", handler->radius, handler->width, handler->height);
        goto error_end;
    }
    for (i = 1; i < CLOCKHANDS; i++) {
        handler->clockhands[i] = dsk_theme_open(clockhands[i]);
        handler->handdata[i] = dsk_theme_hdl2buf(handler->clockhands[i]);
        if (swap_quadrant_getwh(handler->handdata[i], &w, &h)) goto error_end;
        if (handler->width != w || handler->height != h) goto error_end;
    }
#ifdef THIS_DEBUG
    LogD("Width = %d, Height = %d, Size = %d", handler->width, handler->height, dsk_theme_hdl2size(handler->clockhands[0]));
    LogD("Offset = %d, Radius = %d, Length = %d", handler->offset, handler->radius, handler->length);
#endif
    return handler;
error_end:
    picswap_quadrant_end(handler);
    LogW("Error end");
    return NULL;
}

int picswap_quadrant_end(void * handler)
{
    int i;
    swapqua_t * sq;
    if (handler) {
        sq = (swapqua_t *)handler;

        for (i = 0; i < CLOCKHANDS; i++) {
            dsk_theme_close(sq->clockhands[i]);
        }
        if (sq->output) {
            esMEMS_Bfree(sq->output, sq->length* sq->length);
        }
        esMEMS_Mfree(0, sq);
    }
    return 0;
}

static inline int swap_quadrant_0degree(int start_x, int start_y, int length, int width,
                                char * dest, char * src)
{
    int w;
    int h;
    char * line_dest;
    char * line_src;
    for (h = start_y; h < start_y + length; h++) {
        line_dest = &dest[(h - start_y) * length * BYTESPERPIXEL];
        line_src = &src[(h * width + start_x) * BYTESPERPIXEL];
        for (w = 0; w < length; w++) {
            *line_dest++ = *line_src++;
            *line_dest++ = *line_src++;
            *line_dest++ = *line_src++;
            *line_dest++ = *line_src++;
        }
    }
    return 0;
}

static inline int swap_quadrant_45degree(int start_x, int start_y, int length, int width,
                                char * dest, char * src)
{
    int w;
    int h;
    char * line_src;
    int tmp;
    for (h = start_y; h < start_y + length; h++) {
        line_src = &src[(h * width + start_x) * BYTESPERPIXEL];
        for (w = length; w > 0; w--) {
            tmp = (w * length - 1 - (h - start_y)) * BYTESPERPIXEL;
            dest[tmp++] = *line_src++;
            dest[tmp++] = *line_src++;
            dest[tmp++] = *line_src++;
            dest[tmp++] = *line_src++;
        }
    }
    return 0;
}

static inline int swap_quadrant_90degree(int start_x, int start_y, int length, int width,
                                char * dest, char * src)
{
    int w;
    int h;
    char * line_src;
    int tmp;
    for (h = start_y; h < start_y + length; h++) {
        line_src = &src[(h * width + start_x) * BYTESPERPIXEL];
        for (w = 1; w <= length; w++) {
            tmp = (w * length - 1 - (h - start_y)) * BYTESPERPIXEL;
            dest[tmp++] = *line_src++;
            dest[tmp++] = *line_src++;
            dest[tmp++] = *line_src++;
            dest[tmp++] = *line_src++;
        }
    }
    return 0;
}

static inline int swap_quadrant_135degree(int start_x, int start_y, int length, int width,
                                char * dest, char * src)
{
    int w;
    int h;
    char * line_dest;
    char * line_src;
    for (h = start_y; h < start_y + length; h++) {
        line_dest = &dest[(length - 1 - (h - start_y)) * length * BYTESPERPIXEL];
        line_src = &src[(h * width + start_x) * BYTESPERPIXEL];
        for (w = 0; w < length; w++) {
            *line_dest++ = *line_src++;
            *line_dest++ = *line_src++;
            *line_dest++ = *line_src++;
            *line_dest++ = *line_src++;
        }
    }
    return 0;
}

static inline int swap_quadrant_180degree(int start_x, int start_y, int length, int width,
                                char * dest, char * src)
{
    int w;
    int h;
    char * line_src;
    int tmp;
    for (h = start_y; h < start_y + length; h++) {
        line_src = &src[(h * width + start_x) * BYTESPERPIXEL];
        for (w = 0; w < length; w++) {
            tmp = ((length - (h - start_y)) * length - 1 - w) * BYTESPERPIXEL;
            dest[tmp++] = *line_src++;
            dest[tmp++] = *line_src++;
            dest[tmp++] = *line_src++;
            dest[tmp++] = *line_src++;
        }
    }
    return 0;
}

static inline int swap_quadrant_225degree(int start_x, int start_y, int length, int width,
                                char * dest, char * src)
{
    int w;
    int h;
    char * line_src;
    int tmp;
    for (h = start_y; h < start_y + length; h++) {
        line_src = &src[(h * width + start_x) * BYTESPERPIXEL];
        for (w = 0; w < length; w++) {
            tmp = (w * length + (h - start_y)) * BYTESPERPIXEL;
            dest[tmp++] = *line_src++;
            dest[tmp++] = *line_src++;
            dest[tmp++] = *line_src++;
            dest[tmp++] = *line_src++;
        }
    }
    return 0;
}

static inline int swap_quadrant_270degree(int start_x, int start_y, int length, int width,
                                char * dest, char * src)
{
    int w;
    int h;
    char * line_src;
    int tmp;
    for (h = start_y; h < start_y + length; h++) {
        line_src = &src[(h * width + start_x) * BYTESPERPIXEL];
        for (w = length - 1; w >= 0; w--) {
            tmp = (w * length + (h - start_y)) * BYTESPERPIXEL;
            dest[tmp++] = *line_src++;
            dest[tmp++] = *line_src++;
            dest[tmp++] = *line_src++;
            dest[tmp++] = *line_src++;
        }
    }
    return 0;
}

static inline int swap_quadrant_315degree(int start_x, int start_y, int length, int width,
                                char * dest, char * src)
{
    int w;
    int h;
    char * line_src;
    int tmp;
    for (h = start_y; h < start_y + length; h++) {
        line_src = &src[(h * width + start_x) * BYTESPERPIXEL];
        for (w = length - 1; w >= 0; w--) {
            tmp = ((h - start_y) * length + w) * BYTESPERPIXEL;
            dest[tmp++] = *line_src++;
            dest[tmp++] = *line_src++;
            dest[tmp++] = *line_src++;
            dest[tmp++] = *line_src++;
        }
    }
    return 0;
}

static inline int swap_quadrant(swapqua_t * sq, int quadrant, int picidx)
{
    char * src;
    int center_x;
    int center_y;

    if (!sq) {
        LogE("Null handler");
        return NULL;
    }
    if (picidx >= CLOCKHANDS) {
        LogE("Wrong argument %d", picidx);
        return NULL;
    }
    src = (char *)((int)(sq->handdata[picidx]) + BITMAPHEADERSIZE);
    center_x = GG_MATH_HALF(sq->width);
    center_y = GG_MATH_HALF(sq->height);
    sq->start_x = center_x -sq->offset;
    sq->start_y = center_y - sq->radius;
#ifdef THIS_DEBUG
    LogD("center_x = %d, center_y = %d, start_x = %d, start_y = %d, Length = %d", center_x, center_y,
            sq->start_x, sq->start_y, sq->length);
#endif

    switch (quadrant) {
    case 2:// Switch x y
        swap_quadrant_45degree(sq->start_x, sq->start_y, sq->length, sq->width, sq->output, src);
        break;
    case 3:
        swap_quadrant_90degree(sq->start_x, sq->start_y, sq->length, sq->width, sq->output, src);
        sq->start_x = center_x - sq->offset;
        sq->start_y = center_y - sq->offset;
        break;
    case 4:
        swap_quadrant_135degree(sq->start_x, sq->start_y, sq->length, sq->width, sq->output, src);
        sq->start_x = center_x - sq->offset;
        sq->start_y = center_y - sq->offset;
        break;
    case 5:
        swap_quadrant_180degree(sq->start_x, sq->start_y, sq->length, sq->width, sq->output, src);
        sq->start_x = center_x - sq->radius;
        sq->start_y = center_y - sq->offset;
        break;
    case 6:
        swap_quadrant_225degree(sq->start_x, sq->start_y, sq->length, sq->width, sq->output, src);
        sq->start_x = center_x - sq->radius;
        sq->start_y = center_y - sq->offset;
        break;
    case 7:
        swap_quadrant_270degree(sq->start_x, sq->start_y, sq->length, sq->width, sq->output, src);
        sq->start_x = center_x - sq->radius;
        sq->start_y = center_y - sq->radius;
        break;
    case 8:
        swap_quadrant_315degree(sq->start_x, sq->start_y, sq->length, sq->width, sq->output, src);
        sq->start_x = center_x - sq->radius;
        sq->start_y = center_y - sq->radius;
        break;
    case 1:
    default:
        swap_quadrant_0degree(sq->start_x, sq->start_y, sq->length, sq->width, sq->output, src);
        break;
    }
    return 0;
}

void * picget_clock_pic(void * handler, int clock, int * length, int * start_x, int * start_y)
{
    swapqua_t * sq;
    if (!handler) {
        LogE("Null handler");
        return NULL;
    }
    sq = (swapqua_t *)handler;
    if (clock >= 0 && clock < 8) {
        swap_quadrant(handler, 1, clock);
    } else if (clock >= 8 && clock < 15) {
        swap_quadrant(handler, 2, 15 - clock);
    } else if (clock >= 15 && clock < 23) {
        swap_quadrant(handler, 3, clock - 15);
    } else if (clock >= 23 && clock < 30) {
        swap_quadrant(handler, 4, 30 - clock);
    } else if (clock >= 30 && clock < 38) {
        swap_quadrant(handler, 5, clock - 30);
    } else if (clock >= 38 && clock < 45) {
        swap_quadrant(handler, 6, 45 - clock);
    } else if (clock >= 45 && clock < 53) {
        swap_quadrant(handler, 7, clock - 45);
    } else if (clock >= 53 && clock < 60) {
        swap_quadrant(handler, 8, 60 - clock);
    } else {
        LogE("Wrong argument clock = %d", clock);
        return NULL;
    }

    *length = sq->length;
    *start_x = sq->start_x;
    *start_y = sq->start_y;
    return sq->output;
}


