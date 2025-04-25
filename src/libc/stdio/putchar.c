#include "../include/stdio.h"

int putchar(int ic) {
  char c = (char)ic;

  writechar(c, 0xffffff);

  return ic;
}
