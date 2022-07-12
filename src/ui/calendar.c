/*
	Copyright (C) 2022 <alpheratz99@protonmail.com>

	This program is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License version 2 as published by the
	Free Software Foundation.

	This program is distributed in the hope that it will be useful, but WITHOUT ANY
	WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
	FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

	You should have received a copy of the GNU General Public License along with
	this program; if not, write to the Free Software Foundation, Inc., 59 Temple
	Place, Suite 330, Boston, MA 02111-1307 USA

*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#include "../base/bitmap.h"
#include "../base/font.h"
#include "../util/xmalloc.h"
#include "label.h"
#include "calendar.h"

static const char *month_names[] = {
	"Jan", "Feb", "Mar", "Apr", "May", "Jun",
	"Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};

static const int month_numdays[] = {
	31, 28, 31, 30, 31, 30,
	31, 31, 30, 31, 30, 31
};

static const char *
calendar_get_month_name(int month)
{
	return month_names[month];
}

static int
calendar_get_month_offset(int month, int year)
{
	struct tm tm;

	tm = localtime((const time_t[1]){ time(NULL) })[0];

	tm.tm_year = year - 1900;
	tm.tm_mon = month;
	tm.tm_mday = 1;

	mktime(&tm);

	return tm.tm_wday;
}

static int
calendar_is_leap_year(int year)
{
	return year % 400 == 0 || (year % 4 == 0 && year % 100 != 0);
}

static int
calendar_get_month_days(int month, int year)
{
	if (month != 1 || !calendar_is_leap_year(year)) {
		return month_numdays[month];
	}

	return 29;
}

extern struct calendar_style
calendar_style_from(struct font *font,
                    uint32_t foreground,
                    uint32_t background)
{
	struct calendar_style style;

	style.font = font;
	style.foreground = foreground;
	style.background = background;

	return style;
}

extern struct calendar *
calendar_create(struct calendar_style *style)
{
	struct calendar *calendar;

	calendar = xmalloc(sizeof(struct calendar));

	calendar->style = style;

	calendar_goto_current_month(calendar);

	return calendar;
}

extern void
calendar_goto_next_month(struct calendar *calendar)
{
	if (++calendar->month == 12) {
		calendar->month = 0;
		++calendar->year;
	}
}

extern void
calendar_goto_previous_month(struct calendar *calendar)
{
	if (--calendar->month == -1) {
		calendar->month = 11;
		if (--calendar->year == 1752) {
			calendar->year = 1753;
			calendar->month = 0;
		}
	}
}

extern void
calendar_goto_next_year(struct calendar *calendar)
{
	++calendar->year;
}

extern void
calendar_goto_previous_year(struct calendar *calendar)
{
	if (--calendar->year == 1752) {
		calendar->year = 1753;
	}
}

extern void
calendar_goto_current_month(struct calendar *calendar)
{
	const struct tm *now;

	now = localtime((const time_t[1]) { time(NULL) });
	calendar->year = now->tm_year + 1900;
	calendar->month = now->tm_mon;
}

extern void
calendar_render_onto(struct calendar *calendar, struct bitmap *bmp)
{
	/* clear the bitmap */
	bitmap_clear(bmp, calendar->style->background);

	/* render month and year */
	char month_and_year[32];
	int month_and_year_pos_x, month_and_year_pos_y;

	month_and_year_pos_x = (bmp->width - calendar->style->font->width * 21) / 2;
	month_and_year_pos_y = (bmp->height - calendar->style->font->height * 7) / 2;

	snprintf(
		month_and_year, sizeof(month_and_year), "%10s %-10d",
		calendar_get_month_name(calendar->month), calendar->year
	);

	label_render_onto(
		month_and_year, calendar->style->font,
		calendar->style->foreground, month_and_year_pos_x,
		month_and_year_pos_y, bmp
	);

	/* render day names */
	const char *day_names;
	int day_names_pos_x, day_names_pos_y;

	day_names_pos_x = month_and_year_pos_x;
	day_names_pos_y = month_and_year_pos_y + calendar->style->font->height;
	day_names = " Su Mo Tu We Th Fr Sa";

	label_render_onto(
		day_names, calendar->style->font, calendar->style->foreground,
		day_names_pos_x, day_names_pos_y, bmp
	);

	/* render days */
	int i;
	int numdays;
	int month_offset;
	int day_pos_x, day_pos_y;
	int day_max_pos_x;
	char day[32];
	struct tm now;

	numdays = calendar_get_month_days(calendar->month, calendar->year);
	month_offset = calendar_get_month_offset(calendar->month, calendar->year);
	day_pos_x = day_names_pos_x + month_offset * 3 * calendar->style->font->width;
	day_pos_y = day_names_pos_y + calendar->style->font->height;
	day_max_pos_x = day_names_pos_x + calendar->style->font->width * 21;
	now = localtime((const time_t[1]){ time(NULL) })[0];

	for (i = 0; i < numdays; ++i) {
		snprintf(day, sizeof(day), "%3d", i+1);

		if (now.tm_year == calendar->year - 1900 &&
		    now.tm_mon == calendar->month &&
		    now.tm_mday == i + 1)
		{
			bitmap_rect(
				bmp, day_pos_x + calendar->style->font->width * (i >= 9 ? 1 : 2),
				day_pos_y, calendar->style->font->width * (i >= 9 ? 2 : 1),
				calendar->style->font->height, calendar->style->foreground
			);

			label_render_onto(
				day, calendar->style->font, calendar->style->background,
				day_pos_x, day_pos_y, bmp
			);
		}
		else {
			label_render_onto(
				day, calendar->style->font, calendar->style->foreground,
				day_pos_x, day_pos_y, bmp
			);
		}

		day_pos_x += calendar->style->font->width * 3;

		if (day_pos_x == day_max_pos_x) {
			day_pos_x = day_names_pos_x;
			day_pos_y += calendar->style->font->height;
		}
	}
}

extern void
calendar_free(struct calendar *calendar)
{
	free(calendar);
}
