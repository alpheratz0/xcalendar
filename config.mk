# Copyright (C) 2022-2026 <alpheratz99@protonmail.com>
# This program is free software.

VERSION = 0.4.0

PREFIX = /usr/local
MANPREFIX = $(PREFIX)/share/man
APPPREFIX = $(PREFIX)/share/applications

PKG_CONFIG = pkg-config

DEPENDENCIES = xcb freetype2 xcb-image xcb-keysyms fontconfig libgrapheme xkbcommon

INCS = $(shell $(PKG_CONFIG) --cflags $(DEPENDENCIES)) -Iinclude
LIBS = $(shell $(PKG_CONFIG) --libs $(DEPENDENCIES)) -lcalrepeat
CALENDAR_PATH = .config/xcalendar/events.calendar

CFLAGS = -std=c99 -pedantic -Wall -Wextra -Os $(INCS) -DVERSION=\"$(VERSION)\" -DCALENDAR_PATH=\"$(CALENDAR_PATH)\"
LDFLAGS = -s $(LIBS)

CC = cc
