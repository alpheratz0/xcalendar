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
#include "x11/window.h"

static bool
match_opt(const char *in, const char *sh, const char *lo) {
	return (strcmp(in, sh) == 0) ||
		   (strcmp(in, lo) == 0);
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
	calendar_t *calendar;
	window_t *window;

	font = font_load("Iosevka", 40);
	calendar = calendar_from_today(font, 0xffffff, 0x000000);
	window = window_init("xcalendar", "xcalendar", 0x000000);

	window_create_image(window, calendar->bitmap);
	window_loop(window);

	font_unload(font);
	calendar_free(calendar);
	window_free(window);

	return 0;
}
