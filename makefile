
gen-comp-command:
	cd nearmonia && xmake project -k compile_commands --lsp=clangd
	cd narnify && xmake project -k compile_commands --lsp=clangd

build-amd64pc:
	cd nearmonia && xmake build nearmonia-amd64pc
	cd narnify && xmake build narnify-amd64pc
	qemu-img create -f raw nearlands.img 1440K
	mformat -i nearlands.img -f 1440 ::
	mmd -i nearlands.img ::/EFI
	mmd -i nearlands.img ::/EFI/BOOT
	mmd -i nearlands.img ::/SYSTEM
	mcopy -i nearlands.img output/EFI/BOOT/BOOTX64.EFI ::/EFI/BOOT
	mcopy -i nearlands.img output/SYSTEM/narnify.sys ::/SYSTEM


run-amd64pc:
	qemu-system-x86_64 -bios resources/UEFI/amd64pc/OVMF.fd \
		 -drive file=nearlands.img,format=raw -display sdl \
		 -m 96M -serial mon:stdio


run-amd64pc-mon:
	qemu-system-x86_64 -bios resources/UEFI/amd64pc/OVMF.fd \
		 -drive file=nearlands.img,format=raw -display gtk \
		 -m 96M

amd64pc:
	make build-amd64pc
	make run-amd64pc

img-to-vmdk:
	qemu-img convert -f raw -O vmdk nearlands.img nearlands.vmdk

clean-build:
	cd nearmonia && make clean
	cd narnify && make clean


clean:
	make clean-build
	rm -rf output
	rm nearlands.img
