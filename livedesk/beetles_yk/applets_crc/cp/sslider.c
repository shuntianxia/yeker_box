
#include "cp_i.h"
#include "sslider.h"

#ifdef THIS_DEBUG
#undef THIS_DEBUG
#endif
#ifdef MW_PROJ_DEBUG
#define THIS_DEBUG
#endif

#define BYTESPERPIXEL       0x4
#define BITMAPHEADERSIZE    54

typedef struct {
    SimpleSliderClass cls; 

    int drew_position;// -1 -- No paint, >= 0 -- paint position
    int puzzleDataSize;
    void * puzzleData;
    // Be careful, this member must be the last one.
    int bmps[1];
} SimpleSliderDesc;

/**
 * Minimum step <= current >= Maximum step
 */
static inline int getStartPoint(OFFSET * start, OFFSET * top_left, DrawDirection direc,
                                int curr_step, int step_nr, SIZE * step_size)
{
    if (NULL == start || NULL == top_left){
        LogE("Wrong arguments");
        return -1;
    }
    if (curr_step <= 0 || curr_step > step_nr) {
        // Don't need to draw anything if current == 0
        return -1;
    }
    switch (direc) {
        case RIGHT_LEFT:
            LogE("Wrong direction definition");
            break;
        case BOTTOM_UP:
            start->x = top_left->x;
            start->y = top_left->y + ((step_nr - curr_step) * step_size->height);
            break;
        case UP_BOTTOM:
            LogE("Wrong direction definition");
            break;
        case LEFT_RIGHT:
            LogE("Wrong direction definition");
            break;
        default:
            LogE("Wrong direction definition");
            return -1;
    }
    return 0;
}

void * SimpleSliderNew(SimpleSliderClass * cls, int * bmps)
{
    SimpleSliderDesc * create;
    __size_t sz;
    sz = sizeof(SimpleSliderDesc) + (cls->bmp_nr * sizeof(int));
    create = (SimpleSliderDesc *)esMEMS_Malloc(0, sz);
    if (create) {
        eLIBs_memcpy((void *)&create->cls, (void *)cls, sizeof(SimpleSliderClass));
        create->drew_position = -1;
        create->puzzleDataSize = BYTESPERPIXEL * cls->puzzle.width * cls->puzzle.height;
        create->puzzleData = esMEMS_Malloc(0, create->puzzleDataSize);
        if (NULL == create->puzzleData) {
            esMEMS_Mfree(0, create);
            return NULL;
        }
        eLIBs_memcpy((void *)&create->bmps[0], (void *)bmps, (cls->bmp_nr * sizeof(int)));
    } else {
        return NULL;
    }
    return (void *)create;
}

int SimpleSliderDelete(void * handler)
{
    SimpleSliderDesc * desc;
    if (NULL == handler) {
        return -1;
    }
    desc = (SimpleSliderDesc *)handler;
    if (desc->puzzleData) {
        esMEMS_Mfree(0, desc->puzzleData);
    }
    esMEMS_Mfree(0, handler);
    return 0;
}

#define REGULAR_DRAW

