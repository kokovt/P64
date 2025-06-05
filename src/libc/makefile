CC=x86_64-elf-gcc
AR=x86_64-elf-ar
CFLAGS=-g \
			 -O2\
			 -pipe\
			 -Wall\
			 -Wextra\
			 -std=gnu11 \
			 -ffreestanding \
			 -fno-stack-protector \
			 -fno-stack-check -fno-PIC \
			 -m64 -march=x86-64 -mno-80387 \
			 -mno-mmx \
			 -mno-sse \
			 -mno-sse2 \
			 -mno-red-zone \
			 -mcmodel=kernel \
			 -nostdlib \
			 -I../../src/p64lib/include \
			 -I../../src/kernel/include/ \
			 -I../../src/kernel/arch/x86_64-elf/

build:
	rm -rf ../../build/libc
	mkdir ../../build/libc
	cd ../../build/libc/; \
	$(CC) -c $(shell find ../../src/libc/ -name '*.c') $(CFLAGS); \
	$(AR) rcs ./libstd.a $(shell find ../../build/libc/ -name '*.o')

