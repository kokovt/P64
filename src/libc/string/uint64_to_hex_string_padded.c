#include "../include/string.h"

void uint64_to_hex_string_padded(uint64_t num, char *str) {
  char buffer[17];
  int index = 0;
  if (num == 0) {
    buffer[index++] = '0';
  } else {
    while (num > 0) {
      uint8_t digit = num & 0xF;
      if (digit < 10) {
        buffer[index++] = '0' + digit;
      } else {
        buffer[index++] = 'A' + (digit - 10);
      }
      num >>= 4;
    }
  }

  while (index < 16)
    buffer[index++] = '0';

  buffer[index] = '\0';
  reverse(buffer, index);
  memcpy(str, buffer, 17);
}
