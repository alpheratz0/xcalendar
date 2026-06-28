/*
	Copyright (C) 2022-2026 <alpheratz99@protonmail.com>

	This program is free software; you can redistribute it and/or modify it
	under the terms of the GNU General Public License version 2 as published by
	the Free Software Foundation.

	This program is distributed in the hope that it will be useful, but WITHOUT
	ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
	FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
	more details.

	You should have received a copy of the GNU General Public License along
	with this program; if not, write to the Free Software Foundation, Inc., 59
	Temple Place, Suite 330, Boston, MA 02111-1307 USA

*/

#include <langinfo.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#include "../base/bitmap.h"
#include "../base/font.h"
#include "../util/debug.h"
#include "../util/xmalloc.h"
#include "label.h"
#include "events.h"
#include "calendar.h"

/* color of the dot marking days that have events */
#define CALENDAR_EVENT_DOT_COLOR 0xff5555

/* events panel geometry: the gap between the grid and the panel (in calendar
   columns) and the panel's reserved width (in events-font columns). Both are
   used to center the calendar and the panel together as a single block. */
#define CALENDAR_PANEL_GAP 3
#define CALENDAR_PANEL_WIDTH 26

static inline int
get_first_weekday(void)
{
	const char *const s = nl_langinfo(_NL_TIME_FIRST_WEEKDAY);

	if (s && *s >= 1 && *s <= 7)
		return ((int)*s) - 1;

	return 0;
}

static const char *
cp_offset(const char *str, size_t cp)
{
	while (cp && *str++)
		if (!(*str & 0x80) || ((*str & 0xC0) == 0xC0))
			--cp;
	return str;
}

static const char *
calendar_get_week_fmt(void)
{
	static char week_fmt[128] = {0};
	static int days[] = {ABDAY_1, ABDAY_2, ABDAY_3, ABDAY_4, ABDAY_5, ABDAY_6, ABDAY_7};
	if (week_fmt[0] == 0) {
		int first_weekday = get_first_weekday();
		int off = 0;
		for (size_t i = 0; i < sizeof(days) / sizeof(days[0]); ++i) {
			const char *day_fmt = nl_langinfo(days[((i+first_weekday)%7+7)%7]);
			const char *day_fmt_end = cp_offset(day_fmt, 2);

			week_fmt[off++] = ' ';
			strncpy(&week_fmt[off], day_fmt, day_fmt_end - day_fmt);
			off += day_fmt_end - day_fmt;
		}
	}
	return week_fmt;
}

static const char *
calendar_get_month_name(int month)
{
	static const char *month_names[12] = {NULL};
	static int months[sizeof(month_names) / sizeof(month_names[0])] = {
		MON_1, MON_2, MON_3,
		MON_4, MON_5, MON_6,
		MON_7, MON_8, MON_9,
		MON_10, MON_11, MON_12
	};

	if (month_names[0] == NULL) {
		for (size_t i = 0; i < sizeof(months) / sizeof(months[0]); ++i)
			month_names[i] = nl_langinfo(months[i]);
	}

	return month_names[month];
}

static struct tm
calendar_get_current_time(void)
{
	time_t t;
	struct tm *tmb;

	t = time(NULL);

	if (t == (time_t)(-1))
		die("time() failed: ", strerror(errno));

	tmb = localtime(&t);

	if (NULL == tmb)
		die("can't convert time");

	return *tmb;
}

static int
calendar_get_month_offset(int month, int year)
{
	struct tm tm;

	tm = calendar_get_current_time();

	tm.tm_year = year - 1900;
	tm.tm_mon = month;
	tm.tm_mday = 1;

	mktime(&tm);

	return (((tm.tm_wday - get_first_weekday()) % 7) + 7) % 7;
}

static int
calendar_is_leap_year(int year)
{
	return year % 400 == 0 || (year % 4 == 0 && year % 100 != 0);
}

