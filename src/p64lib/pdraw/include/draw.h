#pragma once
#ifndef _P64LIB_PDRAW_DRAW_H
#define _P64LIB_PDRAW_DRAW_H

#include "../../../fonts/basic_font.h"
#include "../../../kernel/arch/x86_64-elf/limine.h"
#include "../../../kernel/kernel/kernel.h"
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

void putpixel(int x, int y, int color);
void fillrect(size_t width, size_t height, size_t x, size_t y, int color);
void writechar(char ch, uint32_t colour);

#ifdef __cplusplus
}
#endif

#endif
