#include <io.h>

// THIS IS HEAVILY BASED OFF OF UNMAPPEDSTACK/TACOS
// Check them out!
// https://github.com/UnmappedStack/TacOS/blob/beee6216b58ee07ad39ac5e5350384cfb39d1aaa/src/mem/paging.c
// This is under a different license from this project.

uint8_t inb(uint16_t port) {
  uint8_t ret;
  __asm__ volatile("inb %w1, %b0" : "=a"(ret) : "Nd"(port) : "memory");
  return ret;
}

void outb(uint16_t port, uint8_t val) {
  __asm__ volatile("outb %b0, %w1" : : "a"(val), "Nd"(port) : "memory");
}
