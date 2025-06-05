#include <hcf.h>
#include <kernel.h>
#include <mem/mapall.h>
#include <mem/paging.h>
#include <mem/pmm.h>
#include <stdint.h>
#include <string.h>

#define PAGE_ALIGN_DOWN(addr) ((addr / 4096) * 4096)
#define PAGE_ALIGN_UP(x) ((((x) + 4095) / 4096) * 4096)

#define TOPBITS 0xFFFF000000000000

// THIS IS HEAVILY BASED OFF OF UNMAPPEDSTACK/TACOS
// Check them out!
// https://github.com/UnmappedStack/TacOS/blob/beee6216b58ee07ad39ac5e5350384cfb39d1aaa/src/mem/paging.c
// This is under a different license from this project.

static void invlpg(void *addr) {
  asm volatile("invlpg (%0)" : : "r"(addr) : "memory");
}

uint64_t virt_to_phys(uint64_t pml4_addr[], uint64_t virt_addr) {
  virt_addr &= ~TOPBITS;
  uint64_t pml1 = (virt_addr >> 12) & 511;
  uint64_t pml2 = (virt_addr >> (12 + 9)) & 511;
  uint64_t pml3 = (virt_addr >> (12 + 18)) & 511;
  uint64_t pml4 = (virt_addr >> (12 + 27)) & 511;

  for (; pml4 < 512; pml4++) {
    uint64_t *pml3_addr = NULL;

    if (pml4_addr[pml4] == 0)
      return 0xDEAD;
    else
      pml3_addr = (uint64_t *)(PAGE_ALIGN_DOWN(pml4_addr[pml4]) + kernel.hhdm);

    for (; pml3 < 512; pml3++) {
      uint64_t *pml2_addr = NULL;
      if (pml3_addr[pml3] == 0)
        return 0xDEAD;
      else
        pml2_addr =
            (uint64_t *)(PAGE_ALIGN_DOWN(pml3_addr[pml3]) + kernel.hhdm);

      for (; pml2 < 512; pml2++) {
        uint64_t *pml1_addr = NULL;
        if (pml2_addr[pml2] == 0) {
          return 0xDEAD;
        } else
          pml1_addr =
              (uint64_t *)(PAGE_ALIGN_DOWN(pml2_addr[pml2]) + kernel.hhdm);

        if (pml1_addr[pml1] == 0)
          return 0xDEAD;

        return (uint64_t)(PAGE_ALIGN_DOWN(pml1_addr[pml1]) +
                          (virt_addr - PAGE_ALIGN_DOWN(virt_addr)));
      }
      pml2 = 0;
    }
    pml3 = 0;
  }
  return 0xDEAD;
}

// writes data to a location in virtual memory
void write_vmem(uint64_t *pml4_addr, uint64_t virt_addr, char *data,
                size_t len) {
  while (len > 0) {
    // get the address of this virtual address in kernel memory
    uint64_t kernel_addr = virt_to_phys(pml4_addr, virt_addr);
    if (kernel_addr == 0xDEAD) {

      hcf();
    }
    kernel_addr += kernel.hhdm;
    uint64_t bytes_to_copy = (len < PAGE_SIZE) ? len : PAGE_SIZE;
    memcpy((char *)kernel_addr, data, bytes_to_copy);
    len -= bytes_to_copy;
    virt_addr += bytes_to_copy;
    data += bytes_to_copy;
  }
}

void read_vmem(uint64_t *pml4_addr, uintptr_t virt_addr, char *buffer,
               size_t len) {
  while (len > 0) {
    // get the address of this virtual address in kernel memory
    uint64_t kernel_addr = virt_to_phys(pml4_addr, virt_addr);
    if (!kernel_addr) {
      hcf();
    }
    kernel_addr += kernel.hhdm;
    uint64_t bytes_to_copy = (len < PAGE_SIZE) ? len : PAGE_SIZE;
    memcpy(buffer, (char *)kernel_addr, bytes_to_copy);
    len -= bytes_to_copy;
    virt_addr += bytes_to_copy;
    buffer += bytes_to_copy;
  }
}

// pushes data onto another stack in another virtual memory address tree thingy
void push_vmem(uint64_t *pml4_addr, uint64_t rsp, char *data, size_t len) {
  rsp -= len;
  write_vmem(pml4_addr, rsp, data, len);
}

