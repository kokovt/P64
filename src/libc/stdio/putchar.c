#include "../include/stdio.h"
#include "draw.h"

int putchar(int ic) {
  char c = (char)ic;

  if (c == '\b') {
    if (kernel.term_column == 0)
      return ic;
    kernel.term_column -= 1;
    writechar('\0', kernel.term_column * 8, kernel.term_row * 16, 0xffffff);
    return ic;
  }

  if (c == '\n') {
    kernel.term_column = 0;
    kernel.term_row += 1;
    kernel.term_text += c;
    return ic;
  }

  writechar(c, kernel.term_column * 8, kernel.term_row * 16, 0xffffff);

  kernel.term_column += 1;
  kernel.term_text += c;

  return ic;
}
