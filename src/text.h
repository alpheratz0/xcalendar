#ifndef __XCALENDAR_TEXT_H__
#define __XCALENDAR_TEXT_H__

#include "bitmap.h"
#include "font.h"
#include "numdef.h"

extern void
text_render(bitmap_t *bmp, font_t *ft, u32 x, u32 y, u32 foreground, u32 background, char *text);

#endif
