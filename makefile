setuplimine:
	git clone https://github.com/limine-bootloader/limine.git --branch=v9.x-binary --depth=1
	make -C limine

setup:
	mkdir ./build/
	mkdir ./iso_root/
	mkdir ./iso_root/boot/
	mkdir ./iso_root/boot/limine/
	mkdir ./iso_root/EFI/
	mkdir ./iso_root/EFI/BOOT/

makekernel:
	cd ./src/kernel/; \
	make

makestdlib:
	cd ./src/libc/; \
	make

makep64lib:
	cd ./src/p64lib/; \
	make

makeiso:
	cp -v ./build/kernel/p64.bin ./iso_root/boot/
	cp -v ./src/limine.conf ./limine/limine-bios.sys ./limine/limine-bios-cd.bin \
  	./limine/limine-uefi-cd.bin ./iso_root/boot/limine/
	cp -v ./limine/BOOTX64.EFI ./iso_root/EFI/BOOT/
	cp -v ./limine/BOOTIA32.EFI ./iso_root/EFI/BOOT/
	xorriso -as mkisofs -R -r -J -b boot/limine/limine-bios-cd.bin \
	  -no-emul-boot -boot-load-size 4 -boot-info-table -hfsplus \
	  -apm-block-size 2048 --efi-boot boot/limine/limine-uefi-cd.bin \
	  -efi-boot-part --efi-boot-image --protective-msdos-label \
	  iso_root -o ./build/p64.iso
	./limine/limine bios-install ./build/p64.iso

build: clean setup makestdlib makep64lib makekernel makeiso run

run:
	qemu-system-x86_64 -cdrom ./build/p64.iso


clean:
	rm -rf ./iso_root/ ./build/

all: setuplimine setup build run 

.PHONY: clean setup build run
