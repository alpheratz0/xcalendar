#include <stdlib.h>

#include "../util/numdef.h"
#include "../util/debug.h"
#include "bitmap.h"

extern bitmap_t *
bitmap_create(u32 width, u32 height, u32 color) {
	bitmap_t *bmp;

	if ((bmp = malloc(sizeof(bitmap_t)))) {
		bmp->width = width;
		bmp->height = height;
		if ((bmp->px = malloc(4*width*height))) {
			for (u32 i = 0; i < width * height; ++i) {
				bmp->px[i] = color;
			}
			return bmp;
		}
	}

	die("error while calling malloc, no memory available");

	return (void *)(0);
}

extern void
bitmap_set(bitmap_t *bmp, u32 x, u32 y, u32 color) {
	if (x < bmp->width && y < bmp->height) {
		bmp->px[y*bmp->width+x] = color;
	}
}

extern void
bitmap_set_unsafe(bitmap_t *bmp, u32 x, u32 y, u32 color) {
	bmp->px[y*bmp->width+x] = color;
}

extern void
bitmap_rect(bitmap_t *bmp, u32 x, u32 y, u32 width, u32 height, u32 color) {
	for (u32 i = 0; i < width; ++i) {
		for (u32 j = 0; j < height; ++j) {
			bitmap_set(bmp, x + i, y + j, color);
		}
	}
}

extern void
bitmap_free(bitmap_t *bmp) {
	free(bmp->px);
	free(bmp);
}
