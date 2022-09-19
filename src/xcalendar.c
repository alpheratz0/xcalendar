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

	 ________________________
	(      December 2027     )
	(  Su Mo Tu We Th Fr Sa  )
	(            1  2  3  4  )
	(   5  6  7  8  9 10 11  )
	(  12 13 14 15 16 17 18  )
	(  19 20 21 22 23 24 25  )
	(  26 27 28 29 30 31     )
	 ------------------------
	    o               ,-----._
	  .  o         .  ,'        `-.__,------._
	 //   o      __\\'                        `-.
	((    _____-'___))                           |
	 `:='/     (alf_/                            |
	 `.=|      |='                               |
	    |)   O |                                  \
	    |      |                               /\  \
	    |     /                          .    /  \  \
	    |    .-..__            ___   .--' \  |\   \  |
	   |o o  |     ``--.___.  /   `-'      \  \\   \ |
	    `--''        '  .' / /             |  | |   | \
	                 |  | / /              |  | |   mmm
	                 |  ||  |              | /| |
	                 ( .' \ \              || | |
	                 | |   \ \            // / /
	                 | |    \ \          || |_|
	                /  |    |_/         /_|
	               /__/


*/

#include <stdint.h>
#include <string.h>
#include <xkbcommon/xkbcommon-keysyms.h>

#include "base/font.h"
#include "ui/calendar.h"
#include "util/debug.h"
#include "x11/window.h"

static struct window *window;
static struct calendar *calendar;

static void
key_press_callback(xcb_keysym_t key)
{
	switch (key) {
		case XKB_KEY_Escape:
		case XKB_KEY_q:
			window_loop_end(window);
			return;
		case XKB_KEY_h:
			calendar_goto_previous_month(calendar);
			break;
		case XKB_KEY_l:
			calendar_goto_next_month(calendar);
			break;
		case XKB_KEY_j:
			calendar_goto_previous_year(calendar);
			break;
		case XKB_KEY_k:
			calendar_goto_next_year(calendar);
			break;
		case XKB_KEY_c:
			calendar_goto_current_month(calendar);
			break;
	}

	calendar_render_onto(calendar, window->bmp);
	window_force_redraw(window);
}

static void
usage(void)
{
	puts("usage: xcalendar [-hv]");
	exit(0);
}

static void
version(void)
{
	puts("xcalendar version "VERSION);
	exit(0);
}

int
main(int argc, char **argv)
{
	struct font *font;
	struct calendar_style style;

	while (++argv, --argc > 0) {
		if ((*argv)[0] == '-' && (*argv)[1] != '\0' && (*argv)[2] == '\0') {
			switch ((*argv)[1]) {
				case 'h': usage(); break;
				case 'v': version(); break;
				default: die("invalid option %s", *argv); break;
			}
		} else {
			die("unexpected argument: %s", *argv);
		}
	}

	window = window_create("xcalendar", "xcalendar");
	font = font_load("Iosevka", 40);
	style = calendar_style_from(font, 0xffffff, 0x000000);
	calendar = calendar_create(&style);

	calendar_render_onto(calendar, window->bmp);
	window_set_key_press_callback(window, key_press_callback);
	window_loop_start(window);

	font_unload(font);
	calendar_free(calendar);
	window_free(window);

	return 0;
}