int SimpleSliderDraw(void * handler, H_LYR layer, RECT * rect, int current)
{
    SimpleSliderDesc * desc;
    int i;
    int blocks;
    OFFSET locat;
    HTHEME bmpTheme;
    void * bmpData;
#ifndef REGULAR_DRAW
    HBLOCK backbg;
    HBLOCK foreg;
    FB fb;
#endif

    if (NULL == handler) {
        return -1;
    }
    desc = (SimpleSliderDesc *)handler;

    // To get the current display position
    current = (current > desc->cls.limit) ? desc->cls.limit : current;
    blocks = (current * desc->cls.bmp_nr) / desc->cls.limit;
    blocks = (blocks > desc->cls.bmp_nr) ? desc->cls.bmp_nr : blocks;

#ifndef REGULAR_DRAW
    GUI_LyrWinGetFB(layer, &fb);
    backbg = GUI_BlockCreateFrom(NULL, fb.size.width, fb.size.height, fb.addr[0], 0);
#endif
    GUI_LyrWinSel(layer);
    if (desc->drew_position < 0) {
        for (i = 1; i <= blocks; i++) {
            if (0 == getStartPoint(&locat, &desc->cls.start_point, desc->cls.direc, i, desc->cls.bmp_nr, &desc->cls.puzzle)) {
                bmpTheme = dsk_theme_open(desc->bmps[i - 1]);
                bmpData = dsk_theme_hdl2buf(bmpTheme);
#ifdef REGULAR_DRAW
                GUI_BMP_Draw(bmpData, locat.x + rect->x, locat.y + rect->y);
#else
                eLIBs_memcpy(desc->puzzleData, (void *)((int)bmpData + BITMAPHEADERSIZE), desc->puzzleDataSize);
                foreg = GUI_BlockCreateFrom(NULL, desc->cls.puzzle.width, desc->cls.puzzle.height, desc->puzzleData, 0);
                GUI_BlockBitBlit(backbg, locat.x + rect->x, locat.y + rect->y,
                                    foreg, NULL, BLITFLAG_COLORALPHA, NULL);
                GUI_BlockDelete(foreg);
#endif
                dsk_theme_close(bmpTheme);
                // LogMI("Simple Slider Piece x = %d, y = %d, bmp = %d", locat.x, locat.y, i - 1);
            }
        }
        for (; i <= desc->cls.bmp_nr; i++) {
            if (0 == getStartPoint(&locat, &desc->cls.start_point, desc->cls.direc, i, desc->cls.bmp_nr, &desc->cls.puzzle)) {
                bmpTheme = dsk_theme_open(desc->cls.bgbmp_piece);
                bmpData = dsk_theme_hdl2buf(bmpTheme);
#ifdef REGULAR_DRAW
                GUI_BMP_Draw(bmpData, locat.x + rect->x, locat.y + rect->y);
#else
                eLIBs_memcpy(desc->puzzleData, (void *)((int)bmpData + BITMAPHEADERSIZE), desc->puzzleDataSize);
                foreg = GUI_BlockCreateFrom(NULL, desc->cls.puzzle.width, desc->cls.puzzle.height, desc->puzzleData, 0);
                GUI_BlockBitBlit(backbg, locat.x + rect->x, locat.y + rect->y,
                                    foreg, NULL, BLITFLAG_COLORALPHA, NULL);
                GUI_BlockDelete(foreg);
#endif
                dsk_theme_close(bmpTheme);
                // LogMI("Simple Slider Piece x = %d, y = %d", locat.x, locat.y);
            }
        }
    } else if (desc->drew_position < blocks) {
        for (i = 1 + desc->drew_position; i <= blocks; i++) {
            if (0 == getStartPoint(&locat, &desc->cls.start_point, desc->cls.direc, i, desc->cls.bmp_nr, &desc->cls.puzzle)) {
                bmpTheme = dsk_theme_open(desc->bmps[i - 1]);
                bmpData = dsk_theme_hdl2buf(bmpTheme);
#ifdef REGULAR_DRAW
                GUI_BMP_Draw(bmpData, locat.x + rect->x, locat.y + rect->y);
#else
                eLIBs_memcpy(desc->puzzleData, (void *)((int)bmpData + BITMAPHEADERSIZE), desc->puzzleDataSize);
                foreg = GUI_BlockCreateFrom(NULL, desc->cls.puzzle.width, desc->cls.puzzle.height, desc->puzzleData, 0);
                GUI_BlockBitBlit(backbg, locat.x + rect->x, locat.y + rect->y,
                                    foreg, NULL, BLITFLAG_COLORALPHA, NULL);
                GUI_BlockDelete(foreg);
#endif
                dsk_theme_close(bmpTheme);
                // LogMI("Simple Slider Piece x = %d, y = %d, bmp = %d", locat.x, locat.y, i - 1);
            }
        }
    } else if (desc->drew_position > blocks) {
        for (i = desc->drew_position; i > blocks; i--) {
            if (0 == getStartPoint(&locat, &desc->cls.start_point, desc->cls.direc, i, desc->cls.bmp_nr, &desc->cls.puzzle)) {
                bmpTheme = dsk_theme_open(desc->cls.bgbmp_piece);
                bmpData = dsk_theme_hdl2buf(bmpTheme);
#ifdef REGULAR_DRAW
                GUI_BMP_Draw(bmpData, locat.x + rect->x, locat.y + rect->y);
#else
                eLIBs_memcpy(desc->puzzleData, (void *)((int)bmpData + BITMAPHEADERSIZE), desc->puzzleDataSize);
                foreg = GUI_BlockCreateFrom(NULL, desc->cls.puzzle.width, desc->cls.puzzle.height, desc->puzzleData, 0);
                GUI_BlockBitBlit(backbg, locat.x + rect->x, locat.y + rect->y,
                                    foreg, NULL, BLITFLAG_COLORALPHA, NULL);
                GUI_BlockDelete(foreg);
#endif
                dsk_theme_close(bmpTheme);
                // LogMI("Simple Slider Piece x = %d, y = %d", locat.x, locat.y);
            }
        }
    }
    desc->drew_position = blocks;
#ifndef REGULAR_DRAW
    GUI_BlockDelete(backbg);
#endif
    return 0;
}

int SimpleSliderRedraw(void * handler, H_LYR layer, RECT * rect)
{
    SimpleSliderDesc * desc;
    int position;

    if (NULL == handler) {
        return -1;
    }
    desc = (SimpleSliderDesc *)handler;

    position = desc->drew_position;
    desc->drew_position = -1;
    SimpleSliderDraw(handler, layer, rect, position);

    return 0;
}



