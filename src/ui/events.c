/*
	Copyright (C) 2026 <alpheratz99@protonmail.com>

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

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <calrepeat.h>

#include "../base/bitmap.h"
#include "../base/font.h"
#include "../util/xmalloc.h"
#include "label.h"
#include "events.h"

struct events {
	cr_calendar cal;
};

static int
events_is_leap_year(int year)
{
	return year % 400 == 0 || (year % 4 == 0 && year % 100 != 0);
}

static int
events_month_days(int month, int year)
{
	static const int numdays[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	if (month == 1 && events_is_leap_year(year))
		return 29;
	return numdays[month];
}

extern const char *
events_default_path(void)
{
	static char buf[4096];
	const char *env, *home;

	env = getenv("XCALENDAR_CALENDAR");
	if (env != NULL && env[0] != '\0')
		return env;

#ifdef CALENDAR_PATH
	if (CALENDAR_PATH[0] == '/')
		return CALENDAR_PATH;

	home = getenv("HOME");
	if (home == NULL || home[0] == '\0')
		return CALENDAR_PATH;

	snprintf(buf, sizeof(buf), "%s/%s", home, CALENDAR_PATH);
	return buf;
#else
	(void)buf;
	(void)home;
	return NULL;
#endif
}

extern struct events *
events_load(const char *path)
{
	struct events *events;
	cr_calendar cal;

	if (path == NULL || path[0] == '\0')
		return NULL;

	memset(&cal, 0, sizeof(cal));

	if (cr_parse_file(path, &cal, NULL, 0) != 0) {
		cr_calendar_free(&cal);
		return NULL;
	}

	events = xmalloc(sizeof(struct events));
	events->cal = cal;

	return events;
}

extern int
events_on_day(const struct events *events, int month, int year, int day)
{
	cr_date date;
	size_t i;
	int count;

	if (events == NULL)
		return 0;

	date.year = year;
	date.month = month + 1; /* calrepeat months are 1-12 */
	date.day = day;

	for (i = 0, count = 0; i < events->cal.count; ++i)
		if (cr_event_matches(&events->cal.events[i], date))
			++count;

	return count;
}

extern int
events_render_month_onto(const struct events *events, int month, int year,
                         struct font *font, uint32_t color,
                         int x, int y, struct bitmap *bmp)
{
	int day, numdays, count;
	size_t i;
	char line[256];

	if (events == NULL)
		return 0;

	numdays = events_month_days(month, year);

	for (day = 1, count = 0; day <= numdays; ++day) {
		cr_date date;

		date.year = year;
		date.month = month + 1; /* calrepeat months are 1-12 */
		date.day = day;

		for (i = 0; i < events->cal.count; ++i) {
			const cr_event *ev = &events->cal.events[i];

			if (!cr_event_matches(ev, date))
				continue;

			/* stop once we run out of vertical room */
			if (y < 0 || (uint32_t)(y + (int)font->height) > bmp->height)
				return count;

			snprintf(line, sizeof(line), "%2d  %s", day,
			         cr_event_label(ev));

			label_render_onto(line, font, color, x, y, bmp);
			y += font->height;
			++count;
		}
	}

	return count;
}

extern void
events_free(struct events *events)
{
	if (events == NULL)
		return;
	cr_calendar_free(&events->cal);
	free(events);
}
