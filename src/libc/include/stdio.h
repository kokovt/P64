#pragma once
#include "../include/string.h"
#include "sys/cdefs.h"
#include <draw.h>
#include <limits.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define EOF (-1)

int printf(const char *__restrict, ...);
int putchar(int);
