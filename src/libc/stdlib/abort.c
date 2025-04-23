#include "../include/stdio.h"
#include "../include/stdlib.h"

#define __is_lbk

void abort(void) {
#if defined(__is_lbk)
  printf("kernel:panic : abort()");
#else
  printf("abort()\n");
#endif
  while (1) {
  }
  __builtin_unreachable();
}
