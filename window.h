#ifndef __XCALENDAR_WINDOW_H__
#define __XCALENDAR_WINDOW_H__

#include <xcb/xcb.h>
#include <xcb/xcb_image.h>

struct window {
	xcb_connection_t *connection;
	xcb_window_t window;
	xcb_screen_t *screen;
	xcb_gcontext_t gc;
	xcb_pixmap_t pixmap;
	xcb_image_t *image;
};

typedef struct window window_t;

extern window_t *
window_init(const char *wm_name, const char *wm_class, u32 background);

extern void
window_create_pixmap(window_t *wnd, bitmap_t *bmp);

extern void
window_loop(window_t *wnd);

extern void
window_free(window_t *wnd);

#endif
