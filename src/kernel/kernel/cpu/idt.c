#include <stdint.h>

struct idt_entry {
  uint16_t offset_low;
  uint16_t selector;
  uint8_t ist; // usually 0
  uint8_t type_attr;
  uint16_t offset_mid;
  uint32_t offset_high;
  uint32_t zero;
} __attribute__((packed));

struct idt_ptr {
  uint16_t limit;
  uint64_t base;
} __attribute__((packed));

#define IDT_ENTRIES 256
struct idt_entry idt[IDT_ENTRIES];

__attribute__((interrupt)) void default_handler(void *frame) {
  while (1) {
    asm volatile("cli; hlt");
  }
}

void set_idt_entry(int vec, void (*handler)(void *), uint8_t type) {
  uint64_t addr = (uint64_t)handler;
  idt[vec] = (struct idt_entry){.offset_low = addr & 0xFFFF,
                                .selector = 0x08, // kernel code segment
                                .ist = 0,
                                .type_attr = type, // e.g. 0x8E = interrupt gate
                                .offset_mid = (addr >> 16) & 0xFFFF,
                                .offset_high = (addr >> 32) & 0xFFFFFFFF,
                                .zero = 0};
}

void init_idt(void) {
  for (int i = 0; i < IDT_ENTRIES; i++) {
    set_idt_entry(i, default_handler, 0x8E);
  }

  struct idt_ptr idtp = {.limit = sizeof(idt) - 1, .base = (uint64_t)&idt};

  asm volatile("lidt %0" : : "m"(idtp));
}
