#include <freetype/freetype.h>

#include "../base/bitmap.h"
#include "../base/font.h"
#include "../util/debug.h"
#include "../util/color.h"
#include "../util/numdef.h"
#include "text.h"

static void
text_render_char(bitmap_t *bmp, font_t *ft, u32 x, u32 y, u32 foreground, u32 background, char c) {
	bitmap_rect(bmp, x, y, ft->width, ft->line_height, background);

	FT_GlyphSlot glyph = font_get_glyph(ft, c);
	u32 glyph_h = glyph->bitmap.rows;
	u32 glyph_w = glyph->bitmap.width;

	for (u32 i = 0; i < glyph_h; ++i) {
		for (u32 j = 0; j < glyph_w; ++j) {
			u32 xmap = x + j + glyph->bitmap_left;
			u32 ymap = y + i - glyph->bitmap_top + ft->size;
			u8  gray = glyph->bitmap.buffer[i*glyph_w+j];

			bitmap_set_safe(bmp, xmap, ymap, clerp(background, foreground, gray));
		}
	}
}

extern void
text_render(bitmap_t *bmp, font_t *ft, u32 x, u32 y, u32 foreground, u32 background, char *text) {
	for (size_t i = 0; i < strlen(text) && text[i] != '\n'; ++i) {
		text_render_char(bmp, ft, x + i * ft->width, y, foreground, background, text[i]);
	}
}
