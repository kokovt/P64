#pragma once

#include "cpu/idt.h"
#include <cpu/gdt.h>
#include <cpu/tss.h>
#include <limine.h>
#include <list.h>
#include <stddef.h>
#include <stdint.h>

// I wasn't originally going to setup  a kernel struct; but I have changed my
// mind.

struct Kernel {
  uintptr_t cr3;
  uintptr_t hhdm;
  uintptr_t kernel_phys_addr;
  uintptr_t kernel_virt_addr;

  size_t memmap_entry_count;
  size_t term_column;
  size_t term_row;
  size_t term_scroll;
  char *term_text;

  char last_pressed;

  // Callback for when a button is pressed. The application for the wm will use
  // this when that is added.
  void (*keypress_callback)();

  uint64_t *GDT;
  IDTEntry *IDT;
  TSS *tss;

  struct limine_framebuffer *framebuffer;
  struct limine_memmap_entry *memmap;

  struct list *pmm_chunklist;
};

extern struct Kernel kernel;
