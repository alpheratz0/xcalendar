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

#include "font.h"
#include "calendar.h"
#include "config.h"
#include "window.h"

int
main(void) {
	font_t *ft = font_load(font_family, font_size);
	calendar_t *cal = calendar_from_today(ft, foreground, background);

	window_t *wnd = window_init("xcalendar", "xcalendar", background);
	window_create_image(wnd, cal->bitmap);
	window_loop(wnd);

	font_free(ft);
	calendar_free(cal);
	window_free(wnd);
	return 0;
}
