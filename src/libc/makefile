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
	$(CC) -c ../../src/libc/stdio/printf.c \
	../../src/libc/stdio/putchar.c \
	../../src/libc/string/memcmp.c \
	../../src/libc/string/memcpy.c \
	../../src/libc/string/memmove.c \
	../../src/libc/string/memset.c \
	../../src/libc/string/strlen.c \
	../../src/libc/string/reverse.c \
	../../src/libc/string/uint64_to_binary_string.c \
	../../src/libc/string/uint64_to_hex_string_padded.c \
	../../src/libc/string/uint64_to_string.c \
	../../src/libc/string/uint64_to_hex_string.c $(CFLAGS); \
	$(AR) rcs ./libstd.a ./memcmp.o ./memcpy.o ./memmove.o ./memset.o ./printf.o ./putchar.o ./strlen.o ./uint64_to_string.o ./uint64_to_hex_string_padded.o ./uint64_to_binary_string.o ./uint64_to_hex_string.o ./reverse.o

