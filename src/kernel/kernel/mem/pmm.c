#include <kernel.h>
#include <mem/pmm.h>

uintptr_t kmalloc(size_t num_pages) {
  size_t i = 0;
  Chunk *chunk;

  for (struct list *at = kernel.pmm_chunklist;
       at != kernel.pmm_chunklist || i == 0; at = at->next) {
  }
}
