#pragma once

#include "cpu/idt.h"
#include "current_process.h"
#include "mem/bindings/page_bindings.h"
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
  char last_pressed;
  struct keypress_owner *keypress_owners[256];
  uint8_t keypress_owners_length;

  uint64_t *GDT;
  IDTEntry *IDT;
  TSS *tss;

  struct limine_framebuffer *framebuffer;
  struct limine_memmap_entry *memmap;
  struct page_binding *page_bindings;

  struct current_process *current_process;

  struct list *pmm_chunklist;
};

struct keypress_owner {
  uint64_t pid;
  void (*keypress_function)(char);
};

extern struct Kernel kernel;