void map_pages(uint64_t pml4_addr[], uint64_t virt_addr, uint64_t phys_addr,
               uint64_t num_pages, uint64_t flags) {
  virt_addr &= ~TOPBITS;
  uint64_t pml1 = (virt_addr >> 12) & 511;
  uint64_t pml2 = (virt_addr >> (12 + 9)) & 511;
  uint64_t pml3 = (virt_addr >> (12 + 18)) & 511;
  uint64_t pml4 = (virt_addr >> (12 + 27)) & 511;
  for (; pml4 < 512; pml4++) {
    uint64_t *pml3_addr = NULL;
    if (pml4_addr[pml4] == 0) {
      pml4_addr[pml4] = (uint64_t)kmalloc(1);
      pml3_addr = (uint64_t *)(pml4_addr[pml4] + kernel.hhdm);
      memset((uint8_t *)pml3_addr, 0, 4096);
      pml4_addr[pml4] |=
          KERNEL_PFLAG_PRESENT | KERNEL_PFLAG_WRITE | KERNEL_PFLAG_USER;
    } else {
      pml3_addr = (uint64_t *)(PAGE_ALIGN_DOWN(pml4_addr[pml4]) + kernel.hhdm);
    }

    for (; pml3 < 512; pml3++) {
      uint64_t *pml2_addr = NULL;
      if (pml3_addr[pml3] == 0) {
        pml3_addr[pml3] = (uint64_t)kmalloc(1);
        pml2_addr = (uint64_t *)(pml3_addr[pml3] + kernel.hhdm);
        memset((uint8_t *)pml2_addr, 0, 4096);
        pml3_addr[pml3] |=
            KERNEL_PFLAG_PRESENT | KERNEL_PFLAG_WRITE | KERNEL_PFLAG_USER;
      } else {
        pml2_addr =
            (uint64_t *)(PAGE_ALIGN_DOWN(pml3_addr[pml3]) + kernel.hhdm);
      }

      for (; pml2 < 512; pml2++) {
        uint64_t *pml1_addr = NULL;
        if (pml2_addr[pml2] == 0) {
          pml2_addr[pml2] = (uint64_t)kmalloc(1);
          pml1_addr = (uint64_t *)(pml2_addr[pml2] + kernel.hhdm);
          memset((uint8_t *)pml1_addr, 0, 4096);
          pml2_addr[pml2] |=
              KERNEL_PFLAG_PRESENT | KERNEL_PFLAG_WRITE | KERNEL_PFLAG_USER;
        } else {
          pml1_addr =
              (uint64_t *)(PAGE_ALIGN_DOWN(pml2_addr[pml2]) + kernel.hhdm);
        }
        for (; pml1 < 512; pml1++) {
          pml1_addr[pml1] = phys_addr | flags;
          num_pages--;
          phys_addr += 4096;
          if (num_pages == 0)
            return;
        }
        pml1 = 0;
      }
      pml2 = 0;
    }
    pml3 = 0;
  }

  hcf();
}

