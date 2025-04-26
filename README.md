# P64

## A basic c/c++ kernel

### Current features

Rendering text to the screen.

New lines.

Rendering squares

## Building

Follow [this tutorial](wiki.osdev.org/GCC_Cross-Compiler) to build a cross compiler. Make sure to follow the instructions for a x86_64 build.

Once you build the cross compiler, add it to your path (Make sure its called x86_64-elf-(xxx))

after that, just run make all and it should work, and after you run make all you can just run make in the future.

It will automatically launch in Qemu
