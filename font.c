#include <stdlib.h>
#include <string.h>
#include <freetype/freetype.h>
#include <fontconfig/fontconfig.h>

#include "debug.h"
#include "font.h"

static char *
font_search(const char *family) {
	FcPattern *pattern;
	FcPattern *match;
	FcResult result;
	char *path = NULL;

	if ((pattern = FcNameParse((const FcChar8 *)(family)))) {
		FcConfigSubstitute(0, pattern, FcMatchPattern);
		FcDefaultSubstitute(pattern);

		if ((match = FcFontMatch(0, pattern, &result))) {
			FcValue v;
			FcPatternGet(match, FC_FILE, 0, &v);
			path = strdup((char *)(v.u.f));
			FcPatternDestroy(match);
		}

		FcPatternDestroy(pattern);
		FcFini();
	}

	return path;
}

extern font_t *
font_load(const char *family, u32 size) {
	char *path;
	font_t *ft;

	if (!(path = font_search(family))) {
		dief("font family not found: %s", family);
	}

	if (!(ft = malloc(sizeof(font_t)))) {
		die("error while calling malloc, no memory available");
	}

	if (FT_Init_FreeType(&ft->library)) {
		die("error while calling FT_Init_FreeType()");
	}

	if (FT_New_Face(ft->library, path, 0, &(ft->face))) {
		die("error while calling FT_New_Face()");
	}

	if (FT_Set_Char_Size(ft->face, 0, size * 64, 72, 72)) {
		die("error while calling FT_Set_Char_Size()");
	}

	if (FT_Load_Glyph(ft->face, FT_Get_Char_Index(ft->face, '0'), 0)) {
		die("error while calling FT_Load_Glyph()");
	}

	if (FT_Render_Glyph(ft->face->glyph, FT_RENDER_MODE_NORMAL)) {
		die("error while calling FT_Render_Glyph()");
	}

	ft->size = size;
	ft->width = ft->face->glyph->advance.x >> 6;
	ft->line_height = (ft->face->size->metrics.ascender - ft->face->size->metrics.descender) >> 6;

	free(path);

	return ft;
}

extern void
font_free(font_t *ft) {
	FT_Done_FreeType(ft->library);
	free(ft);
}

extern FT_GlyphSlot
font_get_glyph(font_t *ft, char c) {
	if (FT_Load_Glyph(ft->face, FT_Get_Char_Index(ft->face, c), 0))
		die("error while calling FT_Load_Glyph()");

	if (FT_Render_Glyph(ft->face->glyph, FT_RENDER_MODE_NORMAL))
		die("error while calling FT_Render_Glyph()");

	return ft->face->glyph;
}
