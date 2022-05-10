#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../base/bitmap.h"
#include "../base/font.h"
#include "../util/numdef.h"
#include "../util/debug.h"
#include "label.h"
#include "calendar.h"

static const char *month_names[] = {
	"Jan", "Feb", "Mar",
	"Apr", "May", "Jun",
	"Jul", "Aug", "Sep",
	"Oct", "Nov", "Dec"
};

static const i32 month_numdays[] = {
	31, 28, 31,
	30, 31, 30,
	31, 31, 30,
	31, 30, 31
};

static const char *
calendar_get_month_name(i32 month) {
	return month_names[month];
}

static i32
calendar_get_month_days(i32 month, i32 year) {
	i32 numdays;
	numdays = month_numdays[month];

	if (month == 1) {
		if (year % 400 == 0 || (year % 4 == 0 && year % 100 != 0)) {
			++numdays;
		}
	}

	return numdays;
}

extern calendar_style_t
calendar_style_from(u32 text_color, u32 background_color) {
	calendar_style_t style;
	style.text_color = text_color;
	style.background_color = background_color;
	return style;
}

extern calendar_t *
calendar_create(font_t *font, calendar_style_t *style) {
	calendar_t *calendar;

	if ((calendar = malloc(sizeof(calendar_t)))) {
		calendar->font = font;
		calendar->style = style;
		calendar_goto_current_month(calendar);

		return calendar;
	}

	die("error while calling malloc, no memory available");

	return (void *)(0);
}

extern void
calendar_goto_next_month(calendar_t *calendar) {
	if (++calendar->month == 12) {
		calendar->month = 0;
		++calendar->year;
	}
}

extern void
calendar_goto_previous_month(calendar_t *calendar) {
	if (--calendar->month == -1) {
		calendar->month = 11;
		if (--calendar->year == 1752) {
			calendar->year = 1753;
			calendar->month = 0;
		}
	}
}

extern void
calendar_goto_next_year(calendar_t *calendar) {
	++calendar->year;
}

extern void
calendar_goto_previous_year(calendar_t *calendar) {
	if (--calendar->year == 1752) {
		calendar->year = 1753;
	}
}

extern void
calendar_goto_current_month(calendar_t *calendar) {
	struct tm *tm;
	tm = localtime((const time_t[1]) { time(NULL) });
	calendar->year = tm->tm_year + 1900;
	calendar->month = tm->tm_mon;
}

extern void
calendar_render_onto(calendar_t *calendar, bitmap_t *bmp) {
	struct tm now, current;
	u32 width, height;
	u32 xstart, xpos, ypos;
	i32 numdays;
	char buff[50];

	now = *(localtime((const time_t[1]) { time(NULL) }));
	current = *(localtime((const time_t[1]) { time(NULL) }));

	current.tm_year = calendar->year - 1900;
	current.tm_mon	= calendar->month;
	current.tm_mday = 1;

	mktime(&current);

	width = calendar->font->width * 21;
	height = calendar->font->height * 7;

	xpos = xstart = (bmp->width - width) / 2;
	ypos = (bmp->height - height) / 2;

	bitmap_clear(bmp, calendar->style->background_color);

	snprintf(buff, sizeof(buff), "%10s %-10d", calendar_get_month_name(calendar->month), calendar->year);
	label_render_onto(bmp, calendar->font, calendar->style->text_color, buff, xpos, ypos);
	ypos += calendar->font->height;

	label_render_onto(bmp, calendar->font, calendar->style->text_color, " Su Mo Tu We Th Fr Sa", xpos, ypos);
	ypos += calendar->font->height;

	xpos = xstart + calendar->font->width * current.tm_wday * 3;

	numdays = calendar_get_month_days(calendar->month, calendar->year);

	for (i32 day = 0; day < numdays; ++day) {
		i32 spaces = day >= 9 ? 1 : 2;
		i32 is_today = current.tm_year == now.tm_year && current.tm_mon == now.tm_mon && (day + 1) == now.tm_mday;

		xpos += spaces * calendar->font->width;
		snprintf(buff, sizeof(buff), "%d", day + 1);

		if (is_today) {
			bitmap_rect(bmp, xpos, ypos, calendar->font->width * (3 - spaces), calendar->font->height, calendar->style->text_color);
			label_render_onto(bmp, calendar->font, calendar->style->background_color, buff, xpos, ypos);
		}
		else {
			label_render_onto(bmp, calendar->font, calendar->style->text_color, buff, xpos, ypos);
		}

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
