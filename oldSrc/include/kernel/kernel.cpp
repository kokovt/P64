// ----- DEPENDANCIES -----
#include "./tty/tty.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// ----- MAKE SURE THIS IS BEING CROSS COMPILED INTO x86
#if defined(__linux__)
#error                                                                         \
    "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

#if !defined(__i386__)
#error "This needs to be compiled with a ix86-elf compiler"
#endif

// ----- FUNCTIONS -----

// We need this to be c linked to avoid name-mangling
#ifdef __cplusplus
extern "C" {
#endif
void kernel_main(void) {
  /* Initialize terminal interface */
  terminal_initialize();

  /* Newline support is left as an exercise. */
  terminal_writestring("P64 TESTING");
  terminal_writestring("\[\0\0dhwuiad");
}
#ifdef __cplusplus
}
#endif
