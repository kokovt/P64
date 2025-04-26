#pragma once
#include <list.h>
#include <stddef.h>
#include <stdint.h>

typedef struct {
  struct list list;
  size_t length;
} Chunk;

uintptr_t kmalloc(size_t num_pages);
