/*
	Copyright (C) 2022-2024 <alpheratz99@protonmail.com>

	This program is free software; you can redistribute it and/or modify it
	under the terms of the GNU General Public License version 2 as published by
	the Free Software Foundation.

	This program is distributed in the hope that it will be useful, but WITHOUT
	ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
	FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
	more details.

	You should have received a copy of the GNU General Public License along
	with this program; if not, write to the Free Software Foundation, Inc., 59
	Temple Place, Suite 330, Boston, MA 02111-1307 USA

*/

#include <grapheme.h>
#include <stdint.h>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "../base/bitmap.h"
#include "../base/font.h"
#include "../util/debug.h"
#include "../util/color.h"
#include "label.h"

static void
label_render_char_onto(uint32_t c, struct font *font, uint32_t color,
                       uint32_t x, uint32_t y, struct bitmap *bmp)
{
	FT_GlyphSlot glyph;
	uint32_t width, height, xmap, ymap, gray, i, j;

	glyph = font_get_glyph(font, c);
	height = glyph->bitmap.rows;
	width = glyph->bitmap.width;

	for (i = 0; i < height; ++i) {
		for (j = 0; j < width; ++j) {
			xmap = x + j + glyph->bitmap_left;
			ymap = y + i - glyph->bitmap_top + font->size;
			gray = glyph->bitmap.buffer[i*width+j];

			bitmap_set(
				bmp, xmap, ymap,
				color_lerp(
					bitmap_get(bmp, xmap, ymap),
					color, gray
				)
			);
		}
	}
}

extern void
label_render_onto(const char *text, struct font *font, uint32_t color,
                  uint32_t x, uint32_t y, struct bitmap *bmp)
{
	size_t i, len, off;
	uint32_t cp;

	for (i = 0, off = 0; (len = grapheme_decode_utf8(text + off,
					SIZE_MAX, &cp)) > 0 &&
			cp != 0; off += len, ++i) {
		label_render_char_onto(cp, font, color, x + i * font->width, y, bmp);
	}
}
