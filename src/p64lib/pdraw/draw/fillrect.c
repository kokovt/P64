#include "../../include/draw.h"
void fillrect(size_t width, size_t height, size_t x, size_t y, int color) {

  volatile uint32_t *screen = kernel.framebuffer->address;

  for (size_t i = 0; i < width; i++) {
    for (size_t j = 0; j < height; j++) {
      unsigned where = x + j * (kernel.framebuffer->pitch / 4) + y + i;
      screen[where + i] = color;
    }
  }
}
