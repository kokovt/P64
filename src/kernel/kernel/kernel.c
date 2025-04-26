// ----- DEPENDANCIES -----
#include "./kernel.h"
#include "../../libc/include/stdio.h"
#include "../../p64lib/pdraw/include/draw.h"
#include "../arch/x86_64-elf/limine.h"
#include "../include/hcf.h"
#include "../include/mem/paging.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// ----- ATTRIBUTES -----
__attribute__((
    used, section(".limine_requests"))) static volatile LIMINE_BASE_REVISION(3);

__attribute__((
    used,
    section(
        ".limine_requests"))) static volatile struct limine_framebuffer_request
    framebuffer_request = {.id = LIMINE_FRAMEBUFFER_REQUEST, .revision = 0};

__attribute__((used,
               section(".limine_requests_"
                       "start"))) static volatile LIMINE_REQUESTS_START_MARKER;

__attribute__((
    used,
    section(
        ".limine_requests_end"))) static volatile LIMINE_REQUESTS_END_MARKER;

void kmain(void) {
  if (LIMINE_BASE_REVISION_SUPPORTED == false) {
    hcf();
  }

  if (framebuffer_request.response == NULL ||
      framebuffer_request.response->framebuffer_count < 1) {
    hcf();
  }

  setuppaging();

  printf("P64 TESTING!\n");
  printf("TESTING NEW LINES\n");
  fillrect(100, 100, 100, 100, 0xfffff);
  hcf();
}

struct limine_framebuffer *getframebuffer() {
  return framebuffer_request.response->framebuffers[0];
}
