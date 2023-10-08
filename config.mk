# Copyright (C) 2022-2023 <alpheratz99@protonmail.com>
# This program is free software.

VERSION = 0.3.17

PREFIX = /usr/local
MANPREFIX = $(PREFIX)/share/man
APPPREFIX = $(PREFIX)/share/applications

PKG_CONFIG = pkg-config

DEPENDENCIES = xcb freetype2 xcb-image xcb-keysyms fontconfig

INCS = $(shell $(PKG_CONFIG) --cflags $(DEPENDENCIES)) -Iinclude
LIBS = $(shell $(PKG_CONFIG) --libs $(DEPENDENCIES))

CFLAGS = -std=c99 -pedantic -Wall -Wextra -Os $(INCS) -DVERSION=\"$(VERSION)\"
LDFLAGS = -s $(LIBS)

CC = cc
