CC=x86_64-elf-gcc
as=nasm
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
			 -I../../src/kernel/arch/x86_64-elf/ \
			 -I../../src/libc/include/ \
			 -I../../src/p64lib/include/ 

LDFLAGS=-T ../../src/kernel/arch/x86_64-elf/linker.ld \
				-nostdlib \
				-static \
				-z max-page-size=0x1000 \
				-L../libc/ \
				-L../p64lib/ \
				-l:libstd.a \
				-l:p64lib.a

ASFLAGS=-Wall \
		-f elf64

build:
	rm -rf ../../build/kernel
	mkdir ../../build/kernel
	cd ../../build/kernel/; \
	$(CC) -c $(shell find ../../src/kernel/ -name '*.c') $(CFLAGS); \
	$(as) ../../src/kernel/kernel/mem/reloadgdt.asm -o ./reloadgdt.o $(ASFLAGS); \
	$(as) ../../src/kernel/kernel/cpu/sse.asm -o ./sse.o $(ASFLAGS); \
	$(CC) $(shell find ../../build/kernel/ -name '*.o') -o p64.bin $(LDFLAGS)


