#include <stdlib.h>
#include <string.h>
#include <xcb/xcb.h>
#include <xcb/xcb_image.h>
#include <xcb/xproto.h>

#include "numdef.h"
#include "bitmap.h"
#include "debug.h"
#include "window.h"

extern window_t *
window_init(const char *wm_name, const char *wm_class, u32 background) {
	window_t *wnd;

	if (!(wnd = malloc(sizeof(window_t)))) {
		die("error while calling malloc, no memory available");
	}

	wnd->connection = xcb_connect(NULL, NULL);

	if (xcb_connection_has_error(wnd->connection)) {
		die("can't open display");
	}

	wnd->screen = xcb_setup_roots_iterator(xcb_get_setup(wnd->connection)).data;

	if (!wnd->screen) {
		xcb_disconnect(wnd->connection);
		die("can't get default screen");
	}

    wnd->window = xcb_generate_id(wnd->connection);

	xcb_void_cookie_t window_cookie = xcb_create_window_checked(
		wnd->connection, XCB_COPY_FROM_PARENT, wnd->window,
		wnd->screen->root, 0, 0, wnd->screen->width_in_pixels, wnd->screen->height_in_pixels, 0,
		XCB_WINDOW_CLASS_INPUT_OUTPUT, wnd->screen->root_visual,
		XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK,
		(const u32[2]) {
			background,
			XCB_EVENT_MASK_EXPOSURE |
			XCB_EVENT_MASK_BUTTON_PRESS |
			XCB_EVENT_MASK_BUTTON_RELEASE |
			XCB_EVENT_MASK_KEY_PRESS |
			XCB_EVENT_MASK_KEY_RELEASE
		}
	);

	if (xcb_request_check(wnd->connection, window_cookie)) {
		xcb_disconnect(wnd->connection);
		die("can't create window");
	}

	xcb_change_property(
		wnd->connection, XCB_PROP_MODE_REPLACE, wnd->window, XCB_ATOM_WM_NAME,
		XCB_ATOM_STRING, 8, strlen(wm_name), wm_name
	);

	xcb_change_property(
		wnd->connection, XCB_PROP_MODE_REPLACE, wnd->window, XCB_ATOM_WM_CLASS,
		XCB_ATOM_STRING, 8, strlen(wm_class), wm_class
	);

	xcb_change_window_attributes(
		wnd->connection, wnd->window, XCB_CW_OVERRIDE_REDIRECT,
		(const u32[1]) { 0x1 }
	);

	xcb_map_window(wnd->connection, wnd->window);
	xcb_flush(wnd->connection);

	xcb_set_input_focus(wnd->connection, XCB_INPUT_FOCUS_POINTER_ROOT, wnd->window, XCB_CURRENT_TIME);

	return wnd;
}

extern void
window_create_pixmap(window_t *wnd, bitmap_t *bmp) {
    wnd->gc = xcb_generate_id(wnd->connection);
    wnd->pixmap = xcb_generate_id(wnd->connection);

    wnd->image = xcb_image_create_native(
		wnd->connection, bmp->width, bmp->height,
        XCB_IMAGE_FORMAT_Z_PIXMAP, wnd->screen->root_depth,
        bmp->px, 4*bmp->width*bmp->height, (u8 *)(bmp->px)
	);

    xcb_create_pixmap(wnd->connection, wnd->screen->root_depth, wnd->pixmap, wnd->window, bmp->width, bmp->height);
    xcb_create_gc(wnd->connection, wnd->gc, wnd->pixmap, 0, 0);

    xcb_flush(wnd->connection);
}

extern void
window_loop(window_t *wnd) {
	xcb_generic_event_t *ev;

	while ((ev = xcb_wait_for_event(wnd->connection))) {
		switch (ev->response_type & ~0x80) {
			case XCB_EXPOSE: {
				xcb_expose_event_t *eev = (xcb_expose_event_t *)(ev);
				xcb_image_put(wnd->connection, wnd->pixmap, wnd->gc, wnd->image, 0, 0, 0);
				xcb_clear_area(wnd->connection, 0, wnd->window, 0, 0, eev->width, eev->height);
				xcb_copy_area(wnd->connection, wnd->pixmap, wnd->window,
						wnd->gc, 0, 0, (eev->width - wnd->image->width) / 2,
						(eev->height - wnd->image->height) / 2,
						wnd->image->width, wnd->image->height);
				xcb_flush(wnd->connection);
				break;
			}
			case XCB_BUTTON_RELEASE:
				free(ev);
				return;
			case XCB_KEY_PRESS:
				/* check if esc key is pressed */
				if (((xcb_key_press_event_t *)(ev))->detail == 9) {
					free(ev);
					return;
				}
				break;
			default:
				break;
		}

		free(ev);
	}
}

extern void
window_free(window_t *wnd) {
	xcb_free_gc(wnd->connection, wnd->gc);
	xcb_free_pixmap(wnd->connection, wnd->pixmap);
	xcb_disconnect(wnd->connection);
	free(wnd->image);
	free(wnd);
}
