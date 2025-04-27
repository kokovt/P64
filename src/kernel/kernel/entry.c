// ----- DEPENDANCIES -----
#include <bootutils.h>
#include <cpu/gdt.h>
#include <cpu/tss.h>
#include <draw.h>
#include <kernel.h>
#include <mem/paging.h>
#include <mem/pmm.h>
#include <stdio.h>

extern void enable_sse();

struct Kernel kernel = {};

void _start(void) {
  init_kernel();
  init_pmm();
  init_tss();
  init_gdt();
  init_idt();
  enable_sse();
  setup_paging();

  printf("P64 TESTING!\n");
  printf("TESTING NEW LINES\n");
  fillrect(100, 100, 100, 100, 0xfffff);
  for (;;) {
    __asm__ __volatile__("hlt");
  }
}
