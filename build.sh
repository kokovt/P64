# I cant get makefiles to work right now, make my own through bash

echo ----- CREATING VARIABLES ------
export __is_libk=1

echo ----- CLEANING -----

rm -rf build
rm -rf isodir

mkdir build
mkdir -p isodir/boot/grub

echo ----- BUILDING FILES -----

# BUILDING BINARIES
echo ----- BUILDING ASSEMBLY -----
echo Building boot.s
i686-elf-as ./src/kernel/arch/i386/boot.s -o ./build/boot.o
echo Building crti.s
i686-elf-as ./src/kernel/arch/i386/crti.s -o ./build/crti.o
echo Building crtn.s
i686-elf-as ./src/kernel/arch/i386/crtn.s -o ./build/crtn.o

# BUILDING C FILES
echo ----- BUILDING C FILES ------
echo Building printf.c
i686-elf-gcc -c ./src/libc/stdio/printf.c -o ./build/printf.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
echo Building abort.c
i686-elf-gcc -c ./src/libc/stdlib/abort.c -o ./build/abort.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
echo Building memcpy.c
i686-elf-gcc -c ./src/libc/string/memcpy.c -o ./build/memcpy.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

# BUILDING CPP FILES
echo ----- BUILDING CPP FILES -----
echo Building tty.cpp
i686-elf-g++ -c ./src/kernel/arch/i386/tty.cpp -o ./build/tty.o -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti
echo Building kernel.cpp
i686-elf-g++ -c ./src/kernel/kernel/kernel.cpp -o ./build/kernel.o -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti
echo Building putchar.cpp
i686-elf-g++ -D'___is_libk(x)' -c ./src/libc/stdio/putchar.cpp -o ./build/putchar.o -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti
echo Building puts.cpp
i686-elf-g++ -c ./src/libc/stdio/puts.cpp -o ./build/puts.o -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti
echo Building memcmp.cpp
i686-elf-g++ -c ./src/libc/string/memcmp.cpp -o ./build/memcmp.o -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti
echo Building memmove.cpp
i686-elf-g++ -c ./src/libc/string/memmove.cpp -o ./build/memmove.o -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti
echo Building memset.cpp
i686-elf-g++ -c ./src/libc/string/memset.cpp -o ./build/memset.o -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti
echo Building strlen.cpp
i686-elf-g++ -c ./src/libc/string/strlen.cpp -o ./build/strlen.o -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti

# LINKING FILES
echo ----- LINKING FILES -----
i686-elf-gcc -T ./src/kernel/arch/i386/linker.ld -o build/p64.bin -ffreestanding -O2 -nostdlib ./build/kernel.o ./build/crti.o ./build/crtn.o ./build/printf.o ./build/abort.o ./build/memcpy.o ./build/tty.o \
  ./build/putchar.o ./build/puts.o ./build/memcmp.o ./build/memmove.o ./build/memset.o ./build/strlen.o ./build/boot.o

# CHECKING MULTIBOOT
echo ----- VERIFYING MULTIBOOT -----
if grub-file --is-x86-multiboot ./build/p64.bin; then
  echo Multiboot confirmed
else
  echo Failed multiboot check, panicking.
  exit
fi

# BUILD THE ISO DIR
echo ----- SETTING UP ISO DIR -----
cp ./build/p64.bin ./isodir/boot/p64.bin
cp ./src/grub.cfg ./isodir/boot/grub/grub.cfg

# BUILD THE ISO
echo ----- BUILDING THE ISO -----
grub-mkrescue -o ./build/p64.iso isodir

# RUN IN QEMU
echo ----- RUNNING IN QEMU ------
qemu-system-i386 -cdrom ./build/p64.iso
