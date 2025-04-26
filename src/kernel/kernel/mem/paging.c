#include <mem/paging.h>
#include <stdint.h>

uint32_t page_directory[1024] __attribute__((aligned(4096)));

void *get_physaddr(void *virtualaddr) {
  unsigned long pdindex = (unsigned long)virtualaddr >> 22;
  unsigned long ptindex = (unsigned long)virtualaddr >> 12 & 0x03FF;

  unsigned long *pd = (unsigned long *)0xFFFFF000;
  unsigned long *pt = ((unsigned long *)0xFFC00000) + (0x400 * pdindex);

  return (void *)((pt[ptindex] & ~0xFFF) +
                  ((unsigned long)virtualaddr & 0xFFF));
}

void map_page(void *physaddr, void *virtualaddr, unsigned int flags) {
  unsigned long pdindex = (unsigned long)virtualaddr >> 22;
  unsigned long ptindex = (unsigned long)virtualaddr >> 12 & 0x03FF;

  unsigned long *pd = (unsigned long *)0xFFFFF000;
  unsigned long *pt = ((unsigned long *)0xFFC00000) + (0x400 * pdindex);

  pt[ptindex] = ((unsigned long)physaddr) | (flags & 0xFFF) | 0x01;
}

void setuppaging(void) {
  for (int i = 0; i < 1024; i++) {
    page_directory[i] = 0x00000002;
  }

  uint32_t first_page_table[1024] __attribute__((aligned(4096)));

  for (int i = 0; i < 1024; i++) {

    first_page_table[i] = (i * 0x1000) | 3;
  }

  page_directory[0] = ((unsigned int)first_page_table) | 3;
}
