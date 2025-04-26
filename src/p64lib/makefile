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
			 -I../../src/kernel/include/ \
			 -I../../src/kernel/arch/x86_64-elf/


build:
	rm -rf ../../build/p64lib
	mkdir ../../build/p64lib
	cd ../../build/p64lib/; \
	$(CC) -c ../../src/p64lib/pdraw/draw/fillrect.c ../../src/p64lib/pdraw/draw/putpixel.c ../../src/p64lib/pdraw/draw/writechar.c $(CFLAGS); \
	$(AR) rcs ./p64lib.a ./fillrect.o ./putpixel.o ./writechar.o  


