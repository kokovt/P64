#pragma once

#include <limine.h>
#include <list.h>

// I wasn't originally going to setup  a kernel struct; but I have changed my
// mind.

struct Kernel {
  uintptr_t cr3;
  uintptr_t hhdm;
  struct limine_framebuffer *framebuffer;
  struct list *pmm_chunklist;
};

extern struct Kernel kernel;
