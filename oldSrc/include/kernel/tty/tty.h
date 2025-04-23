// ----- INCLUDE GUARDS -----
#ifndef TTY_H
#define TTY_H
// ----- DEPENDANCIES -----
#include "../../string/strlen.h"
#include "../../vga/vga.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// ----- DEFINITIONS -----
#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_MEMORY 0xB8000

// ----- FUNCTIONS -----
void terminal_initialize();
void terminal_writestring(const char *data);
void terminal_setcolor(uint8_t color);
#endif
