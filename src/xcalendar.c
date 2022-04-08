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

#include <stdbool.h>
#include <string.h>

#include "base/font.h"
#include "ui/calendar.h"
#include "util/debug.h"
#include "x11/keys.h"
#include "x11/window.h"

static window_t *window;
static calendar_t *calendar;

static bool
match_opt(const char *in, const char *sh, const char *lo) {
	return (strcmp(in, sh) == 0) ||
		   (strcmp(in, lo) == 0);
}

static void
key_press_callback(u32 key) {
	switch (key) {
		case KEY_ESCAPE:
			window_loop_end(window);
			return;
		case KEY_H:
			calendar_goto_previous_month(calendar);
			break;
		case KEY_L:
			calendar_goto_next_month(calendar);
			break;
		case KEY_J:
			calendar_goto_previous_year(calendar);
			break;
		case KEY_K:
			calendar_goto_next_year(calendar);
			break;
		case KEY_C:
			calendar_goto_current_month(calendar);
			break;
		default:
			return;
	}

	calendar_render_onto(calendar, window->bmp);
	window_force_redraw(window);
}

static void
usage(void) {
	puts("Usage: xcalendar [ -hkv ]");
	puts("Options are:");
	puts("     -h | --help                    display this message and exit");
	puts("     -k | --keybindings             display the keybindings");
	puts("     -v | --version                 display the program version");
	exit(0);
}

static void
keybindings(void) {
	puts("Keybindings are:");
	puts("h/l: see the previous/next month");
	puts("j/k: see the previous/next year");
	puts("c: see the current month");
	puts("esc: exit");
	exit(0);
}

static void
version(void) {
	puts("xcalendar version "VERSION);
	exit(0);
}

int
main(int argc, char **argv) {
	/* skip program name */
	--argc; ++argv;

	if (argc > 0) {
		if (match_opt(*argv, "-k", "--keybindings")) keybindings();
		else if (match_opt(*argv, "-h", "--help")) usage();
		else if (match_opt(*argv, "-v", "--version")) version();
		else dief("invalid option %s", *argv);
	}

	font_t *font;
	calendar_style_t style;

	window = window_create("xcalendar", "xcalendar");
	font = font_load("Iosevka", 40);
	style = calendar_style_from(0xffffff, 0x000000);
	calendar = calendar_create(font, &style);

	calendar_render_onto(calendar, window->bmp);
	window_set_key_press_callback(window, key_press_callback);
	window_loop_start(window);

	font_unload(font);
	calendar_free(calendar);
	window_free(window);

	return 0;
}
