// ----- DEPENDANCIES -----
#include "../include/draw.h"

// ----- FUNCTIONS -----
void putpixel(int x, int y, int color) {
  struct limine_framebuffer *framebuffer = getframebuffer();
  unsigned where = y * (framebuffer->pitch / 4) + x;

  volatile uint32_t *screen = framebuffer->address;

  screen[where] = color;
}
