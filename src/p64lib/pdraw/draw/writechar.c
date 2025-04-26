#include "../../include/draw.h"
size_t row = 0;
size_t column = 0;

void newline() {
  column = 0;
  row += 1;
}

void writechar(char ch, uint32_t colour) {
  if (ch == '\n')
    return newline();

  size_t max_columns = kernel.framebuffer->width / 8;

  uint64_t first_byte_idx = ch * 16;
  for (size_t y = 0; y < 16; y++) {
    for (size_t x = 0; x < 8; x++) {
      if ((font[first_byte_idx + y] >> (7 - x)) & 1) {
        putpixel(x + column * 8, y + row * 16, colour);
      }
    }
  }

  if (++column == max_columns) {
    column = 0;
    row += 1;
  }
}
