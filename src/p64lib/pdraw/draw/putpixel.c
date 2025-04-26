// ----- DEPENDANCIES -----
#include "../../include/draw.h"

// ----- FUNCTIONS -----
void putpixel(int x, int y, int color) {
  unsigned where = y * (kernel.framebuffer->pitch / 4) + x;

  volatile uint32_t *screen = kernel.framebuffer->address;

  screen[where] = color;
}
