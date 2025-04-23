#include "../../libc/include/stdio.h"

#include "../include/kernel/tty.h"

#ifdef __cplusplus
extern "C" {
#endif
void kernel_main(void) {
  terminal_initialize();
  printf("P64 TESTING\n");
  printf("New lines work!\n");
  printf("Next task: Fix comments and add terminal scrolling...");
  terminal_editline(1, "Testing terminal line replacing...");
}

#ifdef __cplusplus
}
#endif
