#pragma once
#include <stddef.h>
#include <stdint.h>

#define PAGE_SIZE 4096

#define KERNEL_STACK_PAGES 10LL
#define KERNEL_STACK_PTR 0xFFFFFFFFFFFFF000LL
#define KERNEL_STACK_ADDR KERNEL_STACK_PTR - (KERNEL_STACK_PAGES * PAGE_SIZE)

#define KERNEL_PFLAG_PRESENT 0b00000001
#define KERNEL_PFLAG_WRITE 0b00000010
#define KERNEL_PFLAG_USER 0b00000100
#define KERNEL_PFLAG_WRITE_COMBINE 0b10000000

#define PAGE_ALIGN_UP(x) ((((x) + 4095) / 4096) * 4096)
#define PAGE_ALIGN_DOWN(addr) ((addr / 4096) * 4096)
void setup_paging(void);
void map_pages(uint64_t pml4_addr[], uint64_t virt_addr, uint64_t phys_addr,
               uint64_t num_pages, uint64_t flags);
void alloc_pages(uint64_t pml4_addr[], uint64_t virt_addr, uint64_t num_pages,
                 uint64_t flags);
