#include <kernel.h>
#include <mem/mapall.h>
#include <mem/paging.h>
#include <mem/pmm.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

// THIS IS HEAVILY BASED OFF OF UNMAPPEDSTACK/TACOS
// Check them out!
// https://github.com/UnmappedStack/TacOS/blob/beee6216b58ee07ad39ac5e5350384cfb39d1aaa/src/mem/paging.c
// This is under a different license from this project.

extern uint64_t p_kernel_start[];
extern uint64_t p_writeallowed_start[];
extern uint64_t p_kernel_end[];

uint64_t kernel_start = (uint64_t)p_kernel_start;
uint64_t writeallowed_start = (uint64_t)p_writeallowed_start;
uint64_t kernel_end = (uint64_t)p_kernel_end;

void map_sections(uint64_t pml4[]) {
  uint64_t entry_type;
  for (size_t entry = 0; entry < kernel.memmap_entry_count; entry++) {
    entry_type = kernel.memmap[entry].type;
    if (entry_type == LIMINE_MEMMAP_USABLE ||
        entry_type == LIMINE_MEMMAP_FRAMEBUFFER ||
        entry_type == LIMINE_MEMMAP_BOOTLOADER_RECLAIMABLE ||
        entry_type == LIMINE_MEMMAP_KERNEL_AND_MODULES) {

      size_t flags = KERNEL_PFLAG_PRESENT | KERNEL_PFLAG_WRITE;
      if (entry_type == LIMINE_MEMMAP_FRAMEBUFFER)
        flags |= KERNEL_PFLAG_USER |
                 KERNEL_PFLAG_WRITE_COMBINE; // for when it's mmapped in
      map_pages(pml4, kernel.memmap[entry].base + kernel.hhdm,
                kernel.memmap[entry].base, kernel.memmap[entry].length / 4096,
                flags);
    }
  }
}

void map_kernel(uint64_t pml4[]) {
  uint64_t length_buffer = 0;
  uint64_t phys_buffer = 0;
  /* map from kernel_start to writeallowed_start with only the present flag */
  length_buffer = PAGE_ALIGN_UP(writeallowed_start - kernel_start) / 4096;
  phys_buffer =
      kernel.kernel_phys_addr + (kernel_start - kernel.kernel_virt_addr);

  map_pages(pml4, PAGE_ALIGN_DOWN(kernel_start), phys_buffer, length_buffer,
            KERNEL_PFLAG_PRESENT);
  /* map from writeallowed_start to kernel_end with `present` and `write` flags
   */
  length_buffer = PAGE_ALIGN_UP(kernel_end - writeallowed_start) / 4096;
  phys_buffer =
      kernel.kernel_phys_addr + (writeallowed_start - kernel.kernel_virt_addr);

  map_pages(pml4, PAGE_ALIGN_DOWN(writeallowed_start), phys_buffer,
            length_buffer, KERNEL_PFLAG_PRESENT | KERNEL_PFLAG_WRITE);
  // map the kernel's stack
  alloc_pages(pml4, KERNEL_STACK_ADDR, KERNEL_STACK_PAGES,
              KERNEL_PFLAG_PRESENT | KERNEL_PFLAG_WRITE);
}

void map_all(uint64_t pml4[]) {

  map_sections(pml4);
  map_kernel(pml4);
}
