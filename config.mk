# Copyright (C) 2022 <alpheratz99@protonmail.com>
# This program is free software.

VERSION   = 0.3.15

CC        = cc
INCS      = -I/usr/include/freetype2
CFLAGS    = -std=c99 -pedantic -Wall -Wextra -Os $(INCS) -DVERSION=\"$(VERSION)\"
LDLIBS    = -lxcb -lfreetype -lxcb-image -lxcb-keysyms -lfontconfig
LDFLAGS   = -s

PREFIX    = /usr/local
MANPREFIX = $(PREFIX)/share/man
APPPREFIX = $(PREFIX)/share/applications
