// ----- DEPENDANCIES -----
#include <bootutils.h>
#include <cpu/gdt.h>
#include <cpu/tss.h>
#include <draw.h>
#include <drivers/keyboard.h>
#include <drivers/pic.h>
#include <io.h>
#include <kernel.h>
#include <mem/paging.h>
#include <mem/pmm.h>
#include <stdio.h>

extern void enable_sse();

struct Kernel kernel = {};

void test();

void draw_cursor(void) {
  writechar('_', kernel.term_column * 8, kernel.term_row * 16, 0xffffff);
}

void _start(void) {

  init_kernel();
  init_pmm();
  init_tss();
  init_gdt();
  init_idt();
  init_pic();
  enable_sse();
  setup_paging();

  printf("P64 TESTING!\n");
  printf("TESTING NEW LINES\n");
  fillrect(100, 100, 100, 100, 0xfffff);
  draw_cursor();

  kernel.keypress_callback = test;
  init_keyboard();

  uint8_t mask = inb(0x21);

  outb(0x21, mask & ~(1 << 1));

  outb(0x64, 0xAE);
  outb(0x60, 0xF4);

  asm volatile("sti");

  for (;;)
    ;
}

void test() { putchar(kernel.last_pressed); }
