#ifndef LIBC_STDIO_H
#define LIBC_STDIO_H

#include "../../p64lib/pdraw/include/draw.h"
#include "sys/cdefs.h"
#include <stddef.h>
#include <stdint.h>

#define EOF (-1)

#ifdef __cplusplus
extern "C" {
#endif

int printf(const char *__restrict, ...);
int putchar(int);

#ifdef __cplusplus
}
#endif

#endif
