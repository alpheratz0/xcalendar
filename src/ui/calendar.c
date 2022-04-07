#include <stdio.h>
#include <stdlib.h>

#include "../base/bitmap.h"
#include "../base/font.h"
#include "../base/dateinfo.h"
#include "../util/numdef.h"
#include "../util/debug.h"
#include "label.h"
#include "calendar.h"

extern calendar_t *
calendar_from_today(font_t *ft, u32 foreground, u32 background) {
	calendar_t *cal;
	dateinfo_t di;

	if (!(cal = malloc(sizeof(calendar_t)))) {
		die("error while calling malloc, no memory available");
	}

	di = dateinfo_from_today();

	char buff[3];
	u32 width = ft->width * 21;
	u32 height = ft->height * 7;
	u32 ypos = 0;
	u32 xpos = 0;

	cal->bitmap = bitmap_create(width, height, background);

	/* render month name */
	xpos = (width - (strlen(di.month_name) - 1) * ft->width) / 2;
	label_render_onto(cal->bitmap, ft, foreground, di.month_name, xpos, ypos);
	ypos += ft->height;

	/* render day names */
	xpos = 0;
	label_render_onto(cal->bitmap, ft, foreground, " Su Mo Tu We Th Fr Sa", xpos, ypos);
	ypos += ft->height;

	/* render day numbers */
	xpos = ft->width * di.firstday_weekday * 3;

	for (u32 day = 1; day <= di.num_days_in_month; ++day) {
		u8 spaces = day > 9 ? 1 : 2;
		u32 fg = day == di.day ? background : foreground;
		xpos += spaces * ft->width;
		snprintf(buff, 3, "%d", day);

		if (di.day == day) {
			bitmap_rect(cal->bitmap, xpos, ypos, ft->width * (3 - spaces), ft->height, foreground);
		}

		label_render_onto(cal->bitmap, ft, fg, buff, xpos, ypos);
		xpos += (3 - spaces) * ft->width;
		if (xpos == cal->bitmap->width) {
			xpos = 0;
			ypos += ft->height;
		}
	}

	return cal;
}

extern void
calendar_free(calendar_t *cal) {
	bitmap_free(cal->bitmap);
	free(cal);
}
