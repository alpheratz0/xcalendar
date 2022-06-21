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

typedef struct calendar calendar_t;
typedef struct calendar_style calendar_style_t;

struct calendar_style {
	font_t *font;
	uint32_t foreground;
	uint32_t background;
};

struct calendar {
	calendar_style_t *style;
	int month, year;
};

extern calendar_style_t
calendar_style_from(font_t *font, uint32_t foreground, uint32_t background);

extern calendar_t *
calendar_create(calendar_style_t *style);

extern void
calendar_goto_next_month(calendar_t *calendar);

extern void
calendar_goto_previous_month(calendar_t *calendar);

extern void
calendar_goto_next_year(calendar_t *calendar);

extern void
calendar_goto_previous_year(calendar_t *calendar);

extern void
calendar_goto_current_month(calendar_t *calendar);

extern void
calendar_render_onto(calendar_t *calendar, bitmap_t *bmp);

extern void
calendar_free(calendar_t *calendar);

#endif