void alloc_pages(uint64_t pml4_addr[], uint64_t virt_addr, uint64_t num_pages,
                 uint64_t flags) {
  virt_addr &= ~TOPBITS;
  uint64_t pml1 = (virt_addr >> 12) & 511;
  uint64_t pml2 = (virt_addr >> (12 + 9)) & 511;
  uint64_t pml3 = (virt_addr >> (12 + 18)) & 511;
  uint64_t pml4 = (virt_addr >> (12 + 27)) & 511;
  for (; pml4 < 512; pml4++) {
    uint64_t *pml3_addr = NULL;
    if (pml4_addr[pml4] == 0) {
      pml4_addr[pml4] = (uint64_t)kmalloc(1);
      pml3_addr = (uint64_t *)(pml4_addr[pml4] + kernel.hhdm);
      memset((uint8_t *)pml3_addr, 0, 4096);
      pml4_addr[pml4] |=
          KERNEL_PFLAG_PRESENT | KERNEL_PFLAG_WRITE | KERNEL_PFLAG_USER;
    } else {
      pml3_addr = (uint64_t *)(PAGE_ALIGN_DOWN(pml4_addr[pml4]) + kernel.hhdm);
    }
    for (; pml3 < 512; pml3++) {
      uint64_t *pml2_addr = NULL;
      if (pml3_addr[pml3] == 0) {
        pml3_addr[pml3] = (uint64_t)kmalloc(1);
        pml2_addr = (uint64_t *)(pml3_addr[pml3] + kernel.hhdm);
        memset((uint8_t *)pml2_addr, 0, 4096);
        pml3_addr[pml3] |=
            KERNEL_PFLAG_PRESENT | KERNEL_PFLAG_WRITE | KERNEL_PFLAG_USER;
      } else {
        pml2_addr =
            (uint64_t *)(PAGE_ALIGN_DOWN(pml3_addr[pml3]) + kernel.hhdm);
      }

      for (; pml2 < 512; pml2++) {
        uint64_t *pml1_addr = NULL;
        if (pml2_addr[pml2] == 0) {
          pml2_addr[pml2] = (uint64_t)kmalloc(1);
          pml1_addr = (uint64_t *)(pml2_addr[pml2] + kernel.hhdm);
          memset((uint8_t *)pml1_addr, 0, 4096);
          pml2_addr[pml2] |=
              KERNEL_PFLAG_PRESENT | KERNEL_PFLAG_WRITE | KERNEL_PFLAG_USER;
        } else {
          pml1_addr =
              (uint64_t *)(PAGE_ALIGN_DOWN(pml2_addr[pml2]) + kernel.hhdm);
        }

        for (; pml1 < 512; pml1++) {
          uint64_t phys = (uint64_t)kmalloc(1);
          memset((uint8_t *)(phys + kernel.hhdm), 0, PAGE_SIZE);
          pml1_addr[pml1] = phys | flags;
          num_pages--;
          if (num_pages == 0)
            return;
        }
        pml1 = 0;
      }
      pml2 = 0;
    }
    pml3 = 0;
  }

  hcf();
}

uint64_t *walk_page_table(uint64_t *pml4_addr, uint64_t virt_address) {
  uint64_t pml4_index = (virt_address >> 39) & 0x1FF;
  if (!(pml4_addr[pml4_index] & 1)) {
    return NULL;
  }

  uint64_t *pml3_table =
      (uint64_t *)(PAGE_ALIGN_DOWN(pml4_addr[pml4_index]) + kernel.hhdm);
  uint64_t pml3_index = (virt_address >> 30) & 0x1FF;

  if (!(pml3_table[pml3_index] & 1)) {
    return NULL;
  }

  uint64_t *pml2_table =
      (uint64_t *)(PAGE_ALIGN_DOWN(pml3_table[pml3_index]) + kernel.hhdm);
  uint64_t pml2_index = (virt_address >> 21) & 0x1FF;

  if (!(pml2_table[pml2_index] & 1)) {
    return NULL;
  }

  uint64_t *pml1_table =
      (uint64_t *)(PAGE_ALIGN_DOWN(pml2_table[pml2_index]) + kernel.hhdm);
  uint64_t pml1_index = (virt_address >> 12) & 0x1FF;

  return &pml1_table[pml1_index];
}

void dealloc_pages(uint64_t *pml4_addr, uint64_t virt_addr,
                   uint64_t num_pages) {
  uint64_t aligned_virt_address = PAGE_ALIGN_DOWN(virt_addr);

  for (uint64_t i = 0; i < num_pages; i++) {
    uint64_t current_addr = aligned_virt_address + (i * PAGE_SIZE);

    uint64_t *pte = walk_page_table(pml4_addr, current_addr);

    if (pte == NULL || !(*pte & 1)) {
      continue;
    }

    uint64_t physical_frame_adress = PAGE_ALIGN_DOWN(*pte);

    kfree(physical_frame_adress, 1);

    *pte = 0;

    invlpg((void *)current_addr);
  }
}

void clear_page_cache(uint64_t addr) {
  __asm__ volatile("invlpg (%0)" ::"r"(addr) : "memory");
}

void setup_paging(void) {
  uint64_t pml4_virt = kmalloc(1) + kernel.hhdm;
  memset((uint8_t *)pml4_virt, 0, 4096);
  map_all((uint64_t *)pml4_virt);
  kernel.cr3 = pml4_virt - kernel.hhdm;
}
