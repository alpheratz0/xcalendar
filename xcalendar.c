#include "font.h"
#include "calendar.h"
#include "config.h"
#include "window.h"

int main(void) {
	font_t *ft = font_load(font_family, font_size);
	calendar_t *cal = calendar_from_today(ft, foreground, background);

	window_t *wnd = window_init("xcalendar", "xcalendar", background);
	window_create_pixmap(wnd, cal->bitmap);
	window_loop(wnd);

	font_free(ft);
	calendar_free(cal);
	window_free(wnd);
	return 0;
}
