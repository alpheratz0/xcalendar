#ifndef __XCALENDAR_CALENDAR_H__
#define __XCALENDAR_CALENDAR_H__

#include "font.h"
#include "bitmap.h"
#include "numdef.h"

struct calendar {
	bitmap_t     *bitmap;
};

typedef struct calendar calendar_t;

extern calendar_t *
calendar_from_today(font_t *ft, u32 foreground, u32 background);

extern void
calendar_free(calendar_t *cal);

#endif
