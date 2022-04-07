#include <stdio.h>
#include <stdlib.h>

#include "../base/bitmap.h"
#include "../base/font.h"
#include "../base/dateinfo.h"
#include "../util/numdef.h"
#include "../util/debug.h"
#include "label.h"
#include "calendar.h"

extern calendar_style_t
calendar_style_from(u32 text_color, u32 current_day_background_color) {
	calendar_style_t style;
	style.text_color = text_color;
	style.current_day_background_color = current_day_background_color;
	return style;
}

extern calendar_t *
calendar_create(font_t *font, dateinfo_t *dateinfo, calendar_style_t *style) {
	calendar_t *calendar;

	if ((calendar = malloc(sizeof(calendar_t)))) {
		calendar->font = font;
		calendar->dateinfo = dateinfo;
		calendar->style = style;

		return calendar;
	}

	die("error while calling malloc, no memory available");

	return (void *)(0);
}

extern void
calendar_render_onto(calendar_t *calendar, bitmap_t *bmp) {
	char buff[3];
	u32 width = calendar->font->width * 21;
	u32 height = calendar->font->height * 7;
	u32 ypos = 0;
	u32 xpos = 0;
	u32 xstart = (bmp->width - width) / 2;
	u32 ystart = (bmp->height - height) / 2;

	/* render month name */
	xpos = xstart + (width - (strlen(calendar->dateinfo->month_name) - 1) * calendar->font->width) / 2;
	ypos = ystart;

	label_render_onto(bmp, calendar->font, calendar->style->text_color, calendar->dateinfo->month_name, xpos, ypos);
	ypos += calendar->font->height;

	/* render day names */
	xpos = xstart;
	label_render_onto(bmp, calendar->font, calendar->style->text_color, " Su Mo Tu We Th Fr Sa", xpos, ypos);
	ypos += calendar->font->height;

	/* render day numbers */
	xpos = xstart + calendar->font->width * calendar->dateinfo->firstday_weekday * 3;

	for (u32 day = 1; day <= calendar->dateinfo->num_days_in_month; ++day) {
		u8 spaces = day > 9 ? 1 : 2;
		u32 fg = day == calendar->dateinfo->day ? calendar->style->current_day_background_color : calendar->style->text_color;
		xpos += spaces * calendar->font->width;
		snprintf(buff, 3, "%d", day);

		if (calendar->dateinfo->day == day) {
			bitmap_rect(bmp, xpos, ypos, calendar->font->width * (3 - spaces), calendar->font->height, calendar->style->text_color);
		}

		label_render_onto(bmp, calendar->font, fg, buff, xpos, ypos);
		xpos += (3 - spaces) * calendar->font->width;
		if (xpos == (xstart + width)) {
			xpos = xstart;
			ypos += calendar->font->height;
		}
	}
}

extern void
calendar_free(calendar_t *calendar) {
	free(calendar);
}
