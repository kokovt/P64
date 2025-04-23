// ----- DEPENDANCIES -----
#include "./tty.h"

// ----- VARIABLES -----
size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color = 15;
uint16_t *terminal_buffer = (uint16_t *)VGA_MEMORY;
size_t remaining_array_size = VGA_HEIGHT;
size_t lineColors[VGA_HEIGHT];

enum vga_color BACKGROUND_COLOR = VGA_COLOR_BLACK;
enum vga_color FORGROUND_COLOR = VGA_COLOR_WHITE;

// Array of what is being logged to the terminal
// Max of 50 lines, change later
const char *data[VGA_HEIGHT];
const char *blankCharPointer[2];

// ----- FUNCTIONS -----
void terminal_setcolor(uint8_t color) { terminal_color = color; }

void terminal_settodefaultcolor() {
  terminal_color = vga_entry_color(FORGROUND_COLOR, BACKGROUND_COLOR);
}

void terminal_setenumcolor(enum vga_color fg, enum vga_color bg) {
  terminal_color = vga_entry_color(fg, bg);
}

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) {
  const size_t index = y * VGA_WIDTH + x;
  terminal_buffer[index] = vga_entry(c, color);
}

void terminal_clear() {
  for (uint8_t i = 0; i < VGA_HEIGHT; i++) {
    for (uint8_t x = 0; x < VGA_WIDTH; x++) {
      terminal_column = x;
      terminal_row = i;

      terminal_putentryat(' ', terminal_color, x, i);
    }
  }
}

void terminal_putchar(char c) {
  if (c == '\n') {
    terminal_column = 0;
    terminal_row += 1;
    return;
  }

  terminal_putentryat(c, terminal_color, terminal_column, terminal_row);

  if (++terminal_column == VGA_WIDTH) {
    terminal_column = 0;
    if (++terminal_row == VGA_HEIGHT) {
      for (int line = 1; line <= VGA_HEIGHT - 1; line++) {
      }

      terminal_row = VGA_HEIGHT - 1;
    }
  }
}

void terminal_write(const char *data, size_t size) {
  for (size_t i = 0; i < size; i++) {
    terminal_putchar(data[i]);
  }
}

void terminal_writestring(const char *stringData) {

  if (remaining_array_size == 0) {
    for (int8_t i = 50; i >= 0; i--) {
      if (i != 0) {
        data[i] = data[i - 1];
      } else {
        data[i] = stringData;
      }
    }
  } else {
    data[remaining_array_size] = stringData;
    remaining_array_size = remaining_array_size - 1;
  }

  terminal_column = 0;
  terminal_row = 0;

  for (int8_t i = 50; i >= 0; i--) {
    if (data[i] == blankCharPointer[1]) {
      continue;
    }
    terminal_write(data[i], strlen(data[i]));
  }
}

void terminal_initialize(void) {
  terminal_row = 0;
  terminal_column = 0;
  terminal_color = vga_entry_color(FORGROUND_COLOR, BACKGROUND_COLOR);
  for (size_t y = 0; y < VGA_HEIGHT; y++) {
    for (size_t x = 0; x < VGA_WIDTH; x++) {
      const size_t index = y * VGA_WIDTH + x;
      terminal_buffer[index] = vga_entry(' ', terminal_color);
    }
  }
}
