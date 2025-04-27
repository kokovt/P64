#pragma once
#include "sys/cdefs.h"
#include <stddef.h>
#include <stdint.h>

int memcmp(const void *s1, const void *s2, size_t n);
void *memcpy(void *dest, const void *src, size_t n);
void *memmove(void *dest, const void *src, size_t n);
void *memset(void *s, int c, size_t n);
size_t strlen(const char *str);
void reverse(char str[], int length);
void uint64_to_binary_string(uint64_t num, char *buf);
void uint64_to_string(uint64_t num, char *str);
void uint64_to_hex_string_padded(uint64_t num, char *str);
void uint64_to_hex_string(uint64_t num, char *str);
