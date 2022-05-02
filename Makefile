VERSION = 0.3.13
PREFIX = /usr/local
MANPREFIX = ${PREFIX}/share/man
APPPREFIX = ${PREFIX}/share/applications
LDLIBS = -lxcb -lfreetype -lxcb-image -lfontconfig
LDFLAGS = -s ${LDLIBS}
INCS = -I/usr/include -I/usr/include/freetype2
CFLAGS = -pedantic -Wall -Wextra -Os ${INCS} -DVERSION=\"${VERSION}\"
CC = cc

SRC = src/base/bitmap.c \
	  src/base/font.c \
	  src/xcalendar.c \
	  src/ui/calendar.c \
	  src/ui/label.c \
	  src/x11/window.c \
	  src/util/color.c \
	  src/util/debug.c

OBJ = ${SRC:.c=.o}

all: xcalendar

${OBJ}: src/base/font.h \
		src/base/bitmap.h \
		src/ui/calendar.h \
		src/ui/label.h \
		src/x11/window.h \
		src/x11/keys.h \
		src/util/debug.h \
		src/util/color.h \
		src/util/numdef.h

xcalendar: ${OBJ}
	${CC} -o $@ ${OBJ} ${LDFLAGS}

install: all
	@mkdir -p ${DESTDIR}${PREFIX}/bin
	@cp -f xcalendar ${DESTDIR}${PREFIX}/bin
	@chmod 755 ${DESTDIR}${PREFIX}/bin/xcalendar
	@mkdir -p ${DESTDIR}${MANPREFIX}/man1
	@cp -f man/xcalendar.1 ${DESTDIR}${MANPREFIX}/man1
	@chmod 644 ${DESTDIR}${MANPREFIX}/man1/xcalendar.1
	@mkdir -p ${DESTDIR}${APPPREFIX}
	@cp -f misc/xcalendar.desktop ${DESTDIR}${APPPREFIX}/xcalendar.desktop
	@chmod 644 ${DESTDIR}${APPPREFIX}/xcalendar.desktop

dist: clean
	@mkdir -p xcalendar-${VERSION}
	@cp -R LICENSE Makefile README man src xcalendar-${VERSION}
	@tar -cf xcalendar-${VERSION}.tar xcalendar-${VERSION}
	@gzip xcalendar-${VERSION}.tar
	@rm -rf xcalendar-${VERSION}

uninstall:
	@rm -f ${DESTDIR}${PREFIX}/bin/xcalendar
	@rm -f ${DESTDIR}${MANPREFIX}/man1/xcalendar.1
	@rm -f ${DESTDIR}${APPPREFIX}/xcalendar.desktop

clean:
	@rm -f xcalendar xcalendar-${VERSION}.tar.gz ${OBJ}

.PHONY: all clean install uninstall dist
