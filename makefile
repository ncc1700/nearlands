build-amd64-generic-image:
	cd kernel && xmake build kernel-amd64-generic-all-drivers
	qemu-img create -f raw image.img 1440K
	mformat -i image.img -f 1440 ::
	mmd -i image.img ::/EFI
	mmd -i image.img ::/EFI/BOOT
	mmd -i image.img ::/SYSTEM
	mcopy -i image.img Limine/BOOTX64.EFI ::/EFI/BOOT
	mcopy -i image.img bin/SYSTEM/kernel.elf ::/SYSTEM
	mcopy -i image.img limine.conf ::/

run-amd64-generic:
	qemu-system-x86_64 -bios resources/UEFI/amd64/OVMF.fd \
	-device virtio-gpu-pci -display sdl \
	-drive file=image.img,format=raw -m 4G -serial mon:stdio

amd64-generic:
	make build-amd64-generic-image
	make run-amd64-generic

setup:
	git clone https://codeberg.org/Limine/Limine.git --branch=v10.x-binary --depth=1

clean:
	rm *.img
	cd kernel && make clean
	rm -rf bin

clean-all:
	rm *.img
	rm -rf Limine
	cd kernel && make clean
	rm -rf bin