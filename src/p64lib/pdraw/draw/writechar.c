#include "../../include/draw.h"

void writechar(char ch, size_t pos_x, size_t pos_y, uint32_t colour) {
  if (ch == '\0') {
    for (size_t y = 0; y < 16; y++) {
      for (size_t x = 0; x < 16; x++) {
        putpixel(x + pos_x, y + pos_y, 0x000000);
      }
    }
    return;
  }

  uint64_t first_byte_idx = ch * 16;
  for (size_t y = 0; y < 16; y++) {
    for (size_t x = 0; x < 8; x++) {
      if ((font[first_byte_idx + y] >> (7 - x)) & 1) {
        putpixel(x + pos_x, y + pos_y, colour);
      }
    }
  }
}
