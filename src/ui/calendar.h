#ifndef __XCALENDAR_UI_CALENDAR_H__
#define __XCALENDAR_UI_CALENDAR_H__

#include "../base/font.h"
#include "../base/bitmap.h"
#include "../base/dateinfo.h"
#include "../util/numdef.h"

typedef struct calendar calendar_t;
typedef struct calendar_style calendar_style_t;

struct calendar_style {
	u32 text_color;
	u32 current_day_background_color;
};

struct calendar {
	font_t *font;
	dateinfo_t *dateinfo;
	calendar_style_t *style;
};

extern calendar_style_t
calendar_style_from(u32 text_color, u32 current_day_background_color);

extern calendar_t *
calendar_create(font_t *font, dateinfo_t *dateinfo, calendar_style_t *style);

extern void
calendar_render_onto(calendar_t *calendar, bitmap_t *bmp);

extern void
calendar_free(calendar_t *calendar);

#endif
