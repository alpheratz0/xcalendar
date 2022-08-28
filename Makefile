.POSIX:
.PHONY: all clean install uninstall dist

include config.mk

SRC = src/base/bitmap.c src/base/font.c src/xcalendar.c src/ui/calendar.c src/ui/label.c src/x11/window.c src/util/color.c src/util/debug.c src/util/xmalloc.c
OBJ = src/base/bitmap.o src/base/font.o src/xcalendar.o src/ui/calendar.o src/ui/label.o src/x11/window.o src/util/color.o src/util/debug.o src/util/xmalloc.o

all: xcalendar

xcalendar: $(OBJ)
	$(CC) $(LDFLAGS) -o xcalendar $(OBJ) $(LDLIBS)

clean:
	rm -f xcalendar xcalendar.o xcalendar-$(VERSION).tar.gz $(OBJ)

install: all
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp -f xcalendar $(DESTDIR)$(PREFIX)/bin
	chmod 755 $(DESTDIR)$(PREFIX)/bin/xcalendar
	mkdir -p $(DESTDIR)$(MANPREFIX)/man1
	cp -f man/xcalendar.1 $(DESTDIR)$(MANPREFIX)/man1
	chmod 644 $(DESTDIR)$(MANPREFIX)/man1/xcalendar.1
	mkdir -p $(DESTDIR)$(APPPREFIX)
	cp -f misc/xcalendar.desktop $(DESTDIR)$(APPPREFIX)/xcalendar.desktop
	chmod 644 $(DESTDIR)$(APPPREFIX)/xcalendar.desktop

dist: clean
	mkdir -p xcalendar-$(VERSION)
	cp -R COPYING config.mk Makefile README misc src man xcalendar-$(VERSION)
	tar -cf xcalendar-$(VERSION).tar xcalendar-$(VERSION)
	gzip xcalendar-$(VERSION).tar
	rm -rf xcalendar-$(VERSION)

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/xcalendar
	rm -f $(DESTDIR)$(MANPREFIX)/man1/xcalendar.1
	rm -f $(DESTDIR)$(APPPREFIX)/xcalendar.desktop