static int
calendar_get_month_days(int month, int year)
{
	static const int month_numdays[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	if (month != 1 || !calendar_is_leap_year(year))
		return month_numdays[month];
	return 29;
}

static void
calendar_draw_indicator(struct bitmap *bmp, int cx, int cy, int r, uint32_t color)
{
	int dx, dy;

	for (dy = -r; dy <= r; ++dy)
		for (dx = -r; dx <= r; ++dx)
			if (dx * dx + dy * dy <= r * r)
				bitmap_set(bmp, cx + dx, cy + dy, color);
}

/* top-left position of the month/year title; the rest of the layout derives
   from it. The calendar grid and the events panel are centered together as a
   single block so the whole thing stays balanced on screen. */
static void
calendar_base_pos(const struct calendar *calendar, const struct bitmap *bmp,
                  int *x, int *y)
{
	const struct font *font = calendar->style->font;
	const struct font *events_font = calendar->style->events_font;
	int block_width;

	block_width = (int)font->width * (21 + CALENDAR_PANEL_GAP) +
		(int)events_font->width * CALENDAR_PANEL_WIDTH;

	*x = ((int)bmp->width - block_width) / 2;
	*y = ((int)bmp->height - (int)font->height * 7 - 20) / 2;
}

extern struct calendar_style
calendar_style_from(struct font *font, uint32_t foreground, uint32_t background)
{
	struct calendar_style style;

	style.font = font;
	style.events_font = font; /* overridden by the caller; never left unset */
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
	calendar->events = events_load(events_default_path());

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
	if (--calendar->year == 1752)
		calendar->year = 1753;
}

extern void
calendar_goto_current_month(struct calendar *calendar)
{
	struct tm now;

	now = calendar_get_current_time();
	calendar->year = now.tm_year + 1900;
	calendar->month = now.tm_mon;
}

extern void
calendar_render_onto(struct calendar *calendar, struct bitmap *bmp)
{
	/* clear the bitmap */
	bitmap_clear(bmp, calendar->style->background);

	/* render month and year */
	char month_and_year[50];
	int month_and_year_pos_x, month_and_year_pos_y;

	calendar_base_pos(calendar, bmp, &month_and_year_pos_x, &month_and_year_pos_y);

	snprintf(month_and_year, sizeof(month_and_year), "%s %d", calendar_get_month_name(calendar->month), calendar->year);

	label_render_onto(
		month_and_year, calendar->style->font,
		0xffff00, month_and_year_pos_x + ((float)(22 - strlen(month_and_year)) / 2) * calendar->style->font->width,
		month_and_year_pos_y, bmp
	);

	/* render day names */
	const char *day_names;
	int day_names_pos_x, day_names_pos_y;

	day_names_pos_x = month_and_year_pos_x;
	day_names_pos_y = month_and_year_pos_y + calendar->style->font->height + 20;
	day_names = calendar_get_week_fmt();

	label_render_onto(
		day_names, calendar->style->font, 0xffffff,
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
	now = calendar_get_current_time();

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

		/* mark days that have events with a small dot at the top-right */
		if (events_on_day(calendar->events, calendar->month, calendar->year, i + 1) > 0) {
			int r, cx, cy;

			r = (int)calendar->style->font->height / 12;
			if (r < 3)
				r = 3;

			cx = day_pos_x + (int)calendar->style->font->width * 3 - r - 1;
			cy = day_pos_y + r + 1;

			calendar_draw_indicator(bmp, cx, cy, r, CALENDAR_EVENT_DOT_COLOR);
		}

		day_pos_x += calendar->style->font->width * 3;

		if (day_pos_x == day_max_pos_x) {
			day_pos_x = day_names_pos_x;
			day_pos_y += calendar->style->font->height;
		}
	}

	/* render the viewed month's events in a smaller font to the right; the
	   first event lines up with the weekday names */
	struct font *events_font;
	int panel_x, list_y;

	events_font = calendar->style->events_font;
	panel_x = month_and_year_pos_x +
		(int)calendar->style->font->width * (21 + CALENDAR_PANEL_GAP);
	/* drop the list so the first event's baseline matches the weekday names
	   (a glyph's baseline sits font->size below its top) */
	list_y = day_names_pos_y +
		(int)calendar->style->font->size - (int)events_font->size;

	if (events_render_month_onto(calendar->events, calendar->month,
			calendar->year, events_font, calendar->style->foreground,
			panel_x, list_y, bmp) == 0) {
		label_render_onto("(none)", events_font, calendar->style->foreground,
			panel_x, list_y, bmp);
	}
}

extern void
calendar_free(struct calendar *calendar)
{
	events_free(calendar->events);
	free(calendar);
}
