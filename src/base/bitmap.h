#ifndef __XCALENDAR_BASE_BITMAP_H__
#define __XCALENDAR_BASE_BITMAP_H__

#include "../util/numdef.h"

typedef struct bitmap bitmap_t;

struct bitmap {
	u32 *px;
	u32 width;
	u32 height;
};

extern bitmap_t *
bitmap_create(u32 width, u32 height, u32 color);

extern void
bitmap_set(bitmap_t *bmp, u32 x, u32 y, u32 color);

extern void
bitmap_set_unsafe(bitmap_t *bmp, u32 x, u32 y, u32 color);

extern void
bitmap_rect(bitmap_t *bmp, u32 x, u32 y, u32 width, u32 height, u32 color);

extern void
bitmap_free(bitmap_t *bmp);

#endif