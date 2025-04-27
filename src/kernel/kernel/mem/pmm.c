#include <hcf.h>
#include <kernel.h>
#include <mem/paging.h>
#include <mem/pmm.h>
#include <stdio.h>

// THIS IS HEAVILY BASED OFF OF UNMAPPEDSTACK/TACOS
// Check them out!
// https://github.com/UnmappedStack/TacOS/blob/beee6216b58ee07ad39ac5e5350384cfb39d1aaa/src/mem/paging.c
// This is under a different license from this project.

const char *mem_types[] = {
    "Usable                ", "Reserved              ",
    "ACPI Reclaimable      ", "ACPI NVS              ",
    "Bad Memory            ", "Bootloader Reclaimable",
    "Executable and Modules", "Framebuffer           ",
};

void init_chunk(void *addr, size_t len) {
  Chunk *chunk = (Chunk *)addr;
  chunk->length = PAGE_ALIGN_DOWN(len) / 4096;
  // NOTE: The list is already initialised.
}

void init_pmm() {

  size_t list_len = 0;
  for (size_t i = 0; i < kernel.memmap_entry_count; i++) {

    if (kernel.memmap[i].type != 0)
      continue; // must be marked usable
    list_len++;
    if (!kernel.pmm_chunklist) {
      kernel.pmm_chunklist =
          (struct list *)PAGE_ALIGN_UP(kernel.memmap[i].base + kernel.hhdm);
      list_init(kernel.pmm_chunklist);
    } else {
      list_insert(
          kernel.pmm_chunklist,
          (struct list *)PAGE_ALIGN_UP(kernel.memmap[i].base + kernel.hhdm));
    }
    init_chunk((void *)(PAGE_ALIGN_UP(kernel.memmap[i].base) + kernel.hhdm),
               kernel.memmap[i].length);
  }

  if (!list_len) {
    printf(

        "Failed to initialise physical memory allocator, cannot continue.\n");
    hcf();
  }
}

// Returns a physical address
uintptr_t kmalloc(size_t num_pages) {
  size_t i = 0;
  Chunk *chunk;
  for (struct list *at = kernel.pmm_chunklist;
       at != kernel.pmm_chunklist || i == 0; at = at->next) {
    chunk = (Chunk *)at;
    if (chunk->length <= num_pages + 1) {
      i++;
      continue;
    }
    if (!i)
      kernel.pmm_chunklist = kernel.pmm_chunklist->next;
    list_remove(&chunk->list);
    Chunk *new_chunk =
        (Chunk *)PAGE_ALIGN_UP((uint64_t)chunk + num_pages * PAGE_SIZE + 1);
    list_insert(kernel.pmm_chunklist, &new_chunk->list);
    init_chunk(new_chunk, (chunk->length - (num_pages + 1)) * PAGE_SIZE);
    memset(chunk, 0, PAGE_SIZE);
    return (uintptr_t)chunk - kernel.hhdm;
  }
  printf("\nPANIC: OUT OF MEMORY\n\n");
  hcf();
  return 0;
}

void kfree(uintptr_t addr, size_t num_pages) {
  list_insert(kernel.pmm_chunklist, (struct list *)(addr + kernel.hhdm));
  init_chunk((Chunk *)addr, num_pages + 1);
}
