CC=x86_64-elf-gcc
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

build:
	rm -rf ../../build/kernel
	mkdir ../../build/kernel
	cd ../../build/kernel/; \
	$(CC) -c ../../src/kernel/kernel/entry.c ../../src/kernel/kernel/mem/paging.c ../../src/kernel/kernel/mem/pmm.c $(CFLAGS); \
	$(CC) ./entry.o ./paging.o ./pmm.o -o p64.bin $(LDFLAGS)
	


