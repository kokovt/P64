#include "kernel.h"
#include <io.h>
#include <mem/pmm.h>
#include <stdint.h>

struct interrupt_frame {
  uint64_t rip;
  uint64_t cs;
  uint64_t rflags;
  uint64_t rsp;
  uint64_t ss;
};

void pushmost() {
  asm volatile("push %rbx");
  asm volatile("push %rcx");
  asm volatile("push %rdx");
  asm volatile("push %rsi");
  asm volatile("push %rdi");
  asm volatile("push %rbp");
  asm volatile("push %r8");
  asm volatile("push %r9");
  asm volatile("push %r10");
  asm volatile("push %r11");
  asm volatile("push %r12");
  asm volatile("push %r13");
  asm volatile("push %r14");
  asm volatile("push %r15");
}

void popmost() {
  asm volatile("pop %r15");
  asm volatile("pop %r14");
  asm volatile("pop %r13");
  asm volatile("pop %r12");
  asm volatile("pop %r11");
  asm volatile("pop %r10");
  asm volatile("pop %r9");
  asm volatile("pop %r8");
  asm volatile("pop %rbp");
  asm volatile("pop %rdi");
  asm volatile("pop %rsi");
  asm volatile("pop %rdx");
  asm volatile("pop %rcx");
  asm volatile("pop %rbx");
}

unsigned long get_rax() {
  unsigned long rax_value;
  asm("mov %%rax, %0" : "=r"(rax_value));
  return rax_value;
}

__attribute__((interrupt)) void default_handler(void *) {
  pushmost();

  popmost();
  outb(0x20, 0x20);
}

void set_IDT_entry(uint32_t vector, void *isr, uint8_t flags, IDTEntry *IDT) {
  IDT[vector].offset1 = (uint64_t)isr;
  IDT[vector].offset2 = ((uint64_t)isr) >> 16;
  IDT[vector].offset3 = ((uint64_t)isr) >> 32;
  IDT[vector].flags = flags;
  IDT[vector].segment_selector = 0x08;
}

void init_idt(void) {
  kernel.IDT = (IDTEntry *)(kmalloc(1) + kernel.hhdm);

  set_IDT_entry(0x80, (void *)&default_handler, 0xEF, kernel.IDT);

  IDTR idtr = (IDTR){
      .size = (sizeof(IDTEntry) * 256) - 1,
      .offset = (uint64_t)kernel.IDT,
  };

  asm volatile("lidt %0" : : "m"(idtr));
}
