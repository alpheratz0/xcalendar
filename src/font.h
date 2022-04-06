#ifndef __XCALENDAR_FONT_H__
#define __XCALENDAR_FONT_H__

#include <freetype/freetype.h>
#include "numdef.h"

struct font {
	FT_Library library;
	FT_Face face;
	u32 size;
	u32 line_height;
	u32 width;
};

typedef struct font font_t;

extern font_t *
font_load(const char *family, u32 size);

extern FT_GlyphSlot
font_get_glyph(font_t *ft, char c);

extern void
font_free(font_t *ft);

#endif
