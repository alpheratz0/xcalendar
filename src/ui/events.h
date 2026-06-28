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

#ifndef __XCALENDAR_UI_EVENTS_H__
#define __XCALENDAR_UI_EVENTS_H__

#include <stdint.h>

#include "../base/bitmap.h"
#include "../base/font.h"

struct events;

extern const char *
events_default_path(void);

extern struct events *
events_load(const char *path);

extern int
events_on_day(const struct events *events, int month, int year, int day);

extern int
events_render_month_onto(const struct events *events, int month, int year,
                         struct font *font, uint32_t color,
                         int x, int y, struct bitmap *bmp);

extern void
events_free(struct events *events);

#endif
