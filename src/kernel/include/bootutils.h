#pragma once
#include <hcf.h>
#include <kernel.h>
#include <limine.h>

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

__attribute__((
    used,
    section(".limine_requests"))) static volatile struct limine_hhdm_request
    hhdm_request = {.id = LIMINE_HHDM_REQUEST, .revision = 0};

__attribute((used, section(".limine_requests"))) struct limine_memmap_request
    memmap_request = {.id = LIMINE_MEMMAP_REQUEST, .revision = 0};

__attribute((used,
             section(".limine_requests"))) struct limine_kernel_address_request
    kernel_address_request = {.id = LIMINE_KERNEL_ADDRESS_REQUEST,
                              .revision = 0};

__attribute__((used,
               section(".limine_requests_"
                       "start"))) static volatile LIMINE_REQUESTS_START_MARKER;

__attribute__((used,
               section(".limine_requests_"
                       "end"))) static volatile LIMINE_REQUESTS_END_MARKER;

inline static void init_kernel() {
  if (LIMINE_BASE_REVISION_SUPPORTED == false) {
    hcf();
  }

  if (framebuffer_request.response == NULL ||
      framebuffer_request.response->framebuffer_count < 1) {
    hcf();
  }

  if (hhdm_request.response == NULL)
    hcf();

  kernel.framebuffer = framebuffer_request.response->framebuffers[0];
  kernel.hhdm = hhdm_request.response->offset;
  kernel.kernel_phys_addr = kernel_address_request.response->physical_base;
  kernel.kernel_virt_addr = kernel_address_request.response->virtual_base;
  kernel.memmap = *(memmap_request.response->entries);
  kernel.memmap_entry_count = memmap_request.response->entry_count;

  kernel.term_column = 0;
  kernel.term_row = 0;
}
