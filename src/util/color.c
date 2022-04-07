#include "numdef.h"
#include "color.h"

extern u32
color_lerp(u32 from, u32 to, u8 v) {
	u8 rfrom = (from >> 16) & 0xff;
	u8 rto   = (to >> 16) & 0xff;
	u8 r     = rfrom + ((rto - rfrom) * v) / 0xff;

	u8 gfrom = (from >> 8) & 0xff;
	u8 gto   = (to >> 8) & 0xff;
	u8 g     = gfrom + ((gto - gfrom) * v) / 0xff;

	u8 bfrom = from & 0xff;
	u8 bto   = to & 0xff;
	u8 b     = bfrom + ((bto - bfrom) * v) / 0xff;

	return (r << 16) | (g << 8) | b;
}
