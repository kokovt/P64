# P64

## A basic c/c++ kernel

The goal is to make this a **Exokernel**

### Current drawbacks

Maximum of 256 applications pulling keyboard information. Probably not an issue, but it is a limitation.

### Features

## Building

Follow [this tutorial](wiki.osdev.org/GCC_Cross-Compiler) to build a cross compiler. Make sure to follow the instructions for a x86_64 build.

Once you build the cross compiler, add it to your path (Make sure its called x86_64-elf-(xxx))

after that, just run make all and it should work, and after you run make all you can just run make in the future.

It will automatically launch in Qemu
