#include <cpu/tss.h>
#include <kernel.h>
#include <mem/paging.h>
#include <mem/pmm.h>

void init_tss() {
  kernel.tss = (TSS *)(kmalloc(1) + kernel.hhdm);
  kernel.tss->rsp0 = KERNEL_STACK_PTR;
}
