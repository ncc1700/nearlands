
build-amd64-image:
	cd nearldr && xmake build nearldr-amd64
	cd neark && xmake build neark-amd64
	qemu-img create -f raw image.img 1440K
	mformat -i image.img -f 1440 ::
	mmd -i image.img ::/EFI
	mmd -i image.img ::/EFI/BOOT
	mmd -i image.img ::/SYSTEM
	mcopy -i image.img binaries/BOOT/BOOTX64.efi ::/EFI/BOOT
	mcopy -i image.img binaries/SYSTEM/neark.sys ::/SYSTEM
	mcopy -i image.img nearldr.ini ::/

build-aarch64-image:
	cd nearldr && xmake build nearldr-aarch64
	cd neark && xmake build neark-aarch64
	qemu-img create -f raw image.img 1440K
	mformat -i image.img -f 1440 ::
	mmd -i image.img ::/EFI
	mmd -i image.img ::/EFI/BOOT
	mmd -i image.img ::/SYSTEM
	mcopy -i image.img binaries/BOOT/BOOTAA64.efi ::/EFI/BOOT
	mcopy -i image.img binaries/SYSTEM/neark.sys ::/SYSTEM
	mcopy -i image.img nearldr.ini ::/

run-amd64:
	qemu-system-x86_64 -bios resources/UEFI/amd64/OVMF.fd \
	-device virtio-gpu-pci -display sdl \
	-drive file=image.img,format=raw -m 96M -serial mon:stdio

run-aarch64:
	qemu-system-aarch64 -bios resources/UEFI/aarch64/OVMF.fd \
		-machine virt -cpu cortex-a57 -m 512M -device ramfb -device usb-ehci -device usb-kbd -display sdl \
		-drive if=none,file=image.img,format=raw,id=hd0 \
  		-device virtio-blk-device,drive=hd0 \
		-serial mon:stdio

amd64:
	make build-amd64-image
	make run-amd64

aarch64:
	make build-aarch64-image
	make run-aarch64

clean:
	rm -rf binaries
	rm image.img
	cd nearldr && make clean
	cd neark && make clean
