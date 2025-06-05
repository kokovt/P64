#include "kernel.h"
#include <mem/bindings/page_bindings.h>

// 0 = UNBOUND
// 1 = INVALID PERMISSIONS
long sysUnbindPage(
    uint64_t addr) { // CHANGE THIS WHEN VIRT MEMORY ALLOCATION IS ADDED
  if (kernel.current_process->pid != kernel.page_bindings[addr >> 12].pid)
    return 1;
  kernel.page_bindings[addr >> 12].bound = false;

  return 0;
}
