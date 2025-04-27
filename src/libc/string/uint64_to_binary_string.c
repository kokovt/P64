#include "../include/string.h"

void uint64_to_binary_string(uint64_t num, char *buf) {
  char buffer[65];
  int idx = 0;
  if (num == 0) {
    buffer[idx++] = '0';
  } else {
    while (num > 0) {
      buffer[idx++] = (num & 1) ? '1' : '0';
      num >>= 1;
    }
  }
  buffer[idx] = 0;
  reverse(buffer, idx);
  for (int i = 0; i <= idx; i++)
    buf[i] = buffer[i];
}
