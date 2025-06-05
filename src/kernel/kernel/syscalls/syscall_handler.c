#include "cpu/idt.h"
#include "kernel.h"
#include "mem/paging.h"
#include <io.h>
#include <stdint.h>
#include <syscalls/syscalls.h>

enum syscode {
  ALLOCATE_PAGE,
  DEALLOCATE_PAGE, // This does not exist yet.
};

__attribute__((interrupt)) void syscalls(void *) {
  pushmost();
  uint64_t sys_code;

  uint64_t arg1;
  uint64_t arg2;
  uint64_t arg3;

  asm volatile("mov %%eax %0" : "=r"(sys_code));
  asm volatile("mov %%r8 %0" : "=r"(arg1));
  asm volatile("mov %%r9 %0" : "=r"(arg2));
  asm volatile("mov %%r10 %0" : "=r"(arg3));

  uint64_t result;

  switch (sys_code) {
  case ALLOCATE_PAGE:
    alloc_pages((uint64_t *)kernel.cr3 + kernel.hhdm, arg1, arg2,
                KERNEL_PFLAG_PRESENT | arg3);
    result = 0x0;
    break;

  // WARN: THIS IS UNTESTED. THERE IS NO EVIDENCE THAT THIS WORKS THE WAY IT
  // SHOULD.
  case DEALLOCATE_PAGE:
    dealloc_pages((uint64_t *)kernel.cr3 + kernel.hhdm, arg1, arg3);
    result = 0x0;
    break;

  default:
    result = 0x0;
    break;
  }

  popmost();

  asm volatile("movl %0, %%eax" : : "r"(result));

  outb(0x20, 0x20);
}

void init_syscalls() { set_IDT_entry(0x80, syscalls, 0x8E, kernel.IDT); }
