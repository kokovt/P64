echo ----- BUILDING P64: x86-64 -----

echo ----- CLEANUP ------
echo Cleaning ./iso_root
rm -rf ./iso_root/

echo ----- BUILDING LIMINE -----
make -C limine

echo ----- BUILDING PATHS ------
echo Building ./build/
mkdir ./build/
echo Building ./iso_root/
mkdir ./iso_root/
echo Building ./iso_root/boot/
mkdir ./iso_root/boot/
echo Building ./iso_root/boot/limine/
mkdir ./iso_root/boot/limine/
echo Building ./iso_root/EFI/
mkdir ./iso_root/EFI/
echo Building ./iso_root/EFI/BOOT/
mkdir ./iso_root/EFI/BOOT/

echo ----- BUILDING C FILES -----
echo Building kernel.c
x86_64-elf-gcc -c ./src/kernel/kernel/kernel.c -o ./build/kernel.o -g -O2 -pipe -Wall -Wextra -std=gnu11 -ffreestanding -fno-stack-protector -fno-stack-check -fno-PIC -m64 -march=x86-64 -mno-80387 -mno-mmx -mno-sse -mno-sse2 -mno-red-zone -mcmodel=kernel -nostdlib

echo ----- BUILDING LIBRARIES -----
read -p "Do you want to build the libraries? (y/N)
If this is your first time running this, I recommend building them.
" -n 1 -r
echo " "
if [[ $REPLY =~ ^[Yy]$ ]]; then
  echo Building memcmp.c
  x86_64-elf-gcc -c ./src/libc/string/memcmp.c -o ./build/memcmp.o -g -O2 -pipe -Wall -Wextra -std=gnu11 -ffreestanding -fno-stack-protector -fno-stack-check -fno-PIC -m64 -march=x86-64 -mno-80387 -mno-mmx -mno-sse -mno-sse2 -mno-red-zone -mcmodel=kernel -nostdlib
  echo Building memcpy.c
  x86_64-elf-gcc -c ./src/libc/string/memcpy.c -o ./build/memcpy.o -g -O2 -pipe -Wall -Wextra -std=gnu11 -ffreestanding -fno-stack-protector -fno-stack-check -fno-PIC -m64 -march=x86-64 -mno-80387 -mno-mmx -mno-sse -mno-sse2 -mno-red-zone -mcmodel=kernel -nostdlib
  echo Building memmove.c
  x86_64-elf-gcc -c ./src/libc/string/memmove.c -o ./build/memmove.o -g -O2 -pipe -Wall -Wextra -std=gnu11 -ffreestanding -fno-stack-protector -fno-stack-check -fno-PIC -m64 -march=x86-64 -mno-80387 -mno-mmx -mno-sse -mno-sse2 -mno-red-zone -mcmodel=kernel -nostdlib
  echo Building memset.c
  x86_64-elf-gcc -c ./src/libc/string/memset.c -o ./build/memset.o -g -O2 -pipe -Wall -Wextra -std=gnu11 -ffreestanding -fno-stack-protector -fno-stack-check -fno-PIC -m64 -march=x86-64 -mno-80387 -mno-mmx -mno-sse -mno-sse2 -mno-red-zone -mcmodel=kernel -nostdlib
  echo Building putpixel.c
  x86_64-elf-gcc -c ./src/p64lib/pdraw/draw/putpixel.c -o ./build/putpixel.o -g -O2 -pipe -Wall -Wextra -std=gnu11 -ffreestanding -fno-stack-protector -fno-stack-check -fno-PIC -m64 -march=x86-64 -mno-80387 -mno-mmx -mno-sse -mno-sse2 -mno-red-zone -mcmodel=kernel -nostdlib
  echo Building fillrect.c
  x86_64-elf-gcc -c ./src/p64lib/pdraw/draw/fillrect.c -o ./build/fillrect.o -g -O2 -pipe -Wall -Wextra -std=gnu11 -ffreestanding -fno-stack-protector -fno-stack-check -fno-PIC -m64 -march=x86-64 -mno-80387 -mno-mmx -mno-sse -mno-sse2 -mno-red-zone -mcmodel=kernel -nostdlib
  echo Building writechar.c
  x86_64-elf-gcc -c ./src/p64lib/pdraw/draw/writechar.c -o ./build/writechar.o -g -O2 -pipe -Wall -Wextra -std=gnu11 -ffreestanding -fno-stack-protector -fno-stack-check -fno-PIC -m64 -march=x86-64 -mno-80387 -mno-mmx -mno-sse -mno-sse2 -mno-red-zone -mcmodel=kernel -nostdlib
  echo Building putchar.c
  x86_64-elf-gcc -c ./src/libc/stdio/putchar.c -o ./build/putchar.o -g -O2 -pipe -Wall -Wextra -std=gnu11 -ffreestanding -fno-stack-protector -fno-stack-check -fno-PIC -m64 -march=x86-64 -mno-80387 -mno-mmx -mno-sse -mno-sse2 -mno-red-zone -mcmodel=kernel -nostdlib
  echo Building printf.c
  x86_64-elf-gcc -c ./src/libc/stdio/printf.c -o ./build/printf.o -g -O2 -pipe -Wall -Wextra -std=gnu11 -ffreestanding -fno-stack-protector -fno-stack-check -fno-PIC -m64 -march=x86-64 -mno-80387 -mno-mmx -mno-sse -mno-sse2 -mno-red-zone -mcmodel=kernel -nostdlib
  echo Building strlen.c
  x86_64-elf-gcc -c ./src/libc/string/strlen.c -o ./build/strlen.o -g -O2 -pipe -Wall -Wextra -std=gnu11 -ffreestanding -fno-stack-protector -fno-stack-check -fno-PIC -m64 -march=x86-64 -mno-80387 -mno-mmx -mno-sse -mno-sse2 -mno-red-zone -mcmodel=kernel -nostdlib

fi

echo ----- LINKING FILES ------
x86_64-elf-gcc ./build/kernel.o ./build/memcmp.o ./build/memcpy.o ./build/memmove.o ./build/memset.o -o ./build/p64.bin \
  ./build/putpixel.o ./build/fillrect.o ./build/writechar.o ./build/strlen.o ./build/printf.o ./build/putchar.o -T ./src/kernel/arch/x86_64-elf/linker.ld -nostdlib -static -z max-page-size=0x1000

echo ----- SETTING UP ISO DIR -----
echo Copying files into ./iso_root/boot/
cp -v ./build/p64.bin ./iso_root/boot/
echo Copying files into ./iso_root/boot/limine/
cp -v ./src/limine.conf ./limine/limine-bios.sys ./limine/limine-bios-cd.bin \
  ./limine/limine-uefi-cd.bin ./iso_root/boot/limine/
echo Copying files into ./iso_root/EFI/BOOT/
cp -v ./limine/BOOTX64.EFI ./iso_root/EFI/BOOT/
cp -v ./limine/BOOTIA32.EFI ./iso_root/EFI/BOOT/

echo ----- BUILDING ISO -----
xorriso -as mkisofs -R -r -J -b boot/limine/limine-bios-cd.bin \
  -no-emul-boot -boot-load-size 4 -boot-info-table -hfsplus \
  -apm-block-size 2048 --efi-boot boot/limine/limine-uefi-cd.bin \
  -efi-boot-part --efi-boot-image --protective-msdos-label \
  iso_root -o ./build/p64.iso
./limine/limine bios-install ./build/p64.iso

qemu-system-x86_64 -cdrom ./build/p64.iso
