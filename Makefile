PREFIX = /usr/local
LDLIBS = -lxcb -lfreetype -lxcb-image -lfontconfig
LDFLAGS = -s ${LDLIBS}
INCS = -I/usr/include -I/usr/include/freetype2
CFLAGS = -pedantic -Wall -Wextra -Os ${INCS}
CC = cc

SRC = bitmap.c \
	  calendar.c \
	  dateinfo.c \
	  debug.c \
	  font.c \
	  xcalendar.c \
	  text.c \
	  window.c \
	  color.c

OBJ = ${SRC:.c=.o}

all: xcalendar

${OBJ}: bitmap.h calendar.h dateinfo.h debug.h font.h numdef.h text.h window.h color.h config.h

xcalendar: ${OBJ}
	${CC} -o $@ ${OBJ} ${LDFLAGS}

install: all
	@mkdir -p ${DESTDIR}${PREFIX}/bin
	@cp -f xcalendar ${DESTDIR}${PREFIX}/bin
	@chmod 755 ${DESTDIR}${PREFIX}/bin/xcalendar

uninstall:
	@rm -f ${DESTDIR}${PREFIX}/bin/xcalendar

clean:
	@rm -f xcalendar ${OBJ}

.PHONY: all clean install uninstall
