PREFIX = /usr/local
MANPREFIX = ${PREFIX}/share/man
LDLIBS = -lxcb -lfreetype -lxcb-image -lfontconfig
LDFLAGS = -s ${LDLIBS}
INCS = -I/usr/include -I/usr/include/freetype2
CFLAGS = -pedantic -Wall -Wextra -Os ${INCS}
CC = cc

SRC = src/bitmap.c \
	  src/calendar.c \
	  src/dateinfo.c \
	  src/debug.c \
	  src/font.c \
	  src/xcalendar.c \
	  src/text.c \
	  src/window.c \
	  src/color.c

OBJ = ${SRC:.c=.o}

all: xcalendar

${OBJ}: src/bitmap.h \
		src/calendar.h \
		src/dateinfo.h \
		src/debug.h \
		src/font.h \
		src/numdef.h \
		src/text.h \
		src/window.h \
		src/color.h \
		src/config.h

xcalendar: ${OBJ}
	${CC} -o $@ ${OBJ} ${LDFLAGS}

install: all
	@mkdir -p ${DESTDIR}${PREFIX}/bin
	@cp -f xcalendar ${DESTDIR}${PREFIX}/bin
	@chmod 755 ${DESTDIR}${PREFIX}/bin/xcalendar
	@mkdir -p ${DESTDIR}${MANPREFIX}/man1
	@cp -f man/xcalendar.1 ${DESTDIR}${MANPREFIX}/man1
	@chmod 644 ${DESTDIR}${MANPREFIX}/man1/xcalendar.1

uninstall:
	@rm -f ${DESTDIR}${PREFIX}/bin/xcalendar
	@rm -f ${DESTDIR}${MANPREFIX}/man1/xcalendar.1

clean:
	@rm -f xcalendar ${OBJ}

.PHONY: all clean install uninstall
