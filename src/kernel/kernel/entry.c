// ----- DEPENDANCIES -----
#include <bootutils.h>
#include <draw.h>
#include <kernel.h>
#include <mem/paging.h>
#include <stdio.h>

struct Kernel kernel = {};

void _start(void) {
  initkernel();
  setuppaging();

  printf("P64 TESTING!\n");
  printf("TESTING NEW LINES\n");
  fillrect(100, 100, 100, 100, 0xfffff);
  for (;;) {
    __asm__ __volatile__("hlt");
  }
}
