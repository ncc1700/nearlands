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

build-aarch64-virt-image:
	cd kernel && xmake build kernel-aarch64-virt-all-drivers
	qemu-img create -f raw image.img 1440K
	mformat -i image.img -f 1440 ::
	mmd -i image.img ::/EFI
	mmd -i image.img ::/EFI/BOOT
	mmd -i image.img ::/SYSTEM
	mcopy -i image.img Limine/BOOTAA64.EFI ::/EFI/BOOT
	mcopy -i image.img bin/SYSTEM/kernel.elf ::/SYSTEM
	mcopy -i image.img limine.conf ::/

run-amd64-generic:
	qemu-system-x86_64 -bios resources/UEFI/amd64/OVMF.fd \
	-device virtio-gpu-pci -display sdl \
	-drive file=image.img,format=raw -m 96M -serial mon:stdio
	
run-aarch64-virt:
	qemu-system-aarch64 -bios resources/UEFI/aarch64/OVMF.fd \
		-machine virt -cpu cortex-a57 -m 512M -device ramfb -device usb-ehci -device usb-kbd -display sdl \
		-drive if=none,file=image.img,format=raw,id=hd0 \
  		-device virtio-blk-device,drive=hd0 \
		-serial mon:stdio

run-amd64-generic-smp:
	qemu-system-x86_64 -bios resources/UEFI/amd64/OVMF.fd \
	-device virtio-gpu-pci -display sdl \
	-drive file=image.img,format=raw -m 96M -serial mon:stdio -smp sockets=1,cores=2,threads=2

amd64-generic:
	make build-amd64-generic-image
	make run-amd64-generic

aarch64-virt:
	make build-aarch64-virt-image
	make run-aarch64-virt

amd64-generic-smp:
	make build-amd64-generic-image
	make run-amd64-generic-smp

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