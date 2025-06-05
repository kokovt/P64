#include "../include/string.h"

int get_num_length(uint64_t num) {
  int length = 0;
  do {
    length++;
    num /= 10;
  } while (num > 0);
  return length;
}

void uint64_to_string(uint64_t num, char *str) {
  int length = get_num_length(num);
  str[length] = '\0';
  int index = length - 1;
  do {
    str[index--] = (num % 10) + '0';
    num /= 10;
  } while (num > 0);
}
