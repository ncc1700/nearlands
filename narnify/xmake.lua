



target("narnify-amd64pc")
    set_targetdir("../output/SYSTEM")
    set_filename("narnify.sys")
    set_toolchains("clang-amd64pc")
    add_files("src/*.c")
    add_files("src/arch/amd64pc/*.c", "src/arch/amd64pc/*.S")
    add_cflags("-target x86_64-pc-win32-coff -ffreestanding -nostdlib", {force = true})
    add_asflags("-f win64", {force = true})
    add_ldflags("/machine:amd64 /subsystem:native /entry:KernLoaderEntry /base:0xFFFFFF8000000000", {force = true})


toolchain("clang-amd64pc")
    set_kind("standalone")
    set_toolset("cc", "clang")
    set_toolset("ld", "lld-link")
    set_toolset("as", "nasm")
toolchain_end()