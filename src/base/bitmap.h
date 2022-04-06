#ifndef __XCALENDAR_BITMAP_H__
#define __XCALENDAR_BITMAP_H__

#include "../util/numdef.h"

struct bitmap {
	u32     *px;
	u32     width;
	u32     height;
};

typedef struct bitmap bitmap_t;

extern bitmap_t *
bitmap_alloc(u32 width, u32 height, u32 color);

extern void
bitmap_set(bitmap_t *bmp, u32 x, u32 y, u32 color);

extern void
bitmap_set_safe(bitmap_t *bmp, u32 x, u32 y, u32 color);

extern void
bitmap_rect(bitmap_t *bmp, u32 x, u32 y, u32 width, u32 height, u32 color);

extern void
bitmap_free(bitmap_t *bmp);

#endif
