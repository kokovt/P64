#pragma once
#ifndef _P64LIB_PDRAW_DRAW_H
#define _P64LIB_PDRAW_DRAW_H

#include <fonts/basic_font.h>
#include <kernel.h>
#include <limine.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

void putpixel(int x, int y, int color);
void fillrect(size_t width, size_t height, size_t x, size_t y, int color);
void writechar(char ch, size_t pos_x, size_t pos_y, uint32_t colour);

#ifdef __cplusplus
}
#endif

#endif
