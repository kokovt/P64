#pragma once
#include "kernel.h"
#include <stdbool.h>
#include <stdint.h>

struct page_binding {
  uint64_t physical_page;
  uint64_t pid;
  uint8_t capabilities;
  bool bound;
};

long sysBindPage(uint64_t addr, uint64_t capabilities);
long sysUnbindPage(uint64_t addr);
