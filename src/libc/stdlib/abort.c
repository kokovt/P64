#include "../include/stdio.h"
#include "../include/stdlib.h"

__attribute__((__no_return__)) void abort(void) {
#if defined(__is_lbk)
  printf(kernel:panic : abort());
#else
  printf("abort()\n");
#endif
  while (1) {
  }
  __builtin_unreachable();
}
