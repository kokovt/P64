#include "mem/paging.h"
#include <kernel.h>
#include <mem/bindings/page_bindings.h>

#define CAP_READ (1 << 0)
#define CAP_WRITE (1 << 1)
#define CAP_EXECUTE (1 << 2)

// 0 = BOUND
// 1 = INVALID ADDRESS
// 2 = INVALID CAPABILITIES
// 3 = MISSING PERMISSIONS / ALREADY BINDED BY DIFFERENT APPLICATION
long sysBindPage(uint64_t virt_addr, uint64_t capabilities) {
  uint64_t phys_addr =
      virt_to_phys((uint64_t *)kernel.cr3 + kernel.hhdm, virt_addr);
  uint64_t pfn = phys_addr >> 12;

  if (pfn >= kernel.memmap_entry_count) {
    return 1;
  }

  if (capabilities & ~(CAP_READ | CAP_WRITE | CAP_EXECUTE)) {
    return 2;
  }

  if (kernel.page_bindings[pfn].bound) {
    if (kernel.page_bindings[pfn].pid != kernel.current_process->pid) {
      return 3;
    }

    // For now we will allow rebinding.
  }

  kernel.page_bindings[pfn].physical_page = phys_addr;
  kernel.page_bindings[pfn].pid = kernel.current_process->pid;
  kernel.page_bindings[pfn].bound = true;
  kernel.page_bindings[pfn].capabilities = capabilities;

  return 0;
}
