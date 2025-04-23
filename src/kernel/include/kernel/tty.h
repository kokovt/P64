#ifndef _KERNEL_TTY_H
#define _KERNEL_TTY_H

#ifdef __cplusplus
extern "C" {
#endif
#include <stddef.h>
void terminal_initialize(void);
void terminal_putchar(char c);
void terminal_write(const char *data, size_t size);
void terminal_writestring(const char *data);
void terminal_clear();
void terminal_clearline(size_t line);
void terminal_editline(size_t line, const char *replacement_text);

#ifdef __cplusplus
}
#endif

#endif
