# I am using an sh script to make sure it is using the right gcc.
echo If you added any files you need to modify this script.

# Remove the isodir and build dir to make sure its a clean build
rm -rf build
rm -rf isodir

mkdir build

# build the asm files
echo Building Assembly files...
i686-elf-as ./src/boot.s -o ./build/boot.o

# build the c files
echo Building the C files...
#i686-elf-gcc -c ./src/include/kernel/tty/terminalScroll.c -o ./build/terminalScroll.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

# build the c++ files
echo Building the C++ files...
i686-elf-g++ -c ./src/include/kernel/kernel.cpp -o ./build/kernel.o -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti
i686-elf-g++ -c ./src/include/kernel/tty/tty.cpp -o ./build/tty.o -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti

# Link the files
echo Linking the files...
i686-elf-gcc -T linker.ld -o ./build/p64.bin -ffreestanding -O2 -nostdlib ./build/boot.o ./build/tty.o ./build/kernel.o -lgcc

# build isodir
mkdir -p isodir/boot/grub

# Build grub environment
echo Setting up the grub environment...

if grub-file --is-x86-multiboot ./build/p64.bin; then
  cp build/p64.bin ./isodir/boot/p64.bin
  cp ./src/grub.cfg ./isodir/boot/grub/grub.cfg
  echo Building iso...
  grub-mkrescue -o ./build/p64.iso isodir

  should_launch="y"
  echo "Would you like to launch the ISO in Qemu? (Y/n) "
  read -r should_launch

  if [ "$should_launch" = "n" ]; then
    exit 1
  fi

  qemu-system-i386 -cdrom ./build/p64.iso
else
  echo The file failed the multiboot check, aborting.
fi
