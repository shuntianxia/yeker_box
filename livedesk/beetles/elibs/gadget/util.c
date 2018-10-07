
#include "gadget_i.h"

#ifdef THIS_DEBUG
#undef THIS_DEBUG
#endif

#ifdef GG_DEBUG
//#define THIS_DEBUG
#endif

int GG_FillSize(SIZE * sz, __u32 width, __u32 height)
{
    sz->width = width;
    sz->height = height;
    return 0;
}

int GG_FillRect(RECT * rect, __s32 x, __s32 y, __u32 width, __u32 height)
{
    rect->x = x;
    rect->y = y;
    rect->width = width;
    rect->height = height;
    return 0;
}

int GG_FillOffset(OFFSET * oset, __s32 x, __s32 y)
{
    oset->x = x;
    oset->y = y;
    return 0;
}

int GG_CopySize(SIZE * dest, SIZE * src)
{
    dest->width = src->width;
    dest->height = src->height;
    return 0;
}

int GG_CopyRect(RECT * dest, RECT * src)
{
    dest->x = src->x;
    dest->y = src->y;
    dest->width = src->width;
    dest->height = src->height;
    return 0;
}

// Include the border
int GG_PointInRect(RECT * rect, __s32 x, __s32 y)
{
    if ((x >= rect->x && x <= (rect->x + rect->width)) &&
        (y >= rect->y && y <= (rect->y + rect->height))) {
        return 0;
    }
    return -1;
}

#ifdef THIS_DEBUG
static gg_size malloc_debug = 0;
#endif

void * GG_malloc(gg_size size)
{
    gg_size * p;
    if (0 == size) {
        return GG_NULL;
    }
    p = (gg_size *)esMEMS_Balloc(size + sizeof(gg_size));
    if (p) {
        *p = size + sizeof(gg_size);
#ifdef THIS_DEBUG
        malloc_debug += *p;
        LogMI("Malloc (%d,%08x,%d)", *p, (int)p, malloc_debug);
#endif
        p = (gg_size *)(((gg_size)p) + sizeof(gg_size));
        return p;
    } else {
        LogE("Malloc failed");
        return p;
    }
}

gg_int32 GG_free(void * heap)
{
    if (heap) {
        gg_size * p;
        gg_size size;
        p = (gg_size *)(((gg_size)heap) - sizeof(gg_size));
        size = *p;
#ifdef THIS_DEBUG
        malloc_debug -= *p;
        LogMI("Free (%d,%08x,%d)", size, (int)p, malloc_debug);
#endif
        esMEMS_Bfree(p, size);
    }
    return 0;
}

gg_char * GG_DupStr(gg_char * src)
{
    gg_size len;
    gg_char * dst;

    if (src) {
        len = GG_strlen(src);
        dst = GG_malloc(len + 2);
        if (dst) {
            GG_strcpy(dst, src);
            return dst;
        }
    }
    return GG_NULL;
}

int GG_GetVersion(void)
{
    return VERSION_MERGE(VERSION_MAJOR, VERSION_MINOR, VERSION_BUILD);
}


/*#ifndef BYTE_ORDER
#error BYTE_ORDER is not defined
#endif
#if BYTE_ORDER == LITTLE_ENDIAN*/
/*-----------------------------------------------------------------------------------*/
gg_uint16 htons(gg_uint16 n)
{
  return ((n & 0xff) << 8) | ((n & 0xff00) >> 8);
}
/*-----------------------------------------------------------------------------------*/
gg_uint16 ntohs(gg_uint16 n)
{
  return htons(n);
}
/*-----------------------------------------------------------------------------------*/
gg_uint32 htonl(gg_uint32 n)
{
  return ((n & 0xff) << 24) |
    ((n & 0xff00) << 8) |
    ((n & 0xff0000) >> 8) |
    ((n & 0xff000000) >> 24);
}
/*-----------------------------------------------------------------------------------*/
gg_uint32 ntohl(gg_uint32 n)
{
  return htonl(n);
}
/*-----------------------------------------------------------------------------------*/
//#endif /* BYTE_ORDER == LITTLE_ENDIAN */



