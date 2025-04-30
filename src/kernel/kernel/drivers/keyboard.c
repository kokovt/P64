#include <drivers/keyboard.h>
#include <io.h>
#include <kernel.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

#define PS2_DATA_REG 0x60
#define PS2_STATUS_REG 0x64
#define PS2_COMMAND_REG 0x64

uint8_t scancode_event_queue[30];
uint8_t nkbevents = 0;

char character_table[] = {
    0,    0,    '1',  '2',  '3',  '4',  '5',  '6',  '7',  '8',  '9',  '0',
    '-',  '=',  0,    0x09, 'q',  'w',  'e',  'r',  't',  'y',  'u',  'i',
    'o',  'p',  '[',  ']',  0,    0,    'a',  's',  'd',  'f',  'g',  'h',
    'j',  'k',  'l',  ';',  '\'', '`',  0,    '\\', 'z',  'x',  'c',  'v',
    'b',  'n',  'm',  ',',  '.',  '/',  0,    '*',  0,    ' ',  0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0x1B, 0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0x0E, 0x1C, 0,    0,    0,
    0,    0,    0,    0,    0,    '/',  0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0x1E, 0x1F, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0,
    0,    0,    0,    0,    0,    0,    0,    0x2C};

char shifted_character_table[] = {
    0,    0,    '!',  '@',  '#',  '$',  '%',  '^',  '&',  '*',  '(',  ')',
    '_',  '+',  0,    0x09, 'Q',  'W',  'E',  'R',  'T',  'Y',  'U',  'I',
    'O',  'P',  '{',  '}',  0,    0,    'A',  'S',  'D',  'F',  'G',  'H',
    'J',  'K',  'L',  ':',  '"',  '~',  0,    '|',  'Z',  'X',  'C',  'V',
    'B',  'N',  'M',  '<',  '>',  '?',  0,    '*',  0,    ' ',  0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0x1B, 0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0x0E, 0x1C, 0,    0,    0,
    0,    0,    0,    0,    0,    '?',  0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0x1E, 0x1F, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0,
    0,    0,    0,    0,    0,    0,    0,    0x2C,
};

typedef struct {
  uint64_t input_len;
  bool currently_reading;
  bool shifted;
  bool caps;
  char *current_buffer;
  uint64_t buffer_size;
} KeyboardData;

bool shifted = false;
bool caps = false;

__attribute__((interrupt)) void keyboard_isr(void *) {
  // printf("Keybod");
  if (!inb(PS2_STATUS_REG))
    goto ret;

  uint8_t scancode = inb(PS2_DATA_REG);

  if (scancode & 0x80 || scancode == 0x5B || scancode == 1) {
    if (scancode == 0xAA || scancode == 0xB6)
      shifted = false;
    goto ret;
  }

  if (scancode == 0x3A) {
    caps = !caps;
    goto ret;
  }

  if (scancode == 0x0E) {
    kernel.last_pressed = '\b';

    kernel.keypress_callback();
    goto ret;
  }

  if (scancode == 0x2A || scancode == 0x36) {
    shifted = true;
    goto ret;
  }

  if (scancode == 0x1C) {
    kernel.last_pressed = '\n';

    kernel.keypress_callback();
    goto ret;
  }

  char ch;
  bool a_z_test =
      character_table[scancode] >= 'a' && character_table[scancode] <= 'z';

  if (shifted && !caps) {
    ch = shifted_character_table[scancode];
  } else if (shifted && caps) {
    if (a_z_test)
      ch = character_table[scancode];
    else
      ch = shifted_character_table[scancode];
  } else if (!shifted && caps) {
    if (a_z_test)
      ch = shifted_character_table[scancode];
    else
      ch = character_table[scancode];
  } else {
    ch = character_table[scancode];
  }

  kernel.last_pressed = ch;
  kernel.keypress_callback();
  goto ret;

ret:
  outb(0x20, 0x20);
  outb(0xA0, 0x20);
}

void init_keyboard() {
  set_IDT_entry(33, (void *)keyboard_isr, 0x8E, kernel.IDT);
}
