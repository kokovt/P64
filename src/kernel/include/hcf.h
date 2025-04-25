// ----- INCLUDE GUARDS -----
#ifndef _KERNEL_HLT_H
#define _KERNEL_HLT_H

#ifdef __cplusplus
extern "C" {
#endif
static void hcf(void) {
  for (;;) {
    asm("hlt");
  }
}
#ifdef __cplusplus
}
#endif
#endif
