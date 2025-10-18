# Nearlands

this is a small project I made to continue the ideas I got from furtherium with some stuff I learned from friends

goals is to be a portable, easily extendable and simple operating system for learning

# External Projects Nearlands uses

[Limine](https://codeberg.org/limine/limine) (the bootloader)

[Font8x8](https://github.com/dhepper/font8x8) (the terminals font)

[mini-printf](https://github.com/mludvig/mini-printf) (simple and usable printf impl for embedded projects)





# How to build

you will need clang, nasm (for amd64 builds), xmake, make, mtools and QEMU (the entire suite, including qemu-img and its many emulators)

when done so, clone this repo and run `make setup`, this will fetch limine (our bootloader) and anything else that is needed

after this, you have two choices, you can either build the amd64-generic image or the aarch64-virt image

so far, amd64-generic is more complete so we would advice you build that


for AMD64-GENERIC run:

    `make amd64-generic`
    
for AARCH64-VIRT run:

    `make aarch64-virt`


itll compile the kernel and everything else, build an image and then start the QEMU emulator
