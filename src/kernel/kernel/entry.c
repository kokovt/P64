// ----- DEPENDANCIES -----
#include <bootutils.h>
#include <cpu/gdt.h>
#include <cpu/tss.h>
#include <interrupts/keyboard.h>
#include <interrupts/pic.h>
#include <io.h>
#include <kernel.h>
#include <mem/paging.h>
#include <mem/pmm.h>

extern void enable_sse();

struct Kernel kernel = {};

void _start(void) {
  init_kernel();
  init_pmm();
  init_tss();
  init_gdt();
  init_idt();
  init_pic();
  enable_sse();
  setup_paging();
  init_keyboard();

  uint8_t mask = inb(0x21);
  outb(0x21, mask & ~(1 << 1));
  outb(0x64, 0xAE);
  outb(0x60, 0xF4);

  asm volatile("sti");
  for (;;)
    ;
}
