
target("kernel-amd64-generic-all-drivers")
    set_toolchains("clangelfamd64")
    set_filename("kernel.elf")
    set_targetdir("../bin/SYSTEM/")
    add_files("src/*.c", "src/core/*.c", "src/core/**/*.c");
    add_files("src/hal/amd64-generic/*.c", "src/hal/amd64-generic/*.asm")
    add_files("src/hal/amd64-generic/**/*.c", "src/hal/amd64-generic/**/*.asm")
    add_files("src/drivers/*.c", "src/drivers/**/*.c")
    add_cflags("-target x86_64-pc-none-elf", "-ffreestanding", "-nostdlib", "-fno-exceptions", "-fno-rtti", "-mcmodel=kernel", {force = true})
    add_cxxflags("-target x86_64-pc-none-elf", "-ffreestanding", "-nostdlib", "-fno-exceptions", "-fno-rtti", "-mcmodel=kernel", {force = true})
    add_asflags("-f elf64") 
    add_ldflags("-nostdlib -T linker/amd64-generic/linker.ld", {force = true})

target("kernel-aarch64-virt-all-drivers")
    set_toolchains("clangelfaarch64")
    set_filename("kernel.elf")
    set_targetdir("../bin/SYSTEM/")
    add_files("src/*.c", "src/core/*.c", "src/core/**/*.c");
    add_files("src/hal/aarch64-virt/*.c", "src/hal/aarch64-virt/*.S")
    add_files("src/hal/aarch64-virt/**/*.c", "src/hal/aarch64-virt/**/*.S")
    add_files("src/drivers/*.c", "src/drivers/**/*.c")
    add_cflags("-target aarch64-none-elf", "-ffreestanding", "-nostdlib", "-fno-exceptions", "-fno-rtti", {force = true})
    add_cxxflags("-target aarch64-none-elf", "-ffreestanding", "-nostdlib", "-fno-exceptions", "-fno-rtti", {force = true})
    add_asflags("-target aarch64-none-elf", "-ffreestanding", "-nostdlib", "-fno-exceptions", "-fno-rtti", {force = true})
    add_ldflags("-nostdlib -T linker/aarch64-virt/linker.ld", {force = true})

toolchain("clangelfamd64")
    set_kind("standalone")
    set_toolset("cc", "clang")
    set_toolset("as", "nasm", {force = true})
    add_asflags("-f elf64", {force = true})
    set_toolset("ld", "ld.lld")
toolchain_end();

toolchain("clangelfaarch64")
    set_kind("standalone")
    set_toolset("cc", "clang")
    set_toolset("as", "clang")
    set_toolset("ld", "ld.lld")
toolchain_end();

