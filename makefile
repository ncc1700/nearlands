all-amd64-debug:
	cd nearldr && cargo build --target x86_64-unknown-uefi
	cd nearkrnl && cargo build --target x86_64-unknown-none

all-aarch64-debug:
	cd nearldr && cargo build --target aarch64-unknown-uefi
	cd nearkrnl && cargo build --target aarch64-unknown-none

build-img-amd64-debug:
	mkdir -p bin
	mkdir -p bin/ldr
	mkdir -p bin/system
	cp nearldr/target/x86_64-unknown-uefi/debug/nearldr.efi bin/ldr/BOOTX64.EFI
	cp nearkrnl/target/x86_64-unknown-none/debug/nearkrnl bin/system/nearkrnl
	qemu-img create -f raw image.img 1440K
	mformat -i image.img -f 1440 ::
	mmd -i image.img ::/EFI
	mmd -i image.img ::/EFI/BOOT
	mmd -i image.img ::/SYSTEM
	mcopy -i image.img bin/ldr/BOOTX64.EFI ::/EFI/BOOT
	mcopy -i image.img bin/system/nearkrnl ::/SYSTEM
	mcopy -i image.img nearldr.conf ::/SYSTEM


run-amd64:
	qemu-system-x86_64 -bios resources/UEFI/amd64/OVMF.fd \
	-device virtio-gpu-pci -display sdl \
	-drive file=image.img,format=raw -m 96M -serial mon:stdio


amd64-debug:
	make all-amd64-debug
	make build-img-amd64-debug
	make run-amd64


clean:
	cd nearldr && cargo clean
	cd nearkrnl && cargo clean
	rm -rf bin
	rm image.img