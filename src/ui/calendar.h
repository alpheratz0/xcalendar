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

#ifndef __XCALENDAR_UI_CALENDAR_H__
#define __XCALENDAR_UI_CALENDAR_H__

#include <stdint.h>

#include "../base/font.h"
#include "../base/bitmap.h"

struct calendar_style {
	struct font *font;
	uint32_t foreground;
	uint32_t background;
};

struct calendar {
	struct calendar_style *style;
	int month, year;
};

extern struct calendar_style
calendar_style_from(struct font *font,
                    uint32_t foreground,
                    uint32_t background);

extern struct calendar *
calendar_create(struct calendar_style *style);

extern void
calendar_goto_next_month(struct calendar *calendar);

extern void
calendar_goto_previous_month(struct calendar *calendar);

extern void
calendar_goto_next_year(struct calendar *calendar);

extern void
calendar_goto_previous_year(struct calendar *calendar);

extern void
calendar_goto_current_month(struct calendar *calendar);

extern void
calendar_render_onto(struct calendar *calendar, struct bitmap *bmp);

extern void
calendar_free(struct calendar *calendar);

#endif
