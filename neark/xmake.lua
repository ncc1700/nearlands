
target("neark-amd64")
    set_toolchains("clangpeamd64")
    set_filename("neark.sys")
    set_targetdir("../binaries/SYSTEM/")
    add_files("src/*.c", "src/core/*.c", "src/core/**/*.c", "src/externheaders/**/*.c");
    add_files("src/HAL/amd64/*.c", "src/HAL/amd64/*.asm")
    add_files("src/HAL/amd64/**/*.c", "src/HAL/amd64/**/*.asm")
    add_files("src/drivers/*.c", "src/drivers/**/*.c")
    add_cflags("-target x86_64-pc-none-elf", "-ffreestanding", "-nostdlib", "-fno-exceptions", "-fno-rtti", "-fPIC", {force = true})
    add_cxxflags("-target x86_64-pc-none-elf", "-ffreestanding", "-nostdlib", "-fno-exceptions", "-fPIC", "-fno-rtti", {force = true})
    --add_asflags("-f elf64")
    --add_ldflags("/machine:amd64 /subsystem:native /entry:hal_loader_entry /base:0x100000", {force = true})
    add_ldflags("-nostdlib -T linker/amd64/linker.ld", {force = true})


target("neark-aarch64")
    set_toolchains("clangpeaarch64")
    set_filename("neark.sys")
    set_targetdir("../binaries/SYSTEM/")
    add_files("src/*.c", "src/core/*.c", "src/core/**/*.c", "src/externheaders/**/*.c");
    add_files("src/HAL/aarch64/*.c", "src/HAL/aarch64/*.S")
    add_files("src/HAL/aarch64/**/*.c", "src/HAL/aarch64/**/*.S")
    add_files("src/drivers/*.c", "src/drivers/**/*.c")
    add_cflags("-target aarch64-none-elf", "-ffreestanding", "-nostdlib", "-fno-exceptions", "-fPIC", "-fno-rtti", {force = true})
    add_cxxflags("-target aarch64-none-elf", "-ffreestanding", "-nostdlib", "-fno-exceptions", "-fPIC", "-fno-rtti", {force = true})
    add_asflags("-target aarch64-none-elf", "-ffreestanding", "-nostdlib", "-fno-exceptions", "-fPIC", "-fno-rtti", {force = true})
    --add_ldflags("/machine:arm64 /subsystem:native /entry:hal_loader_entry /base:0x40000000", {force = true})
    add_ldflags("-nostdlib -T linker/aarch64/linker.ld", {force = true})


toolchain("clangpeamd64")
    set_kind("standalone")
    set_toolset("cc", "clang")
    set_toolset("as", "nasm", {force = true})
    add_asflags("-f elf64", {force = true})
    set_toolset("ld", "ld.lld")
toolchain_end();

toolchain("clangpeaarch64")
    set_kind("standalone")
    set_toolset("cc", "clang")
    set_toolset("as", "clang")
    set_toolset("ld", "ld.lld")
toolchain_end();
